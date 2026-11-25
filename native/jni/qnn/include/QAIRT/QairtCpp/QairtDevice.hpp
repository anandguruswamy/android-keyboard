//==============================================================================
//
// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// All rights reserved.
// Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

/**
 * @file
 * @brief C++ wrapper for the QAIRT Device API.
 */

#pragma once

#include <optional>

#include "QairtApi.hpp"
#include "QairtDevice/QairtDevice.h"
#include "QairtDevice/QairtDeviceInfrastructure.h"
#include "QairtLog.hpp"

namespace qairt {

/**
 * @brief Error codes returned by QAIRT device operations.
 *
 * @details
 * | Enumerator           | Description                                         |
 * |----------------------|-----------------------------------------------------|
 * | `NoError`            | Operation succeeded.                                |
 * | `UnsupportedFeature` | Requested feature is not yet supported.             |
 * | `MemAlloc`           | Memory allocation or deallocation failure.          |
 * | `InvalidArgument`    | An argument to the operation was invalid.           |
 * | `OutOfRange`         | Requested element is out of range.                  |
 * | `InvalidHandle`      | The provided handle is not valid.                   |
 * | `InvalidConfig`      | One or more configuration values are invalid.       |
 * | `HardwareUnavailable`| Requested hardware is unavailable.                  |
 * | `AssociatedToContext`| Device is associated to an existing context.        |
 * | `Undefined`          | An undefined or unknown error occurred.             |
 */
QAIRT_CPPAPI_DEFINE_ENUM(DeviceError,
                         QairtDevice_Error_t,
                         NoError             = QAIRT_DEVICE_NO_ERROR,
                         UnsupportedFeature  = QAIRT_DEVICE_ERROR_UNSUPPORTED_FEATURE,
                         MemAlloc            = QAIRT_DEVICE_ERROR_MEM_ALLOC,
                         InvalidArgument     = QAIRT_DEVICE_ERROR_INVALID_ARGUMENT,
                         OutOfRange          = QAIRT_DEVICE_ERROR_OUT_OF_RANGE,
                         InvalidHandle       = QAIRT_DEVICE_ERROR_INVALID_HANDLE,
                         InvalidConfig       = QAIRT_DEVICE_ERROR_INVALID_CONFIG,
                         HardwareUnavailable = QAIRT_DEVICE_ERROR_HARDWARE_UNAVAILABLE,
                         AssociatedToContext = QAIRT_DEVICE_ERROR_ASSOCIATED_TO_CONTEXT,
                         Undefined           = QAIRT_DEVICE_ERROR_UNDEFINED)

/**
 * @brief Placeholder for future backend-specific extended core information.
 *
 *        Reserved for future use; no members are defined at this time.
 */
class DeviceHardwareCoreExtendedInfo {};

/**
 * @brief Hardware core information for a single compute core on a device.
 *
 *        Obtained as a sub-object of @ref DeviceHardwareDeviceInfo. Core objects
 *        are not constructed directly; they are returned by the device info API.
 */
class DeviceHardwareCoreInfo
    : public ApiType<DeviceHardwareCoreInfo, QairtDevice_HardwareCoreInfoV1_t> {
  friend class Api;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  DeviceHardwareCoreInfo() noexcept                                    = default;
  DeviceHardwareCoreInfo(const DeviceHardwareCoreInfo&)                = delete;
  DeviceHardwareCoreInfo(DeviceHardwareCoreInfo&&) noexcept            = default;
  DeviceHardwareCoreInfo& operator=(const DeviceHardwareCoreInfo&)     = delete;
  DeviceHardwareCoreInfo& operator=(DeviceHardwareCoreInfo&&) noexcept = default;

  /**
   * @brief Get the hardware core identifier.
   *
   * @return Numeric core ID for this core. Use @c QAIRT_DEVICE_DEFAULT_CORE_ID
   * to select the default core.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareCoreInfo_getCoreId
   */
  uint32_t getCoreId() const { return callGetter<&interface_type::getCoreId>(); }

  /**
   * @brief Get the hardware core type.
   *
   * @return Backend-defined numeric type code identifying the kind of compute
   * core.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareCoreInfo_getCoreType
   */
  uint32_t getCoreType() const { return callGetter<&interface_type::getCoreType>(); }

  DeviceHardwareCoreExtendedInfo getExtendedInfo() const { throw QAIRT_CPPAPI_STUB_EXCEPTION; }

  /**
   * @brief Set the hardware core identifier.
   *
   * Only meaningful on an instance constructed via @ref Api::make, not one
   * obtained (read-only) from the device info query API.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareCoreInfo_setCoreId
   */
  void setCoreId(uint32_t coreId) { callSetter<&interface_type::setCoreId>(coreId); }

  /**
   * @brief Set the hardware core type.
   *
   * Only meaningful on an instance constructed via @ref Api::make, not one
   * obtained (read-only) from the device info query API.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareCoreInfo_setCoreType
   */
  void setCoreType(uint32_t coreType) { callSetter<&interface_type::setCoreType>(coreType); }

 private:
  explicit DeviceHardwareCoreInfo(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }
};

/**
 * @brief Hardware device information, including identity and core topology.
 *
 *        Obtained as a sub-object of @ref DevicePlatformInfo. Device info objects
 *        are not constructed directly; they are returned by the platform info API.
 */
class DeviceHardwareDeviceInfo
    : public ApiType<DeviceHardwareDeviceInfo, QairtDevice_HardwareDeviceInfoV1_t> {
  friend class Api;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  DeviceHardwareDeviceInfo() noexcept                                      = default;
  DeviceHardwareDeviceInfo(const DeviceHardwareDeviceInfo&)                = delete;
  DeviceHardwareDeviceInfo(DeviceHardwareDeviceInfo&&) noexcept            = default;
  DeviceHardwareDeviceInfo& operator=(const DeviceHardwareDeviceInfo&)     = delete;
  DeviceHardwareDeviceInfo& operator=(DeviceHardwareDeviceInfo&&) noexcept = default;

  /**
   * @brief Get the hardware device identifier.
   *
   * @return Numeric device ID for this hardware device. Use
   * @c QAIRT_DEVICE_DEFAULT_DEVICE_ID to select the default device.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareDeviceInfo_getDeviceId
   */
  uint32_t getDeviceId() const { return callGetter<&interface_type::getDeviceId>(); }

  /**
   * @brief Get the hardware device type.
   *
   * @return Backend-defined numeric type code identifying the class of hardware
   * device.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareDeviceInfo_getDeviceType
   */
  uint32_t getDeviceType() const { return callGetter<&interface_type::getDeviceType>(); }

  /**
   * @brief Get the number of compute cores in this hardware device.
   *
   * @return Number of core info entries reported for this device.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareDeviceInfo_getNumCoreInfos
   */
  uint32_t getNumCores() const { return callGetter<&interface_type::getNumCoreInfos>(); }

  /**
   * @brief Get the compute cores associated with this hardware device.
   *
   * Each call queries the backend and constructs a fresh, non-owning view of
   * the backend-owned core info objects. The returned vector is independent
   * of the @ref DeviceHardwareDeviceInfo instance.
   *
   * @return Vector of @ref DeviceHardwareCoreInfo objects.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareDeviceInfo_getHardwareCoreInfoAt
   */
  std::vector<DeviceHardwareCoreInfo> getCores() const {
    const uint32_t count = getNumCores();
    std::vector<DeviceHardwareCoreInfo> cores;
    cores.reserve(count);
    for (uint32_t i = 0; i < count; ++i) {
      QairtDevice_HardwareCoreInfoHandle_t h{};
      QAIRT_NO_RETURN callAndConfirm<&interface_type::getHardwareCoreInfoAt>(i, &h);
      cores.push_back(constructNonOwning<DeviceHardwareCoreInfo>(h));
    }
    return cores;
  }

  /**
   * @brief Set the hardware device identifier.
   *
   * Only meaningful on an instance constructed via @ref Api::make, not one
   * obtained (read-only) from the platform info query API.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareDeviceInfo_setDeviceId
   */
  void setDeviceId(uint32_t deviceId) { callSetter<&interface_type::setDeviceId>(deviceId); }

  /**
   * @brief Set the hardware device type.
   *
   * Only meaningful on an instance constructed via @ref Api::make, not one
   * obtained (read-only) from the platform info query API.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareDeviceInfo_setDeviceType
   */
  void setDeviceType(uint32_t deviceType) {
    callSetter<&interface_type::setDeviceType>(deviceType);
  }

  /**
   * @brief Append a copy of a hardware core info to this device.
   *
   * Only meaningful on an instance constructed via @ref Api::make, not one
   * obtained (read-only) from the platform info query API. _coreInfo_ is not
   * consumed; the caller retains ownership.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_HardwareDeviceInfo_addHardwareCoreInfo
   */
  void addCore(const DeviceHardwareCoreInfo& coreInfo) {
    callAndConfirm<&interface_type::addHardwareCoreInfo>(getHandle(coreInfo));
  }

 private:
  explicit DeviceHardwareDeviceInfo(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }
};

/**
 * @brief Platform-level hardware information, enumerating the devices present
 * on the platform.
 *
 *        Obtained via @ref DeviceConfiguration::getPlatformInfo().
 */
class DevicePlatformInfo : public ApiType<DevicePlatformInfo, QairtDevice_PlatformInfoV1_t> {
  friend class Api;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  /**
   * @brief Get the number of hardware devices on this platform.
   *
   * @return Number of @ref DeviceHardwareDeviceInfo entries.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_PlatformInfo_getNumHardwareDeviceInfo
   */
  size_t getNumHwDevices() const {
    return static_cast<size_t>(callGetter<&interface_type::getNumHwDeviceInfo>());
  }

  /**
   * @brief Get the hardware devices on this platform.
   *
   * Each call queries the backend and constructs a fresh, non-owning view of
   * the backend-owned device info objects. The returned vector is independent
   * of the @ref DevicePlatformInfo instance.
   *
   * @return Vector of @ref DeviceHardwareDeviceInfo objects.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_PlatformInfo_getHardwareDeviceInfoAt
   */
  std::vector<DeviceHardwareDeviceInfo> getHwDevices() const {
    const uint32_t count = static_cast<uint32_t>(getNumHwDevices());
    std::vector<DeviceHardwareDeviceInfo> hwDevices;
    hwDevices.reserve(count);
    for (uint32_t i = 0; i < count; ++i) {
      QairtDevice_HardwareDeviceInfoHandle_t h{};
      QAIRT_NO_RETURN callAndConfirm<&interface_type::getHwDeviceInfoAt>(i, &h);
      hwDevices.push_back(constructNonOwning<DeviceHardwareDeviceInfo>(h));
    }
    return hwDevices;
  }

  /**
   * @brief Replace the hardware device list on this platform info.
   *
   * Only meaningful on an instance constructed via @ref Api::make, not one
   * obtained (read-only) from the device platform info query API. Each entry
   * is appended as a copy; _hwDevices_ is not consumed.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_PlatformInfo_addHardwareDeviceInfo
   */

  void setHwDevices(std::vector<DeviceHardwareDeviceInfo> hwDevices) {
    for (const auto& hwDevice : hwDevices) {
      callAndConfirm<&interface_type::addHardwareDeviceInfo>(getHandle(hwDevice));
    }
  }

 private:
  void prepareToCross() const {}
  void updateAfterCross() const {}

  explicit DevicePlatformInfo(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }
};

/**
 * @brief Abstract base class for backend-specific device custom configuration.
 *
 *        Subclass to provide a backend-specific custom configuration handle; pass
 *        an instance to @ref DeviceConfiguration::setCustomConfig(). Refer to the
 *        backend documentation for required subclass implementations.
 */
class DeviceCustomConfig : public CustomConfigType {
 protected:
  DeviceCustomConfig()                                         = default;
  DeviceCustomConfig(const DeviceCustomConfig&)                = default;
  DeviceCustomConfig(DeviceCustomConfig&&) noexcept            = default;
  DeviceCustomConfig& operator=(const DeviceCustomConfig&)     = default;
  DeviceCustomConfig& operator=(DeviceCustomConfig&&) noexcept = default;

 public:
  virtual ~DeviceCustomConfig() = default;
  /**
   * @brief Get the underlying C API custom configuration handle.
   *
   * Implement this method in a backend-specific subclass to return the
   * handle that will be passed to @ref QairtDevice_Config_setCustomConfig.
   * Refer to the backend documentation for required subclass implementations
   * and valid handle values.
   *
   * @return Backend-specific custom configuration handle.
   *
   * @see QairtDevice_Config_setCustomConfig
   */
  virtual QairtDevice_CustomConfigHandle_t getCustomConfigHandle() const = 0;
};

/**
 * @brief Abstract base class for backend-specific collections of device custom
 * configurations.
 *
 * Subclass this to provide multiple backend-specific configuration handles to
 * @ref DeviceConfiguration::setCustomConfigs(). Refer to the backend
 * documentation for the concrete subclass and valid handle values.
 */
class DeviceCustomConfiguration {
 protected:
  DeviceCustomConfiguration()                                                = default;
  DeviceCustomConfiguration(const DeviceCustomConfiguration&)                = default;
  DeviceCustomConfiguration(DeviceCustomConfiguration&&) noexcept            = default;
  DeviceCustomConfiguration& operator=(const DeviceCustomConfiguration&)     = default;
  DeviceCustomConfiguration& operator=(DeviceCustomConfiguration&&) noexcept = default;

 public:
  virtual ~DeviceCustomConfiguration() = default;

  /**
   * @brief Get the list of underlying C handles for this custom configuration
   * collection.
   *
   * @return Vector of backend-specific device custom configuration handles.
   */
  virtual std::vector<QairtDevice_CustomConfigHandle_t> getCustomConfigs() const = 0;
};

/**
 * @brief Configuration object for device creation.
 *
 *        Construct via @ref Api::make<DeviceConfiguration>() and pass to the device
 *        creation API. Holds platform info and optional backend-specific custom config
 */
class DeviceConfiguration : public ApiType<DeviceConfiguration, QairtDevice_ConfigV1_t> {
  friend class Api;

  detail::crossable<detail::non_owning<DevicePlatformInfo>,
                    &interface_type::getPlatformInfo,
                    &interface_type::setPlatformInfo>
      m_platformInfo;

 public:
  DeviceConfiguration() noexcept                                 = default;
  DeviceConfiguration(const DeviceConfiguration&)                = delete;
  DeviceConfiguration(DeviceConfiguration&&) noexcept            = default;
  DeviceConfiguration& operator=(DeviceConfiguration&&) noexcept = default;
  DeviceConfiguration& operator=(const DeviceConfiguration&)     = delete;


  /**
   * @brief Set backend-specific custom configuration for this device
   * configuration.
   *
   * @param[in] config  Backend-specific custom configuration object.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtDevice_Config_setCustomConfig
   */
  void setCustomConfig(const DeviceCustomConfig& config) {
    auto cch = config.getCustomConfigHandle();
    callAndConfirm<&interface_type::setCustomConfig>(cch);
  }

  /**
   * @brief Clear all backend-specific custom configurations from this device
   * configuration.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_Config_clearCustomConfigs
   */
  void clearCustomConfigs() { callAndConfirm<&interface_type::clearCustomConfigs>(); }

  /**
   * @brief Get the platform info associated with this device configuration.
   *
   * @return Reference to the platform info for this configuration.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_Config_getPlatformInfo
   */
  DevicePlatformInfo& getPlatformInfo() { return m_platformInfo.bindNonOwning(*this); }

  /**
   * @brief Get the platform info associated with this device configuration.
   *
   * @return Const reference to the platform info for this configuration.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_Config_getPlatformInfo
   */
  const DevicePlatformInfo& getPlatformInfo() const { return m_platformInfo.bindNonOwning(*this); }

  /**
   * @brief Set the platform info for this device configuration.
   *
   * @param[in] platformInfo  Platform info describing the hardware devices to
   *                          request. Must be a valid, fully configured object.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtDevice_Config_setPlatformInfo
   */
  void setPlatformInfo(const DevicePlatformInfo& platformInfo) {
    QAIRT_NO_RETURN m_platformInfo.setNonOwning(*this, platformInfo);
  }

 private:
  // Construct via Api::make<DeviceConfiguration>(). Unlike the inherited
  // ApiType(apiTable) constructor (which leaves the underlying C handle null),
  // this calls construct() so the device-config handle is actually allocated
  // and registered with the QAIRT handle manager. Without this, getHandle()
  // returns a null handle and passing it across the C ABI (e.g. to
  // beforeCreateDevice) fails with QAIRT_COMMON_ERROR_INVALID_HANDLE.
  explicit DeviceConfiguration(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }

  void prepareToCross() const { prepareMultipleToCross(m_platformInfo); }
  void updateAfterCross() const { updateMultipleAfterCross(m_platformInfo); }

  DeviceConfiguration(const std::shared_ptr<ApiTable>& apiTable, QairtDevice_ConfigHandle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

/**
 * @brief Wrapper for the QAIRT Device Platform Info Provider handle.
 *
 *        Produces @ref DevicePlatformInfo handles via @ref getPlatformInfo().
 *        The returned platform info is non-owning; its handle remains valid only
 *        as long as the provider is alive. Most users obtain platform info
 *        through @ref Device::getInfo(), which manages a provider's lifetime
 *        automatically.
 */
class DevicePlatformInfoProvider
    : public ApiType<DevicePlatformInfoProvider, QairtDevice_PlatformInfoProviderV1_t> {
  friend class Api;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  DevicePlatformInfoProvider() noexcept                                        = default;
  DevicePlatformInfoProvider(const DevicePlatformInfoProvider&)                = delete;
  DevicePlatformInfoProvider(DevicePlatformInfoProvider&&) noexcept            = default;
  DevicePlatformInfoProvider& operator=(const DevicePlatformInfoProvider&)     = delete;
  DevicePlatformInfoProvider& operator=(DevicePlatformInfoProvider&&) noexcept = default;

  /**
   * @brief Get a platform info object describing the hardware visible to QAIRT.
   *
   * The returned platform info is non-owning. Its underlying handle is owned by
   * this provider; freeing the provider invalidates any platform info obtained
   * from it.
   *
   * @return Platform info object enumerating the available hardware devices.
   *
   * @throws qairt::Exception on invalid handle, invalid argument, unsupported
   *         feature, or insufficient memory.
   *
   * @see QairtDevice_PlatformInfoProvider_getPlatformInfo
   */
  DevicePlatformInfo getPlatformInfo() {
    QairtDevice_PlatformInfoHandle_t handle{};
    QAIRT_NO_RETURN callAndConfirm<&interface_type::getPlatformInfo>(&handle);
    return constructNonOwning<DevicePlatformInfo>(handle);
  }

 private:
  explicit DevicePlatformInfoProvider(const std::shared_ptr<ApiTable>& apiTable)
      : ApiType(apiTable) {
    construct(QairtLog_Handle_t{});
  }
  DevicePlatformInfoProvider(const std::shared_ptr<ApiTable>& apiTable, const Log& log)
      : ApiType(apiTable) {
    construct(getHandle(log));
  }
};

/**
 * @brief Typed wrapper for a backend-specific device infrastructure handle.
 *
 *        Lightweight, copyable value type that hides the raw
 *        @c QairtDevice_InfrastructureHandle_t. The underlying handle is
 *        backend-owned memory; refer to the backend's documentation for
 *        usage and lifetime semantics.
 *
 *        Returned by @ref Device::getInfrastructure() and
 *        @ref DeviceInfrastructureProvider::getInfrastructure().
 */
class DeviceInfrastructure {
  QairtDevice_InfrastructureHandle_t m_handle{};

 public:
  DeviceInfrastructure() noexcept                                       = default;
  DeviceInfrastructure(const DeviceInfrastructure&) noexcept            = default;
  DeviceInfrastructure(DeviceInfrastructure&&) noexcept                 = default;
  DeviceInfrastructure& operator=(const DeviceInfrastructure&) noexcept = default;
  DeviceInfrastructure& operator=(DeviceInfrastructure&&) noexcept      = default;

  explicit DeviceInfrastructure(QairtDevice_InfrastructureHandle_t handle) noexcept
      : m_handle(handle) {}

  /// @brief Get the underlying QAIRT C handle for use with backend-specific
  /// APIs.
  QairtDevice_InfrastructureHandle_t getHandle() const noexcept { return m_handle; }

  /// @brief True if this wraps a non-null handle.
  explicit operator bool() const noexcept { return m_handle != nullptr; }
};

/**
 * @brief Wrapper for a QAIRT Device handle.
 */
class Device : public ApiType<Device, QairtDevice_V1_t> {
  friend class Api;

  detail::crossable<detail::non_owning<DeviceConfiguration>,
                    &interface_type::getConfig,
                    &interface_type::setConfig>
      m_config;

  // Default-constructed with a null handle; populated on first getInfo() call
  // via create<>() and freed automatically by ~ApiType when this Device is
  // destroyed. The DevicePlatformInfo returned by getInfo() is non-owning and
  // remains valid for as long as this Device lives.
  DevicePlatformInfoProvider m_platformInfoProvider;

  DevicePlatformInfoProvider& ensurePlatformInfoProvider() {
    if (!getHandle(m_platformInfoProvider)) {
      m_platformInfoProvider = create<DevicePlatformInfoProvider>();
    }
    return m_platformInfoProvider;
  }

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  /**
   * @brief Set the configuration for this device.
   *
   * @param[in] deviceConfig  Device configuration to apply. Must be a valid,
   *                          fully constructed object.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtDevice_setConfig
   */
  void setConfig(const DeviceConfiguration& deviceConfig) {
    QAIRT_NO_RETURN m_config.setNonOwning(*this, deviceConfig);
  }

  /**
   * @brief Get the configuration associated with this device.
   *
   * @return Reference to the device configuration.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtDevice_getConfig
   */
  DeviceConfiguration& getConfig() { return m_config.bindNonOwning(*this); }

  /// @see Device::getConfig()
  const DeviceConfiguration& getConfig() const { return m_config.bindNonOwning(*this); }

  /**
   * @brief Get platform info describing the hardware visible to QAIRT.
   *
   * Convenience wrapper that lazily constructs a @ref
   * DevicePlatformInfoProvider tied to this Device's lifetime and queries it.
   * The returned platform info is non-owning and remains valid for as long as
   * this Device lives.
   *
   * @return Platform info object enumerating the available hardware devices.
   *
   * @throws qairt::Exception on invalid argument, unsupported feature, or
   *         insufficient memory.
   *
   * @see QairtDevice_PlatformInfoProvider_getPlatformInfo
   */
  DevicePlatformInfo getInfo() { return ensurePlatformInfoProvider().getPlatformInfo(); }

  /**
   * @brief Get device hardware infrastructure interface object.
   *
   * Optional capability; support is advertised via QairtCapability. If
   * supported, please refer to documentation and/or header file provided by the
   * backend for usage information.
   *
   * Convenience wrapper that dispatches through the
   * @ref DeviceInfrastructureProvider in the API table without holding an
   * instance.
   *
   * @return Infrastructure interface object. The handle returned is
   *         backend-owned memory.
   *
   * @throws qairt::Exception on invalid argument, unsupported feature, or
   *         insufficient memory.
   *
   * @see QairtDevice_InfrastructureProvider_getInfrastructure
   */
  DeviceInfrastructure getInfrastructure() const {
    QairtDevice_InfrastructureHandle_t toret{};
    QAIRT_NO_RETURN
    callNoHandle<&QairtDevice_InfrastructureProviderV1_t::getDeviceInfra>(&toret);
    return DeviceInfrastructure{toret};
  }

 private:
  void prepareToCross() const { prepareMultipleToCross(m_config); }
  void updateAfterCross() const { updateMultipleAfterCross(m_config); }

  Device(const std::shared_ptr<ApiTable>& apiTable,
         const Log& log,
         const DeviceConfiguration& config)
      : ApiType(apiTable) {
    construct(getHandle(log), getHandle(config));
  }
};

/**
 * @brief Wrapper for the QAIRT Device Infrastructure Provider.
 *
 *        Exposes the backend-agnostic entry point for retrieving a
 *        backend-specific device infrastructure handle. Unlike
 *        @ref DevicePlatformInfoProvider, this provider has no instance state
 *        or lifecycle (no @c create / @c free); it is a pure dispatcher.
 *
 *        Most users obtain the infrastructure handle through
 *        @ref Device::getInfrastructure() instead of constructing this class
 *        directly.
 */
class DeviceInfrastructureProvider
    : public ApiType<DeviceInfrastructureProvider, QairtDevice_InfrastructureProviderV1_t> {
  friend class Api;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  DeviceInfrastructureProvider() noexcept                                          = default;
  DeviceInfrastructureProvider(const DeviceInfrastructureProvider&)                = delete;
  DeviceInfrastructureProvider(DeviceInfrastructureProvider&&) noexcept            = default;
  DeviceInfrastructureProvider& operator=(const DeviceInfrastructureProvider&)     = delete;
  DeviceInfrastructureProvider& operator=(DeviceInfrastructureProvider&&) noexcept = default;

  /**
   * @brief Get device hardware infrastructure interface object.
   *
   * Optional capability; support is advertised via QairtCapability. If
   * supported, please refer to documentation and/or header file provided by the
   * backend for usage information.
   *
   * @return Infrastructure interface object. The handle returned is
   *         backend-owned memory.
   *
   * @throws qairt::Exception on invalid argument, unsupported feature, or
   *         insufficient memory.
   *
   * @see QairtDevice_InfrastructureProvider_getInfrastructure
   */
  DeviceInfrastructure getInfrastructure() const {
    QairtDevice_InfrastructureHandle_t toret{};
    QAIRT_NO_RETURN callNoHandle<&interface_type::getDeviceInfra>(&toret);
    return DeviceInfrastructure{toret};
  }
};
//
}  // namespace qairt
