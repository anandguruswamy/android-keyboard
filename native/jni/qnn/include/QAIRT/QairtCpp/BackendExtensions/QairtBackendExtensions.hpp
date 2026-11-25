//==============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================
#pragma once

/**
 * @file
 * @brief C++ wrapper for the QAIRT Backend Extensions API.
 *
 *        A BackendExtensions object augments the lifecycle of a QAIRT backend with hooks
 *        invoked before and after key backend, context, graph, device, and execution
 *        operations, allowing an extension to inspect or modify state at each point.
 */

#include <memory>
#include <vector>

#include "../QairtApi.hpp"
#include "../QairtBackend.hpp"
#include "../QairtContext.hpp"
#include "../QairtDevice.hpp"
#include "QairtBackendExtensionsAllocator.hpp"
#include "QairtBackendExtensionsApi.hpp"
#include "QairtBackendExtensions/QairtBackendExtensions.h"

namespace qairt {

/**
 * @brief Performance profiles supported by QAIRT Backend Extensions.
 *
 * @details
 * | Enumerator                          | Description                                                     |
 * |--------------------------------------|-------------------------------------------------------------------|
 * | `LowBalanced`                        | Balanced performance/power profile, biased toward power savings. |
 * | `Balanced`                           | Balanced performance and power consumption.                      |
 * | `Default`                            | Default performance profile chosen by the backend.               |
 * | `HighPerformance`                    | Maximizes performance for a short burst of execution.            |
 * | `SustainedHighPerformance`           | Maximizes performance while sustaining it over a longer duration.|
 * | `Burst`                              | Maximizes performance for the shortest, most latency-critical bursts of execution. |
 * | `LlmDecodeBurst`                     | Burst profile tuned for LLM decode (token-by-token) execution.   |
 * | `LlmDecodeLowBalanced`               | Low-balanced profile tuned for LLM decode execution.              |
 * | `LlmDecodeBalanced`                  | Balanced profile tuned for LLM decode execution.                  |
 * | `LlmDecodeDefault`                   | Default profile tuned for LLM decode execution.                   |
 * | `LlmDecodeHighPerformance`           | High-performance profile tuned for LLM decode execution.          |
 * | `LlmDecodeSustainedHighPerformance`  | Sustained high-performance profile tuned for LLM decode execution.|
 * | `LlmDecodeExtremePowerSaver`         | Extreme power saver profile tuned for LLM decode execution.       |
 * | `LlmDecodeLowPowerSaver`             | Low power saver profile tuned for LLM decode execution.           |
 * | `LlmDecodePowerSaver`                | Power saver profile tuned for LLM decode execution.               |
 * | `LlmDecodeHighPowerSaver`            | High power saver profile tuned for LLM decode execution.          |
 * | `ExtremePowerSaver`                  | Maximizes power savings at the expense of performance.            |
 * | `LowPowerSaver`                      | Favors power savings over performance, to a lesser degree than `ExtremePowerSaver`. |
 * | `PowerSaver`                         | Favors power savings over performance.                            |
 * | `HighPowerSaver`                     | Favors power savings over performance, to a greater degree than `PowerSaver`. |
 * | `SystemSettings`                     | Defers to the system-wide power/performance settings.             |
 * | `NoUserInput`                        | Indicates no explicit performance profile was requested.          |
 * | `Custom`                             | A custom performance profile defined outside the predefined set.  |
 * | `Invalid`                            | Not a valid performance profile.                                   |
 * | `Undefined`                          | Sentinel value; not a valid performance profile.                   |
 */
QAIRT_CPPAPI_DEFINE_ENUM(BackendExtensionsPerfProfile,
                         QairtBackendExt_PerfProfile_t,
                         LowBalanced = QAIRT_BACKEND_EXT_PERF_PROFILE_LOW_BALANCED,
                         Balanced = QAIRT_BACKEND_EXT_PERF_PROFILE_BALANCED,
                         Default = QAIRT_BACKEND_EXT_PERF_PROFILE_DEFAULT,
                         HighPerformance = QAIRT_BACKEND_EXT_PERF_PROFILE_HIGH_PERFORMANCE,
                         SustainedHighPerformance = QAIRT_BACKEND_EXT_PERF_PROFILE_SUSTAINED_HIGH_PERFORMANCE,
                         Burst = QAIRT_BACKEND_EXT_PERF_PROFILE_BURST,
                         LlmDecodeBurst = QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_BURST,
                         LlmDecodeLowBalanced = QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_LOW_BALANCED,
                         LlmDecodeBalanced = QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_BALANCED,
                         LlmDecodeDefault = QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_DEFAULT,
                         LlmDecodeHighPerformance = QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_HIGH_PERFORMANCE,
                         LlmDecodeSustainedHighPerformance = QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_SUSTAINED_HIGH_PERFORMANCE,
                         LlmDecodeExtremePowerSaver = QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_EXTREME_POWER_SAVER,
                         LlmDecodeLowPowerSaver = QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_LOW_POWER_SAVER,
                         LlmDecodePowerSaver = QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_POWER_SAVER,
                         LlmDecodeHighPowerSaver = QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_HIGH_POWER_SAVER,
                         ExtremePowerSaver = QAIRT_BACKEND_EXT_PERF_PROFILE_EXTREME_POWER_SAVER,
                         LowPowerSaver = QAIRT_BACKEND_EXT_PERF_PROFILE_LOW_POWER_SAVER,
                         PowerSaver = QAIRT_BACKEND_EXT_PERF_PROFILE_POWER_SAVER,
                         HighPowerSaver = QAIRT_BACKEND_EXT_PERF_PROFILE_HIGH_POWER_SAVER,
                         SystemSettings = QAIRT_BACKEND_EXT_PERF_PROFILE_SYSTEM_SETTINGS,
                         NoUserInput = QAIRT_BACKEND_EXT_PERF_PROFILE_NO_USER_INPUT,
                         Custom = QAIRT_BACKEND_EXT_PERF_PROFILE_CUSTOM,
                         Invalid = QAIRT_BACKEND_EXT_PERF_PROFILE_INVALID,
                         // Unused, present to ensure 32 bits.
                         Undefined = QAIRT_BACKEND_EXT_PERF_PROFILE_UNDEFINED)

/**
 * @brief Wrapper for a QAIRT Backend Extensions handle.
 *
 *        Obtained via `BackendExtensionsApi`.
 */
class BackendExtensions : public ApiType<BackendExtensions,
                                         QairtBackendExt_t,
                                         BackendExtensionsApiTable> {
  friend class Api;
  friend class BackendExtensionsApi;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  BackendExtensions() noexcept                               = default;
  BackendExtensions(const BackendExtensions&)                = delete;
  BackendExtensions(BackendExtensions&&) noexcept            = default;
  BackendExtensions& operator=(const BackendExtensions&)     = delete;
  BackendExtensions& operator=(BackendExtensions&&) noexcept = default;

  /**
   * @brief Initialize this backend extensions object with the backend interface it extends.
   *
   * @param[in] backendApi  The backend Api whose interface table this extensions object
   *                         will operate against.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtBackendExt_initialize
   */
  void initialize(const Api& backendApi) {
    void* qairtInterfacePtr = reinterpret_cast<void*>(backendApi.m_getInterfaceFn);
    callAndConfirm<&interface_type::initialize>(qairtInterfacePtr);
  }

  /**
   * @brief Load configuration options for this backend extensions object from a JSON string.
   *
   * @param[in] config  A JSON-formatted configuration string.
   *
   * @throws qairt::Exception on invalid handle or malformed configuration.
   *
   * @see QairtBackendExt_loadConfig
   */
  void loadConfig(const std::string& config) {
    callAndConfirm<&interface_type::loadConfig>(config.c_str());
  }

  /**
   * @brief Set the performance profile this backend extensions object should apply.
   *
   * @param[in] perfProfile  The desired performance profile.
   *
   * @throws qairt::Exception on invalid handle, invalid argument, or unsupported feature.
   *
   * @see QairtBackendExt_setPerfProfile
   */
  void setPerfProfile(BackendExtensionsPerfProfile perfProfile) {
    callAndConfirm<&interface_type::setPerfProfile>(static_cast<QairtBackendExt_PerfProfile_t>(perfProfile));
  }

  /**
   * @brief Query the profiling level requested by this backend extensions object.
   *
   * @return The requested profiling level.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_getProfilingLevel
   */
  uint32_t getProfilingLevel() {
    return callGetter<&interface_type::getProfilingLevel>();
  }

  /**
   * @brief Hook invoked before a backend is initialized, allowing this extension to inspect
   *        or modify the backend configuration before it takes effect.
   *
   * @param[in] configuration  The backend configuration about to be applied.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeBackendInitialize
   */
  void beforeBackendInitialize(qairt::BackendConfiguration& configuration) {
    callAndConfirm<&interface_type::beforeBackendInitialize>(detail::QairtTypeAdmin::getHandle(configuration));
  }

  /**
   * @brief Hook invoked after a backend has finished initializing.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterBackendInitialize
   */
  void afterBackendInitialize() {
    callAndConfirm<&interface_type::afterBackendInitialize>();
  }

  /**
   * @brief Hook invoked before a context is created, allowing this extension to inspect or
   *        modify the context configuration before it takes effect.
   *
   * @param[in] configuration  The context configuration about to be applied.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeContextCreate
   */
  void beforeContextCreate(qairt::ContextConfiguration& configuration) {
    callAndConfirm<&interface_type::beforeContextCreate>(detail::QairtTypeAdmin::getHandle(configuration));
  }

  /**
   * @brief Hook invoked after a context has been created.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterContextCreate
   */
  void afterContextCreate() {
    callAndConfirm<&interface_type::afterContextCreate>();
  }

  /**
   * @brief Hook invoked before a set of graphs are composed within a context, allowing this
   *        extension to inspect or modify each graph's configuration before composition
   *        takes effect.
   *
   * @param[in] configs  Map of graph name to the configuration about to be applied for that
   *                      graph.
   *
   * @throws qairt::Exception if `configs.size()` exceeds `uint32_t` limits, or on invalid
   *         handle.
   *
   * @see QairtBackendExt_beforeComposeGraphs
   */
  void beforeComposeGraphs(std::unordered_map<std::string, qairt::GraphConfiguration>& configs) {
    uint32_t numGraphs{0u};
    if (configs.size() <= std::numeric_limits<uint32_t>::max()) {
      numGraphs = static_cast<uint32_t>(configs.size());
    } else {
      throw Exception("Map size exceeds uint32_t limit", CommonError::InvalidArgument);
    }
    std::vector<const char*> graphNames(numGraphs);
    std::vector<QairtGraph_ConfigHandle_t> configHandles(numGraphs);
    uint32_t idx{0u};
    for (const auto& [name, config] : configs) {
      graphNames[idx]    = name.c_str();
      configHandles[idx] = config.getHandle();
      ++idx;
    }
    callAndConfirm<&interface_type::beforeComposeGraphs>(numGraphs, graphNames.data(), configHandles.data());
  }

  /**
   * @brief Hook invoked after a set of graphs have been composed.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterComposeGraphs
   */
  void afterComposeGraphs() {
    callAndConfirm<&interface_type::afterComposeGraphs>();
  }

  /**
   * @brief Hook invoked before a graph is finalized, allowing this extension to inspect or
   *        modify its configuration before finalization takes effect.
   *
   * @param[in] graphName      Name of the graph about to be finalized.
   * @param[in] graph          The graph about to be finalized.
   * @param[in] configuration  The configuration about to be applied during finalization.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeGraphFinalize
   */
  void beforeGraphFinalize(const std::string& graphName,
                           qairt::Graph& graph,
                           qairt::GraphConfiguration& configuration) {
    callAndConfirm<&interface_type::beforeGraphFinalize>(
      graphName.c_str(),
      detail::QairtTypeAdmin::getHandle(graph),
      detail::QairtTypeAdmin::getHandle(configuration)
    );
  }

  /**
   * @brief Hook invoked after a graph has been finalized.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterGraphFinalize
   */
  void afterGraphFinalize() {
    callAndConfirm<&interface_type::afterGraphFinalize>();
  }

  /**
   * @brief Hook invoked before a graph is executed, allowing this extension to inspect or
   *        modify its execution configuration before execution takes effect.
   *
   * @param[in] graphName      Name of the graph about to be executed.
   * @param[in] configuration  The configuration about to be applied during execution.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeExecute
   */
  void beforeExecute(const std::string& graphName, qairt::GraphConfiguration& configuration) {
    callAndConfirm<&interface_type::beforeExecute>(graphName.c_str(), detail::QairtTypeAdmin::getHandle(configuration));
  }

  /**
   * @brief Hook invoked after a graph has been executed.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterExecute
   */
  void afterExecute() {
    callAndConfirm<&interface_type::afterExecute>();
  }

  /**
   * @brief Hook invoked before one or more contexts are freed.
   *
   * @param[in] contexts  Contexts about to be freed. Null entries are passed through as null
   *                       handles.
   *
   * @throws qairt::Exception if `contexts.size()` exceeds `uint32_t` limits, or on invalid
   *         handle.
   *
   * @see QairtBackendExt_beforeContextFree
   */
  void beforeContextFree(const std::vector<qairt::Context*>& contexts) {
    uint32_t numCtx;
    if (contexts.size() <= std::numeric_limits<uint32_t>::max()) {
      numCtx = static_cast<uint32_t>(contexts.size());
    } else {
      throw Exception("Array size exceeds uint32_t limit", CommonError::InvalidArgument);
    }
    std::vector<QairtContext_Handle_t> contextHandles(numCtx);
    std::transform(contexts.begin(), contexts.end(), contextHandles.begin(),
                   [](qairt::Context* ctx) { return ctx ? ctx->getHandle() : QairtContext_Handle_t{}; });
    callAndConfirm<&interface_type::beforeContextFree>(numCtx, contextHandles.data());
  }

  /**
   * @brief Hook invoked after one or more contexts have been freed.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterContextFree
   */
  void afterContextFree() {
    callAndConfirm<&interface_type::afterContextFree>();
  }

  /**
   * @brief Hook invoked before the backend is terminated, allowing this extension to release
   *        resources tied to the backend's lifetime.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeBackendTerminate
   */
  void beforeBackendTerminate() {
    callAndConfirm<&interface_type::beforeBackendTerminate>();
  }

  /**
   * @brief Hook invoked after the backend has terminated.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterBackendTerminate
   */
  void afterBackendTerminate() {
    callAndConfirm<&interface_type::afterBackendTerminate>();
  }

  /**
   * @brief Hook invoked before a context is created from a binary, allowing this extension to
   *        inspect or modify the context configuration before it takes effect.
   *
   * @param[in] configuration  The context configuration about to be applied.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeCreateFromBinary
   */
  void beforeCreateFromBinary(qairt::ContextConfiguration& configuration) {
    callAndConfirm<&interface_type::beforeCreateFromBinary>(detail::QairtTypeAdmin::getHandle(configuration));
  }

  /**
   * @brief Hook invoked after a context has been created from a binary.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterCreateFromBinary
   */
  void afterCreateFromBinary() {
    callAndConfirm<&interface_type::afterCreateFromBinary>();
  }

  /**
   * @brief Hook invoked before multiple contexts are created from a list of binaries, allowing
   *        this extension to inspect or modify each context's configuration before creation
   *        takes effect.
   *
   * @param[in] contextConfigMap      Map of context key to the per-context configuration about
   *                                  to be applied.
   * @param[in] commonCustomConfigs   Configuration shared across all contexts in the list.
   *
   * @throws qairt::Exception if `contextConfigMap.size()` exceeds `uint32_t` limits, or on
   *         invalid handle.
   *
   * @see QairtBackendExt_beforeCreateContextsFromBinaryList
   */
  void beforeCreateContextsFromBinaryList(
      std::unordered_map<std::string, qairt::ContextConfiguration>& contextConfigMap,
      qairt::ContextConfiguration& commonCustomConfigs) {
    uint32_t numContexts{0u};
    if (contextConfigMap.size() <= std::numeric_limits<uint32_t>::max()) {
      numContexts = static_cast<uint32_t>(contextConfigMap.size());
    } else {
      throw Exception("Map size exceeds uint32_t limit", CommonError::InvalidArgument);
    }
    std::vector<const char*> contextKeys(numContexts);
    std::vector<QairtContext_ConfigHandle_t> configHandles(numContexts);
    uint32_t idx{0u};
    for (const auto& [key, config] : contextConfigMap) {
      contextKeys[idx]   = key.c_str();
      configHandles[idx] = config.getHandle();
      ++idx;
    }
    callAndConfirm<&interface_type::beforeCreateContextsFromBinaryList>(
      numContexts,
      contextKeys.data(),
      configHandles.data(),
      commonCustomConfigs.getHandle());
  }

  /**
   * @brief Hook invoked after all contexts in a binary list have been created.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterCreateContextsFromBinaryList
   */
  void afterCreateContextsFromBinaryList() {
    callAndConfirm<&interface_type::afterCreateContextsFromBinaryList>();
  }

  /**
   * @brief Hook invoked before a device is created, allowing this extension to inspect or
   *        modify the device configuration before it takes effect.
   *
   * @param[in] configuration  The device configuration about to be applied.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeCreateDevice
   */
  void beforeCreateDevice(qairt::DeviceConfiguration& configuration) {
    callAndConfirm<&interface_type::beforeCreateDevice>(detail::QairtTypeAdmin::getHandle(configuration));
  }

  /**
   * @brief Hook invoked after a device has been created.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterCreateDevice
   */
  void afterCreateDevice() {
    callAndConfirm<&interface_type::afterCreateDevice>();
  }

  /**
   * @brief Hook invoked before a device is freed.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeFreeDevice
   */
  void beforeFreeDevice() {
    callAndConfirm<&interface_type::beforeFreeDevice>();
  }

  /**
   * @brief Hook invoked after a device has been freed.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterFreeDevice
   */
  void afterFreeDevice() {
    callAndConfirm<&interface_type::afterFreeDevice>();
  }

  /**
   * @brief Hook invoked before a context is activated, allowing this extension to inspect or
   *        modify the activation configuration before it takes effect.
   *
   * @param[in] configuration  The configuration about to be applied during activation.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeActivateContext
   */
  void beforeActivateContext(qairt::ContextConfiguration& configuration) {
    callAndConfirm<&interface_type::beforeActivateContext>(detail::QairtTypeAdmin::getHandle(configuration));
  }

  /**
   * @brief Hook invoked after a context has been activated.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterActivateContext
   */
  void afterActivateContext() {
    callAndConfirm<&interface_type::afterActivateContext>();
  }

  /**
   * @brief Hook invoked before a context is deactivated, allowing this extension to inspect or
   *        modify the deactivation configuration before it takes effect.
   *
   * @param[in] configuration  The configuration about to be applied during deactivation.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeDeactivateContext
   */
  void beforeDeactivateContext(qairt::ContextConfiguration& configuration) {
    callAndConfirm<&interface_type::beforeDeactivateContext>(detail::QairtTypeAdmin::getHandle(configuration));
  }

  /**
   * @brief Hook invoked after a context has been deactivated.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterDeactivateContext
   */
  void afterDeactivateContext() {
    callAndConfirm<&interface_type::afterDeactivateContext>();
  }

  /**
   * @brief Create a backend extensions allocator.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_getAllocator
   */
  qairt::BackendExtensionsAllocator getAllocator() {
      QairtBackendExt_Allocator_Handle_t allocator{};
    callAndConfirm<&interface_type::getAllocator>(&allocator);
    return constructNonOwning<BackendExtensionsAllocator>(allocator);
  }

  /**
   * @brief Hook invoked before a binary section is applied to a context.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_beforeContextApplyBinarySection
   */
  void beforeContextApplyBinarySection() {
    callAndConfirm<&interface_type::beforeContextApplyBinarySection>();
  }

  /**
   * @brief Hook invoked after a binary section has been applied to a context.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_afterContextApplyBinarySection
   */
  void afterContextApplyBinarySection() {
    callAndConfirm<&interface_type::afterContextApplyBinarySection>();
  }

 private:
  void prepareToCross() const {}
  void updateAfterCross() const {}
  explicit BackendExtensions(const std::shared_ptr<BackendExtensionsApiTable>& apiTable)
      : ApiType(apiTable) {
    construct();
  }
};

}  // namespace qairt
