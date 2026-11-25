//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================
#pragma once

#include "../QairtApi.hpp"
#include "DSP/QairtDspBackend.h"
#include "DSP/QairtDspContext.h"
#include "DSP/QairtDspGraph.h"
#include "DSP/QairtDspPerfInfrastructure.h"

namespace qairt {

class DspBackendExtensionTable : public BackendExtensionTable<DspBackendExtensionTable> {
  friend class Api;
  using BackendExtensionTable::BackendExtensionTable;

 public:
  ~DspBackendExtensionTable() = default;
  DspBackendExtensionTable()  = delete;
  explicit DspBackendExtensionTable(const std::shared_ptr<ApiTable>& apiTable)
      : BackendExtensionTable(apiTable) {}
  DspBackendExtensionTable(const DspBackendExtensionTable&)                = delete;
  DspBackendExtensionTable(DspBackendExtensionTable&&) noexcept            = default;
  DspBackendExtensionTable& operator=(const DspBackendExtensionTable&)     = delete;
  DspBackendExtensionTable& operator=(DspBackendExtensionTable&&) noexcept = default;

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
  struct p2m_to_api_member<T type::*> : apitable_p2m<&DspBackendExtensionTable::name> {};

  QAIRT_INTERFACE_DECLARE_MEMBER(QairtDspBackend_CustomConfigV1_t,
                                 QairtDspBackend_CustomConfigHandle_t,
                                 backendCustomConfig);
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtDspContext_CustomConfigV1_t,
                                 QairtDspContext_CustomConfigHandle_t,
                                 contextCustomConfig);
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtDspGraph_CustomConfigV1_t,
                                 QairtDspGraph_CustomConfigHandle_t,
                                 graphCustomConfig);
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtDspPerfInfrastructureV1_t,
                                 QairtDspPerfInfrastructure_Handle_t,
                                 perfInfrastructure);
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtDspPerfPowerConfigV1_t,
                                 QairtDspPerfPowerConfigHandle_t,
                                 perfPowerConfig);
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtDspPerfPowerConfigListV1_t,
                                 QairtDspPerfPowerConfigListHandle_t,
                                 perfPowerConfigList);
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtDspPerfMemoryConfigV1_t,
                                 QairtDspPerfMemoryConfigHandle_t,
                                 perfMemoryConfig);
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtDspPerfMemoryConfigListV1_t,
                                 QairtDspPerfMemoryConfigListHandle_t,
                                 perfMemoryConfigList);
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtDspPerfThreadConfigV1_t,
                                 QairtDspPerfThreadConfigHandle_t,
                                 perfThreadConfig);
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtDspPerfThreadConfigListV1_t,
                                 QairtDspPerfThreadConfigListHandle_t,
                                 perfThreadConfigList);

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

  static DspBackendExtensionTable create(const std::shared_ptr<ApiTable>& apiTable,
                                         decltype(QairtInterface_getInterface) getInterfaceFn) {
    using detail::populateInterface;

    DspBackendExtensionTable dbe(apiTable);

    dbe.backendCustomConfig =
        populateInterface<QAIRT_DSP_BACKEND_CUSTOM_CONFIG_ID,
                          &QairtDspBackend_CustomConfigV1_t::create,
                          &QairtDspBackend_CustomConfigV1_t::free,
                          &QairtDspBackend_CustomConfigV1_t::setFoldReluActivationIntoConvOff,
                          &QairtDspBackend_CustomConfigV1_t::getFoldReluActivationIntoConvOff,
                          &QairtDspBackend_CustomConfigV1_t::setShortDepthConvOnHmxOff,
                          &QairtDspBackend_CustomConfigV1_t::getShortDepthConvOnHmxOff,
                          &QairtDspBackend_CustomConfigV1_t::setUseSignedProcessDomain,
                          &QairtDspBackend_CustomConfigV1_t::getUseSignedProcessDomain,
                          &QairtDspBackend_CustomConfigV1_t::setArch,
                          &QairtDspBackend_CustomConfigV1_t::getArch>(getInterfaceFn);

    dbe.contextCustomConfig =
        populateInterface<QAIRT_DSP_CONTEXT_CUSTOM_CONFIG_ID,
                          &QairtDspContext_CustomConfigV1_t::create,
                          &QairtDspContext_CustomConfigV1_t::free,
                          &QairtDspContext_CustomConfigV1_t::setFileReadMemoryBudget,
                          &QairtDspContext_CustomConfigV1_t::getFileReadMemoryBudget>(
            getInterfaceFn);

    dbe.graphCustomConfig =
        populateInterface<QAIRT_DSP_GRAPH_CUSTOM_CONFIG_ID,
                          &QairtDspGraph_CustomConfigV1_t::create,
                          &QairtDspGraph_CustomConfigV1_t::free,
                          &QairtDspGraph_CustomConfigV1_t::setOptimization,
                          &QairtDspGraph_CustomConfigV1_t::getOptimization,
                          &QairtDspGraph_CustomConfigV1_t::setEncoding,
                          &QairtDspGraph_CustomConfigV1_t::getEncoding,
                          &QairtDspGraph_CustomConfigV1_t::setPriority,
                          &QairtDspGraph_CustomConfigV1_t::getPriority,
                          &QairtDspGraph_CustomConfigV1_t::setPrecision,
                          &QairtDspGraph_CustomConfigV1_t::getPrecision>(getInterfaceFn);

    dbe.perfPowerConfig =
        populateInterface<QAIRT_DSP_PERF_POWER_CONFIG_ID,
                          &QairtDspPerfPowerConfigV1_t::create,
                          &QairtDspPerfPowerConfigV1_t::free,
                          &QairtDspPerfPowerConfigV1_t::getOption,
                          &QairtDspPerfPowerConfigV1_t::setDcvsEnable,
                          &QairtDspPerfPowerConfigV1_t::getDcvsEnable,
                          &QairtDspPerfPowerConfigV1_t::setSleepLatencyUs,
                          &QairtDspPerfPowerConfigV1_t::getSleepLatencyUs,
                          &QairtDspPerfPowerConfigV1_t::setSleepDisable,
                          &QairtDspPerfPowerConfigV1_t::getSleepDisable,
                          &QairtDspPerfPowerConfigV1_t::setDcvsPowerMode,
                          &QairtDspPerfPowerConfigV1_t::getDcvsPowerMode,
                          &QairtDspPerfPowerConfigV1_t::setDcvsVoltageCorners,
                          &QairtDspPerfPowerConfigV1_t::getDcvsVoltageCorners,
                          &QairtDspPerfPowerConfigV1_t::setBusVoltageCorners,
                          &QairtDspPerfPowerConfigV1_t::getBusVoltageCorners,
                          &QairtDspPerfPowerConfigV1_t::setCoreVoltageCorners,
                          &QairtDspPerfPowerConfigV1_t::getCoreVoltageCorners,
                          &QairtDspPerfPowerConfigV1_t::setRpcControlLatencyUs,
                          &QairtDspPerfPowerConfigV1_t::getRpcControlLatencyUs,
                          &QairtDspPerfPowerConfigV1_t::setRpcPollingTimeUs,
                          &QairtDspPerfPowerConfigV1_t::getRpcPollingTimeUs,
                          &QairtDspPerfPowerConfigV1_t::setHmxTimeoutIntervalUs,
                          &QairtDspPerfPowerConfigV1_t::getHmxTimeoutIntervalUs>(getInterfaceFn);

    dbe.perfPowerConfigList =
        populateInterface<QAIRT_DSP_PERF_POWER_CONFIG_LIST_ID,
                          &QairtDspPerfPowerConfigListV1_t::create,
                          &QairtDspPerfPowerConfigListV1_t::free,
                          &QairtDspPerfPowerConfigListV1_t::addPowerConfig,
                          &QairtDspPerfPowerConfigListV1_t::getNumPowerConfigs,
                          &QairtDspPerfPowerConfigListV1_t::getPowerConfigAt,
                          &QairtDspPerfPowerConfigListV1_t::clear>(getInterfaceFn);

    dbe.perfMemoryConfig =
        populateInterface<QAIRT_DSP_PERF_MEMORY_CONFIG_ID,
                          &QairtDspPerfMemoryConfigV1_t::create,
                          &QairtDspPerfMemoryConfigV1_t::free,
                          &QairtDspPerfMemoryConfigV1_t::getOption,
                          &QairtDspPerfMemoryConfigV1_t::setGrowSizeBytes,
                          &QairtDspPerfMemoryConfigV1_t::getGrowSizeBytes,
                          &QairtDspPerfMemoryConfigV1_t::setVtcmSizeMB,
                          &QairtDspPerfMemoryConfigV1_t::getVtcmSizeMB,
                          &QairtDspPerfMemoryConfigV1_t::setVtcmUsageFactor,
                          &QairtDspPerfMemoryConfigV1_t::getVtcmUsageFactor>(getInterfaceFn);

    dbe.perfMemoryConfigList =
        populateInterface<QAIRT_DSP_PERF_MEMORY_CONFIG_LIST_ID,
                          &QairtDspPerfMemoryConfigListV1_t::create,
                          &QairtDspPerfMemoryConfigListV1_t::free,
                          &QairtDspPerfMemoryConfigListV1_t::addMemoryConfig,
                          &QairtDspPerfMemoryConfigListV1_t::getNumMemoryConfigs,
                          &QairtDspPerfMemoryConfigListV1_t::getMemoryConfigAt,
                          &QairtDspPerfMemoryConfigListV1_t::clear>(getInterfaceFn);

    dbe.perfThreadConfig =
        populateInterface<QAIRT_DSP_PERF_THREAD_CONFIG_ID,
                          &QairtDspPerfThreadConfigV1_t::create,
                          &QairtDspPerfThreadConfigV1_t::free,
                          &QairtDspPerfThreadConfigV1_t::getOption,
                          &QairtDspPerfThreadConfigV1_t::setNumHvxThreads,
                          &QairtDspPerfThreadConfigV1_t::getNumHvxThreads>(getInterfaceFn);

    dbe.perfThreadConfigList =
        populateInterface<QAIRT_DSP_PERF_THREAD_CONFIG_LIST_ID,
                          &QairtDspPerfThreadConfigListV1_t::create,
                          &QairtDspPerfThreadConfigListV1_t::free,
                          &QairtDspPerfThreadConfigListV1_t::addThreadConfig,
                          &QairtDspPerfThreadConfigListV1_t::getNumThreadConfigs,
                          &QairtDspPerfThreadConfigListV1_t::getThreadConfigAt,
                          &QairtDspPerfThreadConfigListV1_t::clear>(getInterfaceFn);

    dbe.perfInfrastructure =
        populateInterface<QAIRT_DSP_PERF_INFRASTRUCTURE_ID,
                          &QairtDspPerfInfrastructureV1_t::getPerfInfrastructure,
                          &QairtDspPerfInfrastructureV1_t::free,
                          &QairtDspPerfInfrastructureV1_t::createPowerConfigId,
                          &QairtDspPerfInfrastructureV1_t::destroyPowerConfigId,
                          &QairtDspPerfInfrastructureV1_t::setPowerConfig,
                          &QairtDspPerfInfrastructureV1_t::setMemoryConfig,
                          &QairtDspPerfInfrastructureV1_t::setThreadConfig>(getInterfaceFn);

    return dbe;
  }
};

using DspBackendExtension = BackendExtension<DspBackendExtensionTable>;
static_assert(detail::is_backend_extension_v<DspBackendExtension>);

template <typename T, typename U>
using DspApiType = ApiType<T, U, DspBackendExtensionTable>;

}  // namespace qairt
