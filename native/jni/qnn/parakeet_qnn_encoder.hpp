// Minimal wrapper around the QAIRT/QNN C++ API to load and execute the
// Parakeet-TDT-0.6B-v3 Android NPU encoder (a pre-compiled QNN HTP context
// binary targeting Hexagon v79) on-device. ONNX Runtime is not involved here:
// the encoder was already compiled ahead-of-time into this binary, and the
// QAIRT SDK's native libraries are all that's needed to load and run it.
//
// Tensor contract (fixed at export time for this specific encoder build):
//   input  "audio_signal": float32 [1, 128, 601]  (128 mel bins x 601 frames)
//   output "outputs":      float32 [1, 1024, 76]  (1024-dim embeddings x 76 frames)
// Shapes are read back from the context binary's own metadata rather than
// hardcoded, so a differently-shaped encoder build still works as long as it
// has exactly one input and one output tensor.
#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace qnnx {

struct ParakeetQnnEncoderImpl;

class ParakeetQnnEncoder {
public:
    // backendLibPath:  absolute path to libQnnHtp.so
    // systemLibPath:   absolute path to libQnnSystem.so
    // contextBinaryPath: absolute path to the parakeet-enc-*_ctx_qnn.bin file
    // Throws std::runtime_error with a descriptive message on any failure.
    ParakeetQnnEncoder(const std::string& backendLibPath,
                        const std::string& systemLibPath,
                        const std::string& contextBinaryPath);
    ~ParakeetQnnEncoder();

    ParakeetQnnEncoder(const ParakeetQnnEncoder&) = delete;
    ParakeetQnnEncoder& operator=(const ParakeetQnnEncoder&) = delete;

    // audioSignal must have exactly getInputElementCount() elements.
    // Returns getOutputElementCount() elements. Throws std::runtime_error on failure.
    std::vector<float> run(const std::vector<float>& audioSignal);

    size_t getInputElementCount() const;
    size_t getOutputElementCount() const;

private:
    std::unique_ptr<ParakeetQnnEncoderImpl> m_impl;
};

} // namespace qnnx
