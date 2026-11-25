//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================
#pragma once

#include <vector>

#include "../QairtDevice.hpp"
#include "HTP/QairtHtpPerfInfrastructure.h"
#include "QairtHtp.hpp"

namespace qairt {

//=============================================================================
// Enums
//=============================================================================

QAIRT_CPPAPI_DEFINE_ENUM(HtpClkPerfMode,
                         QairtHtpPerfInfrastructure_ClkPerfMode_t,
                         High    = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH,
                         Low     = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_LOW,
                         HighD1  = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D1,
                         HighD2  = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D2,
                         HighD3  = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D3,
                         HighD4  = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D4,
                         HighD5  = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D5,
                         HighD6  = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D6,
                         HighD7  = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D7,
                         HighD8  = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D8,
                         HighD9  = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D9,
                         HighD10 = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D10,
                         HighD11 = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D11,
                         HighD12 = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D12,
                         HighD13 = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D13,
                         HighD14 = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D14,
                         HighD15 = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D15,
                         Max     = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_MAX,
                         Unknown = QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(HtpVoltageCorner,
                         QairtHtpPerfInfrastructure_VoltageCorner_t,
                         Disable  = QAIRT_DCVS_VOLTAGE_CORNER_DISABLE,
                         Min      = QAIRT_DCVS_VOLTAGE_VCORNER_MIN_VOLTAGE_CORNER,
                         Svs2     = QAIRT_DCVS_VOLTAGE_VCORNER_SVS2,
                         Svs      = QAIRT_DCVS_VOLTAGE_VCORNER_SVS,
                         SvsPlus  = QAIRT_DCVS_VOLTAGE_VCORNER_SVS_PLUS,
                         Nom      = QAIRT_DCVS_VOLTAGE_VCORNER_NOM,
                         NomPlus  = QAIRT_DCVS_VOLTAGE_VCORNER_NOM_PLUS,
                         Turbo    = QAIRT_DCVS_VOLTAGE_VCORNER_TURBO,
                         TurboPlus = QAIRT_DCVS_VOLTAGE_VCORNER_TURBO_PLUS,
                         TurboL2  = QAIRT_DCVS_VOLTAGE_VCORNER_TURBO_L2,
                         TurboL3  = QAIRT_DCVS_VOLTAGE_VCORNER_TURBO_L3,
                         TurboL4  = QAIRT_DCVS_VOLTAGE_VCORNER_TURBO_L4,
                         TurboL5  = QAIRT_DCVS_VOLTAGE_VCORNER_TURBO_L5,
                         Max      = QAIRT_DCVS_VOLTAGE_VCORNER_MAX_VOLTAGE_CORNER,
                         Unknown  = QAIRT_DCVS_VOLTAGE_VCORNER_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(HtpExpVoltageCorner,
                         QairtHtpPerfInfrastructure_ExpVoltageCorner_t,
                         Disable        = QAIRT_DCVS_EXP_VCORNER_DISABLE,
                         Min            = QAIRT_DCVS_EXP_VCORNER_MIN,
                         Ret            = QAIRT_DCVS_EXP_VCORNER_RET,
                         MinSvs         = QAIRT_DCVS_EXP_VCORNER_MIN_SVS,
                         LowSvsD3_0     = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D3_0,
                         LowSvsD3       = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D3,
                         LowSvsD2_1     = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D2_1,
                         LowSvsD2       = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D2,
                         LowSvsD1_2     = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D1_2,
                         LowSvsD1_1     = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D1_1,
                         LowSvsD1_0     = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D1_0,
                         LowSvsD1       = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D1,
                         LowSvsD0_1     = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D0_1,
                         LowSvsD0_0     = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D0_0,
                         LowSvsD0       = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D0,
                         LowSvs         = QAIRT_DCVS_EXP_VCORNER_LOW_SVS,
                         LowSvsP1       = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_P1,
                         LowSvsL0       = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_L0,
                         LowSvsL1       = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_L1,
                         LowSvsL2       = QAIRT_DCVS_EXP_VCORNER_LOW_SVS_L2,
                         Svs            = QAIRT_DCVS_EXP_VCORNER_SVS,
                         SvsL0          = QAIRT_DCVS_EXP_VCORNER_SVS_L0,
                         SvsL0_1        = QAIRT_DCVS_EXP_VCORNER_SVS_L0_1,
                         SvsL0_2        = QAIRT_DCVS_EXP_VCORNER_SVS_L0_2,
                         SvsL1          = QAIRT_DCVS_EXP_VCORNER_SVS_L1,
                         SvsL2          = QAIRT_DCVS_EXP_VCORNER_SVS_L2,
                         SvsL2_0        = QAIRT_DCVS_EXP_VCORNER_SVS_L2_0,
                         Nom            = QAIRT_DCVS_EXP_VCORNER_NOM,
                         NomL0          = QAIRT_DCVS_EXP_VCORNER_NOM_L0,
                         NomL0_0        = QAIRT_DCVS_EXP_VCORNER_NOM_L0_0,
                         NomL1          = QAIRT_DCVS_EXP_VCORNER_NOM_L1,
                         NomL2          = QAIRT_DCVS_EXP_VCORNER_NOM_L2,
                         NomL3          = QAIRT_DCVS_EXP_VCORNER_NOM_L3,
                         Tur            = QAIRT_DCVS_EXP_VCORNER_TUR,
                         TurL0          = QAIRT_DCVS_EXP_VCORNER_TUR_L0,
                         TurL1          = QAIRT_DCVS_EXP_VCORNER_TUR_L1,
                         TurL1_0        = QAIRT_DCVS_EXP_VCORNER_TUR_L1_0,
                         TurL1_1        = QAIRT_DCVS_EXP_VCORNER_TUR_L1_1,
                         TurL1_2        = QAIRT_DCVS_EXP_VCORNER_TUR_L1_2,
                         TurL2          = QAIRT_DCVS_EXP_VCORNER_TUR_L2,
                         TurL3          = QAIRT_DCVS_EXP_VCORNER_TUR_L3,
                         TurL3_0        = QAIRT_DCVS_EXP_VCORNER_TUR_L3_0,
                         TurL3_1        = QAIRT_DCVS_EXP_VCORNER_TUR_L3_1,
                         TurL3_2        = QAIRT_DCVS_EXP_VCORNER_TUR_L3_2,
                         TurL4          = QAIRT_DCVS_EXP_VCORNER_TUR_L4,
                         TurL5          = QAIRT_DCVS_EXP_VCORNER_TUR_L5,
                         SuperTurbo     = QAIRT_DCVS_EXP_VCORNER_SUPER_TURBO,
                         SuperTurboNoCpr = QAIRT_DCVS_EXP_VCORNER_SUPER_TURBO_NO_CPR,
                         DefinedMax     = QAIRT_DCVS_EXP_VCORNER_DEFINED_MAX,
                         Max            = QAIRT_DCVS_EXP_VCORNER_MAX,
                         Unknown        = QAIRT_DCVS_EXP_VCORNER_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(HtpPowerMode,
                         QairtHtpPerfInfrastructure_PowerMode_t,
                         AdjustUpDown         = QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_ADJUST_UP_DOWN,
                         AdjustOnlyUp         = QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_ADJUST_ONLY_UP,
                         PowerSaver           = QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_POWER_SAVER_MODE,
                         PowerSaverAggressive =
                             QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_POWER_SAVER_AGGRESSIVE_MODE,
                         Performance = QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_PERFORMANCE_MODE,
                         DutyCycle   = QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_DUTY_CYCLE_MODE,
                         Unknown     = QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(HtpPowerConfigOption,
                         QairtHtpPerfInfrastructure_PowerConfigOption_t,
                         DcvsV3 = QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_V3,
                         RpcControlLatency =
                             QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_CONTROL_LATENCY,
                         RpcPollingTime =
                             QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_POLLING_TIME,
                         HmxTimeoutIntervalUs =
                             QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_HMX_TIMEOUT_INTERVAL_US,
                         HmxV2 = QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_HMX_V2,
                         AdaptivePollingTime =
                             QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_ADAPTIVE_POLLING_TIME,
                         DdrPerfMode = QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DDR_PERF_MODE,
                         Ceng        = QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_CENG,
                         DcvsV3Exp   = QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_V3_EXP,
                         CengExp     = QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_CENG_EXP,
                         Unknown     = QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_UNKNOWN)

QAIRT_CPPAPI_DEFINE_ENUM(HtpMemoryConfigOption,
                         QairtHtpPerfInfrastructure_MemoryConfigOption_t,
                         GrowSize = QAIRT_HTP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_GROW_SIZE,
                         Unknown  = QAIRT_HTP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_UNKNOWN)

//=============================================================================
// Grouped return-value structs (multi-out getters)
//=============================================================================

struct HtpDcvsEnable {
  bool set{};
  bool enabled{};
};
struct HtpSleepLatency {
  bool set{};
  uint32_t latencyUs{};
};
struct HtpSleepDisable {
  bool set{};
  bool disabled{};
};
struct HtpVoltageCorners {
  bool set{};
  HtpVoltageCorner min{HtpVoltageCorner::Unknown};
  HtpVoltageCorner target{HtpVoltageCorner::Unknown};
  HtpVoltageCorner max{HtpVoltageCorner::Unknown};
};
struct HtpExpVoltageCorners {
  bool set{};
  HtpExpVoltageCorner min{HtpExpVoltageCorner::Unknown};
  HtpExpVoltageCorner target{HtpExpVoltageCorner::Unknown};
  HtpExpVoltageCorner max{HtpExpVoltageCorner::Unknown};
};
struct HtpExpVoltageCornersNoFlag {
  HtpExpVoltageCorner min{HtpExpVoltageCorner::Unknown};
  HtpExpVoltageCorner target{HtpExpVoltageCorner::Unknown};
  HtpExpVoltageCorner max{HtpExpVoltageCorner::Unknown};
};
struct HtpVoltageCornersNoFlag {
  HtpVoltageCorner min{HtpVoltageCorner::Unknown};
  HtpVoltageCorner target{HtpVoltageCorner::Unknown};
  HtpVoltageCorner max{HtpVoltageCorner::Unknown};
};
struct HtpClkPerfModes {
  HtpClkPerfMode core{HtpClkPerfMode::Unknown};
  HtpClkPerfMode bus{HtpClkPerfMode::Unknown};
};

//=============================================================================
// DcvsV3 configuration
//=============================================================================

class HtpDcvsV3Config : public HtpApiType<HtpDcvsV3Config, QairtHtpPerfDcvsV3ConfigV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  HtpDcvsV3Config()                                    = default;
  HtpDcvsV3Config(const HtpDcvsV3Config& other) : base_type(copy_table_tag, other) {
    construct();
    setContextId(other.getContextId());
    setPowerMode(other.getPowerMode());
    if (auto v = other.getDcvsEnable(); v.set) setDcvsEnable(v.enabled);
    if (auto v = other.getSleepLatency(); v.set) setSleepLatencyUs(v.latencyUs);
    if (auto v = other.getSleepDisable(); v.set) setSleepDisable(v.disabled);
    if (auto v = other.getBusVoltageCorners(); v.set) setBusVoltageCorners(v.min, v.target, v.max);
    if (auto v = other.getCoreVoltageCorners(); v.set) setCoreVoltageCorners(v.min, v.target, v.max);
  }
  HtpDcvsV3Config& operator=(const HtpDcvsV3Config& other) {
    if (this != &other) {
      *this = HtpDcvsV3Config(other);
    }
    return *this;
  }
  HtpDcvsV3Config(HtpDcvsV3Config&& other) noexcept : base_type(static_cast<base_type&&>(other)) {}
  HtpDcvsV3Config& operator=(HtpDcvsV3Config&& other) noexcept {
    if (this != &other) {
      std::swap(static_cast<base_type&>(*this), static_cast<base_type&>(other));
    }
    return *this;
  }
  ~HtpDcvsV3Config() { QAIRT_TRIVIAL }

  void setContextId(uint32_t contextId) { callSetter<&interface_type::setContextId>(contextId); }
  uint32_t getContextId() const { return callGetter<&interface_type::getContextId>(); }

  void setDcvsEnable(bool enable) {
    callAndConfirm<&interface_type::setDcvsEnable>(detail::convertBool(true),
                                                    detail::convertBool(enable));
  }
  HtpDcvsEnable getDcvsEnable() const {
    uint8_t setFlag{}, val{};
    callAndConfirm<&interface_type::getDcvsEnable>(&setFlag, &val);
    return {detail::resolveBool(setFlag), detail::resolveBool(val)};
  }

  void setPowerMode(HtpPowerMode mode) {
    callSetter<&interface_type::setPowerMode>(static_cast<QairtHtpPerfInfrastructure_PowerMode_t>(mode));
  }
  HtpPowerMode getPowerMode() const {
    return static_cast<HtpPowerMode>(callGetter<&interface_type::getPowerMode>());
  }

  void setSleepLatencyUs(uint32_t latencyUs) {
    callAndConfirm<&interface_type::setSleepLatency>(detail::convertBool(true), latencyUs);
  }
  HtpSleepLatency getSleepLatency() const {
    uint8_t setFlag{};
    uint32_t latencyUs{};
    callAndConfirm<&interface_type::getSleepLatency>(&setFlag, &latencyUs);
    return {detail::resolveBool(setFlag), latencyUs};
  }

  void setSleepDisable(bool disable) {
    callAndConfirm<&interface_type::setSleepDisable>(detail::convertBool(true),
                                                      detail::convertBool(disable));
  }
  HtpSleepDisable getSleepDisable() const {
    uint8_t setFlag{}, val{};
    callAndConfirm<&interface_type::getSleepDisable>(&setFlag, &val);
    return {detail::resolveBool(setFlag), detail::resolveBool(val)};
  }

  void setBusVoltageCorners(HtpVoltageCorner min, HtpVoltageCorner target, HtpVoltageCorner max) {
    callAndConfirm<&interface_type::setBusVoltageCorners>(
        detail::convertBool(true),
        static_cast<QairtHtpPerfInfrastructure_VoltageCorner_t>(min),
        static_cast<QairtHtpPerfInfrastructure_VoltageCorner_t>(target),
        static_cast<QairtHtpPerfInfrastructure_VoltageCorner_t>(max));
  }
  HtpVoltageCorners getBusVoltageCorners() const {
    uint8_t setFlag{};
    QairtHtpPerfInfrastructure_VoltageCorner_t min{}, target{}, max{};
    callAndConfirm<&interface_type::getBusVoltageCorners>(&setFlag, &min, &target, &max);
    return {detail::resolveBool(setFlag),
            static_cast<HtpVoltageCorner>(min),
            static_cast<HtpVoltageCorner>(target),
            static_cast<HtpVoltageCorner>(max)};
  }

  void setCoreVoltageCorners(HtpVoltageCorner min, HtpVoltageCorner target, HtpVoltageCorner max) {
    callAndConfirm<&interface_type::setCoreVoltageCorners>(
        detail::convertBool(true),
        static_cast<QairtHtpPerfInfrastructure_VoltageCorner_t>(min),
        static_cast<QairtHtpPerfInfrastructure_VoltageCorner_t>(target),
        static_cast<QairtHtpPerfInfrastructure_VoltageCorner_t>(max));
  }
  HtpVoltageCorners getCoreVoltageCorners() const {
    uint8_t setFlag{};
    QairtHtpPerfInfrastructure_VoltageCorner_t min{}, target{}, max{};
    callAndConfirm<&interface_type::getCoreVoltageCorners>(&setFlag, &min, &target, &max);
    return {detail::resolveBool(setFlag),
            static_cast<HtpVoltageCorner>(min),
            static_cast<HtpVoltageCorner>(target),
            static_cast<HtpVoltageCorner>(max)};
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

  explicit HtpDcvsV3Config(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct HtpDcvsV3Config");
    }
  }
};

//=============================================================================
// HmxV2 configuration
//=============================================================================

class HtpHmxV2Config : public HtpApiType<HtpHmxV2Config, QairtHtpPerfHmxV2ConfigV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  HtpHmxV2Config()                                  = default;
  HtpHmxV2Config(const HtpHmxV2Config& other) : base_type(copy_table_tag, other) {
    construct();
    setPickDefault(other.getPickDefault());
    auto vc = other.getVoltageCorners();
    setVoltageCorners(vc.min, vc.target, vc.max);
    setPerfMode(other.getPerfMode());
  }
  HtpHmxV2Config& operator=(const HtpHmxV2Config& other) {
    if (this != &other) {
      *this = HtpHmxV2Config(other);
    }
    return *this;
  }
  HtpHmxV2Config(HtpHmxV2Config&& other) noexcept : base_type(static_cast<base_type&&>(other)) {}
  HtpHmxV2Config& operator=(HtpHmxV2Config&& other) noexcept {
    if (this != &other) {
      std::swap(static_cast<base_type&>(*this), static_cast<base_type&>(other));
    }
    return *this;
  }
  ~HtpHmxV2Config() { QAIRT_TRIVIAL }

  void setPickDefault(bool pickDefault) {
    callSetter<&interface_type::setPickDefault>(detail::convertBool(pickDefault));
  }
  bool getPickDefault() const {
    return detail::resolveBool(callGetter<&interface_type::getPickDefault>());
  }

  void setVoltageCorners(HtpExpVoltageCorner min, HtpExpVoltageCorner target,
                         HtpExpVoltageCorner max) {
    callAndConfirm<&interface_type::setVoltageCorners>(
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(min),
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(target),
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(max));
  }
  HtpExpVoltageCornersNoFlag getVoltageCorners() const {
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t min{}, target{}, max{};
    callAndConfirm<&interface_type::getVoltageCorners>(&min, &target, &max);
    return {static_cast<HtpExpVoltageCorner>(min), static_cast<HtpExpVoltageCorner>(target),
            static_cast<HtpExpVoltageCorner>(max)};
  }

  void setPerfMode(HtpClkPerfMode mode) {
    callSetter<&interface_type::setPerfMode>(static_cast<QairtHtpPerfInfrastructure_ClkPerfMode_t>(mode));
  }
  HtpClkPerfMode getPerfMode() const {
    return static_cast<HtpClkPerfMode>(callGetter<&interface_type::getPerfMode>());
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

  explicit HtpHmxV2Config(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct HtpHmxV2Config");
    }
  }
};

//=============================================================================
// CENG configuration
//=============================================================================

class HtpCengConfig : public HtpApiType<HtpCengConfig, QairtHtpPerfCengConfigV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  HtpCengConfig()                                 = default;
  HtpCengConfig(const HtpCengConfig& other) : base_type(copy_table_tag, other) {
    construct();
    auto vc = other.getVoltageCorners();
    setVoltageCorners(vc.min, vc.target, vc.max);
    setPerfMode(other.getPerfMode());
  }
  HtpCengConfig& operator=(const HtpCengConfig& other) {
    if (this != &other) {
      *this = HtpCengConfig(other);
    }
    return *this;
  }
  HtpCengConfig(HtpCengConfig&& other) noexcept : base_type(static_cast<base_type&&>(other)) {}
  HtpCengConfig& operator=(HtpCengConfig&& other) noexcept {
    if (this != &other) {
      std::swap(static_cast<base_type&>(*this), static_cast<base_type&>(other));
    }
    return *this;
  }
  ~HtpCengConfig() { QAIRT_TRIVIAL }

  void setVoltageCorners(HtpVoltageCorner min, HtpVoltageCorner target, HtpVoltageCorner max) {
    callAndConfirm<&interface_type::setVoltageCorners>(
        static_cast<QairtHtpPerfInfrastructure_VoltageCorner_t>(min),
        static_cast<QairtHtpPerfInfrastructure_VoltageCorner_t>(target),
        static_cast<QairtHtpPerfInfrastructure_VoltageCorner_t>(max));
  }
  HtpVoltageCornersNoFlag getVoltageCorners() const {
    QairtHtpPerfInfrastructure_VoltageCorner_t min{}, target{}, max{};
    callAndConfirm<&interface_type::getVoltageCorners>(&min, &target, &max);
    return {static_cast<HtpVoltageCorner>(min), static_cast<HtpVoltageCorner>(target),
            static_cast<HtpVoltageCorner>(max)};
  }

  void setPerfMode(HtpClkPerfMode mode) {
    callSetter<&interface_type::setPerfMode>(static_cast<QairtHtpPerfInfrastructure_ClkPerfMode_t>(mode));
  }
  HtpClkPerfMode getPerfMode() const {
    return static_cast<HtpClkPerfMode>(callGetter<&interface_type::getPerfMode>());
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

  explicit HtpCengConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct HtpCengConfig");
    }
  }
};

//=============================================================================
// DcvsV3 expanded-corner configuration
//=============================================================================

class HtpDcvsV3ExpConfig : public HtpApiType<HtpDcvsV3ExpConfig, QairtHtpPerfDcvsV3ExpConfigV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  HtpDcvsV3ExpConfig()                                       = default;
  HtpDcvsV3ExpConfig(const HtpDcvsV3ExpConfig& other) : base_type(copy_table_tag, other) {
    construct();
    setContextId(other.getContextId());
    setPowerMode(other.getPowerMode());
    if (auto v = other.getSleepLatency(); v.set) setSleepLatencyUs(v.latencyUs);
    if (auto v = other.getSleepDisable(); v.set) setSleepDisable(v.disabled);
    if (auto v = other.getBusVoltageCorners(); v.set) setBusVoltageCorners(v.min, v.target, v.max);
    if (auto v = other.getCoreVoltageCorners(); v.set) setCoreVoltageCorners(v.min, v.target, v.max);
    auto pm = other.getPerfModes();
    setPerfModes(pm.core, pm.bus);
  }
  HtpDcvsV3ExpConfig& operator=(const HtpDcvsV3ExpConfig& other) {
    if (this != &other) {
      *this = HtpDcvsV3ExpConfig(other);
    }
    return *this;
  }
  HtpDcvsV3ExpConfig(HtpDcvsV3ExpConfig&& other) noexcept
      : base_type(static_cast<base_type&&>(other)) {}
  HtpDcvsV3ExpConfig& operator=(HtpDcvsV3ExpConfig&& other) noexcept {
    if (this != &other) {
      std::swap(static_cast<base_type&>(*this), static_cast<base_type&>(other));
    }
    return *this;
  }
  ~HtpDcvsV3ExpConfig() { QAIRT_TRIVIAL }

  void setContextId(uint32_t contextId) { callSetter<&interface_type::setContextId>(contextId); }
  uint32_t getContextId() const { return callGetter<&interface_type::getContextId>(); }

  void setPowerMode(HtpPowerMode mode) {
    callSetter<&interface_type::setPowerMode>(static_cast<QairtHtpPerfInfrastructure_PowerMode_t>(mode));
  }
  HtpPowerMode getPowerMode() const {
    return static_cast<HtpPowerMode>(callGetter<&interface_type::getPowerMode>());
  }

  void setSleepLatencyUs(uint32_t latencyUs) {
    callAndConfirm<&interface_type::setSleepLatency>(detail::convertBool(true), latencyUs);
  }
  HtpSleepLatency getSleepLatency() const {
    uint8_t setFlag{};
    uint32_t latencyUs{};
    callAndConfirm<&interface_type::getSleepLatency>(&setFlag, &latencyUs);
    return {detail::resolveBool(setFlag), latencyUs};
  }

  void setSleepDisable(bool disable) {
    callAndConfirm<&interface_type::setSleepDisable>(detail::convertBool(true),
                                                      detail::convertBool(disable));
  }
  HtpSleepDisable getSleepDisable() const {
    uint8_t setFlag{}, val{};
    callAndConfirm<&interface_type::getSleepDisable>(&setFlag, &val);
    return {detail::resolveBool(setFlag), detail::resolveBool(val)};
  }

  void setBusVoltageCorners(HtpExpVoltageCorner min, HtpExpVoltageCorner target,
                            HtpExpVoltageCorner max) {
    callAndConfirm<&interface_type::setBusVoltageCorners>(
        detail::convertBool(true),
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(min),
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(target),
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(max));
  }
  HtpExpVoltageCorners getBusVoltageCorners() const {
    uint8_t setFlag{};
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t min{}, target{}, max{};
    callAndConfirm<&interface_type::getBusVoltageCorners>(&setFlag, &min, &target, &max);
    return {detail::resolveBool(setFlag), static_cast<HtpExpVoltageCorner>(min),
            static_cast<HtpExpVoltageCorner>(target), static_cast<HtpExpVoltageCorner>(max)};
  }

  void setCoreVoltageCorners(HtpExpVoltageCorner min, HtpExpVoltageCorner target,
                             HtpExpVoltageCorner max) {
    callAndConfirm<&interface_type::setCoreVoltageCorners>(
        detail::convertBool(true),
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(min),
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(target),
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(max));
  }
  HtpExpVoltageCorners getCoreVoltageCorners() const {
    uint8_t setFlag{};
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t min{}, target{}, max{};
    callAndConfirm<&interface_type::getCoreVoltageCorners>(&setFlag, &min, &target, &max);
    return {detail::resolveBool(setFlag), static_cast<HtpExpVoltageCorner>(min),
            static_cast<HtpExpVoltageCorner>(target), static_cast<HtpExpVoltageCorner>(max)};
  }

  void setPerfModes(HtpClkPerfMode corePerfMode, HtpClkPerfMode busPerfMode) {
    callAndConfirm<&interface_type::setPerfModes>(
        static_cast<QairtHtpPerfInfrastructure_ClkPerfMode_t>(corePerfMode),
        static_cast<QairtHtpPerfInfrastructure_ClkPerfMode_t>(busPerfMode));
  }
  HtpClkPerfModes getPerfModes() const {
    QairtHtpPerfInfrastructure_ClkPerfMode_t core{}, bus{};
    callAndConfirm<&interface_type::getPerfModes>(&core, &bus);
    return {static_cast<HtpClkPerfMode>(core), static_cast<HtpClkPerfMode>(bus)};
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

  explicit HtpDcvsV3ExpConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct HtpDcvsV3ExpConfig");
    }
  }
};

//=============================================================================
// CENG expanded-corner configuration
//=============================================================================

class HtpCengExpConfig : public HtpApiType<HtpCengExpConfig, QairtHtpPerfCengExpConfigV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  HtpCengExpConfig()                                    = default;
  HtpCengExpConfig(const HtpCengExpConfig& other) : base_type(copy_table_tag, other) {
    construct();
    auto vc = other.getVoltageCorners();
    setVoltageCorners(vc.min, vc.target, vc.max);
    setPerfMode(other.getPerfMode());
  }
  HtpCengExpConfig& operator=(const HtpCengExpConfig& other) {
    if (this != &other) {
      *this = HtpCengExpConfig(other);
    }
    return *this;
  }
  HtpCengExpConfig(HtpCengExpConfig&& other) noexcept : base_type(static_cast<base_type&&>(other)) {}
  HtpCengExpConfig& operator=(HtpCengExpConfig&& other) noexcept {
    if (this != &other) {
      std::swap(static_cast<base_type&>(*this), static_cast<base_type&>(other));
    }
    return *this;
  }
  ~HtpCengExpConfig() { QAIRT_TRIVIAL }

  void setVoltageCorners(HtpExpVoltageCorner min, HtpExpVoltageCorner target,
                         HtpExpVoltageCorner max) {
    callAndConfirm<&interface_type::setVoltageCorners>(
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(min),
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(target),
        static_cast<QairtHtpPerfInfrastructure_ExpVoltageCorner_t>(max));
  }
  HtpExpVoltageCornersNoFlag getVoltageCorners() const {
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t min{}, target{}, max{};
    callAndConfirm<&interface_type::getVoltageCorners>(&min, &target, &max);
    return {static_cast<HtpExpVoltageCorner>(min), static_cast<HtpExpVoltageCorner>(target),
            static_cast<HtpExpVoltageCorner>(max)};
  }

  void setPerfMode(HtpClkPerfMode mode) {
    callSetter<&interface_type::setPerfMode>(static_cast<QairtHtpPerfInfrastructure_ClkPerfMode_t>(mode));
  }
  HtpClkPerfMode getPerfMode() const {
    return static_cast<HtpClkPerfMode>(callGetter<&interface_type::getPerfMode>());
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

  explicit HtpCengExpConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct HtpCengExpConfig");
    }
  }
};

//=============================================================================
// Power configuration (one option active at a time)
//=============================================================================

class HtpPowerConfig : public HtpApiType<HtpPowerConfig, QairtHtpPerfPowerConfigV1_t> {
  template <typename>
  friend class BackendExtension;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

  detail::crossable<detail::non_owning<HtpDcvsV3Config>,
                    &interface_type::getDcvsV3,
                    &interface_type::setDcvsV3>
      m_dcvsV3;
  detail::crossable<detail::non_owning<HtpHmxV2Config>,
                    &interface_type::getHmxV2,
                    &interface_type::setHmxV2>
      m_hmxV2;
  detail::crossable<detail::non_owning<HtpCengConfig>,
                    &interface_type::getCeng,
                    &interface_type::setCeng>
      m_ceng;
  detail::crossable<detail::non_owning<HtpDcvsV3ExpConfig>,
                    &interface_type::getDcvsV3Exp,
                    &interface_type::setDcvsV3Exp>
      m_dcvsV3Exp;
  detail::crossable<detail::non_owning<HtpCengExpConfig>,
                    &interface_type::getCengExp,
                    &interface_type::setCengExp>
      m_cengExp;

 public:
  using base_type::base_type;
  HtpPowerConfig()                                  = default;
  HtpPowerConfig(const HtpPowerConfig&)             = delete;
  HtpPowerConfig& operator=(const HtpPowerConfig&)  = delete;
  HtpPowerConfig(HtpPowerConfig&&) noexcept         = default;
  HtpPowerConfig& operator=(HtpPowerConfig&&) noexcept = default;

  HtpPowerConfigOption getOption() const {
    return static_cast<HtpPowerConfigOption>(callGetter<&interface_type::getOption>());
  }

  /// Copies dcvsV3Config's current value; dcvsV3Config may be freed or reused
  /// afterward.
  void setDcvsV3(const HtpDcvsV3Config& dcvsV3Config) {
    callAndConfirm<&interface_type::setDcvsV3>(getHandle(dcvsV3Config));
  }
  /// Returns a non-owning view of the active DcvsV3 option; throws
  /// QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_OPTION_MISMATCH if a different
  /// option is active.
  HtpDcvsV3Config getDcvsV3() const { return m_dcvsV3.bindNonOwning(*this); }

  void setHmxV2(const HtpHmxV2Config& hmxV2Config) {
    callAndConfirm<&interface_type::setHmxV2>(getHandle(hmxV2Config));
  }
  /// Returns a non-owning view of the active HmxV2 option; throws
  /// QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_OPTION_MISMATCH if a different
  /// option is active.
  HtpHmxV2Config getHmxV2() const { return m_hmxV2.bindNonOwning(*this); }

  void setCeng(const HtpCengConfig& cengConfig) {
    callAndConfirm<&interface_type::setCeng>(getHandle(cengConfig));
  }
  /// Returns a non-owning view of the active Ceng option; throws
  /// QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_OPTION_MISMATCH if a different
  /// option is active.
  HtpCengConfig getCeng() const { return m_ceng.bindNonOwning(*this); }

  void setDcvsV3Exp(const HtpDcvsV3ExpConfig& dcvsV3ExpConfig) {
    callAndConfirm<&interface_type::setDcvsV3Exp>(getHandle(dcvsV3ExpConfig));
  }
  /// Returns a non-owning view of the active DcvsV3Exp option; throws
  /// QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_OPTION_MISMATCH if a different
  /// option is active.
  HtpDcvsV3ExpConfig getDcvsV3Exp() const { return m_dcvsV3Exp.bindNonOwning(*this); }

  void setCengExp(const HtpCengExpConfig& cengExpConfig) {
    callAndConfirm<&interface_type::setCengExp>(getHandle(cengExpConfig));
  }
  /// Returns a non-owning view of the active CengExp option; throws
  /// QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_OPTION_MISMATCH if a different
  /// option is active.
  HtpCengExpConfig getCengExp() const { return m_cengExp.bindNonOwning(*this); }

  void setRpcControlLatencyUs(uint32_t latencyUs) {
    callSetter<&interface_type::setRpcControlLatency>(latencyUs);
  }
  uint32_t getRpcControlLatencyUs() const {
    return callGetter<&interface_type::getRpcControlLatency>();
  }

  void setRpcPollingTimeUs(uint32_t pollingTimeUs) {
    callSetter<&interface_type::setRpcPollingTime>(pollingTimeUs);
  }
  uint32_t getRpcPollingTimeUs() const { return callGetter<&interface_type::getRpcPollingTime>(); }

  void setHmxTimeoutIntervalUs(uint32_t timeoutUs) {
    callSetter<&interface_type::setHmxTimeoutIntervalUs>(timeoutUs);
  }
  uint32_t getHmxTimeoutIntervalUs() const {
    return callGetter<&interface_type::getHmxTimeoutIntervalUs>();
  }

  void setAdaptivePollingTimeUs(uint32_t pollingTimeUs) {
    callSetter<&interface_type::setAdaptivePollingTime>(pollingTimeUs);
  }
  uint32_t getAdaptivePollingTimeUs() const {
    return callGetter<&interface_type::getAdaptivePollingTime>();
  }

  void setDdrPerfMode(bool enable) {
    callSetter<&interface_type::setDdrPerfMode>(detail::convertBool(enable));
  }
  bool getDdrPerfMode() const {
    return detail::resolveBool(callGetter<&interface_type::getDdrPerfMode>());
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

  explicit HtpPowerConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct HtpPowerConfig");
    }
  }

  // Non-owning constructor: wraps an existing entry handle owned by a
  // HtpPowerConfigList. The destructor will NOT free the handle.
  HtpPowerConfig(const std::shared_ptr<api_table_type>& apiTable,
                 QairtHtpPerfPowerConfigHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

//=============================================================================
// Power configuration list
//=============================================================================

class HtpPowerConfigList : public HtpApiType<HtpPowerConfigList, QairtHtpPerfPowerConfigListV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  HtpPowerConfigList()                                     = default;
  HtpPowerConfigList(const HtpPowerConfigList&)            = delete;
  HtpPowerConfigList& operator=(const HtpPowerConfigList&) = delete;
  HtpPowerConfigList(HtpPowerConfigList&&) noexcept        = default;
  HtpPowerConfigList& operator=(HtpPowerConfigList&&) noexcept = default;

  /// Appends a copy of powerConfig's current value; the caller may free or
  /// reuse powerConfig afterward.
  void addPowerConfig(const HtpPowerConfig& powerConfig) {
    callAndConfirm<&interface_type::addPowerConfig>(getHandle(powerConfig));
  }

  uint32_t getNumPowerConfigs() const { return callGetter<&interface_type::getNumPowerConfigs>(); }

  /// Returns a non-owning view of the power config at index; do not free it.
  /// Valid until the list is freed or cleared.
  HtpPowerConfig getPowerConfigAt(uint32_t index) const {
    QairtHtpPerfPowerConfigHandle_t handle{};
    callAndConfirm<&interface_type::getPowerConfigAt>(index, &handle);
    return create<HtpPowerConfig>(handle);
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

  explicit HtpPowerConfigList(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct HtpPowerConfigList");
    }
  }
};

//=============================================================================
// Memory configuration (one option active at a time)
//=============================================================================

class HtpMemoryConfig : public HtpApiType<HtpMemoryConfig, QairtHtpPerfMemoryConfigV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  HtpMemoryConfig()                                   = default;
  HtpMemoryConfig(const HtpMemoryConfig&)             = delete;
  HtpMemoryConfig& operator=(const HtpMemoryConfig&)  = delete;
  HtpMemoryConfig(HtpMemoryConfig&&) noexcept         = default;
  HtpMemoryConfig& operator=(HtpMemoryConfig&&) noexcept = default;

  HtpMemoryConfigOption getOption() const {
    return static_cast<HtpMemoryConfigOption>(callGetter<&interface_type::getOption>());
  }

  void setGrowSizeBytes(uint32_t bytes) { callSetter<&interface_type::setGrowSize>(bytes); }
  uint32_t getGrowSizeBytes() const { return callGetter<&interface_type::getGrowSize>(); }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) {
      throw std::runtime_error("Failed to lock apiTable");
    }
    return locked;
  }

  explicit HtpMemoryConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct HtpMemoryConfig");
    }
  }
};

//=============================================================================
// Memory configuration list
//=============================================================================

class HtpMemoryConfigList : public HtpApiType<HtpMemoryConfigList, QairtHtpPerfMemoryConfigListV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;
  HtpMemoryConfigList()                                      = default;
  HtpMemoryConfigList(const HtpMemoryConfigList&)             = delete;
  HtpMemoryConfigList& operator=(const HtpMemoryConfigList&)  = delete;
  HtpMemoryConfigList(HtpMemoryConfigList&&) noexcept         = default;
  HtpMemoryConfigList& operator=(HtpMemoryConfigList&&) noexcept = default;

  void addMemoryConfig(const HtpMemoryConfig& memoryConfig) {
    callAndConfirm<&interface_type::addMemoryConfig>(getHandle(memoryConfig));
  }

  uint32_t getNumMemoryConfigs() const { return callGetter<&interface_type::getNumMemoryConfigs>(); }

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

  explicit HtpMemoryConfigList(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) {
      throw std::runtime_error("Failed to construct HtpMemoryConfigList");
    }
  }
};

//=============================================================================
// HtpPerfInfrastructure
//=============================================================================

/**
 * @brief C++ wrapper for the QAIRT HTP perf infrastructure.
 *
 * Obtain via qairt::getHtpPerfInfrastructure(extension, deviceInfrastructure).
 * Owns the managed perf-infrastructure handle and frees it on destruction; the
 * backend infrastructure it references is not freed.
 */
class HtpPerfInfrastructure
    : public HtpApiType<HtpPerfInfrastructure, QairtHtpPerfInfrastructureV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;

  HtpPerfInfrastructure()                                        = default;
  HtpPerfInfrastructure(const HtpPerfInfrastructure&)            = delete;
  HtpPerfInfrastructure& operator=(const HtpPerfInfrastructure&) = delete;
  HtpPerfInfrastructure(HtpPerfInfrastructure&&) noexcept        = default;
  HtpPerfInfrastructure& operator=(HtpPerfInfrastructure&&) noexcept = default;

  /// Create a power configuration id for the given device/core.
  uint32_t createPowerConfigId(uint32_t deviceId, uint32_t coreId) {
    uint32_t id = 0;
    callAndConfirm<&interface_type::createPowerConfigId>(deviceId, coreId, &id);
    return id;
  }

  /// Destroy a previously created power configuration id.
  void destroyPowerConfigId(uint32_t powerConfigId) {
    callAndConfirm<&interface_type::destroyPowerConfigId>(powerConfigId);
  }

  /// Apply the power configuration list under the given power config id.
  void setPowerConfig(uint32_t powerConfigId, const HtpPowerConfigList& configs) {
    callAndConfirm<&interface_type::setPowerConfig>(powerConfigId, getHandle(configs));
  }

  /// Apply the memory configuration list on the given device/core.
  void setMemoryConfig(uint32_t deviceId, uint32_t coreId, const HtpMemoryConfigList& configs) {
    callAndConfirm<&interface_type::setMemoryConfig>(deviceId, coreId, getHandle(configs));
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
  HtpPerfInfrastructure(const std::weak_ptr<api_table_type>& apiTable,
                        const DeviceInfrastructure& deviceInfra)
      : base_type(lockOrThrow(apiTable)) {
    construct<&interface_type::getPerfInfrastructure>(deviceInfra.getHandle());
    if (!getHandle()) {
      throw std::runtime_error("Failed to resolve HtpPerfInfrastructure");
    }
  }
};

/**
 * @brief Resolve the HTP perf infrastructure from a device infrastructure.
 *
 * Convenience free function so callers do not construct HtpPerfInfrastructure
 * directly. Keeps the core DeviceInfrastructure type untouched.
 *
 * @throws qairt::Exception if the backend does not expose a perf infrastructure.
 */
inline HtpPerfInfrastructure getHtpPerfInfrastructure(HtpBackendExtension& extension,
                                                      const DeviceInfrastructure& deviceInfra) {
  return extension.make<HtpPerfInfrastructure>(deviceInfra);
}

}  // namespace qairt
