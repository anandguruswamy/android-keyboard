//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================
#pragma once

#include "LPAI/QairtLpaiBackend.h"
#include "LPAI/QairtLpaiContext.h"
#include "LPAI/QairtLpaiGraph.h"
#include "LPAI/QairtLpaiPerfInfrastructure.h"
#include "../QairtApi.hpp"

namespace qairt {

class LpaiBackendExtensionTable : public BackendExtensionTable<LpaiBackendExtensionTable> {
  friend class Api;
  using BackendExtensionTable::BackendExtensionTable;

 public:
  ~LpaiBackendExtensionTable() = default;
  LpaiBackendExtensionTable()  = delete;
  explicit LpaiBackendExtensionTable(const std::shared_ptr<ApiTable>& apiTable)
      : BackendExtensionTable(apiTable) {}
  LpaiBackendExtensionTable(const LpaiBackendExtensionTable&)                = delete;
  LpaiBackendExtensionTable(LpaiBackendExtensionTable&&) noexcept            = default;
  LpaiBackendExtensionTable& operator=(const LpaiBackendExtensionTable&)     = delete;
  LpaiBackendExtensionTable& operator=(LpaiBackendExtensionTable&&) noexcept = default;

  template <typename T>
  static void handle_type_tag(T*);

  template <auto Value>
  struct apitable_p2m {
    static constexpr auto value = Value;
  };
  template <typename T>
  struct p2m_to_api_member : apitable_p2m<nullptr> {};

#define QAIRT_INTERFACE_DECLARE_MEMBER(type, handle, name) \
  static handle handle_type_tag(type*);                    \
  type name;                                               \
  template <typename T>                                    \
  struct p2m_to_api_member<T type::*> : apitable_p2m<&LpaiBackendExtensionTable::name> {};

  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiBackend_HwInfoV1_t,
                                 QairtLpaiBackend_HwInfoHandle_t,
                                 lpaiBackendHwInfo)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiBackend_CustomConfigV1_t,
                                 QairtLpaiBackend_CustomConfigHandle_t,
                                 lpaiBackendCustomConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiBackend_RpcPollingConfigV1_t,
                                 QairtLpaiBackend_RpcPollingConfigHandle_t,
                                 lpaiBackendRpcPollingConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiContext_CustomConfigV1_t,
                                 QairtLpaiContext_CustomConfigHandle_t,
                                 lpaiContextCustomConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiGraph_MemConfigV1_t,
                                 QairtLpaiGraph_MemConfigHandle_t,
                                 lpaiGraphMemConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiGraph_PerfCfgV1_t,
                                 QairtLpaiGraph_PerfCfgHandle_t,
                                 lpaiGraphPerfCfg)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiGraph_CoreAffinityV1_t,
                                 QairtLpaiGraph_CoreAffinityHandle_t,
                                 lpaiGraphCoreAffinity)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiGraph_PrepareCfgV1_t,
                                 QairtLpaiGraph_PrepareCfgHandle_t,
                                 lpaiGraphPrepareCfg)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiGraph_CustomConfigV1_t,
                                 QairtLpaiGraph_CustomConfigHandle_t,
                                 lpaiGraphCustomConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiGraph_EnpuPerfCfgV1_t,
                                 QairtLpaiGraph_EnpuPerfCfgHandle_t,
                                 lpaiGraphEnpuPerfCfg)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiGraph_PropertiesV1_t,
                                 QairtGraph_Handle_t,
                                 lpaiGraphProperties)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiBackend_PropertiesV1_t,
                                 QairtBackend_Handle_t,
                                 lpaiBackendProperties)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiBackend_AlignmentReqV1_t,
                                 QairtLpaiBackend_AlignmentReqHandle_t,
                                 lpaiBackendAlignmentReq)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiBackend_eNPUCoreInfoV1_t,
                                 QairtLpaiBackend_eNPUCoreInfoHandle_t,
                                 lpaiBackendCoreInfo)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiBackend_eNPUCoreUtilDataV1_t,
                                 QairtLpaiBackend_eNPUCoreUtilDataHandle_t,
                                 lpaiBackendCoreUtilData)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiPerfInfrastructureV1_t,
                                 QairtLpaiPerfInfrastructure_Handle_t,
                                 lpaiPerfInfrastructure)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiPerfConfigV1_t,
                                 QairtLpaiPerfConfigHandle_t,
                                 lpaiPerfConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtLpaiPerfConfigListV1_t,
                                 QairtLpaiPerfConfigListHandle_t,
                                 lpaiPerfConfigList)

#undef QAIRT_INTERFACE_DECLARE_MEMBER

  template <typename T>
  struct interface_handle_type {
    using handle_type = decltype(handle_type_tag(std::declval<T*>()));
  };

  template <typename T>
  using interface_handle_type_t = typename interface_handle_type<T>::handle_type;

  template <auto P2m>
  struct p2mv_to_api_member : p2m_to_api_member<decltype(P2m)> {};

  template <auto P2m>
  inline static constexpr auto p2mv_to_api_member_v = p2mv_to_api_member<P2m>::value;

  template <auto P2m_InterfaceFn>
  const auto& getMemberInterface() const {
    static_assert(
        detail::ensure_not_nullptr_v<p2m_to_api_member<decltype(P2m_InterfaceFn)>::value>);
    return this->*p2mv_to_api_member_v<P2m_InterfaceFn>;
  }

  static LpaiBackendExtensionTable create(const std::shared_ptr<ApiTable>& apiTable,
                                          decltype(QairtInterface_getInterface) getInterfaceFn) {
    using detail::populateInterface;

    LpaiBackendExtensionTable lbe(apiTable);

    lbe.lpaiBackendHwInfo =
        populateInterface<QAIRT_LPAI_BACKEND_HW_INFO_ID,
                          &QairtLpaiBackend_HwInfoV1_t::create,
                          &QairtLpaiBackend_HwInfoV1_t::free,
                          &QairtLpaiBackend_HwInfoV1_t::setTarget,
                          &QairtLpaiBackend_HwInfoV1_t::getTarget,
                          &QairtLpaiBackend_HwInfoV1_t::setHwVersion,
                          &QairtLpaiBackend_HwInfoV1_t::getHwVersion>(getInterfaceFn);

    lbe.lpaiBackendCustomConfig =
        populateInterface<QAIRT_LPAI_BACKEND_CUSTOM_CONFIG_ID,
                          &QairtLpaiBackend_CustomConfigV1_t::create,
                          &QairtLpaiBackend_CustomConfigV1_t::free,
                          &QairtLpaiBackend_CustomConfigV1_t::setHwInfo,
                          &QairtLpaiBackend_CustomConfigV1_t::getHwInfo,
                          &QairtLpaiBackend_CustomConfigV1_t::setRpcPolling,
                          &QairtLpaiBackend_CustomConfigV1_t::getRpcPolling,
                          &QairtLpaiBackend_CustomConfigV1_t::setAixDump,
                          &QairtLpaiBackend_CustomConfigV1_t::getAixDump,
                          &QairtLpaiBackend_CustomConfigV1_t::setForceNhwc,
                          &QairtLpaiBackend_CustomConfigV1_t::getForceNhwc,
                          &QairtLpaiBackend_CustomConfigV1_t::setEnableCuRShift,
                          &QairtLpaiBackend_CustomConfigV1_t::getEnableCuRShift,
                          &QairtLpaiBackend_CustomConfigV1_t::setEnableFramerOpt,
                          &QairtLpaiBackend_CustomConfigV1_t::getEnableFramerOpt,
                          &QairtLpaiBackend_CustomConfigV1_t::setSearchElemBcastLayouts,
                          &QairtLpaiBackend_CustomConfigV1_t::getSearchElemBcastLayouts>(
            getInterfaceFn);

    lbe.lpaiBackendRpcPollingConfig =
        populateInterface<QAIRT_LPAI_BACKEND_RPC_POLLING_CONFIG_ID,
                          &QairtLpaiBackend_RpcPollingConfigV1_t::create,
                          &QairtLpaiBackend_RpcPollingConfigV1_t::free,
                          &QairtLpaiBackend_RpcPollingConfigV1_t::setEnable,
                          &QairtLpaiBackend_RpcPollingConfigV1_t::getEnable,
                          &QairtLpaiBackend_RpcPollingConfigV1_t::setDSPPollingTimeoutUs,
                          &QairtLpaiBackend_RpcPollingConfigV1_t::getDSPPollingTimeoutUs,
                          &QairtLpaiBackend_RpcPollingConfigV1_t::setHLOSWakeupLeadTimeUs,
                          &QairtLpaiBackend_RpcPollingConfigV1_t::getHLOSWakeupLeadTimeUs,
                          &QairtLpaiBackend_RpcPollingConfigV1_t::setHLOSPollingTimeoutUs,
                          &QairtLpaiBackend_RpcPollingConfigV1_t::getHLOSPollingTimeoutUs>(
            getInterfaceFn);

    lbe.lpaiContextCustomConfig =
        populateInterface<QAIRT_LPAI_CONTEXT_CUSTOM_CONFIG_ID,
                          &QairtLpaiContext_CustomConfigV1_t::create,
                          &QairtLpaiContext_CustomConfigV1_t::free,
                          &QairtLpaiContext_CustomConfigV1_t::setModelBufferMemType,
                          &QairtLpaiContext_CustomConfigV1_t::getModelBufferMemType,
                          &QairtLpaiContext_CustomConfigV1_t::setEnableIsland,
                          &QairtLpaiContext_CustomConfigV1_t::getEnableIsland,
                          &QairtLpaiContext_CustomConfigV1_t::setDisableAsync,
                          &QairtLpaiContext_CustomConfigV1_t::getDisableAsync,
                          &QairtLpaiContext_CustomConfigV1_t::setDumpDebugInfoFilePath,
                          &QairtLpaiContext_CustomConfigV1_t::getDumpDebugInfoFilePath,
                          &QairtLpaiContext_CustomConfigV1_t::setDebugInfoBuffer,
                          &QairtLpaiContext_CustomConfigV1_t::getDebugInfoBuffer>(getInterfaceFn);

    lbe.lpaiGraphMemConfig =
        populateInterface<QAIRT_LPAI_GRAPH_MEM_CONFIG_ID,
                          &QairtLpaiGraph_MemConfigV1_t::create,
                          &QairtLpaiGraph_MemConfigV1_t::free,
                          &QairtLpaiGraph_MemConfigV1_t::setMemType,
                          &QairtLpaiGraph_MemConfigV1_t::getMemType,
                          &QairtLpaiGraph_MemConfigV1_t::setSize,
                          &QairtLpaiGraph_MemConfigV1_t::getSize,
                          &QairtLpaiGraph_MemConfigV1_t::setAddr,
                          &QairtLpaiGraph_MemConfigV1_t::getAddr>(getInterfaceFn);

    lbe.lpaiGraphPerfCfg =
        populateInterface<QAIRT_LPAI_GRAPH_PERF_CFG_ID,
                          &QairtLpaiGraph_PerfCfgV1_t::create,
                          &QairtLpaiGraph_PerfCfgV1_t::free,
                          &QairtLpaiGraph_PerfCfgV1_t::setFps,
                          &QairtLpaiGraph_PerfCfgV1_t::getFps,
                          &QairtLpaiGraph_PerfCfgV1_t::setFtrtRatio,
                          &QairtLpaiGraph_PerfCfgV1_t::getFtrtRatio,
                          &QairtLpaiGraph_PerfCfgV1_t::setClientType,
                          &QairtLpaiGraph_PerfCfgV1_t::getClientType>(getInterfaceFn);

    lbe.lpaiGraphCoreAffinity =
        populateInterface<QAIRT_LPAI_GRAPH_CORE_AFFINITY_ID,
                          &QairtLpaiGraph_CoreAffinityV1_t::create,
                          &QairtLpaiGraph_CoreAffinityV1_t::free,
                          &QairtLpaiGraph_CoreAffinityV1_t::setAffinity,
                          &QairtLpaiGraph_CoreAffinityV1_t::getAffinity,
                          &QairtLpaiGraph_CoreAffinityV1_t::setCoreSelection,
                          &QairtLpaiGraph_CoreAffinityV1_t::getCoreSelection>(getInterfaceFn);

    lbe.lpaiGraphPrepareCfg =
        populateInterface<QAIRT_LPAI_GRAPH_PREPARE_CFG_ID,
                          &QairtLpaiGraph_PrepareCfgV1_t::create,
                          &QairtLpaiGraph_PrepareCfgV1_t::free,
                          &QairtLpaiGraph_PrepareCfgV1_t::setEnablePerLayer,
                          &QairtLpaiGraph_PrepareCfgV1_t::getEnablePerLayer,
                          &QairtLpaiGraph_PrepareCfgV1_t::setEnableCoreSelection,
                          &QairtLpaiGraph_PrepareCfgV1_t::getEnableCoreSelection>(getInterfaceFn);

    lbe.lpaiGraphCustomConfig =
        populateInterface<QAIRT_LPAI_GRAPH_CUSTOM_CONFIG_ID,
                          &QairtLpaiGraph_CustomConfigV1_t::create,
                          &QairtLpaiGraph_CustomConfigV1_t::free,
                          &QairtLpaiGraph_CustomConfigV1_t::setScratchMem,
                          &QairtLpaiGraph_CustomConfigV1_t::getScratchMem,
                          &QairtLpaiGraph_CustomConfigV1_t::setPersistentMem,
                          &QairtLpaiGraph_CustomConfigV1_t::getPersistentMem,
                          &QairtLpaiGraph_CustomConfigV1_t::setPerfCfg,
                          &QairtLpaiGraph_CustomConfigV1_t::getPerfCfg,
                          &QairtLpaiGraph_CustomConfigV1_t::setCoreAffinity,
                          &QairtLpaiGraph_CustomConfigV1_t::getCoreAffinity,
                          &QairtLpaiGraph_CustomConfigV1_t::setPauseExecution,
                          &QairtLpaiGraph_CustomConfigV1_t::setResumeExecution,
                          &QairtLpaiGraph_CustomConfigV1_t::setPrepareCfg,
                          &QairtLpaiGraph_CustomConfigV1_t::getPrepareCfg,
                          &QairtLpaiGraph_CustomConfigV1_t::setEnpuPerfCfg,
                          &QairtLpaiGraph_CustomConfigV1_t::getEnpuPerfCfg,
                          &QairtLpaiGraph_CustomConfigV1_t::setEnableEaixDump,
                          &QairtLpaiGraph_CustomConfigV1_t::getEnableEaixDump,
                          &QairtLpaiGraph_CustomConfigV1_t::setDebugCfg,
                          &QairtLpaiGraph_CustomConfigV1_t::getDebugCfg,
                          &QairtLpaiGraph_CustomConfigV1_t::setBatchMultipleSupport,
                          &QairtLpaiGraph_CustomConfigV1_t::getBatchMultipleSupport>(
            getInterfaceFn);

    lbe.lpaiGraphEnpuPerfCfg =
        populateInterface<QAIRT_LPAI_GRAPH_ENPU_PERF_CFG_ID,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::create,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::free,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::setFrameRate,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::getFrameRate,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::setEnpuClockScale,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::getEnpuClockScale,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::setEnpuFloorClockLevel,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::getEnpuFloorClockLevel,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::setEnpuBwScale,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::getEnpuBwScale,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::setEnpuFloorBw,
                          &QairtLpaiGraph_EnpuPerfCfgV1_t::getEnpuFloorBw>(getInterfaceFn);

    lbe.lpaiGraphProperties =
        populateInterface<QAIRT_LPAI_GRAPH_PROPERTIES_ID,
                          &QairtLpaiGraph_PropertiesV1_t::getScratchMemSize,
                          &QairtLpaiGraph_PropertiesV1_t::getPersistentMemSize,
                          &QairtLpaiGraph_PropertiesV1_t::getRequireRuntimeMemFromUser>(
            getInterfaceFn);

    lbe.lpaiBackendProperties =
        populateInterface<QAIRT_LPAI_BACKEND_PROPERTIES_ID,
                          &QairtLpaiBackend_PropertiesV1_t::getAlignmentReq,
                          &QairtLpaiBackend_PropertiesV1_t::getRequirePersistentBinary,
                          &QairtLpaiBackend_PropertiesV1_t::getEnpuCoreNum,
                          &QairtLpaiBackend_PropertiesV1_t::getEnpuCoreInfo,
                          &QairtLpaiBackend_PropertiesV1_t::setEnpuCoreUtilEnabled,
                          &QairtLpaiBackend_PropertiesV1_t::getEnpuCoreUtilData,
                          &QairtLpaiBackend_PropertiesV1_t::getEnpuClkFreq>(getInterfaceFn);

    lbe.lpaiBackendAlignmentReq =
        populateInterface<QAIRT_LPAI_BACKEND_ALIGNMENT_REQ_ID,
                          &QairtLpaiBackend_AlignmentReqV1_t::free,
                          &QairtLpaiBackend_AlignmentReqV1_t::getStartAddrAlignment,
                          &QairtLpaiBackend_AlignmentReqV1_t::getSizeAlignment>(getInterfaceFn);

    lbe.lpaiBackendCoreInfo =
        populateInterface<QAIRT_LPAI_BACKEND_ENPU_CORE_INFO_ID,
                          &QairtLpaiBackend_eNPUCoreInfoV1_t::free,
                          &QairtLpaiBackend_eNPUCoreInfoV1_t::getCoreIndex,
                          &QairtLpaiBackend_eNPUCoreInfoV1_t::getMacCnt,
                          &QairtLpaiBackend_eNPUCoreInfoV1_t::getVersion>(getInterfaceFn);

    lbe.lpaiBackendCoreUtilData =
        populateInterface<QAIRT_LPAI_BACKEND_ENPU_CORE_UTIL_DATA_ID,
                          &QairtLpaiBackend_eNPUCoreUtilDataV1_t::free,
                          &QairtLpaiBackend_eNPUCoreUtilDataV1_t::getSampleTs,
                          &QairtLpaiBackend_eNPUCoreUtilDataV1_t::getCycles,
                          &QairtLpaiBackend_eNPUCoreUtilDataV1_t::getClockKhz,
                          &QairtLpaiBackend_eNPUCoreUtilDataV1_t::getDdrBwKbps>(getInterfaceFn);

    lbe.lpaiPerfConfig =
        populateInterface<QAIRT_LPAI_PERF_CONFIG_ID,
                          &QairtLpaiPerfConfigV1_t::create,
                          &QairtLpaiPerfConfigV1_t::free,
                          &QairtLpaiPerfConfigV1_t::getOption,
                          &QairtLpaiPerfConfigV1_t::setEnpuPowerLevel,
                          &QairtLpaiPerfConfigV1_t::getEnpuPowerLevel>(getInterfaceFn);

    lbe.lpaiPerfConfigList =
        populateInterface<QAIRT_LPAI_PERF_CONFIG_LIST_ID,
                          &QairtLpaiPerfConfigListV1_t::create,
                          &QairtLpaiPerfConfigListV1_t::free,
                          &QairtLpaiPerfConfigListV1_t::addConfig,
                          &QairtLpaiPerfConfigListV1_t::getNumConfigs,
                          &QairtLpaiPerfConfigListV1_t::getConfigAt,
                          &QairtLpaiPerfConfigListV1_t::clear>(getInterfaceFn);

    lbe.lpaiPerfInfrastructure =
        populateInterface<QAIRT_LPAI_PERF_INFRASTRUCTURE_ID,
                          &QairtLpaiPerfInfrastructureV1_t::getPerfInfrastructure,
                          &QairtLpaiPerfInfrastructureV1_t::free,
                          &QairtLpaiPerfInfrastructureV1_t::setPowerConfig>(getInterfaceFn);

    return lbe;
  }
};

using LpaiBackendExtension = BackendExtension<LpaiBackendExtensionTable>;
static_assert(detail::is_backend_extension_v<LpaiBackendExtension>);

template <typename T, typename U>
using LpaiApiType = ApiType<T, U, LpaiBackendExtensionTable>;

}  // namespace qairt
