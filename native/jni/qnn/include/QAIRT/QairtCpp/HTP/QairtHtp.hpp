//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================
#pragma once

#include "../QairtApi.hpp"
#include "HTP/QairtHtpContext.h"
#include "HTP/QairtHtpDevice.h"
#include "HTP/QairtHtpGraph.h"
#include "HTP/QairtHtpPerfInfrastructure.h"

namespace qairt {

class HtpBackendExtensionTable : public BackendExtensionTable<HtpBackendExtensionTable> {
  friend class Api;
  using BackendExtensionTable::BackendExtensionTable;

 public:
  ~HtpBackendExtensionTable() = default;
  HtpBackendExtensionTable()  = delete;
  explicit HtpBackendExtensionTable(const std::shared_ptr<ApiTable>& apiTable)
      : BackendExtensionTable(apiTable) {}
  HtpBackendExtensionTable(const HtpBackendExtensionTable&)                = delete;
  HtpBackendExtensionTable(HtpBackendExtensionTable&&) noexcept            = default;
  HtpBackendExtensionTable& operator=(const HtpBackendExtensionTable&)     = delete;
  HtpBackendExtensionTable& operator=(HtpBackendExtensionTable&&) noexcept = default;

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
  struct p2m_to_api_member<T type::*> : apitable_p2m<&HtpBackendExtensionTable::name> {};

  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpContext_GroupRegistrationV1_t,
                                 QairtHtpContext_GroupRegistrationHandle_t,
                                 groupRegistration)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpContext_CustomConfigV1_t,
                                 QairtHtpContext_CustomConfigHandle_t,
                                 customConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpGraph_CustomConfigV1_t,
                                 QairtHtpGraph_CustomConfigHandle_t,
                                 graphCustomConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpGraph_OptimizationOptionV1_t,
                                 QairtHtpGraph_OptimizationOptionHandle_t,
                                 graphOptimizationOption)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtp_HmxBoundingInfoV1_t,
                                 QairtHtp_HmxBoundingInfoHandle_t,
                                 hmxBoundingInfo)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpDevice_CustomConfigV1_t,
                                 QairtHtpDevice_CustomConfigHandle_t,
                                 deviceCustomConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpContext_PropertiesV1_t,
                                 QairtContext_Handle_t,
                                 contextProperties)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpPerfInfrastructureV1_t,
                                 QairtHtpPerfInfrastructure_Handle_t,
                                 perfInfrastructure)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpPerfDcvsV3ConfigV1_t,
                                 QairtHtpPerfDcvsV3ConfigHandle_t,
                                 perfDcvsV3Config)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpPerfHmxV2ConfigV1_t,
                                 QairtHtpPerfHmxV2ConfigHandle_t,
                                 perfHmxV2Config)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpPerfCengConfigV1_t,
                                 QairtHtpPerfCengConfigHandle_t,
                                 perfCengConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpPerfDcvsV3ExpConfigV1_t,
                                 QairtHtpPerfDcvsV3ExpConfigHandle_t,
                                 perfDcvsV3ExpConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpPerfCengExpConfigV1_t,
                                 QairtHtpPerfCengExpConfigHandle_t,
                                 perfCengExpConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpPerfPowerConfigV1_t,
                                 QairtHtpPerfPowerConfigHandle_t,
                                 perfPowerConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpPerfPowerConfigListV1_t,
                                 QairtHtpPerfPowerConfigListHandle_t,
                                 perfPowerConfigList)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpPerfMemoryConfigV1_t,
                                 QairtHtpPerfMemoryConfigHandle_t,
                                 perfMemoryConfig)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpPerfMemoryConfigListV1_t,
                                 QairtHtpPerfMemoryConfigListHandle_t,
                                 perfMemoryConfigList)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpContext_GraphSplitV1_t,
                                 QairtHtpContext_GraphSplitHandle_t,
                                 graphSplit)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpContext_LoraPreloadGroupV1_t,
                                 QairtHtpContext_LoraPreloadGroupHandle_t,
                                 loraPreloadGroup)
  QAIRT_INTERFACE_DECLARE_MEMBER(QairtHtpContext_LoraPreloadBufferGroupsV1_t,
                                 QairtHtpContext_LoraPreloadBufferGroupsHandle_t,
                                 loraPreloadBufferGroups)

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

  static HtpBackendExtensionTable create(const std::shared_ptr<ApiTable>& apiTable,
                                         decltype(QairtInterface_getInterface) getInterfaceFn) {
    using detail::populateInterface;

    HtpBackendExtensionTable hbe(apiTable);
    hbe.groupRegistration =
        populateInterface<QAIRT_HTP_CONTEXT_GROUP_REGISTRATION_ID,
                          &QairtHtpContext_GroupRegistrationV1_t::create,
                          &QairtHtpContext_GroupRegistrationV1_t::free,
                          &QairtHtpContext_GroupRegistrationV1_t::setFirstGroupHandle,
                          &QairtHtpContext_GroupRegistrationV1_t::getFirstGroupHandle,
                          &QairtHtpContext_GroupRegistrationV1_t::setMaxSpillFill,
                          &QairtHtpContext_GroupRegistrationV1_t::getMaxSpillFill>(getInterfaceFn);

    hbe.customConfig =
        populateInterface<QAIRT_HTP_CONTEXT_CUSTOM_CONFIG_ID,
                          &QairtHtpContext_CustomConfigV1_t::create,
                          &QairtHtpContext_CustomConfigV1_t::free,
                          &QairtHtpContext_CustomConfigV1_t::setEnableWeightSharing,
                          &QairtHtpContext_CustomConfigV1_t::getEnableWeightSharing,
                          &QairtHtpContext_CustomConfigV1_t::setGroupRegistration,
                          &QairtHtpContext_CustomConfigV1_t::getGroupRegistration,
                          &QairtHtpContext_CustomConfigV1_t::setFileReadMemoryBudget,
                          &QairtHtpContext_CustomConfigV1_t::getFileReadMemoryBudget,
                          &QairtHtpContext_CustomConfigV1_t::setEnableDspMemoryProfiling,
                          &QairtHtpContext_CustomConfigV1_t::getEnableDspMemoryProfiling,
                          &QairtHtpContext_CustomConfigV1_t::setShareResources,
                          &QairtHtpContext_CustomConfigV1_t::getShareResources,
                          &QairtHtpContext_CustomConfigV1_t::setIoMemEstimation,
                          &QairtHtpContext_CustomConfigV1_t::getIoMemEstimation,
                          &QairtHtpContext_CustomConfigV1_t::setIsPrepareOnly,
                          &QairtHtpContext_CustomConfigV1_t::getIsPrepareOnly,
                          &QairtHtpContext_CustomConfigV1_t::setInitAcceleration,
                          &QairtHtpContext_CustomConfigV1_t::getInitAcceleration,
                          &QairtHtpContext_CustomConfigV1_t::setSkipBinarySectionValidation,
                          &QairtHtpContext_CustomConfigV1_t::getSkipBinarySectionValidation,
                          &QairtHtpContext_CustomConfigV1_t::setShareResourcesOptimizationType,
                          &QairtHtpContext_CustomConfigV1_t::getShareResourcesOptimizationType,
                          &QairtHtpContext_CustomConfigV1_t::setUseExtendedUdma,
                          &QairtHtpContext_CustomConfigV1_t::getUseExtendedUdma,
                          &QairtHtpContext_CustomConfigV1_t::setConcurrentGroupRegistration,
                          &QairtHtpContext_CustomConfigV1_t::getConcurrentGroupRegistration,
                          &QairtHtpContext_CustomConfigV1_t::setLoraWeightSharingEnabled,
                          &QairtHtpContext_CustomConfigV1_t::getLoraWeightSharingEnabled,
                          &QairtHtpContext_CustomConfigV1_t::setGraphSplit,
                          &QairtHtpContext_CustomConfigV1_t::getGraphSplit,
                          &QairtHtpContext_CustomConfigV1_t::setGraphSplittingEnabled,
                          &QairtHtpContext_CustomConfigV1_t::getGraphSplittingEnabled,
                          &QairtHtpContext_CustomConfigV1_t::setNumPrepareThreads,
                          &QairtHtpContext_CustomConfigV1_t::getNumPrepareThreads,
                          &QairtHtpContext_CustomConfigV1_t::setLoraWeightSharingRamPreloadEnabled,
                          &QairtHtpContext_CustomConfigV1_t::getLoraWeightSharingRamPreloadEnabled,
                          &QairtHtpContext_CustomConfigV1_t::setReusedIoLimitMb,
                          &QairtHtpContext_CustomConfigV1_t::getReusedIoLimitMb,
                          &QairtHtpContext_CustomConfigV1_t::setReferenceWeightSharingEnabled,
                          &QairtHtpContext_CustomConfigV1_t::getReferenceWeightSharingEnabled,
                          &QairtHtpContext_CustomConfigV1_t::setConcurrentDeserializationPatchFd,
                          &QairtHtpContext_CustomConfigV1_t::getConcurrentDeserializationPatchFd,
                          &QairtHtpContext_CustomConfigV1_t::setLoraPreloadBufferGroups,
                          &QairtHtpContext_CustomConfigV1_t::getLoraPreloadBufferGroups>(getInterfaceFn);

    hbe.loraPreloadGroup =
        populateInterface<QAIRT_HTP_CONTEXT_LORA_PRELOAD_GROUP_ID,
                          &QairtHtpContext_LoraPreloadGroupV1_t::create,
                          &QairtHtpContext_LoraPreloadGroupV1_t::free,
                          &QairtHtpContext_LoraPreloadGroupV1_t::setGraphNames,
                          &QairtHtpContext_LoraPreloadGroupV1_t::getNumGraphs,
                          &QairtHtpContext_LoraPreloadGroupV1_t::getGraphNameAt>(getInterfaceFn);

    hbe.loraPreloadBufferGroups =
        populateInterface<QAIRT_HTP_CONTEXT_LORA_PRELOAD_BUFFER_GROUPS_ID,
                          &QairtHtpContext_LoraPreloadBufferGroupsV1_t::create,
                          &QairtHtpContext_LoraPreloadBufferGroupsV1_t::free,
                          &QairtHtpContext_LoraPreloadBufferGroupsV1_t::addGroup,
                          &QairtHtpContext_LoraPreloadBufferGroupsV1_t::getNumGroups,
                          &QairtHtpContext_LoraPreloadBufferGroupsV1_t::getGroupAt,
                          &QairtHtpContext_LoraPreloadBufferGroupsV1_t::clear>(getInterfaceFn);

    hbe.graphCustomConfig =
        populateInterface<QAIRT_HTP_GRAPH_CUSTOM_CONFIG_ID,
                          &QairtHtpGraph_CustomConfigV1_t::create,
                          &QairtHtpGraph_CustomConfigV1_t::free,
                          &QairtHtpGraph_CustomConfigV1_t::setOptimizationOption,
                          &QairtHtpGraph_CustomConfigV1_t::getOptimizationOption,
                          &QairtHtpGraph_CustomConfigV1_t::setPrecision,
                          &QairtHtpGraph_CustomConfigV1_t::getPrecision,
                          &QairtHtpGraph_CustomConfigV1_t::setVtcmSizeInMB,
                          &QairtHtpGraph_CustomConfigV1_t::getVtcmSizeInMB,
                          &QairtHtpGraph_CustomConfigV1_t::setVtcmSizeInBytes,
                          &QairtHtpGraph_CustomConfigV1_t::getVtcmSizeInBytes,
                          &QairtHtpGraph_CustomConfigV1_t::setFoldReluActivationIntoConvOff,
                          &QairtHtpGraph_CustomConfigV1_t::getFoldReluActivationIntoConvOff,
                          &QairtHtpGraph_CustomConfigV1_t::setShortDepthConvOnHmxOff,
                          &QairtHtpGraph_CustomConfigV1_t::getShortDepthConvOnHmxOff,
                          &QairtHtpGraph_CustomConfigV1_t::setNumHvxThreads,
                          &QairtHtpGraph_CustomConfigV1_t::getNumHvxThreads,
                          &QairtHtpGraph_CustomConfigV1_t::setNumCores,
                          &QairtHtpGraph_CustomConfigV1_t::getNumCores,
                          &QairtHtpGraph_CustomConfigV1_t::setHmxBoundingInfo,
                          &QairtHtpGraph_CustomConfigV1_t::getHmxBoundingInfo,
                          &QairtHtpGraph_CustomConfigV1_t::setWeightsPacking,
                          &QairtHtpGraph_CustomConfigV1_t::getWeightsPacking,
                          &QairtHtpGraph_CustomConfigV1_t::setAssumeSameQuant,
                          &QairtHtpGraph_CustomConfigV1_t::getAssumeSameQuant,
                          &QairtHtpGraph_CustomConfigV1_t::setShareIOBuffer,
                          &QairtHtpGraph_CustomConfigV1_t::getShareIOBuffer,
                          &QairtHtpGraph_CustomConfigV1_t::setAdvancedActivationFusion,
                          &QairtHtpGraph_CustomConfigV1_t::getAdvancedActivationFusion,
                          &QairtHtpGraph_CustomConfigV1_t::setHighPrecisionSigmoid,
                          &QairtHtpGraph_CustomConfigV1_t::getHighPrecisionSigmoid,
                          &QairtHtpGraph_CustomConfigV1_t::setMonolithicLstm,
                          &QairtHtpGraph_CustomConfigV1_t::getMonolithicLstm,
                          &QairtHtpGraph_CustomConfigV1_t::setFp16ClampOverflow,
                          &QairtHtpGraph_CustomConfigV1_t::getFp16ClampOverflow,
                          &QairtHtpGraph_CustomConfigV1_t::setPrecisionCompensation,
                          &QairtHtpGraph_CustomConfigV1_t::getPrecisionCompensation,
                          &QairtHtpGraph_CustomConfigV1_t::setEnablePredication,
                          &QairtHtpGraph_CustomConfigV1_t::getEnablePredication>(getInterfaceFn);

    hbe.graphOptimizationOption =
        populateInterface<QAIRT_HTP_GRAPH_OPTIMIZATION_OPTION_ID,
                          &QairtHtpGraph_OptimizationOptionV1_t::create,
                          &QairtHtpGraph_OptimizationOptionV1_t::free,
                          &QairtHtpGraph_OptimizationOptionV1_t::setType,
                          &QairtHtpGraph_OptimizationOptionV1_t::getType,
                          &QairtHtpGraph_OptimizationOptionV1_t::setFloatValue,
                          &QairtHtpGraph_OptimizationOptionV1_t::getFloatValue>(getInterfaceFn);

    hbe.hmxBoundingInfo =
        populateInterface<QAIRT_HTP_GRAPH_HMX_BOUNDING_INFO_ID,
                          &QairtHtp_HmxBoundingInfoV1_t::create,
                          &QairtHtp_HmxBoundingInfoV1_t::free,
                          &QairtHtp_HmxBoundingInfoV1_t::setTargetHmxFreqHz,
                          &QairtHtp_HmxBoundingInfoV1_t::getTargetHmxFreqHz,
                          &QairtHtp_HmxBoundingInfoV1_t::setTargetDspCoreFreq,
                          &QairtHtp_HmxBoundingInfoV1_t::getTargetDspCoreFreq>(getInterfaceFn);

    hbe.deviceCustomConfig =
        populateInterface<QAIRT_HTP_DEVICE_CUSTOM_CONFIG_ID,
                          &QairtHtpDevice_CustomConfigV1_t::create,
                          &QairtHtpDevice_CustomConfigV1_t::free,
                          &QairtHtpDevice_CustomConfigV1_t::setSocModel,
                          &QairtHtpDevice_CustomConfigV1_t::getSocModel,
                          &QairtHtpDevice_CustomConfigV1_t::setArch,
                          &QairtHtpDevice_CustomConfigV1_t::getArch,
                          &QairtHtpDevice_CustomConfigV1_t::setUseSignedProcessDomain,
                          &QairtHtpDevice_CustomConfigV1_t::getUseSignedProcessDomain,
                          &QairtHtpDevice_CustomConfigV1_t::setUseSecureProcessDomain,
                          &QairtHtpDevice_CustomConfigV1_t::getUseSecureProcessDomain,
                          &QairtHtpDevice_CustomConfigV1_t::setEnableAsyncExecute,
                          &QairtHtpDevice_CustomConfigV1_t::getEnableAsyncExecute,
                          &QairtHtpDevice_CustomConfigV1_t::setUseDynamicPartitioning,
                          &QairtHtpDevice_CustomConfigV1_t::getUseDynamicPartitioning>(
            getInterfaceFn);

    hbe.contextProperties =
        populateInterface<QAIRT_HTP_CONTEXT_PROPERTIES_ID,
                          &QairtHtpContext_PropertiesV1_t::getBufferStartAlignment,
                          &QairtHtpContext_PropertiesV1_t::getSpillfillBufferSize,
                          &QairtHtpContext_PropertiesV1_t::getWeightsBufferSize,
                          &QairtHtpContext_PropertiesV1_t::getVtcmBackupBufferSize>(getInterfaceFn);

    hbe.perfDcvsV3Config =
        populateInterface<QAIRT_HTP_PERF_DCVS_V3_CONFIG_ID,
                          &QairtHtpPerfDcvsV3ConfigV1_t::create,
                          &QairtHtpPerfDcvsV3ConfigV1_t::free,
                          &QairtHtpPerfDcvsV3ConfigV1_t::setContextId,
                          &QairtHtpPerfDcvsV3ConfigV1_t::getContextId,
                          &QairtHtpPerfDcvsV3ConfigV1_t::setDcvsEnable,
                          &QairtHtpPerfDcvsV3ConfigV1_t::getDcvsEnable,
                          &QairtHtpPerfDcvsV3ConfigV1_t::setPowerMode,
                          &QairtHtpPerfDcvsV3ConfigV1_t::getPowerMode,
                          &QairtHtpPerfDcvsV3ConfigV1_t::setSleepLatency,
                          &QairtHtpPerfDcvsV3ConfigV1_t::getSleepLatency,
                          &QairtHtpPerfDcvsV3ConfigV1_t::setSleepDisable,
                          &QairtHtpPerfDcvsV3ConfigV1_t::getSleepDisable,
                          &QairtHtpPerfDcvsV3ConfigV1_t::setBusVoltageCorners,
                          &QairtHtpPerfDcvsV3ConfigV1_t::getBusVoltageCorners,
                          &QairtHtpPerfDcvsV3ConfigV1_t::setCoreVoltageCorners,
                          &QairtHtpPerfDcvsV3ConfigV1_t::getCoreVoltageCorners>(getInterfaceFn);

    hbe.perfHmxV2Config =
        populateInterface<QAIRT_HTP_PERF_HMX_V2_CONFIG_ID,
                          &QairtHtpPerfHmxV2ConfigV1_t::create,
                          &QairtHtpPerfHmxV2ConfigV1_t::free,
                          &QairtHtpPerfHmxV2ConfigV1_t::setPickDefault,
                          &QairtHtpPerfHmxV2ConfigV1_t::getPickDefault,
                          &QairtHtpPerfHmxV2ConfigV1_t::setVoltageCorners,
                          &QairtHtpPerfHmxV2ConfigV1_t::getVoltageCorners,
                          &QairtHtpPerfHmxV2ConfigV1_t::setPerfMode,
                          &QairtHtpPerfHmxV2ConfigV1_t::getPerfMode>(getInterfaceFn);

    hbe.perfCengConfig =
        populateInterface<QAIRT_HTP_PERF_CENG_CONFIG_ID,
                          &QairtHtpPerfCengConfigV1_t::create,
                          &QairtHtpPerfCengConfigV1_t::free,
                          &QairtHtpPerfCengConfigV1_t::setVoltageCorners,
                          &QairtHtpPerfCengConfigV1_t::getVoltageCorners,
                          &QairtHtpPerfCengConfigV1_t::setPerfMode,
                          &QairtHtpPerfCengConfigV1_t::getPerfMode>(getInterfaceFn);

    hbe.perfDcvsV3ExpConfig =
        populateInterface<QAIRT_HTP_PERF_DCVS_V3_EXP_CONFIG_ID,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::create,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::free,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::setContextId,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::getContextId,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::setPowerMode,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::getPowerMode,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::setSleepLatency,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::getSleepLatency,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::setSleepDisable,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::getSleepDisable,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::setBusVoltageCorners,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::getBusVoltageCorners,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::setCoreVoltageCorners,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::getCoreVoltageCorners,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::setPerfModes,
                          &QairtHtpPerfDcvsV3ExpConfigV1_t::getPerfModes>(getInterfaceFn);

    hbe.perfCengExpConfig =
        populateInterface<QAIRT_HTP_PERF_CENG_EXP_CONFIG_ID,
                          &QairtHtpPerfCengExpConfigV1_t::create,
                          &QairtHtpPerfCengExpConfigV1_t::free,
                          &QairtHtpPerfCengExpConfigV1_t::setVoltageCorners,
                          &QairtHtpPerfCengExpConfigV1_t::getVoltageCorners,
                          &QairtHtpPerfCengExpConfigV1_t::setPerfMode,
                          &QairtHtpPerfCengExpConfigV1_t::getPerfMode>(getInterfaceFn);

    hbe.perfPowerConfig =
        populateInterface<QAIRT_HTP_PERF_POWER_CONFIG_ID,
                          &QairtHtpPerfPowerConfigV1_t::create,
                          &QairtHtpPerfPowerConfigV1_t::free,
                          &QairtHtpPerfPowerConfigV1_t::getOption,
                          &QairtHtpPerfPowerConfigV1_t::setDcvsV3,
                          &QairtHtpPerfPowerConfigV1_t::getDcvsV3,
                          &QairtHtpPerfPowerConfigV1_t::setHmxV2,
                          &QairtHtpPerfPowerConfigV1_t::getHmxV2,
                          &QairtHtpPerfPowerConfigV1_t::setCeng,
                          &QairtHtpPerfPowerConfigV1_t::getCeng,
                          &QairtHtpPerfPowerConfigV1_t::setDcvsV3Exp,
                          &QairtHtpPerfPowerConfigV1_t::getDcvsV3Exp,
                          &QairtHtpPerfPowerConfigV1_t::setCengExp,
                          &QairtHtpPerfPowerConfigV1_t::getCengExp,
                          &QairtHtpPerfPowerConfigV1_t::setRpcControlLatency,
                          &QairtHtpPerfPowerConfigV1_t::getRpcControlLatency,
                          &QairtHtpPerfPowerConfigV1_t::setRpcPollingTime,
                          &QairtHtpPerfPowerConfigV1_t::getRpcPollingTime,
                          &QairtHtpPerfPowerConfigV1_t::setHmxTimeoutIntervalUs,
                          &QairtHtpPerfPowerConfigV1_t::getHmxTimeoutIntervalUs,
                          &QairtHtpPerfPowerConfigV1_t::setAdaptivePollingTime,
                          &QairtHtpPerfPowerConfigV1_t::getAdaptivePollingTime,
                          &QairtHtpPerfPowerConfigV1_t::setDdrPerfMode,
                          &QairtHtpPerfPowerConfigV1_t::getDdrPerfMode>(getInterfaceFn);

    hbe.perfPowerConfigList =
        populateInterface<QAIRT_HTP_PERF_POWER_CONFIG_LIST_ID,
                          &QairtHtpPerfPowerConfigListV1_t::create,
                          &QairtHtpPerfPowerConfigListV1_t::free,
                          &QairtHtpPerfPowerConfigListV1_t::addPowerConfig,
                          &QairtHtpPerfPowerConfigListV1_t::getNumPowerConfigs,
                          &QairtHtpPerfPowerConfigListV1_t::getPowerConfigAt,
                          &QairtHtpPerfPowerConfigListV1_t::clear>(getInterfaceFn);

    hbe.perfMemoryConfig =
        populateInterface<QAIRT_HTP_PERF_MEMORY_CONFIG_ID,
                          &QairtHtpPerfMemoryConfigV1_t::create,
                          &QairtHtpPerfMemoryConfigV1_t::free,
                          &QairtHtpPerfMemoryConfigV1_t::getOption,
                          &QairtHtpPerfMemoryConfigV1_t::setGrowSize,
                          &QairtHtpPerfMemoryConfigV1_t::getGrowSize>(getInterfaceFn);

    hbe.perfMemoryConfigList =
        populateInterface<QAIRT_HTP_PERF_MEMORY_CONFIG_LIST_ID,
                          &QairtHtpPerfMemoryConfigListV1_t::create,
                          &QairtHtpPerfMemoryConfigListV1_t::free,
                          &QairtHtpPerfMemoryConfigListV1_t::addMemoryConfig,
                          &QairtHtpPerfMemoryConfigListV1_t::getNumMemoryConfigs,
                          &QairtHtpPerfMemoryConfigListV1_t::getMemoryConfigAt,
                          &QairtHtpPerfMemoryConfigListV1_t::clear>(getInterfaceFn);

    hbe.perfInfrastructure =
        populateInterface<QAIRT_HTP_PERF_INFRASTRUCTURE_ID,
                          &QairtHtpPerfInfrastructureV1_t::getPerfInfrastructure,
                          &QairtHtpPerfInfrastructureV1_t::free,
                          &QairtHtpPerfInfrastructureV1_t::createPowerConfigId,
                          &QairtHtpPerfInfrastructureV1_t::destroyPowerConfigId,
                          &QairtHtpPerfInfrastructureV1_t::setPowerConfig,
                          &QairtHtpPerfInfrastructureV1_t::setMemoryConfig>(getInterfaceFn);
    hbe.graphSplit =
        populateInterface<QAIRT_HTP_CONTEXT_GRAPH_SPLIT_ID,
                          &QairtHtpContext_GraphSplitV1_t::create,
                          &QairtHtpContext_GraphSplitV1_t::free,
                          &QairtHtpContext_GraphSplitV1_t::setGraphSplittingEnabled,
                          &QairtHtpContext_GraphSplitV1_t::getGraphSplittingEnabled>(getInterfaceFn);

    return hbe;
  }
};

using HtpBackendExtension = BackendExtension<HtpBackendExtensionTable>;
static_assert(detail::is_backend_extension_v<HtpBackendExtension>);

template <typename T, typename U>
using HtpApiType = ApiType<T, U, HtpBackendExtensionTable>;

}  // namespace qairt
