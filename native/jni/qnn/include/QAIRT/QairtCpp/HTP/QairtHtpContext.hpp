//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================
#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../QairtContext.hpp"
#include "HTP/QairtHtpContext.h"
#include "QairtHtp.hpp"

namespace qairt {

QAIRT_CPPAPI_DEFINE_ENUM(
    HtpContextShareResourcesOptimizationType,
    QairtHtpContext_ShareResourcesOptimizationType_t,
    SequentialWithVaOptimization =
        QAIRT_HTP_CONTEXT_RESOURCE_OPTIMIZATION_SEQUENTIAL_WITH_VA_OPTIMIZATION,
    SequentialWithoutVaOptimization =
        QAIRT_HTP_CONTEXT_RESOURCE_OPTIMIZATION_SEQUENTIAL_WITHOUT_VA_OPTIMIZATION,
    ConcurrentOptimization = QAIRT_HTP_CONTEXT_RESOURCE_OPTIMIZATION_CONCURRENT_OPTIMIZATION,
    Undefined              = 0x7fffffff)

class HtpContextGroupRegistration
    : public HtpApiType<HtpContextGroupRegistration, QairtHtpContext_GroupRegistrationV1_t> {
  template <typename>
  friend class BackendExtension;

  detail::crossable<detail::non_owning<Context>,
                    &interface_type::getFirstGroupHandle,
                    &interface_type::setFirstGroupHandle>
      m_firstGroup;
  friend detail::TempWorkaround;

 public:
  using base_type::base_type;
  HtpContextGroupRegistration() = default;
  HtpContextGroupRegistration(const HtpContextGroupRegistration& other)
      : base_type(copy_table_tag, other) {
    construct();
    setMaxSpillFill(other.getMaxSpillFill());
    setFirstGroup(other.getFirstGroup());
  }
  HtpContextGroupRegistration(HtpContextGroupRegistration&& other) noexcept
      : base_type(static_cast<base_type&&>(other)), m_firstGroup(std::move(other.m_firstGroup)) {}
  HtpContextGroupRegistration& operator=(const HtpContextGroupRegistration& other) {
    if (this != &other) {
      *this = HtpContextGroupRegistration(other);
    }
    return *this;
  }
  HtpContextGroupRegistration& operator=(HtpContextGroupRegistration&& other) noexcept {
    if (this != &other) {
      m_firstGroup = std::move(other.m_firstGroup);
      base_type::operator=(std::move(other));
    }
    return *this;
  }
  ~HtpContextGroupRegistration() { QAIRT_TRIVIAL }

  void setMaxSpillFill(uint64_t spillFill) {
    callSetter<&interface_type::setMaxSpillFill>(spillFill);
  }
  uint64_t getMaxSpillFill() const { return callGetter<&interface_type::getMaxSpillFill>(); }

  const Context& getFirstGroup() const { return m_firstGroup.bindNonOwning(*this); }

  void setFirstGroup(const Context& context) { m_firstGroup.setNonOwning(*this, context); }

 private:
  explicit HtpContextGroupRegistration(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(apiTable.lock()) {
    construct();
  }
};

class HtpContextGraphSplit : public HtpApiType<HtpContextGraphSplit, QairtHtpContext_GraphSplitV1_t> {
  template <typename>
  friend class BackendExtension;
  friend detail::TempWorkaround;

 public:
  using base_type::base_type;
  HtpContextGraphSplit() = default;
  HtpContextGraphSplit(const HtpContextGraphSplit& other) : base_type(copy_table_tag, other) {
    construct();
    setGraphSplittingEnabled(other.getGraphSplittingEnabled());
  }
  HtpContextGraphSplit(HtpContextGraphSplit&& other) noexcept : base_type(static_cast<base_type&&>(other)) {}
  HtpContextGraphSplit& operator=(HtpContextGraphSplit&& other) noexcept {
    if (this != &other) {
      base_type::operator=(std::move(other));
    }
    return *this;
  }
  HtpContextGraphSplit& operator=(const HtpContextGraphSplit& other) {
    if (this != &other) {
      *this = HtpContextGraphSplit(other);
    }
    return *this;
  }
  ~HtpContextGraphSplit() { QAIRT_TRIVIAL }

  void setGraphSplittingEnabled(bool enable) {
    callSetter<&interface_type::setGraphSplittingEnabled>(detail::convertBool(enable));
  }
  bool getGraphSplittingEnabled() const {
    return detail::resolveBool(callGetter<&interface_type::getGraphSplittingEnabled>());
  }

 private:
  explicit HtpContextGraphSplit(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(apiTable.lock()) {
    construct();
  }
};

class HtpContextLoraPreloadGroup
    : public HtpApiType<HtpContextLoraPreloadGroup, QairtHtpContext_LoraPreloadGroupV1_t> {
  template <typename>
  friend class BackendExtension;
  friend detail::TempWorkaround;

 public:
  using base_type::base_type;
  HtpContextLoraPreloadGroup() = default;
  HtpContextLoraPreloadGroup(HtpContextLoraPreloadGroup&& other) noexcept
      : base_type(static_cast<base_type&&>(other)) {}
  HtpContextLoraPreloadGroup& operator=(HtpContextLoraPreloadGroup&& other) noexcept {
    if (this != &other) {
      base_type::operator=(std::move(other));
    }
    return *this;
  }
  HtpContextLoraPreloadGroup(const HtpContextLoraPreloadGroup&)            = delete;
  HtpContextLoraPreloadGroup& operator=(const HtpContextLoraPreloadGroup&) = delete;
  ~HtpContextLoraPreloadGroup() { QAIRT_TRIVIAL }

  void setGraphNames(const char** graphNames, uint32_t numGraphs) {
    callAndConfirm<&interface_type::setGraphNames>(graphNames, numGraphs);
  }
  void setGraphNames(const std::vector<std::string>& graphNames) {
    std::vector<const char*> graphNamePtrs;
    graphNamePtrs.reserve(graphNames.size());
    for (const auto& graphName : graphNames) {
      graphNamePtrs.push_back(graphName.c_str());
    }
    setGraphNames(graphNamePtrs.empty() ? nullptr : graphNamePtrs.data(),
                  static_cast<uint32_t>(graphNamePtrs.size()));
  }
  uint32_t getNumGraphs() const { return callGetter<&interface_type::getNumGraphs>(); }
  const char* getGraphNameAt(uint32_t index) const {
    const char* graphName{};
    callAndConfirm<&interface_type::getGraphNameAt>(index, &graphName);
    return graphName;
  }

 private:
  explicit HtpContextLoraPreloadGroup(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(apiTable.lock()) {
    construct();
  }
};

class HtpContextLoraPreloadBufferGroups
    : public HtpApiType<HtpContextLoraPreloadBufferGroups,
                        QairtHtpContext_LoraPreloadBufferGroupsV1_t> {
  template <typename>
  friend class BackendExtension;
  friend detail::TempWorkaround;

 public:
  using base_type::base_type;
  HtpContextLoraPreloadBufferGroups() = default;
  HtpContextLoraPreloadBufferGroups(HtpContextLoraPreloadBufferGroups&& other) noexcept
      : base_type(static_cast<base_type&&>(other)) {}
  HtpContextLoraPreloadBufferGroups& operator=(
      HtpContextLoraPreloadBufferGroups&& other) noexcept {
    if (this != &other) {
      base_type::operator=(std::move(other));
    }
    return *this;
  }
  HtpContextLoraPreloadBufferGroups(const HtpContextLoraPreloadBufferGroups&)            = delete;
  HtpContextLoraPreloadBufferGroups& operator=(const HtpContextLoraPreloadBufferGroups&) = delete;
  ~HtpContextLoraPreloadBufferGroups() { QAIRT_TRIVIAL }

  void addGroup(const HtpContextLoraPreloadGroup& loraPreloadGroup) {
    callAndConfirm<&interface_type::addGroup>(getHandle(loraPreloadGroup));
  }
  uint32_t getNumGroups() const { return callGetter<&interface_type::getNumGroups>(); }
  HtpContextLoraPreloadGroup getGroupAt(uint32_t index) const {
    QairtHtpContext_LoraPreloadGroupHandle_t handle{};
    callAndConfirm<&interface_type::getGroupAt>(index, &handle);
    return constructNonOwning<HtpContextLoraPreloadGroup>(handle);
  }
  void clear() { callAndConfirm<&interface_type::clear>(); }

 private:
  explicit HtpContextLoraPreloadBufferGroups(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(apiTable.lock()) {
    construct();
  }
};

class HtpContextCustomConfig
    : public HtpApiType<HtpContextCustomConfig, QairtHtpContext_CustomConfigV1_t>,
      public ContextCustomConfig {
  detail::crossable<detail::non_owning<HtpContextGroupRegistration>,
                    &interface_type::getConcurrentGroupRegistration,
                    &interface_type::setConcurrentGroupRegistration>
      m_concurrentGroupRegistration;
  detail::crossable<detail::non_owning<HtpContextGroupRegistration>,
                    &interface_type::getGroupRegistration,
                    &interface_type::setGroupRegistration>
      m_groupRegistration;
  detail::crossable<detail::non_owning<HtpContextGraphSplit>,
                    &interface_type::getGraphSplit,
                    &interface_type::setGraphSplit>
      m_graphSplit;

  friend detail::TempWorkaround;
  template <typename>
  friend class BackendExtension;

 public:
  QairtContext_CustomConfigHandle_t getCustomConfigHandle() const override { return getHandle(); }

  HtpContextCustomConfig()                                             = default;
  HtpContextCustomConfig(const HtpContextCustomConfig&)                = delete;
  HtpContextCustomConfig(HtpContextCustomConfig&&) noexcept            = default;
  HtpContextCustomConfig& operator=(const HtpContextCustomConfig&)     = delete;
  HtpContextCustomConfig& operator=(HtpContextCustomConfig&&) noexcept = default;

  void setEnableWeightSharing(bool enable) {
    callSetter<&interface_type::setEnableWeightSharing>(detail::convertBool(enable));
  }
  bool getEnableWeightSharing() const {
    return detail::resolveBool(callGetter<&interface_type::getEnableWeightSharing>());
  }

  void setGroupRegistration(const HtpContextGroupRegistration& groupRegistration) {
    m_groupRegistration.setNonOwning(*this, groupRegistration);
  }
  HtpContextGroupRegistration& getGroupRegistration() {
    return m_groupRegistration.bindNonOwning(*this);
  }

  void setFileReadMemoryBudget(uint64_t budgetInMb) {
    callSetter<&interface_type::setFileReadMemoryBudget>(budgetInMb);
  }
  uint64_t getFileReadMemoryBudget() const {
    return callGetter<&interface_type::getFileReadMemoryBudget>();
  }

  void setEnableDspMemoryProfiling(bool enable) {
    callSetter<&interface_type::setEnableDspMemoryProfiling>(detail::convertBool(enable));
  }
  bool getEnableDspMemoryProfiling() const {
    return detail::resolveBool(callGetter<&interface_type::getEnableDspMemoryProfiling>());
  }

  void setShareResources(bool share) {
    callSetter<&interface_type::setShareResources>(detail::convertBool(share));
  }
  bool getShareResources() const {
    return detail::resolveBool(callGetter<&interface_type::getShareResources>());
  }

  void setIoMemEstimation(bool enable) {
    callSetter<&interface_type::setIoMemEstimation>(detail::convertBool(enable));
  }
  bool getIoMemEstimation() const {
    return detail::resolveBool(callGetter<&interface_type::getIoMemEstimation>());
  }

  void setIsPrepareOnly(bool isPrepareOnly) {
    callSetter<&interface_type::setIsPrepareOnly>(detail::convertBool(isPrepareOnly));
  }
  bool getIsPrepareOnly() const {
    return detail::resolveBool(callGetter<&interface_type::getIsPrepareOnly>());
  }

  void setInitAcceleration(bool enable) {
    callSetter<&interface_type::setInitAcceleration>(detail::convertBool(enable));
  }
  bool getInitAcceleration() const {
    return detail::resolveBool(callGetter<&interface_type::getInitAcceleration>());
  }

  void setSkipBinarySectionValidation(bool skip) {
    callSetter<&interface_type::setSkipBinarySectionValidation>(detail::convertBool(skip));
  }
  bool getSkipBinarySectionValidation() const {
    return detail::resolveBool(callGetter<&interface_type::getSkipBinarySectionValidation>());
  }

  void setShareResourcesOptimizationType(HtpContextShareResourcesOptimizationType type) {
    callSetter<&interface_type::setShareResourcesOptimizationType>(
        static_cast<QairtHtpContext_ShareResourcesOptimizationType_t>(type));
  }
  HtpContextShareResourcesOptimizationType getShareResourcesOptimizationType() const {
    return static_cast<HtpContextShareResourcesOptimizationType>(
        callGetter<&interface_type::getShareResourcesOptimizationType>());
  }

  void setUseExtendedUdma(bool enable) {
    callSetter<&interface_type::setUseExtendedUdma>(detail::convertBool(enable));
  }
  bool getUseExtendedUdma() const {
    return detail::resolveBool(callGetter<&interface_type::getUseExtendedUdma>());
  }

  void setConcurrentGroupRegistration(const HtpContextGroupRegistration& groupRegistration) {
    m_concurrentGroupRegistration.setNonOwning(*this, groupRegistration);
  }
  HtpContextGroupRegistration& getConcurrentGroupRegistration() {
    return m_concurrentGroupRegistration.bindNonOwning(*this);
  }

  void setLoraWeightSharingEnabled(bool enable) {
    callSetter<&interface_type::setLoraWeightSharingEnabled>(detail::convertBool(enable));
  }
  bool getLoraWeightSharingEnabled() const {
    return detail::resolveBool(callGetter<&interface_type::getLoraWeightSharingEnabled>());
  }

  void setGraphSplit(const HtpContextGraphSplit& graphSplit) {
    static_cast<void>(m_graphSplit.setNonOwning(*this, graphSplit));
  }
  HtpContextGraphSplit& getGraphSplit() { return m_graphSplit.bindNonOwning(*this); }
  void setLoraWeightSharingRamPreloadEnabled(bool enable) {
    callSetter<&interface_type::setLoraWeightSharingRamPreloadEnabled>(
        detail::convertBool(enable));
  }
  bool getLoraWeightSharingRamPreloadEnabled() const {
    return detail::resolveBool(
        callGetter<&interface_type::getLoraWeightSharingRamPreloadEnabled>());
  }

  void setLoraPreloadBufferGroups(
      const HtpContextLoraPreloadBufferGroups& loraPreloadBufferGroups) {
    callSetter<&interface_type::setLoraPreloadBufferGroups>(
        getHandle(loraPreloadBufferGroups));
  }
  HtpContextLoraPreloadBufferGroups getLoraPreloadBufferGroups() const {
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t loraPreloadBufferGroups{};
    callAndConfirm<&interface_type::getLoraPreloadBufferGroups>(&loraPreloadBufferGroups);
    return constructNonOwning<HtpContextLoraPreloadBufferGroups>(loraPreloadBufferGroups);
  }

  void setReusedIoLimitMb(uint64_t limitMb) {
    callSetter<&interface_type::setReusedIoLimitMb>(limitMb);
  }
  uint64_t getReusedIoLimitMb() const {
    return callGetter<&interface_type::getReusedIoLimitMb>();
  }

  void setReferenceWeightSharingEnabled(bool enable) {
    callSetter<&interface_type::setReferenceWeightSharingEnabled>(detail::convertBool(enable));
  }
  bool getReferenceWeightSharingEnabled() const {
    return detail::resolveBool(callGetter<&interface_type::getReferenceWeightSharingEnabled>());
  }

  void setGraphSplittingEnabled(bool enable) {
    callSetter<&interface_type::setGraphSplittingEnabled>(detail::convertBool(enable));
  }
  bool getGraphSplittingEnabled() const {
    return detail::resolveBool(callGetter<&interface_type::getGraphSplittingEnabled>());
  }

  void setNumPrepareThreads(uint32_t numPrepareThreads) {
    callSetter<&interface_type::setNumPrepareThreads>(numPrepareThreads);
  }
  uint32_t getNumPrepareThreads() const {
    return callGetter<&interface_type::getNumPrepareThreads>();
  }
  void setConcurrentDeserializationPatchFd(int32_t patchFd) {
    callSetter<&interface_type::setConcurrentDeserializationPatchFd>(patchFd);
  }
  int32_t getConcurrentDeserializationPatchFd() const {
    return callGetter<&interface_type::getConcurrentDeserializationPatchFd>();
  }

 private:
  explicit HtpContextCustomConfig(const std::weak_ptr<api_table_type>& apiTable)
      : ApiType(apiTable.lock()) {
    if (!apiTable.lock()) {
      throw std::runtime_error("Failed to lock apiTable");
    }
    construct();

    if (!getHandle()) {
      throw std::runtime_error("Failed to construct HtpContextCustomConfig");
    }
  }
};

class HtpContextCustomConfiguration : public detail::TabularBEType<HtpBackendExtensionTable>,
                                      public ContextCustomConfiguration {
  std::optional<bool> m_enableWeightSharing{};
  std::optional<HtpContextGroupRegistration> m_groupRegistration{};
  std::optional<uint64_t> m_fileReadMemoryBudgetInMb{};
  std::optional<bool> m_dspMemoryProfilingEnabled{};
  std::optional<bool> m_shareResources{};
  std::optional<HtpContextShareResourcesOptimizationType> m_shareResourcesOptimizationType{};
  std::optional<bool> m_ioMemEstimation{};
  std::optional<bool> m_isPrepareOnly{};
  std::optional<bool> m_initAcceleration{};
  std::optional<bool> m_skipValidationOnBinarySection{};
  std::optional<bool> m_useExtendedUdma{};
  std::optional<HtpContextGroupRegistration> m_concurrentGroupRegistration{};
  std::optional<bool> m_loraWeightSharingEnabled{};
  std::optional<HtpContextGraphSplit> m_graphSplit{};
  std::optional<bool> m_graphSplittingEnabled{};
  std::optional<uint32_t> m_numPrepareThreads{};
  std::optional<bool> m_loraWeightSharingRamPreloadEnabled{};
  std::optional<std::vector<std::vector<std::string>>> m_loraPreloadBufferGroups{};
  std::optional<uint64_t> m_reusedIoLimitMb{};
  std::optional<bool> m_referenceWeightSharingEnabled{};
  std::optional<int32_t> m_concurrentDeserializationPatchFd{};

  mutable std::vector<HtpContextCustomConfig> m_customCfgs;
  friend Context;

 public:
  std::vector<QairtContext_CustomConfigHandle_t> getCustomConfigs() const override {
    std::vector<QairtContext_CustomConfigHandle_t> toret;
    m_customCfgs = getCustomConfigs2();
    toret.reserve(m_customCfgs.size());

    for (auto& e : m_customCfgs) {
      toret.push_back(e.getCustomConfigHandle());
    }
    return toret;
  }
  std::vector<HtpContextCustomConfig> getCustomConfigs2() const {
    if (!m_backendExtensionTable.lock()) {
      throw std::runtime_error("Malformed HtpContextCustomConfiguration: missing API table.");
    }
    std::vector<HtpContextCustomConfig> toret;
    if (m_enableWeightSharing.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setEnableWeightSharing(m_enableWeightSharing.value());
    }
    if (m_groupRegistration.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setGroupRegistration(m_groupRegistration.value());
    }
    if (m_fileReadMemoryBudgetInMb.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setFileReadMemoryBudget(m_fileReadMemoryBudgetInMb.value());
    }
    if (m_dspMemoryProfilingEnabled.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setEnableDspMemoryProfiling(m_dspMemoryProfilingEnabled.value());
    }
    if (m_shareResources.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setShareResources(m_shareResources.value());
    }
    if (m_shareResourcesOptimizationType.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setShareResourcesOptimizationType(m_shareResourcesOptimizationType.value());
    }
    if (m_ioMemEstimation.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setIoMemEstimation(m_ioMemEstimation.value());
    }
    if (m_isPrepareOnly.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setIsPrepareOnly(m_isPrepareOnly.value());
    }
    if (m_initAcceleration.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setInitAcceleration(m_initAcceleration.value());
    }
    if (m_skipValidationOnBinarySection.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setSkipBinarySectionValidation(m_skipValidationOnBinarySection.value());
    }
    if (m_useExtendedUdma.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setUseExtendedUdma(m_useExtendedUdma.value());
    }
    if (m_concurrentGroupRegistration.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setConcurrentGroupRegistration(m_concurrentGroupRegistration.value());
    }
    if (m_loraWeightSharingEnabled.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setLoraWeightSharingEnabled(m_loraWeightSharingEnabled.value());
    }
    if (m_graphSplit.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setGraphSplit(m_graphSplit.value());
    }
    if (m_loraWeightSharingRamPreloadEnabled.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setLoraWeightSharingRamPreloadEnabled(m_loraWeightSharingRamPreloadEnabled.value());
    }
    if (m_loraPreloadBufferGroups.has_value() && !m_loraPreloadBufferGroups->empty()) {
      HtpContextLoraPreloadBufferGroups groups = make<HtpContextLoraPreloadBufferGroups>();
      for (const auto& group : *m_loraPreloadBufferGroups) {
        HtpContextLoraPreloadGroup loraPreloadGroup = make<HtpContextLoraPreloadGroup>();
        loraPreloadGroup.setGraphNames(group);
        groups.addGroup(loraPreloadGroup);
      }
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setLoraPreloadBufferGroups(groups);
    }
    if (m_reusedIoLimitMb.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setReusedIoLimitMb(m_reusedIoLimitMb.value());
    }
    if (m_referenceWeightSharingEnabled.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setReferenceWeightSharingEnabled(m_referenceWeightSharingEnabled.value());
    }
    if (m_graphSplittingEnabled.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setGraphSplittingEnabled(m_graphSplittingEnabled.value());
    }
    if (m_numPrepareThreads.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setNumPrepareThreads(m_numPrepareThreads.value());
    }
    if (m_concurrentDeserializationPatchFd.has_value()) {
      HtpContextCustomConfig& back = toret.emplace_back(make<HtpContextCustomConfig>());
      back.setConcurrentDeserializationPatchFd(m_concurrentDeserializationPatchFd.value());
    }

    return toret;
  }

 public:
  using base_type::base_type;

  // Enable Weight Sharing
  void clearEnableWeightSharing() { m_enableWeightSharing.reset(); }
  void setEnableWeightSharing(bool enableWeightSharing) {
    m_enableWeightSharing = enableWeightSharing;
  }
  std::optional<bool>& getEnableWeightSharing() { return m_enableWeightSharing; }
  const std::optional<bool>& getEnableWeightSharing() const { return m_enableWeightSharing; }

  // Group Registration
  void clearGroupRegistration() { m_groupRegistration.reset(); }
  void setGroupRegistration(HtpContextGroupRegistration groupRegistration) {
    m_groupRegistration = std::move(groupRegistration);
  }
  std::optional<HtpContextGroupRegistration>& getGroupRegistration() { return m_groupRegistration; }
  const std::optional<HtpContextGroupRegistration>& getGroupRegistration() const {
    return m_groupRegistration;
  }

  // File Read Memory Budget
  void clearFileReadMemoryBudget() { m_fileReadMemoryBudgetInMb.reset(); }
  void setFileReadMemoryBudget(uint64_t budgetInMb) { m_fileReadMemoryBudgetInMb = budgetInMb; }
  std::optional<uint64_t>& getFileReadMemoryBudget() { return m_fileReadMemoryBudgetInMb; }
  const std::optional<uint64_t>& getFileReadMemoryBudget() const {
    return m_fileReadMemoryBudgetInMb;
  }

  // DSP Memory Profiling Enabled
  void clearDspMemoryProfilingEnabled() { m_dspMemoryProfilingEnabled.reset(); }
  void setDspMemoryProfilingEnabled(bool enable) { m_dspMemoryProfilingEnabled = enable; }
  std::optional<bool>& getDspMemoryProfilingEnabled() { return m_dspMemoryProfilingEnabled; }
  const std::optional<bool>& getDspMemoryProfilingEnabled() const {
    return m_dspMemoryProfilingEnabled;
  }

  // Share Resources
  void clearShareResources() { m_shareResources.reset(); }
  void setShareResources(bool share) { m_shareResources = share; }
  std::optional<bool>& getShareResources() { return m_shareResources; }
  const std::optional<bool>& getShareResources() const { return m_shareResources; }

  // Share Resources Optimization Type
  void clearShareResourcesOptimizationType() { m_shareResourcesOptimizationType.reset(); }
  void setShareResourcesOptimizationType(HtpContextShareResourcesOptimizationType type) {
    m_shareResourcesOptimizationType = type;
  }
  std::optional<HtpContextShareResourcesOptimizationType>& getShareResourcesOptimizationType() {
    return m_shareResourcesOptimizationType;
  }
  const std::optional<HtpContextShareResourcesOptimizationType>& getShareResourcesOptimizationType()
      const {
    return m_shareResourcesOptimizationType;
  }

  // IO Mem Estimation
  void clearIoMemEstimation() { m_ioMemEstimation.reset(); }
  void setIoMemEstimation(bool enable) { m_ioMemEstimation = enable; }
  std::optional<bool>& getIoMemEstimation() { return m_ioMemEstimation; }
  const std::optional<bool>& getIoMemEstimation() const { return m_ioMemEstimation; }

  // Is Prepare Only
  void clearIsPrepareOnly() { m_isPrepareOnly.reset(); }
  void setIsPrepareOnly(bool isPrepareOnly) { m_isPrepareOnly = isPrepareOnly; }
  std::optional<bool>& getIsPrepareOnly() { return m_isPrepareOnly; }
  const std::optional<bool>& getIsPrepareOnly() const { return m_isPrepareOnly; }

  // Init Acceleration
  void clearInitAcceleration() { m_initAcceleration.reset(); }
  void setInitAcceleration(bool enable) { m_initAcceleration = enable; }
  std::optional<bool>& getInitAcceleration() { return m_initAcceleration; }
  const std::optional<bool>& getInitAcceleration() const { return m_initAcceleration; }

  // Skip Validation On Binary Section
  void clearSkipValidationOnBinarySection() { m_skipValidationOnBinarySection.reset(); }
  void setSkipValidationOnBinarySection(bool skip) { m_skipValidationOnBinarySection = skip; }
  std::optional<bool>& getSkipValidationOnBinarySection() {
    return m_skipValidationOnBinarySection;
  }
  const std::optional<bool>& getSkipValidationOnBinarySection() const {
    return m_skipValidationOnBinarySection;
  }

  // Use Extended UDMA
  void clearUseExtendedUdma() { m_useExtendedUdma.reset(); }
  void setUseExtendedUdma(bool enable) { m_useExtendedUdma = enable; }
  std::optional<bool>& getUseExtendedUdma() { return m_useExtendedUdma; }
  const std::optional<bool>& getUseExtendedUdma() const { return m_useExtendedUdma; }

  // Concurrent Group Registration
  void clearConcurrentGroupRegistration() { m_concurrentGroupRegistration.reset(); }
  void setConcurrentGroupRegistration(HtpContextGroupRegistration groupRegistration) {
    m_concurrentGroupRegistration = std::move(groupRegistration);
  }
  std::optional<HtpContextGroupRegistration>& getConcurrentGroupRegistration() {
    return m_concurrentGroupRegistration;
  }
  const std::optional<HtpContextGroupRegistration>& getConcurrentGroupRegistration() const {
    return m_concurrentGroupRegistration;
  }

  // LoRA Weight Sharing Enabled
  void clearLoraWeightSharingEnabled() { m_loraWeightSharingEnabled.reset(); }
  void setLoraWeightSharingEnabled(bool enable) { m_loraWeightSharingEnabled = enable; }
  std::optional<bool>& getLoraWeightSharingEnabled() { return m_loraWeightSharingEnabled; }
  const std::optional<bool>& getLoraWeightSharingEnabled() const {
    return m_loraWeightSharingEnabled;
  }

  // Graph Split
  void clearGraphSplit() { m_graphSplit.reset(); }
  void setGraphSplit(HtpContextGraphSplit graphSplit) { m_graphSplit = std::move(graphSplit); }
  std::optional<HtpContextGraphSplit>& getGraphSplit() { return m_graphSplit; }
  const std::optional<HtpContextGraphSplit>& getGraphSplit() const { return m_graphSplit; }

  // LoRA Weight Sharing RAM Preload Enabled
  void clearLoraWeightSharingRamPreloadEnabled() {
    m_loraWeightSharingRamPreloadEnabled.reset();
  }
  void setLoraWeightSharingRamPreloadEnabled(bool enable) {
    m_loraWeightSharingRamPreloadEnabled = enable;
  }
  std::optional<bool>& getLoraWeightSharingRamPreloadEnabled() {
    return m_loraWeightSharingRamPreloadEnabled;
  }
  const std::optional<bool>& getLoraWeightSharingRamPreloadEnabled() const {
    return m_loraWeightSharingRamPreloadEnabled;
  }

  // LoRA Preload Buffer Groups
  void clearLoraPreloadBufferGroups() { m_loraPreloadBufferGroups.reset(); }
  void setLoraPreloadBufferGroups(std::vector<std::vector<std::string>> groups) {
    m_loraPreloadBufferGroups = std::move(groups);
  }
  std::optional<std::vector<std::vector<std::string>>>& getLoraPreloadBufferGroups() {
    return m_loraPreloadBufferGroups;
  }
  const std::optional<std::vector<std::vector<std::string>>>& getLoraPreloadBufferGroups() const {
    return m_loraPreloadBufferGroups;
  }

  // Reused IO Limit
  void clearReusedIoLimitMb() { m_reusedIoLimitMb.reset(); }
  void setReusedIoLimitMb(uint64_t limitMb) { m_reusedIoLimitMb = limitMb; }
  std::optional<uint64_t>& getReusedIoLimitMb() { return m_reusedIoLimitMb; }
  const std::optional<uint64_t>& getReusedIoLimitMb() const { return m_reusedIoLimitMb; }

  // Reference Weight Sharing Enabled
  void clearReferenceWeightSharingEnabled() { m_referenceWeightSharingEnabled.reset(); }
  void setReferenceWeightSharingEnabled(bool enable) {
    m_referenceWeightSharingEnabled = enable;
  }
  std::optional<bool>& getReferenceWeightSharingEnabled() {
    return m_referenceWeightSharingEnabled;
  }
  const std::optional<bool>& getReferenceWeightSharingEnabled() const {
    return m_referenceWeightSharingEnabled;
  }

  // Graph Splitting Enabled
  void clearGraphSplittingEnabled() { m_graphSplittingEnabled.reset(); }
  void setGraphSplittingEnabled(bool enable) { m_graphSplittingEnabled = enable; }
  std::optional<bool>& getGraphSplittingEnabled() { return m_graphSplittingEnabled; }
  const std::optional<bool>& getGraphSplittingEnabled() const { return m_graphSplittingEnabled; }

  // Num Prepare Threads
  void clearNumPrepareThreads() { m_numPrepareThreads.reset(); }
  void setNumPrepareThreads(uint32_t numPrepareThreads) {
    m_numPrepareThreads = numPrepareThreads;
  }
  std::optional<uint32_t>& getNumPrepareThreads() { return m_numPrepareThreads; }
  const std::optional<uint32_t>& getNumPrepareThreads() const { return m_numPrepareThreads; }
  // Concurrent Deserialization Patch
  void clearConcurrentDeserializationPatchFd() { m_concurrentDeserializationPatchFd.reset(); }
  void setConcurrentDeserializationPatchFd(int32_t patchFd) {
    m_concurrentDeserializationPatchFd = patchFd;
  }
  std::optional<int32_t>& getConcurrentDeserializationPatchFd() {
    return m_concurrentDeserializationPatchFd;
  }
  const std::optional<int32_t>& getConcurrentDeserializationPatchFd() const {
    return m_concurrentDeserializationPatchFd;
  }
};

/// C++ wrapper for HTP context properties.
/// Constructed via BackendExtension::make<HtpContextProperties>(context).
class HtpContextProperties : public HtpApiType<HtpContextProperties, QairtHtpContext_PropertiesV1_t> {
  template <typename>
  friend class BackendExtension;

 public:
  using base_type::base_type;

  HtpContextProperties() = default;

  // Non-owning: binds the context handle so the getters can be dispatched with
  // it. Does not own or free the context.
  explicit HtpContextProperties(const std::weak_ptr<api_table_type>& apiTable, const Context& context)
      : base_type(apiTable.lock()) {
    setHandle(detail::QairtTypeAdmin::getHandle(context));
    setNonOwning(true);
  }

  HtpContextProperties(const HtpContextProperties&)                = delete;
  HtpContextProperties& operator=(const HtpContextProperties&)     = delete;
  HtpContextProperties(HtpContextProperties&&) noexcept            = default;
  HtpContextProperties& operator=(HtpContextProperties&&) noexcept = default;

  uint64_t getBufferStartAlignment() const {
    return callGetter<&interface_type::getBufferStartAlignment>();
  }

  uint64_t getSpillfillBufferSize() const {
    return callGetter<&interface_type::getSpillfillBufferSize>();
  }

  uint64_t getWeightsBufferSize() const {
    return callGetter<&interface_type::getWeightsBufferSize>();
  }

  uint64_t getVtcmBackupBufferSize() const {
    return callGetter<&interface_type::getVtcmBackupBufferSize>();
  }
};

}  // namespace qairt
