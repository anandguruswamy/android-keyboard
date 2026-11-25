//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================
#pragma once

#include <optional>
#include <vector>

#include "../QairtBackend.hpp"
#include "LPAI/QairtLpaiBackend.h"
#include "QairtLpai.hpp"

namespace qairt {

class LpaiBackendHwInfoConfig
    : public LpaiApiType<LpaiBackendHwInfoConfig, QairtLpaiBackend_HwInfoV1_t> {
  friend detail::TempWorkaround;
  template <typename>
  friend class BackendExtension;

 public:
  LpaiBackendHwInfoConfig()                                              = default;
  LpaiBackendHwInfoConfig(const LpaiBackendHwInfoConfig&)                = delete;
  LpaiBackendHwInfoConfig(LpaiBackendHwInfoConfig&&) noexcept            = default;
  LpaiBackendHwInfoConfig& operator=(const LpaiBackendHwInfoConfig&)     = delete;
  LpaiBackendHwInfoConfig& operator=(LpaiBackendHwInfoConfig&&) noexcept = default;

  // Target
  void setTarget(QairtLpaiBackend_Target_t target) {
    callSetter<&interface_type::setTarget>(target);
  }
  QairtLpaiBackend_Target_t getTarget() const { return callGetter<&interface_type::getTarget>(); }

  // HW Version
  void setHwVersion(QairtLpaiBackend_HwVersion_t hwVersion) {
    callSetter<&interface_type::setHwVersion>(hwVersion);
  }
  QairtLpaiBackend_HwVersion_t getHwVersion() const {
    return callGetter<&interface_type::getHwVersion>();
  }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) throw std::runtime_error("Failed to lock apiTable");
    return locked;
  }

  LpaiBackendHwInfoConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) throw std::runtime_error("Failed to construct LpaiBackendHwInfoConfig");
  }
};

class LpaiBackendRpcPollingConfig
    : public LpaiApiType<LpaiBackendRpcPollingConfig, QairtLpaiBackend_RpcPollingConfigV1_t> {
  friend detail::TempWorkaround;
  template <typename>
  friend class BackendExtension;

 public:
  LpaiBackendRpcPollingConfig()                                                    = default;
  LpaiBackendRpcPollingConfig(const LpaiBackendRpcPollingConfig&)                = delete;
  LpaiBackendRpcPollingConfig(LpaiBackendRpcPollingConfig&&) noexcept            = default;
  LpaiBackendRpcPollingConfig& operator=(const LpaiBackendRpcPollingConfig&)     = delete;
  LpaiBackendRpcPollingConfig& operator=(LpaiBackendRpcPollingConfig&&) noexcept = default;

  // Enable
  void setEnable(uint32_t enable) { callSetter<&interface_type::setEnable>(enable); }
  uint32_t getEnable() const { return callGetter<&interface_type::getEnable>(); }

  // DSP Polling Timeout (us)
  void setDSPPollingTimeoutUs(uint32_t dspPollingTimeoutUs) {
    callSetter<&interface_type::setDSPPollingTimeoutUs>(dspPollingTimeoutUs);
  }
  uint32_t getDSPPollingTimeoutUs() const {
    return callGetter<&interface_type::getDSPPollingTimeoutUs>();
  }

  // HLOS Wakeup Lead Time (us)
  void setHLOSWakeupLeadTimeUs(uint32_t hlosWakeupLeadTimeUs) {
    callSetter<&interface_type::setHLOSWakeupLeadTimeUs>(hlosWakeupLeadTimeUs);
  }
  uint32_t getHLOSWakeupLeadTimeUs() const {
    return callGetter<&interface_type::getHLOSWakeupLeadTimeUs>();
  }

  // HLOS Polling Timeout (us)
  void setHLOSPollingTimeoutUs(uint32_t hlosPollingTimeoutUs) {
    callSetter<&interface_type::setHLOSPollingTimeoutUs>(hlosPollingTimeoutUs);
  }
  uint32_t getHLOSPollingTimeoutUs() const {
    return callGetter<&interface_type::getHLOSPollingTimeoutUs>();
  }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) throw std::runtime_error("Failed to lock apiTable");
    return locked;
  }

  LpaiBackendRpcPollingConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle())
      throw std::runtime_error("Failed to construct LpaiBackendRpcPollingConfig");
  }
};

class LpaiBackendCustomConfig
    : public LpaiApiType<LpaiBackendCustomConfig, QairtLpaiBackend_CustomConfigV1_t>,
      public BackendCustomConfig {
  friend detail::TempWorkaround;
  template <typename>
  friend class BackendExtension;

  detail::crossable<detail::non_owning<LpaiBackendHwInfoConfig>,
                    &interface_type::getHwInfo,
                    &interface_type::setHwInfo>
      m_hwInfo;

  detail::crossable<detail::non_owning<LpaiBackendRpcPollingConfig>,
                    &interface_type::getRpcPolling,
                    &interface_type::setRpcPolling>
      m_rpcPolling;

 public:
  QairtBackend_CustomConfigHandle_t getCustomConfigHandle() const override { return getHandle(); }

  LpaiBackendCustomConfig()                                              = default;
  LpaiBackendCustomConfig(const LpaiBackendCustomConfig&)                = delete;
  LpaiBackendCustomConfig(LpaiBackendCustomConfig&&) noexcept            = default;
  LpaiBackendCustomConfig& operator=(const LpaiBackendCustomConfig&)     = delete;
  LpaiBackendCustomConfig& operator=(LpaiBackendCustomConfig&&) noexcept = default;

  void setHwInfo(const LpaiBackendHwInfoConfig& hwInfo) { m_hwInfo.setNonOwning(*this, hwInfo); }
  LpaiBackendHwInfoConfig& getHwInfo() { return m_hwInfo.bindNonOwning(*this); }

  void setRpcPolling(const LpaiBackendRpcPollingConfig& rpcPolling) {
    m_rpcPolling.setNonOwning(*this, rpcPolling);
  }
  LpaiBackendRpcPollingConfig& getRpcPolling() { return m_rpcPolling.bindNonOwning(*this); }

  // Debug Flags (lpai_debug's 5 backend-global boolean sub-keys). Each flag is independently
  // settable/gettable so callers can change one flag without needing to read back the others.
  void setAixDump(bool aixDump) {
    callSetter<&interface_type::setAixDump>(aixDump ? uint8_t{1} : uint8_t{0});
  }
  bool getAixDump() const {
    return detail::resolveBool(callGetter<&interface_type::getAixDump>());
  }

  void setForceNhwc(bool forceNhwc) {
    callSetter<&interface_type::setForceNhwc>(forceNhwc ? uint8_t{1} : uint8_t{0});
  }
  bool getForceNhwc() const {
    return detail::resolveBool(callGetter<&interface_type::getForceNhwc>());
  }

  void setEnableCuRShift(bool enableCuRShift) {
    callSetter<&interface_type::setEnableCuRShift>(enableCuRShift ? uint8_t{1} : uint8_t{0});
  }
  bool getEnableCuRShift() const {
    return detail::resolveBool(callGetter<&interface_type::getEnableCuRShift>());
  }

  void setEnableFramerOpt(bool enableFramerOpt) {
    callSetter<&interface_type::setEnableFramerOpt>(enableFramerOpt ? uint8_t{1} : uint8_t{0});
  }
  bool getEnableFramerOpt() const {
    return detail::resolveBool(callGetter<&interface_type::getEnableFramerOpt>());
  }

  void setSearchElemBcastLayouts(bool searchElemBcastLayouts) {
    callSetter<&interface_type::setSearchElemBcastLayouts>(
        searchElemBcastLayouts ? uint8_t{1} : uint8_t{0});
  }
  bool getSearchElemBcastLayouts() const {
    return detail::resolveBool(callGetter<&interface_type::getSearchElemBcastLayouts>());
  }

 private:
  static std::shared_ptr<api_table_type> lockOrThrow(
      const std::weak_ptr<api_table_type>& apiTable) {
    auto locked = apiTable.lock();
    if (!locked) throw std::runtime_error("Failed to lock apiTable");
    return locked;
  }

  LpaiBackendCustomConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(lockOrThrow(apiTable)) {
    construct();
    if (!getHandle()) throw std::runtime_error("Failed to construct LpaiBackendCustomConfig");
  }
};

class LpaiBackendCustomConfiguration : public detail::TabularBEType<LpaiBackendExtensionTable>,
                                       public BackendCustomConfiguration {
  // Adding a flag here also requires adding it to anySet() below and to the apply block in
  // buildCustomConfigsVector() -- a flag missing from either is silently never applied.
  struct DebugFlags {
    std::optional<bool> aixDump{};
    std::optional<bool> forceNhwc{};
    std::optional<bool> enableCuRShift{};
    std::optional<bool> enableFramerOpt{};
    std::optional<bool> searchElemBcastLayouts{};

    bool anySet() const {
      return aixDump.has_value() || forceNhwc.has_value() ||
             enableCuRShift.has_value() || enableFramerOpt.has_value() ||
             searchElemBcastLayouts.has_value();
    }
  };

  std::optional<LpaiBackendHwInfoConfig> m_hwInfo{};
  std::optional<LpaiBackendRpcPollingConfig> m_rpcPolling{};
  DebugFlags m_debugFlags{};

  mutable std::vector<LpaiBackendCustomConfig> m_customCfgs;

 public:
  std::vector<QairtBackend_CustomConfigHandle_t> getCustomConfigs() const override {
    std::vector<QairtBackend_CustomConfigHandle_t> toret;
    m_customCfgs = buildCustomConfigsVector();
    toret.reserve(m_customCfgs.size());
    for (auto& e : m_customCfgs) {
      toret.push_back(e.getCustomConfigHandle());
    }
    return toret;
  }

  using base_type::base_type;

  // HW Info
  void clearHwInfo() { m_hwInfo.reset(); }
  void setHwInfo(LpaiBackendHwInfoConfig hwInfo) { m_hwInfo = std::move(hwInfo); }
  std::optional<LpaiBackendHwInfoConfig>& getHwInfo() { return m_hwInfo; }
  const std::optional<LpaiBackendHwInfoConfig>& getHwInfo() const { return m_hwInfo; }

  // RPC Polling
  void clearRpcPolling() { m_rpcPolling.reset(); }
  void setRpcPolling(LpaiBackendRpcPollingConfig rpcPolling) {
    m_rpcPolling = std::move(rpcPolling);
  }
  std::optional<LpaiBackendRpcPollingConfig>& getRpcPolling() { return m_rpcPolling; }
  const std::optional<LpaiBackendRpcPollingConfig>& getRpcPolling() const { return m_rpcPolling; }

  // Debug Flags — AIX Dump
  void clearAixDump() { m_debugFlags.aixDump.reset(); }
  void setAixDump(bool aixDump) { m_debugFlags.aixDump = aixDump; }
  const std::optional<bool>& getAixDump() const { return m_debugFlags.aixDump; }

  // Debug Flags — Force NHWC
  void clearForceNhwc() { m_debugFlags.forceNhwc.reset(); }
  void setForceNhwc(bool forceNhwc) { m_debugFlags.forceNhwc = forceNhwc; }
  const std::optional<bool>& getForceNhwc() const { return m_debugFlags.forceNhwc; }

  // Debug Flags — Enable CU Right Shift
  void clearEnableCuRShift() { m_debugFlags.enableCuRShift.reset(); }
  void setEnableCuRShift(bool enableCuRShift) { m_debugFlags.enableCuRShift = enableCuRShift; }
  const std::optional<bool>& getEnableCuRShift() const { return m_debugFlags.enableCuRShift; }

  // Debug Flags — Enable Framer Opt
  void clearEnableFramerOpt() { m_debugFlags.enableFramerOpt.reset(); }
  void setEnableFramerOpt(bool enableFramerOpt) {
    m_debugFlags.enableFramerOpt = enableFramerOpt;
  }
  const std::optional<bool>& getEnableFramerOpt() const { return m_debugFlags.enableFramerOpt; }

  // Debug Flags — Search Elem Bcast Layouts
  void clearSearchElemBcastLayouts() { m_debugFlags.searchElemBcastLayouts.reset(); }
  void setSearchElemBcastLayouts(bool searchElemBcastLayouts) {
    m_debugFlags.searchElemBcastLayouts = searchElemBcastLayouts;
  }
  const std::optional<bool>& getSearchElemBcastLayouts() const {
    return m_debugFlags.searchElemBcastLayouts;
  }

 private:
  std::vector<LpaiBackendCustomConfig> buildCustomConfigsVector() const {
    if (!m_backendExtensionTable.lock()) {
      throw std::runtime_error("LpaiBackendCustomConfiguration has no API table");
    }
    std::vector<LpaiBackendCustomConfig> toret;
    if (m_hwInfo.has_value()) {
      LpaiBackendCustomConfig& cfg = toret.emplace_back(make<LpaiBackendCustomConfig>());
      cfg.setHwInfo(m_hwInfo.value());
    }
    if (m_rpcPolling.has_value()) {
      LpaiBackendCustomConfig& cfg = toret.emplace_back(make<LpaiBackendCustomConfig>());
      cfg.setRpcPolling(m_rpcPolling.value());
    }
    // Each debug flag is independently settable; only apply the ones the caller actually set.
    if (m_debugFlags.anySet()) {
      LpaiBackendCustomConfig& cfg = toret.emplace_back(make<LpaiBackendCustomConfig>());
      if (m_debugFlags.aixDump.has_value()) {
        cfg.setAixDump(m_debugFlags.aixDump.value());
      }
      if (m_debugFlags.forceNhwc.has_value()) {
        cfg.setForceNhwc(m_debugFlags.forceNhwc.value());
      }
      if (m_debugFlags.enableCuRShift.has_value()) {
        cfg.setEnableCuRShift(m_debugFlags.enableCuRShift.value());
      }
      if (m_debugFlags.enableFramerOpt.has_value()) {
        cfg.setEnableFramerOpt(m_debugFlags.enableFramerOpt.value());
      }
      if (m_debugFlags.searchElemBcastLayouts.has_value()) {
        cfg.setSearchElemBcastLayouts(m_debugFlags.searchElemBcastLayouts.value());
      }
    }
    return toret;
  }
};

class LpaiBackendAlignmentReq
    : public LpaiApiType<LpaiBackendAlignmentReq, QairtLpaiBackend_AlignmentReqV1_t> {
  template <typename>
  friend class BackendExtension;
  friend class LpaiBackendProperties;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using base_type::base_type;

  LpaiBackendAlignmentReq()                                              = default;
  LpaiBackendAlignmentReq(const LpaiBackendAlignmentReq&)                = delete;
  LpaiBackendAlignmentReq(LpaiBackendAlignmentReq&&) noexcept            = default;
  LpaiBackendAlignmentReq& operator=(const LpaiBackendAlignmentReq&)     = delete;
  LpaiBackendAlignmentReq& operator=(LpaiBackendAlignmentReq&&) noexcept = default;

  uint32_t getStartAddrAlignment() const {
    return callGetter<&interface_type::getStartAddrAlignment>();
  }

  uint32_t getSizeAlignment() const { return callGetter<&interface_type::getSizeAlignment>(); }

 private:
  LpaiBackendAlignmentReq(const std::shared_ptr<api_table_type>& apiTable,
                          QairtLpaiBackend_AlignmentReqHandle_t handle)
      : base_type(apiTable) {
    setHandle(handle);
  }
};

class LpaiBackendCoreInfo
    : public LpaiApiType<LpaiBackendCoreInfo, QairtLpaiBackend_eNPUCoreInfoV1_t> {
  template <typename>
  friend class BackendExtension;
  friend class LpaiBackendProperties;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using base_type::base_type;

  LpaiBackendCoreInfo()                                          = default;
  LpaiBackendCoreInfo(const LpaiBackendCoreInfo&)                = delete;
  LpaiBackendCoreInfo(LpaiBackendCoreInfo&&) noexcept            = default;
  LpaiBackendCoreInfo& operator=(const LpaiBackendCoreInfo&)     = delete;
  LpaiBackendCoreInfo& operator=(LpaiBackendCoreInfo&&) noexcept = default;

  uint32_t getCoreIndex() const { return callGetter<&interface_type::getCoreIndex>(); }
  uint32_t getMacCnt() const { return callGetter<&interface_type::getMacCnt>(); }
  uint32_t getVersion() const { return callGetter<&interface_type::getVersion>(); }

 private:
  LpaiBackendCoreInfo(const std::shared_ptr<api_table_type>& apiTable,
                      QairtLpaiBackend_eNPUCoreInfoHandle_t handle)
      : base_type(apiTable) {
    setHandle(handle);
  }
};

class LpaiBackendCoreUtilData
    : public LpaiApiType<LpaiBackendCoreUtilData, QairtLpaiBackend_eNPUCoreUtilDataV1_t> {
  template <typename>
  friend class BackendExtension;
  friend class LpaiBackendProperties;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using base_type::base_type;

  LpaiBackendCoreUtilData()                                              = default;
  LpaiBackendCoreUtilData(const LpaiBackendCoreUtilData&)                = delete;
  LpaiBackendCoreUtilData(LpaiBackendCoreUtilData&&) noexcept            = default;
  LpaiBackendCoreUtilData& operator=(const LpaiBackendCoreUtilData&)     = delete;
  LpaiBackendCoreUtilData& operator=(LpaiBackendCoreUtilData&&) noexcept = default;

  uint64_t getSampleTs() const { return callGetter<&interface_type::getSampleTs>(); }
  uint64_t getCycles() const { return callGetter<&interface_type::getCycles>(); }
  uint32_t getClockKhz() const { return callGetter<&interface_type::getClockKhz>(); }
  uint32_t getDdrBwKbps() const { return callGetter<&interface_type::getDdrBwKbps>(); }

 private:
  LpaiBackendCoreUtilData(const std::shared_ptr<api_table_type>& apiTable,
                          QairtLpaiBackend_eNPUCoreUtilDataHandle_t handle)
      : base_type(apiTable) {
    setHandle(handle);
  }
};

class LpaiBackendProperties
    : public LpaiApiType<LpaiBackendProperties, QairtLpaiBackend_PropertiesV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;

  LpaiBackendProperties() = default;

  // Non-owning: binds the backend handle so the getters can be dispatched with
  // it. Does not own or free the backend.
  explicit LpaiBackendProperties(const std::weak_ptr<api_table_type>& apiTable,
                                 const Backend& backend)
      : base_type(apiTable.lock()) {
    setHandle(detail::QairtTypeAdmin::getHandle(backend));
    setNonOwning(true);
  }

  LpaiBackendProperties(const LpaiBackendProperties&)                = delete;
  LpaiBackendProperties& operator=(const LpaiBackendProperties&)     = delete;
  LpaiBackendProperties(LpaiBackendProperties&&) noexcept            = default;
  LpaiBackendProperties& operator=(LpaiBackendProperties&&) noexcept = default;

  LpaiBackendAlignmentReq getAlignmentReq() const {
    return create<LpaiBackendAlignmentReq>(callGetter<&interface_type::getAlignmentReq>());
  }

  bool getRequirePersistentBinary() const {
    return detail::resolveBool(callGetter<&interface_type::getRequirePersistentBinary>());
  }

  uint32_t getEnpuCoreNum() const { return callGetter<&interface_type::getEnpuCoreNum>(); }

  LpaiBackendCoreInfo getEnpuCoreInfo(uint32_t coreIndex) const {
    QairtLpaiBackend_eNPUCoreInfoHandle_t handle{};
    callAndConfirm<&interface_type::getEnpuCoreInfo>(coreIndex, &handle);
    return create<LpaiBackendCoreInfo>(handle);
  }

  // Enable or disable eNPU core utilization measurement (DEBUG). Enable before creating any
  // context or graph. Silently ignored (no error) if already in the requested state.
  void setEnpuCoreUtilEnabled(bool enable) {
    callAndConfirm<&interface_type::setEnpuCoreUtilEnabled>(enable ? uint8_t{1} : uint8_t{0});
  }

  // Read a per-core utilization snapshot. Measurement must have been enabled first via
  // setEnpuCoreUtilEnabled(). DEBUG feature — not for production code paths.
  LpaiBackendCoreUtilData getEnpuCoreUtilData(uint32_t coreIndex) const {
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t handle{};
    callAndConfirm<&interface_type::getEnpuCoreUtilData>(coreIndex, &handle);
    return create<LpaiBackendCoreUtilData>(handle);
  }

  uint32_t getEnpuClkFreq(uint32_t coreIndex) const {
    uint32_t clkFreqHz{};
    callAndConfirm<&interface_type::getEnpuClkFreq>(coreIndex, &clkFreqHz);
    return clkFreqHz;
  }
};

}  // namespace qairt
