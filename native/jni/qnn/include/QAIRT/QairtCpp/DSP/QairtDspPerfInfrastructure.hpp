//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================
#pragma once

#include "../QairtDevice.hpp"
#include "DSP/QairtDspPerfInfrastructure.h"
#include "QairtDsp.hpp"

namespace qairt {

//=============================================================================
// Enums
//=============================================================================

QAIRT_CPPAPI_DEFINE_ENUM(DspVoltageCorner,
                         QairtDspPerfInfrastructure_VoltageCorner_t,
                         Disable  = QAIRT_DSP_DCVS_VOLTAGE_CORNER_DISABLE,
                         Min      = QAIRT_DSP_DCVS_VOLTAGE_VCORNER_MIN_VOLTAGE_CORNER,
                         Svs2     = QAIRT_DSP_DCVS_VOLTAGE_VCORNER_SVS2,
                         Svs      = QAIRT_DSP_DCVS_VOLTAGE_VCORNER_SVS,
                         SvsPlus  = QAIRT_DSP_DCVS_VOLTAGE_VCORNER_SVS_PLUS,
                         Nom      = QAIRT_DSP_DCVS_VOLTAGE_VCORNER_NOM,
                         NomPlus  = QAIRT_DSP_DCVS_VOLTAGE_VCORNER_NOM_PLUS,
                         Turbo    = QAIRT_DSP_DCVS_VOLTAGE_VCORNER_TURBO,
                         TurboPlus = QAIRT_DSP_DCVS_VOLTAGE_VCORNER_TURBO_PLUS,
                         Max      = QAIRT_DSP_DCVS_VOLTAGE_VCORNER_MAX_VOLTAGE_CORNER,
                         Unknown  = QAIRT_DSP_DCVS_VOLTAGE_VCORNER_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(DspPowerMode,
                         QairtDspPerfInfrastructure_PowerMode_t,
                         AdjustUpDown         = QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_ADJUST_UP_DOWN,
                         AdjustOnlyUp         = QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_ADJUST_ONLY_UP,
                         PowerSaver           = QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_POWER_SAVER_MODE,
                         PowerSaverAggressive =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_POWER_SAVER_AGGRESSIVE_MODE,
                         Performance = QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_PERFORMANCE_MODE,
                         DutyCycle   = QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_DUTY_CYCLE_MODE,
                         Unknown     = QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(DspPowerConfigOption,
                         QairtDspPerfInfrastructure_PowerConfigOption_t,
                         DcvsEnable = QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_ENABLE,
                         SleepLatency =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_SLEEP_LATENCY,
                         SleepDisable =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_SLEEP_DISABLE,
                         DcvsPowerMode =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_POWER_MODE,
                         DcvsVoltageCorner =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_VOLTAGE_CORNER,
                         BusVoltageCorner =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_BUS_VOLTAGE_CORNER,
                         CoreVoltageCorner =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_CORE_VOLTAGE_CORNER,
                         RpcControlLatency =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_CONTROL_LATENCY,
                         RpcPollingTime =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_POLLING_TIME,
                         HmxTimeoutIntervalUs =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_HMX_TIMEOUT_INTERVAL_US,
                         Unknown = QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(DspMemoryConfigOption,
                         QairtDspPerfInfrastructure_MemoryConfigOption_t,
                         GrowSize = QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_GROW_SIZE,
                         VtcmSize = QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_VTCM_SIZE,
                         VtcmUsageFactor =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_VTCM_USAGE_FACTOR,
                         Unknown = QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(DspVtcmUsageFactor,
                         QairtDspPerfInfrastructure_VtcmUsageFactor_t,
                         Full    = QAIRT_DSP_PERF_INFRASTRUCTURE_VTCM_USE_FULL,
                         Min     = QAIRT_DSP_PERF_INFRASTRUCTURE_VTCM_USE_MIN,
                         Unknown = QAIRT_DSP_PERF_INFRASTRUCTURE_VTCM_USE_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(DspThreadConfigOption,
                         QairtDspPerfInfrastructure_ThreadConfigOption_t,
                         NumberOfHvxThreads =
                             QAIRT_DSP_PERF_INFRASTRUCTURE_THREAD_CONFIGOPTION_NUMBER_OF_HVX_THREADS,
                         Unknown = QAIRT_DSP_PERF_INFRASTRUCTURE_THREAD_CONFIGOPTION_UNKNOWN)

//=============================================================================
// Grouped return-value structs (multi-out getters)
//=============================================================================

struct DspVoltageCorners {
  DspVoltageCorner min{DspVoltageCorner::Unknown};
  DspVoltageCorner target{DspVoltageCorner::Unknown};
  DspVoltageCorner max{DspVoltageCorner::Unknown};
};

//=============================================================================
// Power configuration (one option active at a time)
//=============================================================================

class DspPowerConfig : public DspApiType<DspPowerConfig, QairtDspPerfPowerConfigV1_t> {
  template <typename>
  friend class BackendExtension;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using base_type::base_type;
  DspPowerConfig()                                   = default;
  DspPowerConfig(const DspPowerConfig&)              = delete;
  DspPowerConfig& operator=(const DspPowerConfig&)   = delete;
  DspPowerConfig(DspPowerConfig&&) noexcept          = default;
  DspPowerConfig& operator=(DspPowerConfig&&) noexcept = default;

  DspPowerConfigOption getOption() const {
    return static_cast<DspPowerConfigOption>(callGetter<&interface_type::getOption>());
  }

  void setDcvsEnable(bool enable) {
    callSetter<&interface_type::setDcvsEnable>(detail::convertBool(enable));
  }
  bool getDcvsEnable() const {
    return detail::resolveBool(callGetter<&interface_type::getDcvsEnable>());
  }

  void setSleepLatencyUs(uint32_t latencyUs) {
    callSetter<&interface_type::setSleepLatencyUs>(latencyUs);
  }
  uint32_t getSleepLatencyUs() const { return callGetter<&interface_type::getSleepLatencyUs>(); }

  void setSleepDisable(bool disable) {
    callSetter<&interface_type::setSleepDisable>(detail::convertBool(disable));
  }
  bool getSleepDisable() const {
    return detail::resolveBool(callGetter<&interface_type::getSleepDisable>());
  }

  void setDcvsPowerMode(DspPowerMode mode) {
    callSetter<&interface_type::setDcvsPowerMode>(
        static_cast<QairtDspPerfInfrastructure_PowerMode_t>(mode));
  }
  DspPowerMode getDcvsPowerMode() const {
    return static_cast<DspPowerMode>(callGetter<&interface_type::getDcvsPowerMode>());
  }

  void setDcvsVoltageCorners(DspVoltageCorner min, DspVoltageCorner target, DspVoltageCorner max) {
    callAndConfirm<&interface_type::setDcvsVoltageCorners>(
        static_cast<QairtDspPerfInfrastructure_VoltageCorner_t>(min),
        static_cast<QairtDspPerfInfrastructure_VoltageCorner_t>(target),
        static_cast<QairtDspPerfInfrastructure_VoltageCorner_t>(max));
  }
  DspVoltageCorners getDcvsVoltageCorners() const {
    QairtDspPerfInfrastructure_VoltageCorner_t min{}, target{}, max{};
    callAndConfirm<&interface_type::getDcvsVoltageCorners>(&min, &target, &max);
    return {static_cast<DspVoltageCorner>(min), static_cast<DspVoltageCorner>(target),
            static_cast<DspVoltageCorner>(max)};
  }

  void setBusVoltageCorners(DspVoltageCorner min, DspVoltageCorner target, DspVoltageCorner max) {
    callAndConfirm<&interface_type::setBusVoltageCorners>(
        static_cast<QairtDspPerfInfrastructure_VoltageCorner_t>(min),
        static_cast<QairtDspPerfInfrastructure_VoltageCorner_t>(target),
        static_cast<QairtDspPerfInfrastructure_VoltageCorner_t>(max));
  }
  DspVoltageCorners getBusVoltageCorners() const {
    QairtDspPerfInfrastructure_VoltageCorner_t min{}, target{}, max{};
    callAndConfirm<&interface_type::getBusVoltageCorners>(&min, &target, &max);
    return {static_cast<DspVoltageCorner>(min), static_cast<DspVoltageCorner>(target),
            static_cast<DspVoltageCorner>(max)};
  }

  void setCoreVoltageCorners(DspVoltageCorner min, DspVoltageCorner target, DspVoltageCorner max) {
    callAndConfirm<&interface_type::setCoreVoltageCorners>(
        static_cast<QairtDspPerfInfrastructure_VoltageCorner_t>(min),
        static_cast<QairtDspPerfInfrastructure_VoltageCorner_t>(target),
        static_cast<QairtDspPerfInfrastructure_VoltageCorner_t>(max));
  }
  DspVoltageCorners getCoreVoltageCorners() const {
    QairtDspPerfInfrastructure_VoltageCorner_t min{}, target{}, max{};
    callAndConfirm<&interface_type::getCoreVoltageCorners>(&min, &target, &max);
    return {static_cast<DspVoltageCorner>(min), static_cast<DspVoltageCorner>(target),
            static_cast<DspVoltageCorner>(max)};
  }

  void setRpcControlLatencyUs(uint32_t latencyUs) {
    callSetter<&interface_type::setRpcControlLatencyUs>(latencyUs);
  }
  uint32_t getRpcControlLatencyUs() const {
    return callGetter<&interface_type::getRpcControlLatencyUs>();
  }

  void setRpcPollingTimeUs(uint32_t pollingTimeUs) {
    callSetter<&interface_type::setRpcPollingTimeUs>(pollingTimeUs);
  }
  uint32_t getRpcPollingTimeUs() const {
    return callGetter<&interface_type::getRpcPollingTimeUs>();
  }

  void setHmxTimeoutIntervalUs(uint32_t timeoutUs) {
    callSetter<&interface_type::setHmxTimeoutIntervalUs>(timeoutUs);
  }
  uint32_t getHmxTimeoutIntervalUs() const {
    return callGetter<&interface_type::getHmxTimeoutIntervalUs>();
  }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) {
      throw std::runtime_error("Failed to lock apiTable");
    }
    return locked;
  }

  explicit DspPowerConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct DspPowerConfig");
    }
  }

  // Non-owning constructor: wraps an existing entry handle owned by a
  // DspPowerConfigList. The destructor will NOT free the handle.
  DspPowerConfig(const std::shared_ptr<api_table_type>& apiTable,
                 QairtDspPerfPowerConfigHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

//=============================================================================
// Power configuration list
//=============================================================================

class DspPowerConfigList : public DspApiType<DspPowerConfigList, QairtDspPerfPowerConfigListV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  DspPowerConfigList()                                       = default;
  DspPowerConfigList(const DspPowerConfigList&)              = delete;
  DspPowerConfigList& operator=(const DspPowerConfigList&)   = delete;
  DspPowerConfigList(DspPowerConfigList&&) noexcept          = default;
  DspPowerConfigList& operator=(DspPowerConfigList&&) noexcept = default;

  /// Appends a copy of powerConfig's current value; the caller may free or
  /// reuse powerConfig afterward.
  void addPowerConfig(const DspPowerConfig& powerConfig) {
    callAndConfirm<&interface_type::addPowerConfig>(getHandle(powerConfig));
  }

  uint32_t getNumPowerConfigs() const { return callGetter<&interface_type::getNumPowerConfigs>(); }

  /// Returns a non-owning view of the power config at index; do not free it.
  /// Valid until the list is freed or cleared.
  DspPowerConfig getPowerConfigAt(uint32_t index) const {
    QairtDspPerfPowerConfigHandle_t handle{};
    callAndConfirm<&interface_type::getPowerConfigAt>(index, &handle);
    return create<DspPowerConfig>(handle);
  }

  void clear() { callAndConfirm<&interface_type::clear>(); }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) {
      throw std::runtime_error("Failed to lock apiTable");
    }
    return locked;
  }

  explicit DspPowerConfigList(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct DspPowerConfigList");
    }
  }
};

//=============================================================================
// Memory configuration (one option active at a time)
//=============================================================================

class DspMemoryConfig : public DspApiType<DspMemoryConfig, QairtDspPerfMemoryConfigV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  DspMemoryConfig()                                    = default;
  DspMemoryConfig(const DspMemoryConfig&)              = delete;
  DspMemoryConfig& operator=(const DspMemoryConfig&)   = delete;
  DspMemoryConfig(DspMemoryConfig&&) noexcept          = default;
  DspMemoryConfig& operator=(DspMemoryConfig&&) noexcept = default;

  DspMemoryConfigOption getOption() const {
    return static_cast<DspMemoryConfigOption>(callGetter<&interface_type::getOption>());
  }

  void setGrowSizeBytes(uint32_t bytes) { callSetter<&interface_type::setGrowSizeBytes>(bytes); }
  uint32_t getGrowSizeBytes() const { return callGetter<&interface_type::getGrowSizeBytes>(); }

  void setVtcmSizeMB(uint32_t sizeMB) { callSetter<&interface_type::setVtcmSizeMB>(sizeMB); }
  uint32_t getVtcmSizeMB() const { return callGetter<&interface_type::getVtcmSizeMB>(); }

  void setVtcmUsageFactor(DspVtcmUsageFactor usage) {
    callSetter<&interface_type::setVtcmUsageFactor>(
        static_cast<QairtDspPerfInfrastructure_VtcmUsageFactor_t>(usage));
  }
  DspVtcmUsageFactor getVtcmUsageFactor() const {
    return static_cast<DspVtcmUsageFactor>(callGetter<&interface_type::getVtcmUsageFactor>());
  }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) {
      throw std::runtime_error("Failed to lock apiTable");
    }
    return locked;
  }

  explicit DspMemoryConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct DspMemoryConfig");
    }
  }
};

//=============================================================================
// Memory configuration list
//=============================================================================

class DspMemoryConfigList
    : public DspApiType<DspMemoryConfigList, QairtDspPerfMemoryConfigListV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  DspMemoryConfigList()                                        = default;
  DspMemoryConfigList(const DspMemoryConfigList&)              = delete;
  DspMemoryConfigList& operator=(const DspMemoryConfigList&)   = delete;
  DspMemoryConfigList(DspMemoryConfigList&&) noexcept          = default;
  DspMemoryConfigList& operator=(DspMemoryConfigList&&) noexcept = default;

  void addMemoryConfig(const DspMemoryConfig& memoryConfig) {
    callAndConfirm<&interface_type::addMemoryConfig>(getHandle(memoryConfig));
  }

  uint32_t getNumMemoryConfigs() const {
    return callGetter<&interface_type::getNumMemoryConfigs>();
  }

  void clear() { callAndConfirm<&interface_type::clear>(); }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) {
      throw std::runtime_error("Failed to lock apiTable");
    }
    return locked;
  }

  explicit DspMemoryConfigList(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct DspMemoryConfigList");
    }
  }
};

//=============================================================================
// Thread configuration (one option active at a time)
//=============================================================================

class DspThreadConfig : public DspApiType<DspThreadConfig, QairtDspPerfThreadConfigV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  DspThreadConfig()                                    = default;
  DspThreadConfig(const DspThreadConfig&)              = delete;
  DspThreadConfig& operator=(const DspThreadConfig&)   = delete;
  DspThreadConfig(DspThreadConfig&&) noexcept          = default;
  DspThreadConfig& operator=(DspThreadConfig&&) noexcept = default;

  DspThreadConfigOption getOption() const {
    return static_cast<DspThreadConfigOption>(callGetter<&interface_type::getOption>());
  }

  void setNumHvxThreads(uint32_t numThreads) {
    callSetter<&interface_type::setNumHvxThreads>(numThreads);
  }
  uint32_t getNumHvxThreads() const { return callGetter<&interface_type::getNumHvxThreads>(); }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) {
      throw std::runtime_error("Failed to lock apiTable");
    }
    return locked;
  }

  explicit DspThreadConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct DspThreadConfig");
    }
  }
};

//=============================================================================
// Thread configuration list
//=============================================================================

class DspThreadConfigList
    : public DspApiType<DspThreadConfigList, QairtDspPerfThreadConfigListV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  DspThreadConfigList()                                        = default;
  DspThreadConfigList(const DspThreadConfigList&)              = delete;
  DspThreadConfigList& operator=(const DspThreadConfigList&)   = delete;
  DspThreadConfigList(DspThreadConfigList&&) noexcept          = default;
  DspThreadConfigList& operator=(DspThreadConfigList&&) noexcept = default;

  void addThreadConfig(const DspThreadConfig& threadConfig) {
    callAndConfirm<&interface_type::addThreadConfig>(getHandle(threadConfig));
  }

  uint32_t getNumThreadConfigs() const {
    return callGetter<&interface_type::getNumThreadConfigs>();
  }

  void clear() { callAndConfirm<&interface_type::clear>(); }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) {
      throw std::runtime_error("Failed to lock apiTable");
    }
    return locked;
  }

  explicit DspThreadConfigList(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct DspThreadConfigList");
    }
  }
};

//=============================================================================
// DspPerfInfrastructure
//=============================================================================

/**
 * @brief C++ wrapper for the QAIRT DSP perf infrastructure.
 *
 * Obtain via qairt::getDspPerfInfrastructure(extension, deviceInfrastructure).
 * Owns the managed perf-infrastructure handle and frees it on destruction; the
 * backend infrastructure it references is not freed.
 */
class DspPerfInfrastructure
    : public DspApiType<DspPerfInfrastructure, QairtDspPerfInfrastructureV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;

  DspPerfInfrastructure()                                        = default;
  DspPerfInfrastructure(const DspPerfInfrastructure&)            = delete;
  DspPerfInfrastructure& operator=(const DspPerfInfrastructure&) = delete;
  DspPerfInfrastructure(DspPerfInfrastructure&&) noexcept        = default;
  DspPerfInfrastructure& operator=(DspPerfInfrastructure&&) noexcept = default;

  /// Create a power configuration id.
  uint32_t createPowerConfigId() {
    uint32_t id = 0;
    callAndConfirm<&interface_type::createPowerConfigId>(&id);
    return id;
  }

  /// Destroy a previously created power configuration id.
  void destroyPowerConfigId(uint32_t powerConfigId) {
    callAndConfirm<&interface_type::destroyPowerConfigId>(powerConfigId);
  }

  /// Apply the power configuration list under the given power config id.
  void setPowerConfig(uint32_t powerConfigId, const DspPowerConfigList& configs) {
    callAndConfirm<&interface_type::setPowerConfig>(powerConfigId, getHandle(configs));
  }

  /// Apply the memory configuration list.
  void setMemoryConfig(const DspMemoryConfigList& configs) {
    callAndConfirm<&interface_type::setMemoryConfig>(getHandle(configs));
  }

  /// Apply the thread configuration list. Must be called before context/graph
  /// creation.
  void setThreadConfig(const DspThreadConfigList& configs) {
    callAndConfirm<&interface_type::setThreadConfig>(getHandle(configs));
  }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) {
      throw std::runtime_error("Failed to lock apiTable");
    }
    return locked;
  }

  // Resolve the perf infrastructure from a device infrastructure handle. The
  // interface's `free` member drives ApiType RAII on destruction.
  DspPerfInfrastructure(const std::weak_ptr<api_table_type>& apiTable,
                        const DeviceInfrastructure& deviceInfra)
      : base_type(lockOrThrow(apiTable)) {
    construct<&interface_type::getPerfInfrastructure>(deviceInfra.getHandle());
    if (!getHandle()) {
      throw std::runtime_error("Failed to resolve DspPerfInfrastructure");
    }
  }
};

/**
 * @brief Resolve the DSP perf infrastructure from a device infrastructure.
 *
 * Convenience free function so callers do not construct DspPerfInfrastructure
 * directly.
 *
 * @throws qairt::Exception if the backend does not expose a perf infrastructure.
 */
inline DspPerfInfrastructure getDspPerfInfrastructure(DspBackendExtension& extension,
                                                      const DeviceInfrastructure& deviceInfra) {
  return extension.make<DspPerfInfrastructure>(deviceInfra);
}

}  // namespace qairt
