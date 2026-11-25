//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================

/**
 * @file
 * @brief C++ wrapper for the QAIRT context API.
 *
 *        A Backend must be created before constructing a Context object.
 */

#pragma once

#include <unordered_map>

#include "QairtApi.hpp"
#include "QairtContext/QairtContext.h"
#include "QairtContext/QairtContextConfig.h"
#include "QairtDevice.hpp"
#include "QairtGraph.hpp"
#include "QairtLog.hpp"
#include "QairtMem.hpp"
#include "QairtProfile.hpp"

namespace qairt {

/**
 * @brief Binary compatibility policy used when loading a cached context binary.
 *
 * @details
 * | Enumerator   | Description                                                              |
 * |--------------|--------------------------------------------------------------------------|
 * | `Permissive` | Binary is accepted if it can run on the device. Default policy.          |
 * | `Strict`     | Binary is accepted only if it fully utilizes hardware capability.        |
 * | `Undefined`  | Sentinel value; not a valid policy selection.                            |
 */
QAIRT_CPPAPI_DEFINE_ENUM(ContextBinaryCompatibilityType,
                         QairtContext_BinaryCompatibilityType_t,
                         Permissive = QAIRT_CONTEXT_BINARY_COMPATIBILITY_PERMISSIVE,
                         Strict     = QAIRT_CONTEXT_BINARY_COMPATIBILITY_STRICT,
                         Undefined  = QAIRT_CONTEXT_BINARY_COMPATIBILITY_TYPE_UNDEFINED)

/**
 * @brief Error codes returned by QAIRT context operations.
 *
 * @details
 * | Enumerator              | Description                                                        |
 * |-------------------------|--------------------------------------------------------------------|
 * | `NoError`               | Operation succeeded.                                               |
 * | `UnsupportedFeature`    | An optional API feature is not supported by the backend.           |
 * | `MemAlloc`              | Memory allocation or deallocation failure.                         |
 * | `InvalidArgument`       | An argument to the operation was invalid.                          |
 * | `CtxDoesNotExist`       | The context has not yet been created in the backend.               |
 * | `InvalidHandle`         | The provided handle is not valid.                                  |
 * | `NotFinalized`          | Operation attempted before all graphs in the context were finalized.|
 * | `BinaryVersion`         | The context binary has an incompatible version.                    |
 * | `CreateFromBinary`      | Failed to create a context from a binary.                          |
 * | `GetBinarySizeFailed`   | Failed to retrieve the size of the serialized context.             |
 * | `GetBinaryFailed`       | Failed to generate the serialized context.                         |
 * | `BinaryConfiguration`   | The context binary configuration is invalid.                       |
 * | `SetProfile`            | Failed to set profiling information.                               |
 * | `InvalidConfig`         | One or more configuration values are invalid.                      |
 * | `BinarySuboptimal`      | A suboptimal binary was used with strict compatibility mode.       |
 * | `Aborted`               | Call was aborted early due to a signal trigger.                    |
 * | `TimedOut`              | Call was aborted early due to a signal timeout.                    |
 * | `IncrementInvalidBuffer`| The incremental binary buffer was not allocated by the backend.    |
 * | `Undefined`             | An undefined or unknown error occurred.                            |
 */
QAIRT_CPPAPI_DEFINE_ENUM(ContextError,
                         QairtContext_Error_t,
                         NoError                = QAIRT_CONTEXT_NO_ERROR,
                         UnsupportedFeature     = QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE,
                         MemAlloc               = QAIRT_CONTEXT_ERROR_MEM_ALLOC,
                         InvalidArgument        = QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT,
                         CtxDoesNotExist        = QAIRT_CONTEXT_ERROR_CTX_DOES_NOT_EXIST,
                         InvalidHandle          = QAIRT_CONTEXT_ERROR_INVALID_HANDLE,
                         NotFinalized           = QAIRT_CONTEXT_ERROR_NOT_FINALIZED,
                         BinaryVersion          = QAIRT_CONTEXT_ERROR_BINARY_VERSION,
                         CreateFromBinary       = QAIRT_CONTEXT_ERROR_CREATE_FROM_BINARY,
                         GetBinarySizeFailed    = QAIRT_CONTEXT_ERROR_GET_BINARY_SIZE_FAILED,
                         GetBinaryFailed        = QAIRT_CONTEXT_ERROR_GET_BINARY_FAILED,
                         BinaryConfiguration    = QAIRT_CONTEXT_ERROR_BINARY_CONFIGURATION,
                         SetProfile             = QAIRT_CONTEXT_ERROR_SET_PROFILE,
                         InvalidConfig          = QAIRT_CONTEXT_ERROR_INVALID_CONFIG,
                         BinarySuboptimal       = QAIRT_CONTEXT_ERROR_BINARY_SUBOPTIMAL,
                         Aborted                = QAIRT_CONTEXT_ERROR_ABORTED,
                         TimedOut               = QAIRT_CONTEXT_ERROR_TIMED_OUT,
                         IncrementInvalidBuffer = QAIRT_CONTEXT_ERROR_INCREMENT_INVALID_BUFFER,
                         Undefined              = QAIRT_CONTEXT_ERROR_UNDEFINED)

/**
 * @brief Storage format of a context binary.
 *
 * @details
 * | Enumerator   | Description                                              |
 * |--------------|----------------------------------------------------------|
 * | `Raw`        | Binary stored as a raw memory buffer.                    |
 * | `MemHandle`  | Binary referenced via a memory handle.                   |
 * | `Undefined`  | Sentinel value; not a valid binary type.                 |
 */
QAIRT_CPPAPI_DEFINE_ENUM(ContextBinaryType,
                         QairtContext_BinaryType_t,
                         Raw       = QAIRT_CONTEXT_BINARY_TYPE_RAW,
                         MemHandle = QAIRT_CONTEXT_BINARY_TYPE_MEM_HANDLE,
                         Undefined = QAIRT_CONTEXT_BINARY_TYPE_UNDEFINED)

/**
 * @brief Portion of the context binary targeted by section operations.
 *
 * @details
 * | Enumerator            | Description                                                          |
 * |-----------------------|----------------------------------------------------------------------|
 * | `Updatable`           | Section containing all recent updates applied via tensor update APIs.|
 * | `UpdatableWeights`    | Section containing recent static weight updates only.                |
 * | `UpdatableQuantParams`| Section containing recent quantization parameter updates only.       |
 * | `Undefined`           | Sentinel value; not a valid section type.                            |
 */
QAIRT_CPPAPI_DEFINE_ENUM(ContextSectionType,
                         QairtContext_SectionType_t,
                         Updatable            = QAIRT_CONTEXT_SECTION_UPDATABLE,
                         UpdatableWeights     = QAIRT_CONTEXT_SECTION_UPDATABLE_WEIGHTS,
                         UpdatableQuantParams = QAIRT_CONTEXT_SECTION_UPDATABLE_QUANT_PARAMS,
                         Undefined            = QAIRT_CONTEXT_SECTION_UNDEFINED)

/**
 * @brief Notification kind delivered to a callback registered via
 *        `ContextBinaryListParams::setNotify`.
 *
 * @details
 * | Enumerator    | Description                                                      |
 * |---------------|-------------------------------------------------------------------|
 * | `GraphInit`   | A graph belonging to the context has finished initializing.       |
 * | `ContextInit` | The overall context has finished initializing (terminal, once).   |
 * | `Undefined`   | Sentinel value; not a valid notification kind.                    |
 */
QAIRT_CPPAPI_DEFINE_ENUM(ContextCreateFromBinaryAsyncNotifyType,
                         QairtContext_CreateFromBinaryAsyncNotifyType_t,
                         GraphInit   = QAIRT_CONTEXT_NOTIFY_TYPE_GRAPH_INIT,
                         ContextInit = QAIRT_CONTEXT_NOTIFY_TYPE_CONTEXT_INIT,
                         Undefined   = QAIRT_CONTEXT_NOTIFY_TYPE_UNDEFINED)

/**
 * @brief Buffer descriptor for a serialized context binary.
 *
 *        Obtained via `Api::make<ContextBinaryBuffer>()`.
 */
class ContextBinaryBuffer : public ApiType<ContextBinaryBuffer, QairtContext_BinaryBufferV1_t> {
  friend Api;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  ContextBinaryBuffer() noexcept                                 = default;
  ContextBinaryBuffer(ContextBinaryBuffer&&) noexcept            = default;
  ContextBinaryBuffer& operator=(ContextBinaryBuffer&&) noexcept = default;

  /**
   * @brief Non-owning constructor: wraps a backend-owned handle without taking ownership.
   *        The destructor will NOT call free() on the handle.
   *        Caller must call some other API (corresponding to the one they retrieved the bufer
   *        from) to release backend memory.
   */
  ContextBinaryBuffer(const std::shared_ptr<ApiTable>& apiTable,
                      QairtContext_BinaryBufferHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }

  /**
   * @brief Get the raw data pointer stored in this buffer.
   *
   * @return Pointer to the underlying buffer memory, or nullptr if none has been set.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryBuffer_getData
   */
  void* getData() { return callGetter<&interface_type::getData>(); }

  /**
   * @brief Get the raw data pointer stored in this buffer.
   *
   * @return Pointer to the underlying buffer memory, or nullptr if none has been set.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryBuffer_getData
   */
  const void* getData() const { return callGetter<&interface_type::getData>(); }

  /**
   * @brief Set the raw data pointer for this buffer.
   *
   * @param[in] data  Pointer to the memory region to associate with this buffer.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryBuffer_setData
   */
  void setData(void* data) { callSetter<&interface_type::setData>(data); }

  /**
   * @brief Get the size of this buffer in bytes.
   *
   * @return Buffer size in bytes, or 0 if no size has been set.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryBuffer_getSize
   */
  uint64_t getSize() const { return callGetter<&interface_type::getSize>(); }

  /**
   * @brief Set the size of this buffer in bytes.
   *
   * @param[in] size  Size in bytes of the memory region referenced by this buffer.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryBuffer_setSize
   */
  void setSize(uint64_t size) const { callSetter<&interface_type::setSize>(size); }

 private:
  explicit ContextBinaryBuffer(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }
};

/**
 * @brief Queue depth configuration for asynchronous context execution.
 */
class ContextAsyncExecutionQueueDepth
    : public ApiType<ContextAsyncExecutionQueueDepth, QairtContext_AsyncExecutionDepthV1_t> {
  friend class Api;

 public:
  ContextAsyncExecutionQueueDepth() noexcept                                             = default;
  ContextAsyncExecutionQueueDepth(ContextAsyncExecutionQueueDepth&&) noexcept            = default;
  ContextAsyncExecutionQueueDepth& operator=(ContextAsyncExecutionQueueDepth&&) noexcept = default;

  /**
   * @brief Get the current queue depth for asynchronous execution.
   *
   * @return Maximum number of outstanding asynchronous execution requests.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_AsyncExecutionGetDepth
   */
  uint32_t getDepth() const { return callGetter<&interface_type::getAsyncExecutionDepth>(); }

  /**
   * @brief Set the queue depth for asynchronous execution.
   *
   * @param[in] depth  Maximum number of outstanding asynchronous execution requests.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtContext_AsyncExecutionSetDepth
   */
  void setDepth(uint32_t depth) {
    return callSetter<&interface_type::setAsyncExecutionDepth>(depth);
  }

 private:
  explicit ContextAsyncExecutionQueueDepth(const std::shared_ptr<ApiTable>& apiTable)
      : ApiType(apiTable) {
    construct();
  }
};

class ContextConfiguration;

/**
 * @brief Abstract base class for a single backend-specific context custom configuration entry.
 */
class ContextCustomConfig : public CustomConfigType {
 protected:
  ContextCustomConfig()                                          = default;
  ContextCustomConfig(const ContextCustomConfig&)                = default;
  ContextCustomConfig(ContextCustomConfig&&) noexcept            = default;
  ContextCustomConfig& operator=(const ContextCustomConfig&)     = default;
  ContextCustomConfig& operator=(ContextCustomConfig&&) noexcept = default;

 public:
  virtual ~ContextCustomConfig()                                          = default;
  virtual QairtContext_CustomConfigHandle_t getCustomConfigHandle() const = 0;
};

/**
 * @brief Abstract base class for a collection of backend-specific context custom configuration
 * entries.
 */
class ContextCustomConfiguration {
 protected:
  ContextCustomConfiguration()                                                 = default;
  ContextCustomConfiguration(const ContextCustomConfiguration&)                = default;
  ContextCustomConfiguration(ContextCustomConfiguration&&) noexcept            = default;
  ContextCustomConfiguration& operator=(const ContextCustomConfiguration&)     = default;
  ContextCustomConfiguration& operator=(ContextCustomConfiguration&&) noexcept = default;

 public:
  virtual ~ContextCustomConfiguration()                                           = default;
  virtual std::vector<QairtContext_CustomConfigHandle_t> getCustomConfigs() const = 0;
};

/**
 * @brief Configuration object for context creation and reconfiguration.
 */
class ContextConfiguration : public ApiType<ContextConfiguration, QairtContext_ConfigV1_t> {
  friend Api;

  /// @brief Original Equipment Manufacturer (OEM) key string for backend authentication.
  detail::crossable<std::string, &interface_type::getOemKey, &interface_type::setOemKey> m_oemKey;
  /// @brief Maximum number of outstanding asynchronous execution requests.
  detail::crossable<detail::non_owning<ContextAsyncExecutionQueueDepth>,
                    &interface_type::getAsyncQueueDepth,
                    &interface_type::setAsyncQueueDepth>
      m_depth;
  /// @brief Names of graphs selectively enabled for this context configuration.
  mutable std::vector<std::string> m_enabledGraphs;

 public:
  ContextConfiguration() noexcept                                  = default;
  ContextConfiguration(ContextConfiguration&&) noexcept            = default;
  ContextConfiguration& operator=(ContextConfiguration&&) noexcept = default;

  /**
   * @brief Set the scheduling priority for this context configuration.
   *
   * @param[in] p  Desired execution priority.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtContext_Config_setPriority
   */
  void setPriority(Priority p) {
    callSetter<&interface_type::setPriority>(static_cast<Qairt_Priority_t>(p));
  }

  /**
   * @brief Get the scheduling priority for this context configuration.
   *
   * @return Current execution priority.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getPriority
   */
  Priority getPriority() const {
    return static_cast<Priority>(callGetter<&interface_type::getPriority>());
  }

  /**
   * @brief Get the Original Equipment Manufacturer (OEM) key string for this context configuration.
   *
   * @return Reference to the OEM authentication key string. Empty if not set.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getOemKey
   */
  std::string& getOemKey() { return m_oemKey.get(); }

  /**
   * @brief Get the Original Equipment Manufacturer (OEM) key string for this context configuration.
   *
   * @return Const reference to the OEM authentication key string. Empty if not set.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getOemKey
   */
  const std::string& getOemKey() const { return m_oemKey.get(); }

  /**
   * @brief Set the Original Equipment Manufacturer (OEM) key string for this context configuration.
   *
   * @param[in] oemKey  OEM authentication key string to set.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_setOemKey
   */
  void setOemKey(std::string&& oemKey) { m_oemKey.assignString(std::move(oemKey)); }

  void getOemKey(const std::string& oemKey) { m_oemKey.assignString(oemKey); }

  /**
   * @brief Set the asynchronous execution queue depth for this context configuration.
   *
   * @param[in] aed  Async execution queue depth configuration object.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_setAsyncQueueDepth
   */
  void setAsyncExecutionQueueDepth(const ContextAsyncExecutionQueueDepth& aed) {
    QAIRT_NO_RETURN m_depth.setNonOwning(*this, aed);
  }

  /**
   * @brief Get the asynchronous execution queue depth configuration for this context.
   *
   * @return Reference to the async execution queue depth object.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getAsyncQueueDepth
   */
  ContextAsyncExecutionQueueDepth& getAsyncExecutionQueueDepth() {
    return m_depth.bindNonOwning(*this);
  }

  /**
   * @brief Get the asynchronous execution queue depth configuration for this context.
   *
   * @return Const reference to the async execution queue depth object.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getAsyncQueueDepth
   */
  const ContextAsyncExecutionQueueDepth& getAsyncExecutionQueueDepth() const {
    return m_depth.bindNonOwning(*this);
  }

  /**
   * @brief Set a single backend-specific custom configuration entry on this context configuration.
   *
   * @param[in] config  Backend-specific custom configuration entry to apply.
   *
   * @return Reference to this configuration object, allowing method chaining.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_setCustomConfigs
   */
  ContextConfiguration& setCustomConfig(const ContextCustomConfig& config) {
    auto cch = config.getCustomConfigHandle();
    callAndConfirm<&interface_type::setCustomConfigs>(&cch, 1u);
    return *this;
  }

  /**
   * @brief Set a collection of backend-specific custom configuration entries on this context
   * configuration.
   *
   * @param[in] config  Collection of backend-specific custom configuration entries to apply.
   *
   * @return Reference to this configuration object, allowing method chaining.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_setCustomConfigs
   */
  ContextConfiguration& setCustomConfigs(const ContextCustomConfiguration& config) {
    std::vector<QairtContext_CustomConfigHandle_t> cfgHandles = config.getCustomConfigs();

    callAndConfirm<&interface_type::setCustomConfigs>(cfgHandles.data(),
                                                      static_cast<uint32_t>(cfgHandles.size()));
    return *this;
  }

  /**
   * @brief Get the list of graph names selectively enabled for this context configuration.
   *
   * @return Reference to the vector of enabled graph name strings.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getNumEnabledGraphs
   */
  std::vector<std::string>& getEnableGraphs() { return m_enabledGraphs; }

  /**
   * @brief Get the list of graph names selectively enabled for this context configuration.
   *
   * @return Const reference to the vector of enabled graph name strings.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getNumEnabledGraphs
   */
  const std::vector<std::string>& getEnableGraphs() const { return m_enabledGraphs; }

  /**
   * @brief Set the list of graph names selectively enabled for this context configuration.
   *
   * @param[in] enabledGraphs  Names of graphs to enable.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_setEnableGraph
   */
  void setEnableGraphs(std::vector<std::string> enabledGraphs) {
    m_enabledGraphs = std::move(enabledGraphs);
  }

  /**
   * @brief Set a hint on the maximum memory the backend should use for this context.
   *
   * This is advisory only; the backend may exceed the limit if required.
   *
   * @param[in] limit  Memory limit hint in bytes.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtContext_Config_setMemoryLimitHint
   */
  void setMemoryLimitHint(uint64_t limit) {
    callSetter<&interface_type::setMemoryLimitHint>(limit);
  }

  /**
   * @brief Get the memory limit hint for this context configuration.
   *
   * @return Memory limit hint in bytes, or 0 if no limit has been set.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getMemoryLimitHint
   */
  uint64_t getMemoryLimitHint() const { return callGetter<&interface_type::getMemoryLimitHint>(); }

  /**
   * @brief Set whether the context binary should be treated as persistent.
   *
   * @param[in] isPersistentBinary  True to enable persistent binary mode; false to disable.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtContext_Config_setIsPersistentBinary
   */
  void setIsPersistentBinary(bool isPersistentBinary) {
    callSetter<&interface_type::setIsPersistentBinary>(detail::convertBool(isPersistentBinary));
  }

  /**
   * @brief Get whether the context binary is configured as persistent.
   *
   * @return True if persistent binary mode is enabled; false otherwise.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getIsPersistentBinary
   */
  bool getIsPersistentBinary() const {
    return detail::resolveBool(callGetter<&interface_type::getIsPersistentBinary>());
  }

  /**
   * @brief Set the binary compatibility policy for loading cached context binaries.
   *
   * @param[in] bct  Binary compatibility policy to enforce.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtContext_Config_setBinaryCompatibilityType
   */
  void setBinaryCompatibilityType(ContextBinaryCompatibilityType bct) {
    callSetter<&interface_type::setBinaryCompatibilityType>(
        static_cast<QairtContext_BinaryCompatibilityType_t>(bct));
  }

  /**
   * @brief Get the binary compatibility policy for loading cached context binaries.
   *
   * @return Current binary compatibility policy.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getBinaryCompatibilityType
   */
  ContextBinaryCompatibilityType getBinaryCompatibilityType() const {
    return static_cast<ContextBinaryCompatibilityType>(
        callGetter<&interface_type::getBinaryCompatibilityType>());
  }

  /**
   * @brief Enable deferred graph initialization for use with createContextFromBinary.
   *
   * When enabled, graphs in the context are not initialized on the device until
   * Context::finalize() is called. This allows querying context properties before committing
   * device resources.
   *
   * @param[in] deferred  True to enable deferred graph initialization.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see Context::finalize
   * @see QairtContext_Config_setIsGraphInitDeferred
   */
  void setIsGraphInitDeferred(bool deferred) {
    callSetter<&interface_type::setIsGraphInitDeferred>(detail::convertBool(deferred));
  }

  /**
   * @brief Get the deferred graph initialization flag.
   *
   * @return True if deferred graph initialization is enabled.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_Config_getIsGraphInitDeferred
   */
  bool getIsGraphInitDeferred() const {
    return detail::resolveBool(callGetter<&interface_type::getIsGraphInitDeferred>());
  }

 private:
  void prepareToCross() const {
    prepareMultipleToCross(m_oemKey, m_depth);
    for (const auto& graph : m_enabledGraphs) {
      callSetter<&interface_type::setEnableGraph>(graph.c_str());
    }
  }
  void updateAfterCross() const {
    updateMultipleAfterCross(m_oemKey, m_depth);
    m_enabledGraphs.clear();
    uint32_t numEnabledGraphs;
    callAndConfirm<&interface_type::getNumEnabledGraphs>(&numEnabledGraphs);
    for (uint32_t i = 0u; i < numEnabledGraphs; ++i) {
      const char* graphName;
      callAndConfirm<&interface_type::getEnabledGraphAt>(i, &graphName);
      QAIRT_NO_RETURN m_enabledGraphs.emplace_back(graphName);
    }
  }
  explicit ContextConfiguration(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }

  ContextConfiguration(const std::shared_ptr<ApiTable>& apiTable, QairtContext_ConfigHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

/**
 * @brief Descriptor pairing a binary type with its associated buffer for context serialization.
 */
class ContextBinary : public ApiType<ContextBinary, QairtContext_BinaryV1_t> {
  friend Api;

  /// @brief Binary buffer associated with this context binary object.
  detail::crossable<detail::non_owning<ContextBinaryBuffer>,
                    &interface_type::getBuffer,
                    &interface_type::setBuffer>
      m_buffer;

 public:
  ContextBinary() noexcept                           = default;
  ContextBinary(ContextBinary&&) noexcept            = default;
  ContextBinary& operator=(ContextBinary&&) noexcept = default;

  /**
   * @brief Get the storage format type of this context binary.
   *
   * @return Storage format of this binary (e.g., Raw or MemHandle).
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_binaryGetType
   */
  ContextBinaryType getType() const {
    return static_cast<ContextBinaryType>(callGetter<&interface_type::getType>());
  }

  /**
   * @brief Get the binary buffer associated with this context binary.
   *
   * @return Reference to the associated ContextBinaryBuffer.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_binaryGetBuffer
   */
  ContextBinaryBuffer& getBuffer() { return m_buffer.bindNonOwning(*this); }

  /**
   * @brief Get the binary buffer associated with this context binary.
   *
   * @return Const reference to the associated ContextBinaryBuffer.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_binaryGetBuffer
   */
  const ContextBinaryBuffer& getBuffer() const { return m_buffer.bindNonOwning(*this); }

  /**
   * @brief Set the binary buffer for this context binary.
   *
   * @param[in] buffer  Binary buffer to associate with this context binary object.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_binarySetBuffer
   */
  void setBuffer(ContextBinaryBuffer&& buffer) {
    QAIRT_NO_RETURN m_buffer.setNonOwning(*this, std::move(buffer));
  }
};

/**
 * @brief Wrapper for a QAIRT context handle.
 *
 *        Obtained via Backend::createContext() or Backend::createContextFromBinary().
 */
class Backend;
class Context : public ApiType<Context, QairtContext_V1_t> {
  friend Api;

  template <typename, typename, typename>
  friend class ::qairt::ApiType;

  friend class ContextBinaryListNotifyCallback;

  /// @brief Profile handle used to collect metrics during context teardown.
  QairtProfile_Handle_t m_freeProfileHandle = nullptr;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  Context()                              = default;
  Context(const Context&)                = delete;
  Context(Context&&) noexcept            = default;
  Context& operator=(const Context&)     = delete;
  Context& operator=(Context&&) noexcept = default;

  // Non-owning constructor: wraps an existing context handle without taking ownership.
  // The destructor will NOT call free() on the handle.
  Context(const std::shared_ptr<ApiTable>& apiTable, QairtContext_Handle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }

  /**
   * @brief Set or modify configuration options on this context.
   *
   * Backends are not required to support reconfiguration after context creation.
   * If the backend does not support the provided configuration, this call will fail.
   *
   * @param[in] config  Context configuration object containing the options to apply.
   *
   * @throws qairt::Exception on invalid handle, unsupported feature, or invalid configuration.
   *
   * @see QairtContext_setConfig
   */
  void setConfig(const ContextConfiguration& config) {
    callSetter<&interface_type::setConfig>(getHandle(config));
  }

  /**
   * @brief Apply a collection of backend-specific custom configuration entries to this context.
   *
   * @param[in] customConfigs  Backend-specific custom configuration collection whose handles
   *                           are applied individually to this context.
   *
   * @throws qairt::Exception on invalid handle.
   */
  template <typename T>
  std::enable_if_t<std::is_base_of_v<ContextCustomConfiguration, T>> setConfiguration(
      const T& customConfigs) {
    for (const auto& e : customConfigs.getCustomConfigs()) {
      setConfig(e);
    }
  }

  /**
   * @brief Finish context creation when originally created with deferred graph
   *        initialization enabled.
   *
   * @see ContextConfiguration::setIsGraphInitDeferred
   * @see QairtContext_finalize
   */
  void finalize() { callAndConfirm<&interface_type::finalize>(nullptr); }

  /**
   * @brief Finish context creation when originally created with deferred graph
   *        initialization enabled, collecting profiling metrics for the operation.
   *
   * @param[in] profile  Profile object to be populated with metrics captured
   *                      during finalization (e.g. graph initialization timing).
   *
   * @see ContextConfiguration::setIsGraphInitDeferred
   * @see QairtContext_finalize
   */
  void finalize(Profile& profile) {
    callAndConfirm<&interface_type::finalize>(getHandle(profile));
  }

  /**
   * @brief Get the size in bytes of the serialized binary representation of this context.
   *
   * All graphs in the context must be finalized before calling this method.
   * Call getBinary() or getBinary(void*, uint64_t) after allocating a buffer of
   * at least this size.
   *
   * @return Size in bytes required to hold the serialized context binary.
   *
   * @throws qairt::Exception on:
   *          - invalid handle
   *          - unsupported feature
   *          - unfinalized graphs in the context
   *          - other retrieval failure
   *
   * @see QairtContext_getBinarySize
   */
  uint64_t getBinarySize() const { return callGetter<&interface_type::getBinarySize>(); }

  /**
   * @brief Serialize this context into the provided binary buffer.
   *
   * All graphs in the context must be finalized before calling this method.
   * Call getBinarySize() first to determine the required buffer size.
   * The buffer's data pointer and size must be set before calling this method.
   *
   * @param[in,out] buffer  Pre-allocated binary buffer to receive the serialized context.
   *                        The buffer's size field must be at least getBinarySize() bytes.
   *
   * @return Number of bytes written into the buffer.
   *
   * @throws qairt::Exception on:
   *          - invalid handle
   *          - unsupported feature
   *          - unfinalized graphs in the context
   *          - other serialization failure
   *
   * @see QairtContext_getBinary
   */
  uint64_t getBinary(ContextBinaryBuffer& buffer) {
    uint64_t toRet{0U};
    QAIRT_NO_RETURN call<&interface_type::getBinary>(
        detail::getHandle(ApiTypeRef<const ContextBinaryBuffer&>{buffer}), &toRet);
    return toRet;
  }

  /**
   * @brief Wrapper which allows for serializing directly into a caller-managed raw memory buffer.
   *
   * @see Context::getBinary(ContextBinaryBuffer&)
   */
  // Write into the user allocated buffer. Return the number of bytes written
  uint64_t getBinary(void* buffer, uint64_t bufferSize) {
    ContextBinaryBuffer buff = create<ContextBinaryBuffer>();
    buff.setData(buffer);
    buff.setSize(bufferSize);
    return getBinary(buff);
  }

  /**
   * @brief Update the data and quantization parameters of previously created context tensors.
   *
   * Valid fields to update depend on tensor type:
   * - UPDATEABLE_STATIC: data and quantization parameters.
   * - UPDATEABLE_NATIVE, UPDATEABLE_APP_READ, UPDATEABLE_APP_WRITE,
   *   UPDATEABLE_APP_READWRITE: quantization parameters only.
   *
   * Updates take effect only after QairtGraph_finalize() is called for one or more
   * of the graphs to which the context tensors are associated.
   *
   * @param[in] tensors  Pointers to tensors to update. Each tensor must carry the ID
   *                     assigned during tensor creation.
   *
   * @throws qairt::Exception on:
   *          - invalid context or tensor handle
   *          - NULL tensor handle array
   *          - incompatible tensor type
   *          - unsupported feature
   *
   * @see QairtContext_updateContextTensors
   */
  void updateContextTensors(const std::vector<Tensor*>& tensors) {
    std::vector<QairtTensor_Handle_t> tensorHandles(tensors.size());
    for (size_t i = 0; i < tensors.size(); ++i) {
      detail::QairtTypeAdmin::prepareToCross(*tensors[i]);
      tensorHandles[i] = getHandle(*tensors[i]);
    }
    callAndConfirm<&interface_type::updateContextTensors>(
        tensorHandles.data(), static_cast<uint64_t>(tensorHandles.size()));
  }

  /**
   * @brief Get the size in bytes of a binary section for a specific graph.
   *
   * All graphs in the context must be finalized before calling this method.
   * Use this to determine the buffer size needed before calling getBinarySection().
   *
   * @param[in] graph    Graph whose binary section size is queried.
   * @param[in] section  Portion of the context binary to query.
   *
   * @return Size in bytes needed to hold the requested binary section.
   *
   * @throws qairt::Exception on:
   *          - invalid handle
   *          - unsupported feature
   *          - unfinalized graphs in the context
   *          - other retrieval failure
   *
   * @see QairtContext_getBinarySectionSize
   */
  uint64_t getBinarySectionSize(const Graph& graph, ContextSectionType section) const {
    uint64_t toRet{0U};
    callAndConfirm<&interface_type::getBinarySectionSize>(
        getHandle(graph), static_cast<QairtContext_SectionType_t>(section), &toRet);
    return toRet;
  }

  /**
   * @brief Retrieve a section of the context binary for a specific graph.
   *
   * All graphs in the context must be finalized before calling this method.
   * Call getBinarySectionSize() first to determine the required buffer size.
   * The signal, if non-null, is considered in-use for the duration of this call.
   *
   * @param[in] graph    Graph whose binary section is retrieved.
   * @param[in] section  Portion of the context binary to retrieve.
   * @param[in,out] buffer  Pre-allocated binary buffer to receive the section.
   *                        Must be sized to at least getBinarySectionSize() bytes.
   * @param[in] profile  Optional profile handle to collect metrics.
   * @param[in] signal   Optional signal handle for controlling the operation.
   *
   * @return Number of bytes written into the buffer.
   *
   * @throws qairt::Exception on:
   *          - invalid handle
   *          - unsupported feature
   *          - unfinalized graphs in the context
   *          - other serialization failure
   *
   * @see QairtContext_getBinarySection
   */
  uint64_t getBinarySection(const Graph& graph,
                            ContextSectionType section,
                            ContextBinaryBuffer& buffer,
                            ApiTypeRef<const Profile&> profile,
                            ApiTypeRef<const Signal&> signal) {
    uint64_t toRet;
    QAIRT_NO_RETURN call<&interface_type::getBinarySection>(
        getHandle(graph),
        static_cast<QairtContext_SectionType_t>(section),
        detail::getHandle(ApiTypeRef<const ContextBinaryBuffer&>{buffer}),
        detail::getHandle(profile),
        detail::getHandle(signal),
        &toRet);
    return toRet;
  }

  /**
   * @brief Apply a previously retrieved binary section to this context.
   *
   * @param[in] graph    Graph to which the binary section applies.
   * @param[in] section  Portion of the context binary being applied.
   * @param[in] buffer   Binary buffer containing the section to apply. When persistent
   *                     binary mode is enabled, this buffer must remain valid through
   *                     context teardown.
   * @param[in] profile  Optional profile handle to collect metrics.
   * @param[in] signal   Optional signal handle for controlling the operation.
   *
   * @throws qairt::Exception on:
   *          - invalid handle
   *          - unsupported feature
   *          - memory allocation failure
   *          - profiling error
   *
   * @see QairtContext_applyBinarySection
   */
  void applyBinarySection(const Graph& graph,
                          ContextSectionType section,
                          ContextBinaryBuffer& buffer,
                          ApiTypeRef<const Profile&> profile,
                          ApiTypeRef<const Signal&> signal) {
    callAndConfirm<&interface_type::applyBinarySection>(
        getHandle(graph),
        static_cast<QairtContext_SectionType_t>(section),
        detail::getHandle(ApiTypeRef<const ContextBinaryBuffer&>{buffer}),
        detail::getHandle(profile),
        detail::getHandle(signal));
  }

  /**
   * @brief Apply updates to a previously created binary section. Memory is owned by the backend
   *        and must be released with freeBinarySectionUpdate().
   *
   * @param[in] binaryBuffer  A previously retrieved updatable binary section.
   * @param[in] tensors       Tensor updates to apply to the binary section.
   * @param[in] keepUpdatable Whether the output should remain updatable for future calls.
   * @param[in] logger        Optional logger handle.
   * @param[in] profile       Optional profile handle.
   * @param[in] signal        Optional signal handle.
   *
   * @return A non-owning ContextBinaryBuffer wrapping backend-owned memory.
   *         Caller MUST call freeBinarySectionUpdate() to release it.
   */
  static ContextBinaryBuffer getBinarySectionUpdate(
      Api& api,
      ContextBinaryBuffer& binaryBuffer,
      const std::vector<Tensor*>& tensors,
      bool keepUpdatable,
      ApiTypeRef<const Log&> logger      = std::nullopt,
      ApiTypeRef<const Profile&> profile = std::nullopt,
      ApiTypeRef<const Signal&> signal   = std::nullopt) {
    std::vector<QairtTensor_Handle_t> tensorHandles(tensors.size());
    for (size_t i = 0; i < tensors.size(); ++i) {
      if (tensors[i] == nullptr) {
        throw Exception("getBinarySectionUpdate: null tensor pointer at index " + std::to_string(i),
                        CommonError::InvalidArgument);
      }
      detail::QairtTypeAdmin::prepareToCross(*tensors[i]);
      tensorHandles[i] = detail::QairtTypeAdmin::getHandle(*tensors[i]);
    }

    QairtContext_BinaryBufferHandle_t outputHandle{};
    auto status = api.call<&QairtContext_V1_t::getBinarySectionUpdate>(
        detail::getHandle(ApiTypeRef<const ContextBinaryBuffer&>{binaryBuffer}),
        nullptr,  // auxiliaryBuffer (currently unused, must be NULL)
        tensorHandles.empty() ? nullptr : tensorHandles.data(),
        static_cast<uint64_t>(tensorHandles.size()),
        keepUpdatable ? uint8_t{1} : uint8_t{0},
        detail::getHandle(logger),
        detail::getHandle(profile),
        detail::getHandle(signal),
        &outputHandle);
    if (status != QAIRT_SUCCESS) {
      throw Exception("getBinarySectionUpdate failed", static_cast<CommonError>(status));
    }

    return ContextBinaryBuffer(api.m_apiTable, outputHandle);
  }

  static void freeBinarySectionUpdate(Api& api,
                                      ContextBinaryBuffer& binarySectionUpdate,
                                      ApiTypeRef<const Log&> logger = std::nullopt) {
    auto status = api.call<&QairtContext_V1_t::freeBinarySectionUpdate>(
        detail::getHandle(ApiTypeRef<const ContextBinaryBuffer&>{binarySectionUpdate}),
        detail::getHandle(logger));
    if (status != QAIRT_SUCCESS) {
      throw Exception("freeBinarySectionUpdate failed", static_cast<CommonError>(status));
    }
  }

  /**
   * @brief Create a new graph within this context.
   *
   * @param[in] graphName           Unique null-terminated identifier for the graph within
   *                                this context.
   * @param[in] graphConfiguration  Configuration options for the graph. Optional.
   *
   * @return The newly created Graph object.
   *
   * @throws qairt::Exception on:
   *          - invalid context handle
   *          - NULL or duplicate graph name
   *          - memory or resource allocation failure
   *          - unsupported configuration options
   *
   * @see QairtContext_createGraph
   */
  Graph createGraph(const char* graphName,
                    ApiTypeRef<const qairt::GraphConfiguration&> graphConfiguration) {
    return create<Graph>(getHandle(*this), graphName, graphConfiguration);
  }

  /// @see Context::retrieveGraph(const std::string&)
  std::shared_ptr<Graph> retrieveGraph(const char* graphName) {
    return retrieveGraph(std::string(graphName));
  }

  /**
   * @brief Retrieve an existing graph from this context by name.
   *
   * @param[in] graphName  Name of the graph to retrieve.
   *
   * @return Shared pointer to the retrieved Graph object.
   *
   * @throws qairt::Exception on:
   *          - invalid context handle
   *          - NULL or invalid graph name
   *          - no graph with the specified name exists in this context
   *          - memory allocation failure
   *
   * @see QairtContext_retrieveGraph
   */
  std::shared_ptr<Graph> retrieveGraph(const std::string& graphName) {
    return std::make_shared<Graph>(create<Graph>(getHandle(*this), graphName.c_str()));
  }

  /**
   * @brief Set the profile handle used to collect metrics during context teardown.
   *
   * @param[in] profile  Profile object to populate during context teardown.
   *
   * @see QairtContext_free
   */
  void setFreeProfile(Profile& profile) { m_freeProfileHandle = getHandle(profile); }

  /**
   * @brief Register existing memory with this context.
   *
   * For each input @ref MemDescriptor, registers the described memory with the
   * backend and returns a corresponding @ref MemHandle. Returned handles
   * remain valid until @ref deregisterMem is called or this Context is
   * destroyed (whichever comes first).
   *
   * @param[in,out] descriptors  Fully configured memory descriptors to register.
   *                             Order is preserved in the returned handles.
   *
   * @return Vector of registered @ref MemHandle values, one per input
   *         descriptor in the same order.
   *
   * @throws qairt::Exception on invalid handle, invalid argument, unsupported
   *         feature, mapping failure, or memory-allocation failure.
   *
   * @see QairtContext_registerMem
   */
  std::vector<MemHandle> registerMem(std::vector<MemDescriptor>& descriptors) {
    if (descriptors.empty()) {
      return {};
    }
    std::vector<QairtMem_DescriptorHandle_t> descHandles;
    descHandles.reserve(descriptors.size());
    for (auto& d : descriptors) {
      detail::QairtTypeAdmin::prepareToCross(d);
      descHandles.push_back(getHandle(d));
    }
    std::vector<QairtMem_Handle_t> raw(descriptors.size());
    QAIRT_NO_RETURN callAndConfirm<&interface_type::registerMem>(
        descHandles.data(), static_cast<uint32_t>(descHandles.size()), raw.data());
    std::vector<MemHandle> out;
    out.reserve(raw.size());
    for (auto h : raw) {
      out.emplace_back(h);
    }
    return out;
  }

  /**
   * @brief Deregister memory handles previously obtained from @ref registerMem.
   *
   * After this call returns, each input @ref MemHandle is reset to a null
   * state and the underlying backend memory registration is released. Calling
   * this is required to release backend resources before the underlying
   * memory is freed by the caller; the QAIRT C API does not free these
   * handles automatically when the Context is destroyed.
   *
   * @param[in,out] handles  Memory handles to deregister. Cleared on return.
   *                         An empty vector is a no-op.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtMem_Descriptor_deregisterMem
   */
  void deregisterMem(std::vector<MemHandle>& handles) {
    if (handles.empty()) {
      return;
    }
    std::vector<QairtMem_Handle_t> raw;
    raw.reserve(handles.size());
    for (auto& h : handles) {
      raw.push_back(h.getHandle());
    }
    QAIRT_NO_RETURN callNoHandleAndConfirm<&QairtMem_DescriptorV1_t::deregisterMem>(
        raw.data(), static_cast<uint32_t>(raw.size()));
    for (auto& h : handles) {
      h = MemHandle{};
    }
  }

 private:
  Context(const std::shared_ptr<ApiTable>& apiTable,
          QairtBackend_Handle_t backendHandle,
          QairtDevice_Handle_t deviceHandle,
          QairtContext_ConfigHandle_t contextConfigHandle)
      : ApiType(apiTable) {
    construct(backendHandle, deviceHandle, contextConfigHandle);
  }

  Context(const std::shared_ptr<ApiTable>& apiTable,
          QairtBackend_Handle_t backendHandle,
          QairtDevice_Handle_t deviceHandle,
          QairtContext_ConfigHandle_t contextConfigHandle,
          QairtContext_BinaryBufferHandle_t binaryBufferHandle,
          QairtSignal_Handle_t signalHandle   = nullptr,
          QairtProfile_Handle_t profileHandle = nullptr)
      : ApiType(apiTable) {
    construct<&interface_type::createFromBinary>(backendHandle,
                                                 deviceHandle,
                                                 contextConfigHandle,
                                                 signalHandle,
                                                 profileHandle,
                                                 binaryBufferHandle);
  }

  // Owning constructor: takes ownership of a context handle delivered via the
  // async context creation.
  Context(const std::shared_ptr<ApiTable>& apiTable,
          QairtContext_Handle_t handle,
          ContextCreateFromBinaryAsyncNotifyType)
      : ApiType(apiTable) {
    setHandle(handle);
  }

  void customFree(QairtContext_Handle_t handle) const {
    QAIRT_NO_RETURN callNoHandle<&interface_type::free>(handle, m_freeProfileHandle);
  }
};

/**
 * @brief Describes the byte range a raw/dma buffer callback is being asked to provide.
 *
 *        Delivered to a `ContextBinaryListRawBufferCallback`/`ContextBinaryListDmaBufferCallback`
 *        provider function; not constructed directly by callers.
 */
class ContextBinaryDataRequest
    : public ApiType<ContextBinaryDataRequest, QairtContext_BinaryDataRequestV1_t> {
  friend Api;
  friend class ContextBinaryListRawBufferCallback;
  friend class ContextBinaryListDmaBufferCallback;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  ContextBinaryDataRequest() noexcept                                    = default;
  ContextBinaryDataRequest(ContextBinaryDataRequest&&) noexcept            = default;
  ContextBinaryDataRequest& operator=(ContextBinaryDataRequest&&) noexcept = default;

  uint64_t getOffset() const { return callGetter<&interface_type::getOffset>(); }
  void setOffset(uint64_t offset) { callSetter<&interface_type::setOffset>(offset); }

  uint64_t getSize() const { return callGetter<&interface_type::getSize>(); }
  void setSize(uint64_t size) { callSetter<&interface_type::setSize>(size); }

  bool getIsBackendMappingNeeded() const {
    return detail::resolveBool(callGetter<&interface_type::getIsBackendMappingNeeded>());
  }
  void setIsBackendMappingNeeded(bool value) {
    callSetter<&interface_type::setIsBackendMappingNeeded>(detail::convertBool(value));
  }

 private:
  explicit ContextBinaryDataRequest(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }

  // Non-owning constructor: wraps a backend-owned handle without taking ownership.
  ContextBinaryDataRequest(const std::shared_ptr<ApiTable>& apiTable,
                           QairtContext_BinaryDataRequestHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

/**
 * @brief Raw-buffer response a `ContextBinaryListRawBufferCallback` provider populates.
 *
 *        Delivered to the provider function; the caller sets memPtr/dataStartOffset/alignedSize
 *        before returning.
 */
class ContextBinaryRawDataResponse
    : public ApiType<ContextBinaryRawDataResponse, QairtContext_BinaryRawDataResponseV1_t> {
  friend Api;
  friend class ContextBinaryListRawBufferCallback;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  ContextBinaryRawDataResponse() noexcept                                       = default;
  ContextBinaryRawDataResponse(ContextBinaryRawDataResponse&&) noexcept            = default;
  ContextBinaryRawDataResponse& operator=(ContextBinaryRawDataResponse&&) noexcept = default;

  void* getMemPtr() const { return callGetter<&interface_type::getMemPtr>(); }
  void setMemPtr(void* memPtr) { callSetter<&interface_type::setMemPtr>(memPtr); }

  uint64_t getDataStartOffset() const { return callGetter<&interface_type::getDataStartOffset>(); }
  void setDataStartOffset(uint64_t offset) {
    callSetter<&interface_type::setDataStartOffset>(offset);
  }

  uint64_t getAlignedSize() const { return callGetter<&interface_type::getAlignedSize>(); }
  void setAlignedSize(uint64_t size) { callSetter<&interface_type::setAlignedSize>(size); }

 private:
  explicit ContextBinaryRawDataResponse(const std::shared_ptr<ApiTable>& apiTable)
      : ApiType(apiTable) {
    construct();
  }

  // Non-owning constructor: wraps a backend-owned handle without taking ownership.
  ContextBinaryRawDataResponse(const std::shared_ptr<ApiTable>& apiTable,
                               QairtContext_BinaryRawDataResponseHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

/**
 * @brief Dma-buffer response a `ContextBinaryListDmaBufferCallback` provider populates.
 *
 *        Delivered to the provider function; the caller sets fd/data/dataStartOffset/alignedSize
 *        before returning.
 */
class ContextBinaryDmaDataResponse
    : public ApiType<ContextBinaryDmaDataResponse, QairtContext_BinaryDmaDataResponseV1_t> {
  friend Api;
  friend class ContextBinaryListDmaBufferCallback;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  ContextBinaryDmaDataResponse() noexcept                                       = default;
  ContextBinaryDmaDataResponse(ContextBinaryDmaDataResponse&&) noexcept            = default;
  ContextBinaryDmaDataResponse& operator=(ContextBinaryDmaDataResponse&&) noexcept = default;

  int32_t getFd() const { return callGetter<&interface_type::getFd>(); }
  void setFd(int32_t fd) { callSetter<&interface_type::setFd>(fd); }

  void* getData() const { return callGetter<&interface_type::getData>(); }
  void setData(void* data) { callSetter<&interface_type::setData>(data); }

  uint64_t getDataStartOffset() const { return callGetter<&interface_type::getDataStartOffset>(); }
  void setDataStartOffset(uint64_t offset) {
    callSetter<&interface_type::setDataStartOffset>(offset);
  }

  uint64_t getAlignedSize() const { return callGetter<&interface_type::getAlignedSize>(); }
  void setAlignedSize(uint64_t size) { callSetter<&interface_type::setAlignedSize>(size); }

 private:
  explicit ContextBinaryDmaDataResponse(const std::shared_ptr<ApiTable>& apiTable)
      : ApiType(apiTable) {
    construct();
  }

  // Non-owning constructor: wraps a backend-owned handle without taking ownership.
  ContextBinaryDmaDataResponse(const std::shared_ptr<ApiTable>& apiTable,
                               QairtContext_BinaryDmaDataResponseHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

/**
 * @brief Raw memory descriptor passed to a `ContextBinaryListRawBufferCallback` release function.
 */
class ContextBinaryRawDataMem
    : public ApiType<ContextBinaryRawDataMem, QairtContext_BinaryRawDataMemV1_t> {
  friend Api;
  friend class ContextBinaryListRawBufferCallback;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  ContextBinaryRawDataMem() noexcept                                  = default;
  ContextBinaryRawDataMem(ContextBinaryRawDataMem&&) noexcept            = default;
  ContextBinaryRawDataMem& operator=(ContextBinaryRawDataMem&&) noexcept = default;

  void* getMemPtr() const { return callGetter<&interface_type::getMemPtr>(); }
  void setMemPtr(void* memPtr) { callSetter<&interface_type::setMemPtr>(memPtr); }

  uint64_t getMemSize() const { return callGetter<&interface_type::getMemSize>(); }
  void setMemSize(uint64_t size) { callSetter<&interface_type::setMemSize>(size); }

 private:
  explicit ContextBinaryRawDataMem(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }

  // Non-owning constructor: wraps a backend-owned handle without taking ownership.
  ContextBinaryRawDataMem(const std::shared_ptr<ApiTable>& apiTable,
                          QairtContext_BinaryRawDataMemHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

/**
 * @brief Dma memory descriptor passed to a `ContextBinaryListDmaBufferCallback` release function.
 */
class ContextBinaryDmaDataMem
    : public ApiType<ContextBinaryDmaDataMem, QairtContext_BinaryDmaDataMemV1_t> {
  friend Api;
  friend class ContextBinaryListDmaBufferCallback;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  ContextBinaryDmaDataMem() noexcept                                  = default;
  ContextBinaryDmaDataMem(ContextBinaryDmaDataMem&&) noexcept            = default;
  ContextBinaryDmaDataMem& operator=(ContextBinaryDmaDataMem&&) noexcept = default;

  int32_t getFd() const { return callGetter<&interface_type::getFd>(); }
  void setFd(int32_t fd) { callSetter<&interface_type::setFd>(fd); }

  void* getData() const { return callGetter<&interface_type::getData>(); }
  void setData(void* data) { callSetter<&interface_type::setData>(data); }

  uint64_t getMemSize() const { return callGetter<&interface_type::getMemSize>(); }
  void setMemSize(uint64_t size) { callSetter<&interface_type::setMemSize>(size); }

 private:
  explicit ContextBinaryDmaDataMem(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }

  // Non-owning constructor: wraps a backend-owned handle without taking ownership.
  ContextBinaryDmaDataMem(const std::shared_ptr<ApiTable>& apiTable,
                          QairtContext_BinaryDmaDataMemHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

/**
 * @brief Per-context callback for `Backend::createContextFromBinaryListAsync`.
 *
 *        Obtained via `Api::make<ContextBinaryListNotifyCallback>()`.
 */
class ContextBinaryListNotifyCallback
    : public ApiType<ContextBinaryListNotifyCallback, QairtContext_BinaryListNotifyCallbackV1_t> {
  friend class Api;
  friend class ContextBinaryListParams;

 public:
  using NotifyCallback = std::function<void(std::shared_ptr<Context> context,
                                            std::shared_ptr<Graph> graph,
                                            std::string graphName,
                                            ContextCreateFromBinaryAsyncNotifyType notifyType,
                                            CommonError status)>;

  ContextBinaryListNotifyCallback() noexcept = default;
  ContextBinaryListNotifyCallback(const ContextBinaryListNotifyCallback&) = delete;
  ContextBinaryListNotifyCallback(ContextBinaryListNotifyCallback&&) noexcept = default;
  ContextBinaryListNotifyCallback& operator=(const ContextBinaryListNotifyCallback&) = delete;
  ContextBinaryListNotifyCallback& operator=(ContextBinaryListNotifyCallback&&) noexcept =
      default;

  static void trampoline(QairtContext_Handle_t contextHandle,
                         QairtGraph_Handle_t graphHandle,
                         const char* graphName,
                         QairtContext_CreateFromBinaryAsyncNotifyType_t notifyType,
                         void* userData,
                         Qairt_Status_t status) noexcept {
    auto* self = static_cast<ContextBinaryListNotifyCallback*>(userData);
    if (!self || !self->m_callback) {
      return;
    }

    try {
      std::shared_ptr<Context> contextObj;
      if (notifyType == QAIRT_CONTEXT_NOTIFY_TYPE_CONTEXT_INIT && contextHandle) {
        contextObj = std::make_shared<Context>(self->create<Context>(
            contextHandle, ContextCreateFromBinaryAsyncNotifyType::ContextInit));
      }

      std::shared_ptr<Graph> graphObj;
      if (notifyType == QAIRT_CONTEXT_NOTIFY_TYPE_GRAPH_INIT && graphHandle) {
        graphObj = std::make_shared<Graph>(self->create<Graph>(graphHandle));
      }

      self->m_callback(std::move(contextObj),
                       std::move(graphObj),
                       graphName ? std::string(graphName) : std::string(),
                       static_cast<ContextCreateFromBinaryAsyncNotifyType>(notifyType),
                       static_cast<CommonError>(status));
    } catch (...) {
    }
  }

 private:
  NotifyCallback m_callback;

  ContextBinaryListNotifyCallback(const std::shared_ptr<ApiTable>& apiTable,
                                  NotifyCallback callback)
      : ApiType(apiTable), m_callback(std::move(callback)) {
    construct(&ContextBinaryListNotifyCallback::trampoline);
  }
};

/**
 * @brief Raw-buffer data-provider/release callback for `ContextBinaryListParams`.
 *
 *        Obtained via `Api::make<ContextBinaryListRawBufferCallback>()`.
 */
class ContextBinaryListRawBufferCallback
    : public ApiType<ContextBinaryListRawBufferCallback,
                     QairtContext_BinaryListRawBufferCallbackV1_t> {
  friend class Api;
  friend class ContextBinaryListParams;

 public:
  using ProvideCallback = std::function<CommonError(std::shared_ptr<ContextBinaryDataRequest>,
                                                     std::shared_ptr<ContextBinaryRawDataResponse>,
                                                     void* userData)>;
  using ReleaseCallback =
      std::function<CommonError(std::shared_ptr<ContextBinaryRawDataMem>, void* userData)>;

  ContextBinaryListRawBufferCallback() noexcept = default;
  ContextBinaryListRawBufferCallback(const ContextBinaryListRawBufferCallback&) = delete;
  ContextBinaryListRawBufferCallback(ContextBinaryListRawBufferCallback&&) noexcept = default;
  ContextBinaryListRawBufferCallback& operator=(const ContextBinaryListRawBufferCallback&) =
      delete;
  ContextBinaryListRawBufferCallback& operator=(ContextBinaryListRawBufferCallback&&) noexcept =
      default;

  static Qairt_Status_t provideTrampoline(QairtContext_BinaryDataRequestHandle_t requestHandle,
                                          QairtContext_BinaryRawDataResponseHandle_t responseHandle,
                                          void* userData) noexcept {
    auto* self = static_cast<ContextBinaryListRawBufferCallback*>(userData);
    if (!self || !self->m_provide) {
      return static_cast<Qairt_Status_t>(CommonError::InvalidArgument);
    }
    try {
      auto request  = std::make_shared<ContextBinaryDataRequest>(self->create<ContextBinaryDataRequest>(requestHandle));
      auto response = std::make_shared<ContextBinaryRawDataResponse>(
          self->create<ContextBinaryRawDataResponse>(responseHandle));
      return static_cast<Qairt_Status_t>(self->m_provide(request, response, self->m_userData));
    } catch (...) {
      return static_cast<Qairt_Status_t>(CommonError::General);
    }
  }

  static Qairt_Status_t releaseTrampoline(QairtContext_BinaryRawDataMemHandle_t memHandle,
                                          void* userData) noexcept {
    auto* self = static_cast<ContextBinaryListRawBufferCallback*>(userData);
    if (!self || !self->m_release) {
      return static_cast<Qairt_Status_t>(CommonError::InvalidArgument);
    }
    try {
      auto mem = std::make_shared<ContextBinaryRawDataMem>(self->create<ContextBinaryRawDataMem>(memHandle));
      return static_cast<Qairt_Status_t>(self->m_release(mem, self->m_userData));
    } catch (...) {
      return static_cast<Qairt_Status_t>(CommonError::General);
    }
  }

 private:
  ProvideCallback m_provide;
  ReleaseCallback m_release;
  void* m_userData = nullptr;

  ContextBinaryListRawBufferCallback(const std::shared_ptr<ApiTable>& apiTable,
                                     ProvideCallback provide,
                                     ReleaseCallback release,
                                     void* userData)
      : ApiType(apiTable),
        m_provide(std::move(provide)),
        m_release(std::move(release)),
        m_userData(userData) {
    construct(&ContextBinaryListRawBufferCallback::provideTrampoline,
              &ContextBinaryListRawBufferCallback::releaseTrampoline);
  }
};

/**
 * @brief Dma-buffer data-provider/release callback for `ContextBinaryListParams`.
 *
 *        Obtained via `Api::make<ContextBinaryListDmaBufferCallback>()`.
 */
class ContextBinaryListDmaBufferCallback
    : public ApiType<ContextBinaryListDmaBufferCallback,
                     QairtContext_BinaryListDmaBufferCallbackV1_t> {
  friend class Api;
  friend class ContextBinaryListParams;

 public:
  using ProvideCallback = std::function<CommonError(std::shared_ptr<ContextBinaryDataRequest>,
                                                     std::shared_ptr<ContextBinaryDmaDataResponse>,
                                                     void* userData)>;
  using ReleaseCallback =
      std::function<CommonError(std::shared_ptr<ContextBinaryDmaDataMem>, void* userData)>;

  ContextBinaryListDmaBufferCallback() noexcept = default;
  ContextBinaryListDmaBufferCallback(const ContextBinaryListDmaBufferCallback&) = delete;
  ContextBinaryListDmaBufferCallback(ContextBinaryListDmaBufferCallback&&) noexcept = default;
  ContextBinaryListDmaBufferCallback& operator=(const ContextBinaryListDmaBufferCallback&) =
      delete;
  ContextBinaryListDmaBufferCallback& operator=(ContextBinaryListDmaBufferCallback&&) noexcept =
      default;

  static Qairt_Status_t provideTrampoline(QairtContext_BinaryDataRequestHandle_t requestHandle,
                                          QairtContext_BinaryDmaDataResponseHandle_t responseHandle,
                                          void* userData) noexcept {
    auto* self = static_cast<ContextBinaryListDmaBufferCallback*>(userData);
    if (!self || !self->m_provide) {
      return static_cast<Qairt_Status_t>(CommonError::InvalidArgument);
    }
    try {
      auto request  = std::make_shared<ContextBinaryDataRequest>(self->create<ContextBinaryDataRequest>(requestHandle));
      auto response = std::make_shared<ContextBinaryDmaDataResponse>(
          self->create<ContextBinaryDmaDataResponse>(responseHandle));
      return static_cast<Qairt_Status_t>(self->m_provide(request, response, self->m_userData));
    } catch (...) {
      return static_cast<Qairt_Status_t>(CommonError::General);
    }
  }

  static Qairt_Status_t releaseTrampoline(QairtContext_BinaryDmaDataMemHandle_t memHandle,
                                          void* userData) noexcept {
    auto* self = static_cast<ContextBinaryListDmaBufferCallback*>(userData);
    if (!self || !self->m_release) {
      return static_cast<Qairt_Status_t>(CommonError::InvalidArgument);
    }
    try {
      auto mem = std::make_shared<ContextBinaryDmaDataMem>(self->create<ContextBinaryDmaDataMem>(memHandle));
      return static_cast<Qairt_Status_t>(self->m_release(mem, self->m_userData));
    } catch (...) {
      return static_cast<Qairt_Status_t>(CommonError::General);
    }
  }

 private:
  ProvideCallback m_provide;
  ReleaseCallback m_release;
  void* m_userData = nullptr;

  ContextBinaryListDmaBufferCallback(const std::shared_ptr<ApiTable>& apiTable,
                                     ProvideCallback provide,
                                     ReleaseCallback release,
                                     void* userData)
      : ApiType(apiTable),
        m_provide(std::move(provide)),
        m_release(std::move(release)),
        m_userData(userData) {
    construct(&ContextBinaryListDmaBufferCallback::provideTrampoline,
              &ContextBinaryListDmaBufferCallback::releaseTrampoline);
  }
};

/**
 * @brief Per-context inputs for `Backend::createContextFromBinaryListAsync`.
 *
 *        Obtained via `Api::make<ContextBinaryListParams>()`.
 */
class ContextBinaryListParams
    : public ApiType<ContextBinaryListParams, QairtContext_BinaryListParamsV1_t> {
  friend class Api;

 public:
  ContextBinaryListParams() noexcept                                  = default;
  ContextBinaryListParams(const ContextBinaryListParams&)             = delete;
  ContextBinaryListParams(ContextBinaryListParams&&) noexcept         = default;
  ContextBinaryListParams& operator=(const ContextBinaryListParams&) = delete;
  ContextBinaryListParams& operator=(ContextBinaryListParams&&) noexcept = default;

  /**
   * @brief Set the configuration to use when creating this context.
   *
   * @param[in] config  Context configuration. Optional; omit for default configuration.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryListParams_setConfig
   */
  void setConfig(ApiTypeRef<const ContextConfiguration&> config) {
    callSetter<&interface_type::setConfig>(detail::getHandle(config));
  }

  /**
   * @brief Set the binary buffer to deserialize this context from.
   *
   * @param[in] buffer  The serialized context binary to load.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryListParams_setBinaryBuffer
   */
  void setBinaryBuffer(ApiTypeRef<const ContextBinaryBuffer&> buffer) {
    callSetter<&interface_type::setBinaryBuffer>(detail::getHandle(buffer));
  }

  /**
   * @brief Set the profile handle used to collect metrics while creating this context.
   *
   * @param[in] profile  Optional profile to populate during context creation.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryListParams_setProfile
   */
  void setProfile(ApiTypeRef<const Profile&> profile) {
    callSetter<&interface_type::setProfile>(detail::getHandle(profile));
  }

  /**
   * @brief Register the notify callback invoked as this context and its graphs become available.
   *
   * @param[in] callback  A caller-owned ContextBinaryListNotifyCallback (see
   *                       Api::make<ContextBinaryListNotifyCallback>()). Invoked once per graph
   *                       (GraphInit) and once for the overall context (ContextInit, terminal).
   *                       On ContextInit the delivered Context is owning — retain the shared_ptr
   *                       to keep it alive. The callback must outlive the
   *                       createContextFromBinaryListAsync call.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryListParams_setNotify
   */
  void setNotify(ContextBinaryListNotifyCallback& callback) {
    callSetter<&interface_type::setNotify>(getHandle(callback), static_cast<void*>(&callback));
  }

  /**
   * @brief Register a raw-buffer data-provider/release callback for this context.
   *
   *        Mutually exclusive with `setDmaBufferCallback`; the last one set wins.
   *
   * @param[in] callback  A caller-owned ContextBinaryListRawBufferCallback (see
   *                       Api::make<ContextBinaryListRawBufferCallback>()). Must outlive the
   *                       createContextFromBinaryListAsync call.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryListParams_setRawBufferCallback
   */
  void setRawBufferCallback(ContextBinaryListRawBufferCallback& callback) {
    callSetter<&interface_type::setRawBufferCallback>(getHandle(callback),
                                                       static_cast<void*>(&callback));
  }

  /**
   * @brief Register a dma-buffer data-provider/release callback for this context.
   *
   *        Mutually exclusive with `setRawBufferCallback`; the last one set wins.
   *
   * @param[in] callback  A caller-owned ContextBinaryListDmaBufferCallback (see
   *                       Api::make<ContextBinaryListDmaBufferCallback>()). Must outlive the
   *                       createContextFromBinaryListAsync call.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtContext_BinaryListParams_setDmaBufferCallback
   */
  void setDmaBufferCallback(ContextBinaryListDmaBufferCallback& callback) {
    callSetter<&interface_type::setDmaBufferCallback>(getHandle(callback),
                                                       static_cast<void*>(&callback));
  }

 private:
  explicit ContextBinaryListParams(const std::shared_ptr<ApiTable>& apiTable)
      : ApiType(apiTable) {
    construct();
  }
};

}  // namespace qairt
