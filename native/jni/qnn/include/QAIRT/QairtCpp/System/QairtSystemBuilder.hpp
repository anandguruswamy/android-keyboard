//==============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include "../QairtApi.hpp"
#include "../QairtLog.hpp"
#include "../QairtProfile.hpp"
#include "QairtSystemDlc.hpp"
#include "Qairt.hpp"
#include "QairtSystem/QairtSystemBuilder.h"

namespace qairt {

/**
 * @brief Lightweight op-index range for setBackendPartition().
 *
 * Aggregate-initializable — users write {start, end} directly:
 * @code
 *   builder.setBackendPartition(BackendType::Htp, {0, 100}, QAIRT_PRIORITY_HIGH, PerfProfile::Burst);
 * @endcode
 */
struct OpRange {
    uint32_t start;
    uint32_t end;
};

/**
 * @brief Enum wrapper for backend type
 *
 * @details
 * | Enumerator | Description                                              |
 * |------------|----------------------------------------------------------|
 * | `Cpu`      | CPU backend. Supported in QAIRT.                         |
 * | `Gpu`      | GPU backend. Supported in QAIRT.                         |
 * | `Dsp`      | DSP backend (HexNN v2). Supported in QAIRT.              |
 * | `Htp`      | HTP backend (HexNN v3). Supported in QAIRT.              |
 * | `Hta`      | HTA backend. Not supported in QAIRT.                     |
 * | `Aip`      | AIP backend. Not supported in QAIRT.                     |
 * | `Lpai`     | LPAI/EAI backend. Not Supported in QAIRT.                |
 * | `Eai`      | Alias for `Lpai`. Not Supported in QAIRT.                |
 */
QAIRT_CPPAPI_DEFINE_ENUM(BackendType,
                         QairtSystem_BackendType_t,
                         Cpu = QAIRT_BACKEND_TYPE_CPU,
                         Gpu = QAIRT_BACKEND_TYPE_GPU,
                         Dsp = QAIRT_BACKEND_TYPE_DSP,
                         Htp = QAIRT_BACKEND_TYPE_HTP,
                         Hta = QAIRT_BACKEND_TYPE_HTA,
                         Aip = QAIRT_BACKEND_TYPE_AIP,
                         Lpai = QAIRT_BACKEND_TYPE_LPAI,
                         Eai = QAIRT_BACKEND_TYPE_EAI)

/**
 * @brief Enum wrapper for performance profile
 *
 * @details
 * | Enumerator                    | Description                                               |
 * |-------------------------------|-----------------------------------------------------------|
 * | `Default`                     | Backend default behavior.                                 |
 * | `LowBalanced`                 | Low-power balanced mode.                                  |
 * | `Balanced`                    | Balanced power and performance.                           |
 * | `HighPerformance`             | Maximum throughput.                                       |
 * | `SustainedHighPerformance`    | Sustained maximum throughput over extended runs.          |
 * | `Burst`                       | Short burst at maximum performance.                       |
 * | `ExtremePowerSaver`           | Extreme power reduction.                                  |
 * | `LowPowerSaver`               | Low power saving mode.                                    |
 * | `PowerSaver`                  | Standard power saving mode.                               |
 * | `HighPowerSaver`              | High power saving mode.                                   |
 * | `SystemSettings`              | Defer to OS or system-level settings.                     |
 * | `NoUserInput`                 | No preference specified; runtime chooses.                 |
 */
QAIRT_CPPAPI_DEFINE_ENUM(PerfProfile,
                         QairtSystem_PerfProfile_t,
                         Default = QAIRT_PERF_PROFILE_DEFAULT,
                         LowBalanced = QAIRT_PERF_PROFILE_LOW_BALANCED,
                         Balanced = QAIRT_PERF_PROFILE_BALANCED,
                         HighPerformance = QAIRT_PERF_PROFILE_HIGH_PERFORMANCE,
                         SustainedHighPerformance = QAIRT_PERF_PROFILE_SUSTAINED_HIGH_PERFORMANCE,
                         Burst = QAIRT_PERF_PROFILE_BURST,
                         ExtremePowerSaver = QAIRT_PERF_PROFILE_EXTREME_POWER_SAVER,
                         LowPowerSaver = QAIRT_PERF_PROFILE_LOW_POWER_SAVER,
                         PowerSaver = QAIRT_PERF_PROFILE_POWER_SAVER,
                         HighPowerSaver = QAIRT_PERF_PROFILE_HIGH_POWER_SAVER,
                         SystemSettings = QAIRT_PERF_PROFILE_SYSTEM_SETTINGS,
                         NoUserInput = QAIRT_PERF_PROFILE_NO_USER_INPUT)

/**
 * @brief Enum wrapper for backend-sharing instance design.
 *
 * @details
 * | Enumerator    | Description                                                        |
 * |---------------|--------------------------------------------------------------------|
 * | `GateKeeping` | One shared backend/device/log per backend type per process (default, SNPE-style). |
 * | `Passthrough` | Each Qairt instance owns a fully private backend.                  |
 */
QAIRT_CPPAPI_DEFINE_ENUM(InstanceDesign,
                         QairtSystem_InstanceDesign_t,
                         GateKeeping = QAIRT_INSTANCE_DESIGN_GATE_KEEPING,
                         Passthrough = QAIRT_INSTANCE_DESIGN_PASSTHROUGH)

/**
 * @brief C++ wrapper for the QAIRT System Builder.
 *
 * QairtSystemBuilder follows the builder pattern for configuring and
 * constructing a Qairt High Level Runtime instance. All configuration is
 * accumulated via fluent setter calls and then materialised in a single
 * build() call which performs the full backend + context + graph
 * initialisation sequence.
 *
 * ## Lifecycle
 * Instances must be created through SystemApi::make<QairtSystemBuilder>()
 * using the same SystemApi that owns the SystemDlc and SystemLog objects.
 * Using a builder created from a different SystemApi than the objects
 * passed to it (e.g. setDlc) results in undefined behaviour.
 * Corresponds to QairtSystemBuilder_create/_free in the C API;
 * construction/destruction here map to those calls automatically.
 *
 * QairtSystemBuilder is **move-only** (copy constructor deleted).
 *
 * ## Mandatory configuration
 * The following must be set before calling build():
 * - setDlc()          — the DLC model to load
 * - setBackendType()  — which hardware backend to use
 *
 * All other setters are optional and have documented defaults.
 *
 * ## Example
 * @code
 *   auto systemApi = std::make_shared<qairt::SystemApi>("libQairtSystem.so");
 *   auto systemLog = systemApi->makeShared<qairt::SystemLog>(nullptr, logLevel);
 *   auto systemDlc = systemApi->makeShared<qairt::SystemDlc>(
 *       qairt::detail::ApiTypeRef<qairt::SystemLog>(*systemLog), "model.dlc");
 *
 *   auto builder = systemApi->make<qairt::QairtSystemBuilder>();
 *   builder.setDlc(systemDlc)
 *          .setBackendType(qairt::BackendType::Cpu)
 *          .setLogLevel(qairt::LogLevel::Warn)
 *          .setPerfProfile(qairt::PerfProfile::HighPerformance);
 *
 *   // Optional: validate configuration before committing
 *   std::string err;
 *   if (!builder.validate(err)) {
 *     std::cerr << "Validation failed: " << err << "\n";
 *     return;
 *   }
 *
 *   auto qairt = builder.build();  // fully initialised, ready for inference
 * @endcode
 */
class QairtSystemBuilder : public ApiType<QairtSystemBuilder, QairtSystemBuilder_V1_t, SystemApiTable> {
  friend SystemApi;
  friend Api;

 public:
  using base_type::base_type;
  using base_type::operator=;

  QairtSystemBuilder() noexcept                                  = default;
  QairtSystemBuilder(const QairtSystemBuilder&)                  = delete;
  QairtSystemBuilder(QairtSystemBuilder&&) noexcept              = default;
  QairtSystemBuilder& operator=(const QairtSystemBuilder&)       = delete;
  QairtSystemBuilder& operator=(QairtSystemBuilder&&) noexcept   = default;

  // ============================================================================
  // Core Configuration
  // ============================================================================

  /**
   * @brief Set the DLC model to load. **Required before build().**
   *
   * Registers the SystemDlc that contains the compiled model. The builder
   * stores a non-owning raw pointer to the underlying SystemDlc object; the
   * caller's shared_ptr must remain alive until build() has returned.
   *
   * @param dlc  Shared pointer to a SystemDlc created via the same SystemApi
   *             that was used to create this builder. Passing a SystemDlc from
   *             a different SystemApi instance results in undefined behaviour.
   * @return     Reference to this builder for method chaining.
   * @throws qairt::Exception  If @p dlc is null or the CAPI call fails.
   *
   * @note The caller retains ownership of the SystemDlc. The builder (and the
   *       resulting Qairt instance) hold a non-owning reference internally.
   *       The SystemDlc must outlive both the builder and the Qairt object.
   *
   * Example:
   * @code
   *   auto systemDlc = systemApi->makeShared<qairt::SystemDlc>(
   *       qairt::detail::ApiTypeRef<qairt::SystemLog>(*systemLog), "model.dlc");
   *   builder.setDlc(systemDlc);
   * @endcode
   */
  QairtSystemBuilder& setDlc(std::shared_ptr<SystemDlc> dlc) {
    QairtSystemDlc_Handle_t dlcHandle{};
    if (dlc) {
      dlcHandle = detail::QairtTypeAdmin::getHandle(*dlc);
    }
    callAndConfirm<&QairtSystemBuilder_V1_t::setDlc>(dlcHandle);
    return *this;
  }

  // ============================================================================
  // Logging and Profiling Configuration
  // ============================================================================

  /**
   * @brief Set the logging verbosity level. Default: Error.
   *
   * Controls which log messages are emitted by the runtime. Higher levels
   * produce more output and may impact performance.
   *
   * @param level  One of:
   *               - LogLevel::Error   — errors only (default)
   *               - LogLevel::Warn    — errors and warnings
   *               - LogLevel::Info    — general operational messages
   *               - LogLevel::Verbose — detailed trace messages
   *               - LogLevel::Debug   — full debug output
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   *
   * Example:
   * @code
   *   builder.setLogLevel(qairt::LogLevel::Warn);
   * @endcode
   */
  QairtSystemBuilder& setLogLevel(LogLevel level) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setLogLevel>(static_cast<QairtLog_Level_t>(level));
    return *this;
  }

  /**
   * @brief Set the profiling detail level. Default: OFF (0).
   *
   * Controls how much execution timing and performance data is collected
   * during inference. Higher levels produce richer data at the cost of
   * additional overhead.
   *
   * @param level  One of:
   *               - 0                           — profiling disabled (default)
   *               - QAIRT_PROFILE_LEVEL_BASIC    — op-level latency
   *               - QAIRT_PROFILE_LEVEL_DETAILED — per-op detailed breakdown
   *               - QAIRT_PROFILE_LEVEL_BACKEND  — backend-specific counters (≥1000)
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   *
   * @note Takes QairtProfile_Level_t (C enum) because no C++ wrapper exists
   *       for this type yet.
   *
   * Example:
   * @code
   *   builder.setProfilingLevel(QAIRT_PROFILE_LEVEL_BASIC);
   * @endcode
   */
  QairtSystemBuilder& setProfilingLevel(QairtProfile_Level_t level) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setProfilingLevel>(level);
    return *this;
  }

  /**
   * @brief Set the profiling detail level, and the directory profiling outputs
   *        are written to.
   *
   * Equivalent to setProfilingLevel(), plus specifying where the profiling log
   * produced by that level (and any options set via setProfilingOption()) is
   * written.
   *
   * @param level      One of:
   *                   - 0                           — profiling disabled (default)
   *                   - QAIRT_PROFILE_LEVEL_BASIC    — op-level latency
   *                   - QAIRT_PROFILE_LEVEL_DETAILED — per-op detailed breakdown
   *                   - QAIRT_PROFILE_LEVEL_BACKEND  — backend-specific counters (≥1000)
   * @param outputDir  Directory to be used for profiling outputs.
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   *
   * @note Takes QairtProfile_Level_t (C enum) because no C++ wrapper exists
   *       for this type yet.
   *
   * Example:
   * @code
   *   builder.setProfilingLevelWithOutputDir(QAIRT_PROFILE_LEVEL_BASIC, "./profilingoutputs");
   * @endcode
   */
  QairtSystemBuilder& setProfilingLevelWithOutputDir(QairtProfile_Level_t level, const std::string& outputDir) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setProfilingLevelWithOutputDir>(level, outputDir.c_str());
    return *this;
  }

  /**
   * @brief Enable optional profiling features. Default: NONE (0).
   *
   * Activates supplementary profiling capabilities on top of the base
   * profiling level set by setProfilingLevel().
   *
   * @param option  Bitfield of profiling options:
   *                - 0 — no additional features (default)
   *                - 1 — OPTRACE: records per-op execution traces
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   *
   * @note Has no effect if setProfilingLevel() is OFF (0).
   * @note Takes uint32_t because no enum wrapper for profiling options exists yet.
   *
   * Example:
   * @code
   *   builder.setProfilingLevel(QAIRT_PROFILE_LEVEL_DETAILED)
   *          .setProfilingOption(1);  // enable OpTrace
   * @endcode
   */
  QairtSystemBuilder& setProfilingOption(uint32_t option) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setProfilingOption>(option);
    return *this;
  }

  /**
   * @brief Set the power/performance trade-off profile. Default: NoUserInput.
   *
   * Selects how aggressively the backend trades power consumption against
   * execution speed. The effective behaviour depends on the backend and
   * target hardware.
   *
   * @param profile  One of the PerfProfile enum values:
   *                 - NoUserInput              — no preference (default)
   *                 - Default                  — backend default
   *                 - Balanced                 — balanced power and performance
   *                 - HighPerformance          — maximum throughput
   *                 - SustainedHighPerformance — sustained high throughput
   *                 - Burst                    — short burst at maximum performance
   *                 - PowerSaver               — reduced power consumption
   *                 - LowPowerSaver / ExtremePowerSaver — progressively lower power
   *                 - SystemSettings           — defer to OS/system settings
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   *
   * Example:
   * @code
   *   builder.setPerfProfile(qairt::PerfProfile::HighPerformance);
   * @endcode
   */
  QairtSystemBuilder& setPerfProfile(PerfProfile profile) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setPerfProfile>(static_cast<QairtSystem_PerfProfile_t>(profile));
    return *this;
  }

  // ============================================================================
  // Platform and Device Configuration
  // ============================================================================
/** @cond INTERNAL */
  /**
   * @brief Set platform options via a semicolon-separated key:value string. Optional.
   *
   * Accepts a semicolon-separated list of key:value pairs. Keys and values are
   * case-sensitive. Parsing is deferred to build(); unknown keys produce
   * a warning and are skipped; a malformed value causes build() to throw.
   *
   * **Graph-level keys** (apply to all graphs):
   *
   * | Key | Value | Description |
   * |-----|-------|-------------|
   * | `htpVtcmSizeMb` | unsigned int | VTCM size in MB for graph compilation |
   * | `numHvxThreads` | unsigned int | Number of HVX threads |
   * | `htpDLBC` | ON / OFF | Deep Learning Buffer Compression |
   * | `htpDLBCWeights` | ON / OFF | DLBC for weight tensors |
   * | `htpCachedWeights` | ON / OFF | Weight pinning in TCM |
   * | `htpSparseWeightsCompression` | ON / OFF | Sparse weight compression |
   * | `htpFp16RelaxedPrecision` | ON / OFF | FP16 relaxed precision mode |
   * | `htpWeightsPacking` | ON / OFF | Weight packing optimization |
   * | `htpAssumeSameQuant` | ON / OFF | Assume same quantization across graphs |
   * | `htpShareIoBuffer` | ON / OFF | Share I/O buffers between graphs |
   * | `htpAdvancedActivationFusion` | ON / OFF | Advanced activation fusion |
   * | `htpHighPrecisionSigmoid` | ON / OFF | High-precision sigmoid |
   * | `htpMonolithicLstm` | ON / OFF | Monolithic LSTM mode |
   * | `htpPrecisionCompensation` | ON / OFF | Precision compensation |
   * | `htpShortDepthConvOnHmxOff` | ON / OFF | Disable short-depth conv on HMX |
   * | `htpFoldReluActivationIntoConvOff` | ON / OFF | Disable ReLU fusion into conv |
   * | `htpVtcmRetention` | ON / OFF | VTCM retention across inferences |
   * | `htpSlcAlloc` | ON / OFF | System Level Cache allocator |
   *
   * **Device-level keys** (apply to device 0):
   *
   * | Key | Value | Description |
   * |-----|-------|-------------|
   * | `pdSession` | unsigned / signed | Process domain session type |
   * | `htpDeviceProfilingLevel` | off / basic / detailed | Device profiling verbosity |
   * | `dspPowerSettingContext` | ON / OFF | Unique power context for device |
   * | `devicePerfProfile` | burst / high_performance / … | Device performance profile |
   * | `dspSleepLatency` | unsigned int (µs) | RPC control latency hint |
   * | `rpcPollTime` | unsigned int (µs) | RPC polling interval (0 = disabled) |
   * | `htpHmxTimeoutUs` | unsigned int (µs) | HMX operation timeout |
   * | `adaptivePollTime` | unsigned int (µs) | Adaptive polling time (0 = disabled) |
   *
   * **Context-level keys** (apply to the HTP context):
   *
   * | Key | Value | Description |
   * |-----|-------|-------------|
   * | `htpWeightSharing` | ON / OFF | Enable weight sharing across graphs |
   * | `htpReferenceWeightSharing` | ON / OFF | Enable reference weight sharing |
   * | `htpFileReadMemoryBudget` | unsigned int (MB) | Memory budget for binary deserialization |
   * | `htpDspMemoryProfiling` | ON / OFF | Enable DSP memory profiling |
   * | `htpIoMemEstimation` | ON / OFF | Enable I/O memory estimation heuristics |
   * | `htpPrepareOnly` | ON / OFF | Prepare-only mode (no execution) |
   * | `htpInitAcceleration` | ON / OFF | Enable accelerated initialization |
   * | `htpSkipBinaryValidation` | ON / OFF | Skip binary section validation |
   * | `htpUseExtendedUdma` | ON / OFF | Enable extended UDMA |
   * | `htpLoraWeightSharing` | ON / OFF | Enable LoRA weight sharing |
   * | `htpLoraWeightSharingRamPreload` | ON / OFF | Enable LoRA weight sharing RAM preload |
   *
   * @param options  Semicolon-separated key:value option string. Empty is a no-op.
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the C API call fails.
   *
   * @note Parsing happens inside build(). A malformed value for a known key
   *       causes build() to fail with an error message. Unknown keys are skipped
   *       with a stderr warning for forward compatibility.
   * @note Settings apply globally (all graphs, device 0) — per-graph or
   *       per-device scoping is not supported.
   *
   * Example:
   * @code
   *   builder.setPlatformOptions("htpDLBC:ON;numHvxThreads:4;rpcPollTime:9999");
   * @endcode
   */
  QairtSystemBuilder& setPlatformOptions(const std::string& options) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setPlatformOptions>(options.c_str());
    return *this;
  }
/** @endcond */
  /**
   * @brief Set device-specific options for multi-device or multi-core targets. Optional.
   *
   * Controls which physical device and/or compute core is used for inference.
   * Only relevant when the backend supports the Device API (i.e.
   * QNN_PROPERTY_GROUP_DEVICE capability is present).
   *
   * @param options  Device selection string in the form "device_id:0;core_id:1".
   *                 Omit to let the runtime choose automatically.
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   *
   * @note Has no effect on backends that do not support the Device API.
   *
   * Example:
   * @code
   *   builder.setDeviceOptions("device_id:0;core_id:1");
   * @endcode
   */
  QairtSystemBuilder& setDeviceOptions(const std::string& options) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setDeviceOptions>(options.c_str());
    return *this;
  }

  /**
   * @brief Register a custom operator package. Optional; may be called multiple times.
   *
   * Adds a shared library that provides custom ops not built into the backend.
   * Each call registers one package; call this multiple times to register
   * several packages. Packages are registered during build() in the order they
   * were added.
   *
   * @param path               Absolute or relative path to the operator package
   *                           shared library (e.g. "/path/to/libMyOps.so").
   * @param interfaceProvider  Name of the interface provider exported by the
   *                           package. Pass an empty string to use the default
   *                           provider exposed by the library.
   * @param target             Backend target the package is compiled for (e.g.
   *                           "CPU", "HTP"). Pass an empty string to use the
   *                           package for all applicable backends.
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If @p path is empty or the CAPI call fails.
   *
   * Example:
   * @code
   *   builder.addOpPackage("/path/to/libCustomOps.so", "CustomProvider", "HTP");
   * @endcode
   */
  QairtSystemBuilder& addOpPackage(const std::string& path,
                                   const std::string& interfaceProvider = "",
                                   const std::string& target = "") {
    const char* providerPtr = interfaceProvider.empty() ? nullptr : interfaceProvider.c_str();
    const char* targetPtr = target.empty() ? nullptr : target.c_str();
    callAndConfirm<&QairtSystemBuilder_V1_t::addOpPackage>(path.c_str(), providerPtr, targetPtr);
    return *this;
  }

  /**
   * @brief Add an operator package to be loaded from an in-memory buffer.
   *
   * Registers an op package supplied as a raw memory buffer rather than a
   * filesystem path. The buffer must remain valid until build() is called.
   * Required for backends without filesystem access (e.g. SecurePD).
   *
   * @param buffer             Pointer to the in-memory op package binary.
   * @param bufferSize         Size of the buffer in bytes.
   * @param interfaceProvider  Name of the interface provider exported by the
   *                           package. Pass an empty string for the default.
   * @param target             Backend target the package is compiled for (e.g.
   *                           "CPU", "HTP"). Pass an empty string for all
   *                           applicable backends.
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If @p buffer is null, @p bufferSize is 0, or the CAPI call fails.
   */
  QairtSystemBuilder& addOpPackageFromBinary(const void* buffer,
                                             uint64_t bufferSize,
                                             const std::string& interfaceProvider = "",
                                             const std::string& target = "") {
    const char* providerPtr = interfaceProvider.empty() ? nullptr : interfaceProvider.c_str();
    const char* targetPtr = target.empty() ? nullptr : target.c_str();
    callAndConfirm<&QairtSystemBuilder_V1_t::addOpPackageFromBinary>(
        buffer, bufferSize, providerPtr, targetPtr);
    return *this;
  }

  /**
   * @brief Set the inference backend. **Required before build().**
   *
   * Unified backend assignment. Both single-backend and multi-backend use cases
   * go through the same code path: each call adds one partition spec.
   *
   * **Overload 1 — catch-all** (all remaining ops on this backend):
   * @code
   *   builder.setBackendType(qairt::BackendType::Cpu);
   *   // equivalent: single-backend, all ops on CPU
   * @endcode
   * **setBackendPartition** — explicit op range (multi-backend partition):
   * @code
   *   builder.setBackendPartition(BackendType::Htp, {0, 100}, QAIRT_PRIORITY_HIGH)
   *          .setBackendType(BackendType::Cpu);  // catch-all for remaining ops
   * @endcode
   */

  /// Catch-all: all ops (or uncovered remaining ops) run on @p backend.
  QairtSystemBuilder& setBackendType(BackendType backend) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setBackendType>(
        static_cast<QairtSystem_BackendType_t>(backend));
    return *this;
  }

  /// Explicit op range: ops [range.start, range.end] (inclusive) run on @p backend.
  QairtSystemBuilder& setBackendPartition(BackendType      backend,
                                           OpRange          range,
                                           Qairt_Priority_t priority = QAIRT_PRIORITY_NORMAL,
                                           PerfProfile      perf     = PerfProfile::NoUserInput) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setBackendPartition>(
        static_cast<QairtSystem_BackendType_t>(backend),
        range.start,
        range.end,
        priority,
        static_cast<QairtSystem_PerfProfile_t>(perf));
    return *this;
  }

  /**
   * @brief Set backends in priority order for automatic op-level partitioning.
   *
   * Ops are assigned to the first backend in the list that supports them.
   * Mutually exclusive with setBackendType() and setBackendPartition().
   *
   * @code
   *   builder.setBackendOrder({BackendType::Htp, BackendType::Cpu});
   * @endcode
   */
  QairtSystemBuilder& setBackendOrder(std::initializer_list<BackendType> backends) {
    std::vector<QairtSystem_BackendType_t> order;
    order.reserve(backends.size());
    for (auto b : backends)
      order.push_back(static_cast<QairtSystem_BackendType_t>(b));
    callAndConfirm<&QairtSystemBuilder_V1_t::setBackendOrder>(
        order.data(), static_cast<uint32_t>(order.size()));
    return *this;
  }

  QairtSystemBuilder& setBackendOrder(const std::vector<BackendType>& backends) {
    std::vector<QairtSystem_BackendType_t> order;
    order.reserve(backends.size());
    for (auto b : backends)
      order.push_back(static_cast<QairtSystem_BackendType_t>(b));
    callAndConfirm<&QairtSystemBuilder_V1_t::setBackendOrder>(
        order.data(), static_cast<uint32_t>(order.size()));
    return *this;
  }

  // ============================================================================
  // Runtime Configuration (Backend Extensions)
  // ============================================================================

  /**
   * @brief Set the scheduling priority for the created context. Default: NORMAL.
   *
   * Controls how the OS and backend scheduler prioritise this context relative
   * to others. Higher priorities may reduce latency but can starve lower-priority
   * workloads.
   *
   * @param priority  One of the Qairt_Priority_t values, from lowest to highest:
   *                  QAIRT_PRIORITY_LOW / QAIRT_PRIORITY_LOWEST →
   *                  QAIRT_PRIORITY_NORMAL_LOW →
   *                  QAIRT_PRIORITY_NORMAL / QAIRT_PRIORITY_DEFAULT (default) →
   *                  QAIRT_PRIORITY_NORMAL_HIGH →
   *                  QAIRT_PRIORITY_HIGH →
   *                  QAIRT_PRIORITY_HIGH_PLUS →
   *                  QAIRT_PRIORITY_CRITICAL →
   *                  QAIRT_PRIORITY_CRITICAL_PLUS / QAIRT_PRIORITY_HIGHEST
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the priority value is invalid or the CAPI call fails.
   *
   * @note Priorities above QAIRT_PRIORITY_LOW may be restricted or ignored on
   *       some platforms depending on process permissions.
   *
   * Example:
   * @code
   *   builder.setPriority(QAIRT_PRIORITY_HIGH);
   * @endcode
   */
  QairtSystemBuilder& setPriority(Qairt_Priority_t priority) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setPriority>(priority);
    return *this;
  }

  /**
   * @brief Restrict context creation to a named subset of graphs. Optional.
   *
   * By default all graphs found in the DLC are loaded into the context.
   * Call this to load only a specific subset, which can reduce memory usage
   * and initialisation time when the DLC contains multiple graphs.
   *
   * @param graphNames  Non-empty vector of graph name strings. Each name must
   *                    exactly match a graph name present in the DLC; a mismatch
   *                    will cause build() to fail.
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If @p graphNames is empty or the CAPI call fails.
   *
   * @note **Not yet effective** — will take effect once backend extensions are
   *       fully implemented for the QAIRT high-level API.
   *
   * Example:
   * @code
   *   builder.setEnabledGraphs({"inception_v3"});
   * @endcode
   */
  QairtSystemBuilder& setEnabledGraphs(const std::vector<std::string>& graphNames) {
    if (graphNames.empty()) {
      throw Exception("Graph names vector cannot be empty", CommonError::InvalidArgument);
    }

    std::vector<const char*> graphNamePtrs;
    graphNamePtrs.reserve(graphNames.size());
    for (const auto& name : graphNames) {
      graphNamePtrs.push_back(name.c_str());
    }

    callAndConfirm<&QairtSystemBuilder_V1_t::setEnabledGraphs>(
        graphNamePtrs.data(), static_cast<uint32_t>(graphNamePtrs.size()));
    return *this;
  }

  /**
   * @brief Hint the maximum memory the context should consume. Default: 0 (unlimited).
   *
   * Provides a soft upper bound on memory usage to the backend. The backend
   * may use this to tune allocation strategies, but is not required to enforce
   * it strictly — actual usage can exceed the hint if the model demands it.
   *
   * @param memoryLimitHint  Maximum suggested memory in bytes.
   *                         0 means no limit (default).
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   *
   * @note This is a hint, not a hard cap. The backend may exceed it.
   * @note **Not yet effective** — will take effect once backend extensions are
   *       fully implemented for the QAIRT high-level API.
   *
   * Example:
   * @code
   *   builder.setMemoryLimitHint(512ULL * 1024 * 1024);  // 512 MB hint
   * @endcode
   */
  QairtSystemBuilder& setMemoryLimitHint(uint64_t memoryLimitHint) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setMemoryLimitHint>(memoryLimitHint);
    return *this;
  }

  /**
   * @brief Request that the context binary be saved to persistent storage. Default: false.
   *
   * When enabled, the compiled context binary may be written to disk after
   * the first build so that subsequent builds can load it directly, skipping
   * the model compilation step and reducing initialisation time.
   *
   * @param isPersistent  true to request persistence, false to keep the
   *                      binary in memory only (default).
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   *
   * @note Support for persistent binaries depends on the backend and platform.
   *       Not all backends implement binary caching.
   * @note **Not yet effective** — will take effect once backend extensions are
   *       fully implemented for the QAIRT high-level API.
   *
   * Example:
   * @code
   *   builder.setIsPersistentBinary(true);
   * @endcode
   */
  QairtSystemBuilder& setIsPersistentBinary(bool isPersistent) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setIsPersistentBinary>(
        isPersistent ? static_cast<uint8_t>(1U) : static_cast<uint8_t>(0U));
    return *this;
  }

  /**
   * @brief Set how strictly the cached binary is checked for compatibility. Default: PERMISSIVE.
   *
   * When a cached context binary is available, this setting controls whether
   * the backend accepts it (PERMISSIVE) or requires an exact match of the
   * current hardware/software configuration (STRICT) before using it.
   *
   * @param compatibilityMode  One of:
   *   - QAIRT_CONTEXT_BINARY_COMPATIBILITY_PERMISSIVE (default) — maximises
   *     cache reuse; accepts binaries compiled on similar but not identical
   *     configurations. May not achieve peak performance on the current HW.
   *   - QAIRT_CONTEXT_BINARY_COMPATIBILITY_STRICT — rejects binaries that do
   *     not exactly match the current configuration; guarantees optimal
   *     performance but may increase cache misses.
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the mode value is invalid or the CAPI call fails.
   *
   * @note Only relevant when setIsPersistentBinary(true) is also set.
   * @note **Not yet effective** — will take effect once backend extensions are
   *       fully implemented for the QAIRT high-level API.
   *
   * Example:
   * @code
   *   builder.setIsPersistentBinary(true)
   *          .setCacheCompatibilityMode(QAIRT_CONTEXT_BINARY_COMPATIBILITY_STRICT);
   * @endcode
   */
  QairtSystemBuilder& setCacheCompatibilityMode(
      QairtContext_BinaryCompatibilityType_t compatibilityMode) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setCacheCompatibilityMode>(compatibilityMode);
    return *this;
  }

  /**
   * @brief Cap the number of profiling events recorded per inference. Default: 0 (unlimited).
   *
   * Limits the profiling event buffer to prevent unbounded memory growth when
   * running many inferences with profiling enabled. Once the cap is reached,
   * the behaviour (discard oldest vs. drop newest) is backend-specific.
   *
   * @param maxEvents  Maximum events to store. 0 means no cap (default).
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   *
   * @note Has no effect unless setProfilingLevel() is set to a non-zero level.
   * @note **Not yet effective** — will take effect once backend extensions are
   *       fully implemented for the QAIRT high-level API.
   *
   * Example:
   * @code
   *   builder.setProfilingLevel(QAIRT_PROFILE_LEVEL_BASIC)
   *          .setProfileMaxEvents(500);
   * @endcode
   */
  QairtSystemBuilder& setProfileMaxEvents(uint64_t maxEvents) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setProfileMaxEvents>(maxEvents);
    return *this;
  }

  // ============================================================================
  /** @brief Enable init cache: after build() compiles the model, embed the
   *         cache record into the source SystemDlc object in memory.
   *         The caller is responsible for saving the DLC to disk. */
  QairtSystemBuilder& enableInitCache() {
    callAndConfirm<&QairtSystemBuilder_V1_t::enableInitCache>();
    return *this;
  }

  /**
   * @brief Select the backend-sharing instance design. Default: GateKeeping.
   *
   * - GateKeeping: one shared backend/device/log per backend type per process. Multiple
   *   Qairt instances of the same backend type reuse a single RuntimeApi (one dlopen /
   *   one QnnBackend_create), matching SNPE's QnnBackendManager gate-keeping.
   * - Passthrough: each Qairt instance owns a fully private backend (legacy behavior).
   *
   * @param design  InstanceDesign::GateKeeping or InstanceDesign::Passthrough.
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   */
  QairtSystemBuilder& setInstanceDesign(InstanceDesign design) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setInstanceDesign>(
        static_cast<QairtSystem_InstanceDesign_t>(design));
    return *this;
  }

  /**
   * @brief Request process-wide deferred backend cleanup (GateKeeping only). Default: false.
   *
   * Building a Qairt with a true request pins all current and future shared backend types until
   * QairtSystemLifecycle_releaseGlobalBackends() is called. False does not disable a mode
   * previously enabled by another Qairt; only the lifecycle release call disables it. Has no
   * effect under Passthrough.
   *
   * @param enabled  true to enable process-wide deferred cleanup; false makes no global change.
   * @return  Reference to this builder for method chaining.
   * @throws qairt::Exception  If the CAPI call fails.
   */
  QairtSystemBuilder& setDeferredBackendCleanup(bool enabled) {
    callAndConfirm<&QairtSystemBuilder_V1_t::setDeferredBackendCleanup>(
        static_cast<uint8_t>(enabled ? 1 : 0));
    return *this;
  }

  // ============================================================================
  // Validation and Building
  // ============================================================================

  /**
   * @brief Set input tensor dimensions for dynamic resizing.
   *        Call before build() to override model input shapes.
   *
   * @param graphName   "" targets the first/only graph
   * @param dimensions  map of tensor name → new shape (as size_t vector)
   */
  QairtSystemBuilder& setInputDimensions(
      const std::string& graphName,
      const std::unordered_map<std::string, std::vector<size_t>>& dimensions) {
    // Flatten the map into C-compatible arrays for the vtable call
    std::vector<const char*>   names;
    std::vector<const size_t*> dimPtrs;
    std::vector<uint32_t>      numDims;
    std::vector<std::vector<size_t>> dimStorage;  // keep data alive through the call

    names.reserve(dimensions.size());
    dimPtrs.reserve(dimensions.size());
    numDims.reserve(dimensions.size());
    dimStorage.reserve(dimensions.size());

    for (const auto& [tensorName, dims] : dimensions) {
      names.push_back(tensorName.c_str());
      dimStorage.push_back(dims);
      dimPtrs.push_back(dimStorage.back().data());
      numDims.push_back(static_cast<uint32_t>(dims.size()));
    }

    callAndConfirm<&QairtSystemBuilder_V1_t::setInputDimensions>(
        graphName.c_str(),
        names.empty()   ? nullptr : names.data(),
        dimPtrs.empty() ? nullptr : dimPtrs.data(),
        numDims.empty() ? nullptr : numDims.data(),
        static_cast<uint32_t>(dimensions.size()));
    return *this;
  }

  /**
   * @brief Check whether the current configuration is ready to build. Non-throwing.
   *
   * Validates the accumulated configuration without performing any
   * initialisation. Call this before build() when you want to surface
   * configuration problems early and handle them without catching exceptions.
   *
   * Currently validates that:
   * - A DLC has been supplied via setDlc()
   * - A backend type has been set via setBackendType()
   *
   * @param[out] errorMsg  Populated with a human-readable description of the
   *                       first validation error found. Unchanged on success.
   * @return  true if the configuration is valid, false otherwise.
   *
   * @note This method does not throw on a validation failure — it returns false
   *       and writes to @p errorMsg instead. It may still throw for unexpected
   *       internal CAPI errors.
   *
   * Example:
   * @code
   *   std::string err;
   *   if (!builder.validate(err)) {
   *     std::cerr << "Config error: " << err << "\n";
   *     return;
   *   }
   *   auto qairt = builder.build();
   * @endcode
   */
  bool validate(std::string& errorMsg) {
    std::array<char, 1024U> buffer{};
    auto status = call<&QairtSystemBuilder_V1_t::validate>(
        buffer.data(), static_cast<uint32_t>(buffer.size()));
    if (status != QAIRT_SUCCESS) {
      errorMsg = buffer.data();  // .data() returns char* — no array-to-pointer decay
      return false;
    }
    return true;
  }

  /**
   * @brief Finalise configuration and construct a ready-to-use Qairt instance.
   *
   * Performs the full one-shot initialisation sequence:
   *   1. Validates the configuration (equivalent to validate())
   *   2. Loads the backend shared library
   *   3. Initialises the backend
   *   4. Registers any operator packages added via addOpPackage()
   *   5. Creates a device handle if the backend supports it
   *   6. Creates a QAIRT context from the DLC
   *   7. Composes and finalises all graphs
   *
   * On success the returned Qairt instance is fully initialised and ready
   * for immediate inference via executeGraph() / execute().
   *
   * @return  A fully initialised Qairt instance (move-only).
   * @throws qairt::Exception  If validation, any initialisation step, or graph
   *                            loading fails. The exception message describes
   *                            which step failed.
   *
   * @note The builder may be destroyed or reconfigured after build() returns —
   *       the resulting Qairt instance is self-contained.
   * @note The SystemDlc passed to setDlc() must remain alive for the entire
   *       lifetime of the returned Qairt instance.
   * @note Qairt is move-only; store the return value with auto, not auto&:
   * @code
   *   auto qairt = builder.build();  // correct
   * @endcode
   *
   * Example:
   * @code
   *   try {
   *     auto qairt = builder.build();
   *     // ready for inference
   *   } catch (const qairt::Exception& e) {
   *     std::cerr << "Build failed: " << e.what() << "\n";
   *   }
   * @endcode
   */
  Qairt build() {
    Qairt_Handle_t qairtHandle{};
    callAndConfirm<&QairtSystemBuilder_V1_t::build>(&qairtHandle);
    return constructNonOwning<Qairt>(qairtHandle);
  }

 private:
  /**
   * @brief Private constructor called by SystemApi::make
   * @param apiTable Shared pointer to SystemApiTable
   */
  explicit QairtSystemBuilder(const std::shared_ptr<SystemApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }

  void prepareToCross() const {}
  void updateAfterCross() const {}
};

}  // namespace qairt
