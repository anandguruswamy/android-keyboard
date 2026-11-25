//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================
#pragma once

#include "../QairtDevice.hpp"
#include "LPAI/QairtLpaiPerfInfrastructure.h"
#include "QairtLpai.hpp"

namespace qairt {

//=============================================================================
// Enums
//=============================================================================

QAIRT_CPPAPI_DEFINE_ENUM(LpaiPerformanceMode,
                         QairtLpaiPerfInfrastructure_PerformanceMode_t,
                         Default = QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_DEFAULT,
                         Burst   = QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_BURST,
                         Turbo   = QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_TURBO,
                         Nominal = QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_NOMINAL,
                         SvsL1   = QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_SVS_L1,
                         Svs     = QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_SVS,
                         LowSvs  = QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_LOWSVS,
                         Unknown = QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(LpaiPerfConfigOption,
                         QairtLpaiPerfInfrastructure_ConfigOption_t,
                         EnpuPowerLevel = QAIRT_LPAI_PERF_CONFIGOPTION_ENPU_POWER_LEVEL,
                         EnpuWithDramPowerLevel =
                             QAIRT_LPAI_PERF_CONFIGOPTION_ENPU_WITH_DRAM_POWER_LEVEL,
                         Unknown = QAIRT_LPAI_PERF_CONFIGOPTION_UNKNOWN)

//=============================================================================
// Grouped return-value structs (multi-out getters)
//=============================================================================

struct LpaiEnpuPowerLevel {
  LpaiPerformanceMode performanceMode{LpaiPerformanceMode::Unknown};
  bool maxDdrBandwidth{};
};

//=============================================================================
// Perf configuration (one option active at a time)
//=============================================================================

class LpaiPerfConfig : public LpaiApiType<LpaiPerfConfig, QairtLpaiPerfConfigV1_t> {
  template <typename>
  friend class BackendExtension;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using base_type::base_type;
  LpaiPerfConfig()                                  = default;
  LpaiPerfConfig(const LpaiPerfConfig&)             = delete;
  LpaiPerfConfig& operator=(const LpaiPerfConfig&)  = delete;
  LpaiPerfConfig(LpaiPerfConfig&&) noexcept         = default;
  LpaiPerfConfig& operator=(LpaiPerfConfig&&) noexcept = default;

  LpaiPerfConfigOption getOption() const {
    return static_cast<LpaiPerfConfigOption>(callGetter<&interface_type::getOption>());
  }

  void setEnpuPowerLevel(LpaiPerformanceMode performanceMode, bool maxDdrBandwidth = false) {
    callAndConfirm<&interface_type::setEnpuPowerLevel>(
        static_cast<QairtLpaiPerfInfrastructure_PerformanceMode_t>(performanceMode),
        detail::convertBool(maxDdrBandwidth));
  }
  LpaiEnpuPowerLevel getEnpuPowerLevel() const {
    QairtLpaiPerfInfrastructure_PerformanceMode_t mode{};
    uint8_t maxDdrBandwidth{};
    callAndConfirm<&interface_type::getEnpuPowerLevel>(&mode, &maxDdrBandwidth);
    return {static_cast<LpaiPerformanceMode>(mode), detail::resolveBool(maxDdrBandwidth)};
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

  explicit LpaiPerfConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct LpaiPerfConfig");
    }
  }

  // Non-owning constructor: wraps an existing entry handle owned by a
  // LpaiPerfConfigList. The destructor will NOT free the handle.
  LpaiPerfConfig(const std::shared_ptr<api_table_type>& apiTable,
                 QairtLpaiPerfConfigHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

//=============================================================================
// Perf configuration list
//=============================================================================

class LpaiPerfConfigList : public LpaiApiType<LpaiPerfConfigList, QairtLpaiPerfConfigListV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  LpaiPerfConfigList()                                      = default;
  LpaiPerfConfigList(const LpaiPerfConfigList&)             = delete;
  LpaiPerfConfigList& operator=(const LpaiPerfConfigList&)  = delete;
  LpaiPerfConfigList(LpaiPerfConfigList&&) noexcept         = default;
  LpaiPerfConfigList& operator=(LpaiPerfConfigList&&) noexcept = default;

  /// Appends a copy of config's current value; the caller may free or reuse
  /// config afterward.
  void addConfig(const LpaiPerfConfig& config) {
    callAndConfirm<&interface_type::addConfig>(getHandle(config));
  }

  uint32_t getNumConfigs() const { return callGetter<&interface_type::getNumConfigs>(); }

  /// Returns a non-owning view of the config at index; do not free it. Valid
  /// until the list is freed or cleared.
  LpaiPerfConfig getConfigAt(uint32_t index) const {
    QairtLpaiPerfConfigHandle_t handle{};
    callAndConfirm<&interface_type::getConfigAt>(index, &handle);
    return create<LpaiPerfConfig>(handle);
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

  explicit LpaiPerfConfigList(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct LpaiPerfConfigList");
    }
  }
};

//=============================================================================
// LpaiPerfInfrastructure
//=============================================================================

/**
 * @brief C++ wrapper for the QAIRT LPAI perf infrastructure.
 *
 * Obtain via qairt::getLpaiPerfInfrastructure(extension, deviceInfrastructure).
 * Owns the managed perf-infrastructure handle and frees it on destruction; the
 * backend infrastructure it references is not freed.
 */
class LpaiPerfInfrastructure
    : public LpaiApiType<LpaiPerfInfrastructure, QairtLpaiPerfInfrastructureV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;

  LpaiPerfInfrastructure()                                         = default;
  LpaiPerfInfrastructure(const LpaiPerfInfrastructure&)            = delete;
  LpaiPerfInfrastructure& operator=(const LpaiPerfInfrastructure&) = delete;
  LpaiPerfInfrastructure(LpaiPerfInfrastructure&&) noexcept        = default;
  LpaiPerfInfrastructure& operator=(LpaiPerfInfrastructure&&) noexcept = default;

  /// Apply the perf configuration list for the given hardware version.
  void setPowerConfig(uint32_t hwVersion, const LpaiPerfConfigList& configs) {
    callAndConfirm<&interface_type::setPowerConfig>(hwVersion, getHandle(configs));
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
  LpaiPerfInfrastructure(const std::weak_ptr<api_table_type>& apiTable,
                        const DeviceInfrastructure& deviceInfra)
      : base_type(lockOrThrow(apiTable)) {
    construct<&interface_type::getPerfInfrastructure>(deviceInfra.getHandle());
    if (!getHandle()) {
      throw std::runtime_error("Failed to resolve LpaiPerfInfrastructure");
    }
  }
};

/**
 * @brief Resolve the LPAI perf infrastructure from a device infrastructure.
 *
 * Convenience free function so callers do not construct LpaiPerfInfrastructure
 * directly.
 *
 * @throws qairt::Exception if the backend does not expose a perf infrastructure.
 */
inline LpaiPerfInfrastructure getLpaiPerfInfrastructure(LpaiBackendExtension& extension,
                                                        const DeviceInfrastructure& deviceInfra) {
  return extension.make<LpaiPerfInfrastructure>(deviceInfra);
}

}  // namespace qairt
