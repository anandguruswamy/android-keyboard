#include "parakeet_qnn_encoder.hpp"

#include <cstdio>
#include <fstream>
#include <stdexcept>

#include "QairtApi.hpp"
#include "QairtBackend.hpp"
#include "QairtContext.hpp"
#include "QairtGraph.hpp"
#include "QairtTensor.hpp"
#include "QairtCommon/QairtBackendCapabilityKeys.h"
#include "System/QairtSystemContext.hpp"

namespace qnnx {

namespace {

size_t elementCount(const std::vector<uint32_t>& dims) {
    size_t n = 1;
    for (auto d : dims) n *= d;
    return n;
}

} // namespace

struct ParakeetQnnEncoderImpl {
    std::shared_ptr<qairt::Api> api;
    std::shared_ptr<qairt::SystemApi> systemApi;
    std::shared_ptr<qairt::Backend> backend;
    std::shared_ptr<qairt::Context> context;
    std::shared_ptr<qairt::SystemContext> systemContext;
    std::shared_ptr<qairt::Graph> graph;

    uint64_t inputId = 0;
    uint64_t outputId = 0;
    std::string inputName;
    std::string outputName;
    std::vector<uint32_t> inputDims;
    std::vector<uint32_t> outputDims;
};

ParakeetQnnEncoder::ParakeetQnnEncoder(const std::string& backendLibPath,
                                       const std::string& systemLibPath,
                                       const std::string& contextBinaryPath)
    : m_impl(std::make_unique<ParakeetQnnEncoderImpl>()) {
    try {
        m_impl->api = std::make_shared<qairt::Api>(backendLibPath.c_str());
        m_impl->systemApi = std::make_shared<qairt::SystemApi>(systemLibPath.c_str());

        auto log = m_impl->api->makeShared<qairt::Log>(
            [](const char* fmt, QairtLog_Level_t /*level*/, uint64_t /*timestamp*/,
               void* /*userData*/, va_list argp) {
                char str[512];
                str[0] = '\0';
                vsnprintf(str, sizeof(str), fmt, argp);
                fprintf(stderr, "[ParakeetQnn] %s\n", str);
            },
            QAIRT_LOG_LEVEL_WARN);

        m_impl->backend = m_impl->api->makeShared<qairt::Backend>(*log);

        // Read the context binary file into memory.
        std::ifstream f(contextBinaryPath, std::ios::binary | std::ios::ate);
        if (!f) {
            throw std::runtime_error("Failed to open context binary: " + contextBinaryPath);
        }
        const std::streamsize size = f.tellg();
        if (size <= 0) {
            throw std::runtime_error("Context binary is empty: " + contextBinaryPath);
        }
        f.seekg(0, std::ios::beg);
        std::vector<char> buffer(static_cast<size_t>(size));
        if (!f.read(buffer.data(), size)) {
            throw std::runtime_error("Failed to read context binary: " + contextBinaryPath);
        }

        qairt::ContextBinaryBuffer contextBinaryBuffer = m_impl->api->make<qairt::ContextBinaryBuffer>();
        contextBinaryBuffer.setData(static_cast<void*>(buffer.data()));
        contextBinaryBuffer.setSize(static_cast<uint64_t>(size));

        // Must stay alive as long as we access tensor metadata derived from it.
        m_impl->systemContext = m_impl->systemApi->makeShared<qairt::SystemContext>();
        auto binaryInfo = m_impl->systemContext->getBinaryInfo(contextBinaryBuffer.getData(),
                                                                 contextBinaryBuffer.getSize());

        m_impl->context = std::make_shared<qairt::Context>(
            m_impl->backend->createContextFromBinary(std::nullopt, std::nullopt, contextBinaryBuffer));

        auto& graphInfos = binaryInfo.getGraphs();
        if (graphInfos.size() != 1) {
            throw std::runtime_error("Expected exactly one graph in context binary, found " +
                                      std::to_string(graphInfos.size()));
        }
        auto& graphInfo = graphInfos.front();

        if (graphInfo.getGraphInputs().size() != 1 || graphInfo.getGraphOutputs().size() != 1) {
            throw std::runtime_error("Expected exactly one input and one output tensor");
        }
        auto& inputTensorInfo = graphInfo.getGraphInputs().front();
        auto& outputTensorInfo = graphInfo.getGraphOutputs().front();
        m_impl->inputId = inputTensorInfo.getId();
        m_impl->outputId = outputTensorInfo.getId();
        m_impl->inputName = inputTensorInfo.getName();
        m_impl->outputName = outputTensorInfo.getName();
        m_impl->inputDims.assign(inputTensorInfo.getDimensions(),
                                 inputTensorInfo.getDimensions() + inputTensorInfo.getRank());
        m_impl->outputDims.assign(outputTensorInfo.getDimensions(),
                                  outputTensorInfo.getDimensions() + outputTensorInfo.getRank());

        auto info = m_impl->api->makeShared<qairt::Info>();
        const std::string graphName = graphInfo.getGraphName();
        m_impl->graph = m_impl->context->retrieveGraph(graphName);
        if (info->hasCapability(QAIRT_CAPABILITY_GRAPH_SUPPORT_FINALIZE_DESERIALIZED_GRAPH)) {
            m_impl->graph->finalize();
        }
        // Else: the backend (e.g. HTP) can't finalize a deserialized graph, but a
        // context binary is already fully compiled, so the retrieved graph is usable as-is.
    } catch (qairt::Exception& e) {
        throw std::runtime_error(std::string("QAIRT error: ") + e.what());
    }
}

ParakeetQnnEncoder::~ParakeetQnnEncoder() = default;

size_t ParakeetQnnEncoder::getInputElementCount() const {
    return elementCount(m_impl->inputDims);
}

size_t ParakeetQnnEncoder::getOutputElementCount() const {
    return elementCount(m_impl->outputDims);
}

std::vector<float> ParakeetQnnEncoder::run(const std::vector<float>& audioSignal) {
    if (audioSignal.size() != getInputElementCount()) {
        throw std::runtime_error("audioSignal has " + std::to_string(audioSignal.size()) +
                                  " elements, expected " + std::to_string(getInputElementCount()));
    }

    try {
        auto inputTensor = m_impl->api->makeShared<qairt::Tensor>();
        qairt::TensorProperties inputProps = m_impl->api->make<qairt::TensorProperties>();
        inputProps.setIsInput(true);
        inputProps.setIsOutput(false);
        inputTensor->setId(m_impl->inputId);
        inputTensor->setName(m_impl->inputName);
        inputTensor->setTensorProperties(inputProps);
        inputTensor->setDataFormat(0); // Dense
        inputTensor->setDataType(qairt::DataType::Float32);
        inputTensor->setDimensions(m_impl->inputDims);
        inputTensor->setIsDynamicDimensions(std::vector<bool>(m_impl->inputDims.size(), false));

        auto& inputClientBuffer = inputTensor->getClientBuffer();
        inputClientBuffer.setData(const_cast<void*>(static_cast<const void*>(audioSignal.data())));
        inputClientBuffer.setDataSize(static_cast<uint32_t>(audioSignal.size() * sizeof(float)));

        auto outputTensor = m_impl->api->makeShared<qairt::Tensor>();
        qairt::TensorProperties outputProps = m_impl->api->make<qairt::TensorProperties>();
        outputProps.setIsInput(false);
        outputProps.setIsOutput(true);
        outputTensor->setId(m_impl->outputId);
        outputTensor->setName(m_impl->outputName);
        outputTensor->setTensorProperties(outputProps);
        outputTensor->setDataFormat(0);
        outputTensor->setDataType(qairt::DataType::Float32);
        outputTensor->setDimensions(m_impl->outputDims);
        outputTensor->setIsDynamicDimensions(std::vector<bool>(m_impl->outputDims.size(), false));

        std::vector<float> output(getOutputElementCount());
        auto& outputClientBuffer = outputTensor->getClientBuffer();
        outputClientBuffer.setData(static_cast<void*>(output.data()));
        outputClientBuffer.setDataSize(static_cast<uint32_t>(output.size() * sizeof(float)));

        std::vector<std::shared_ptr<qairt::Tensor>> inputs{inputTensor};
        std::vector<std::shared_ptr<qairt::Tensor>> outputs{outputTensor};
        m_impl->graph->execute(inputs, outputs);

        return output;
    } catch (qairt::Exception& e) {
        throw std::runtime_error(std::string("QAIRT execution error: ") + e.what());
    }
}

} // namespace qnnx
