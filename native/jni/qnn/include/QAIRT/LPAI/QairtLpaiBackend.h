//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================

/** @file
 *  @brief QAIRT LPAI Backend API specialization.

 */

#ifndef QAIRT_LPAI_BACKEND_H
#define QAIRT_LPAI_BACKEND_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#include "QairtBackend/QairtBackend.h"
#include "QairtBackend/QairtBackendConfig.h"
#include "QairtCommon/QairtCommon.h"

//=============================================================================
// Macros
//=============================================================================

/// Base ID for all LPAI backend interface IDs.
#define QAIRT_MIN_ID_LPAI_BACKEND (QAIRT_MIN_ID_LPAI + 1000)

/// Interface ID for QairtLpaiBackend_HwInfoV1_t.
#define QAIRT_LPAI_BACKEND_HW_INFO_ID (QAIRT_MIN_ID_LPAI_BACKEND + 0)

/// Interface ID for QairtLpaiBackend_CustomConfigV1_t.
#define QAIRT_LPAI_BACKEND_CUSTOM_CONFIG_ID (QAIRT_MIN_ID_LPAI_BACKEND + 1)

/// Interface ID for QairtLpaiBackend_PropertiesV1_t.
#define QAIRT_LPAI_BACKEND_PROPERTIES_ID (QAIRT_MIN_ID_LPAI_BACKEND + 2)

/// Interface ID for QairtLpaiBackend_AlignmentReqV1_t.
#define QAIRT_LPAI_BACKEND_ALIGNMENT_REQ_ID (QAIRT_MIN_ID_LPAI_BACKEND + 3)

/// Interface ID for QairtLpaiBackend_eNPUCoreInfoV1_t.
#define QAIRT_LPAI_BACKEND_ENPU_CORE_INFO_ID (QAIRT_MIN_ID_LPAI_BACKEND + 4)

/// Interface ID for QairtLpaiBackend_eNPUCoreUtilDataV1_t.
#define QAIRT_LPAI_BACKEND_ENPU_CORE_UTIL_DATA_ID (QAIRT_MIN_ID_LPAI_BACKEND + 5)

/// Interface ID for QairtLpaiBackend_RpcPollingConfigV1_t.
#define QAIRT_LPAI_BACKEND_RPC_POLLING_CONFIG_ID (QAIRT_MIN_ID_LPAI_BACKEND + 6)

//=============================================================================
// Data Types
//=============================================================================

/**
 * @brief Handle for an LPAI backend HW info configuration object.
 */
typedef struct _QairtLpaiBackend_HwInfoHandle_t* QairtLpaiBackend_HwInfoHandle_t;

/**
 * @brief Handle for an LPAI backend custom configuration object.
 */
typedef QairtBackend_CustomConfigHandle_t QairtLpaiBackend_CustomConfigHandle_t;

/**
 * @brief Handle to buffer alignment requirement data.
 */
typedef struct _QairtLpaiBackend_AlignmentReqHandle_t* QairtLpaiBackend_AlignmentReqHandle_t;

/**
 * @brief Handle to eNPU core info data.
 */
typedef struct _QairtLpaiBackend_eNPUCoreInfoHandle_t* QairtLpaiBackend_eNPUCoreInfoHandle_t;

/**
 * @brief Handle to an eNPU core utilization data snapshot for a single core.
 *
 *        DEBUG feature for performance analysis only — must NOT be used in production code
 *        paths. Measurement must be enabled first via
 *        QairtLpaiBackend_Properties_setEnpuCoreUtilEnabled().
 */
typedef struct _QairtLpaiBackend_eNPUCoreUtilDataHandle_t*
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t;

/**
 * @brief Handle for an LPAI backend RPC polling configuration object.
 */
typedef struct _QairtLpaiBackend_RpcPollingConfigHandle_t*
    QairtLpaiBackend_RpcPollingConfigHandle_t;

/**
 * @brief Compilation target for the LPAI backend.
 */
typedef enum {
  /// LPAI model will be compiled for x86
  QAIRT_LPAI_BACKEND_TARGET_X86 = 0,
  /// LPAI model will be compiled for ARM
  QAIRT_LPAI_BACKEND_TARGET_ARM = 1,
  /// LPAI model will be compiled for ADSP
  QAIRT_LPAI_BACKEND_TARGET_ADSP = 2,
  /// LPAI model will be compiled for TENSILICA
  QAIRT_LPAI_BACKEND_TARGET_TENSILICA = 3,
  // Unused, present to ensure 32 bits.
  QAIRT_LPAI_BACKEND_TARGET_UNDEFINED = 0x7FFFFFFF
} QairtLpaiBackend_Target_t;

/**
 * @brief LPAI hardware version.
 */
typedef enum {
  /// No LPAI HW will be used
  QAIRT_LPAI_BACKEND_HW_VERSION_NA = 0,
  /// LPAI HW version v1
  QAIRT_LPAI_BACKEND_HW_VERSION_V1 = 0x00000001,
  /// LPAI HW version v2
  QAIRT_LPAI_BACKEND_HW_VERSION_V2 = 0x00000002,
  /// LPAI HW version v3
  QAIRT_LPAI_BACKEND_HW_VERSION_V3 = 0x00000003,
  /// LPAI HW version v4
  QAIRT_LPAI_BACKEND_HW_VERSION_V4 = 0x00000004,
  /// LPAI HW version v5
  QAIRT_LPAI_BACKEND_HW_VERSION_V5 = 0x00000005,
  /// LPAI HW version v5.1
  QAIRT_LPAI_BACKEND_HW_VERSION_V5_1 = 0x00010005,
  /// LPAI HW version v6
  QAIRT_LPAI_BACKEND_HW_VERSION_V6 = 0x00000006,
  /// LPAI HW version v7
  QAIRT_LPAI_BACKEND_HW_VERSION_V7 = 0x00000007,
  /// LPAI HW default version v7
  QAIRT_LPAI_BACKEND_HW_VERSION_DEFAULT = QAIRT_LPAI_BACKEND_HW_VERSION_V7,
  // Unused, present to ensure 32 bits.
  QAIRT_LPAI_BACKEND_HW_VERSION_UNDEFINED = 0x7FFFFFFF
} QairtLpaiBackend_HwVersion_t;

//=============================================================================
// Public Functions — HW Info
//=============================================================================

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create an LPAI backend HW info handle.
 *
 * @param[out] hwInfo Receives the newly created handle on success.
 *                    Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if hwInfo is NULL.
 *         QAIRT_COMMON_ERROR_MEM_ALLOC if allocation fails.
 *
 * @note Use corresponding API through QairtLpaiBackend_HwInfoV1_t.
 */
Qairt_Status_t QairtLpaiBackend_HwInfo_create(QairtLpaiBackend_HwInfoHandle_t* hwInfo);

/**
 * @brief Free an LPAI backend HW info handle.
 *
 * @param[in] hwInfo The handle to free.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if hwInfo is NULL or not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_HwInfoV1_t.
 */
Qairt_Status_t QairtLpaiBackend_HwInfo_free(QairtLpaiBackend_HwInfoHandle_t hwInfo);

/**
 * @brief Set the compilation target on an HW info handle.
 *
 * @param[in] hwInfo The handle to set the configuration on.
 *
 * @param[in] target The desired compilation target.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if hwInfo is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_HwInfoV1_t.
 */
Qairt_Status_t QairtLpaiBackend_HwInfo_setTarget(QairtLpaiBackend_HwInfoHandle_t hwInfo,
                                                 QairtLpaiBackend_Target_t target);

/**
 * @brief Get the compilation target from an HW info handle.
 *
 * @param[in] hwInfo The handle to get the configuration from.
 *
 * @param[out] target Receives the currently configured target.
 *                    Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if hwInfo is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if target is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_HwInfoV1_t.
 */
Qairt_Status_t QairtLpaiBackend_HwInfo_getTarget(QairtLpaiBackend_HwInfoHandle_t hwInfo,
                                                 QairtLpaiBackend_Target_t* target);

/**
 * @brief Set the hardware version on an HW info handle.
 *
 * @param[in] hwInfo The handle to set the configuration on.
 *
 * @param[in] hwVersion The desired hardware version.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if hwInfo is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_HwInfoV1_t.
 */
Qairt_Status_t QairtLpaiBackend_HwInfo_setHwVersion(QairtLpaiBackend_HwInfoHandle_t hwInfo,
                                                    QairtLpaiBackend_HwVersion_t hwVersion);

/**
 * @brief Get the hardware version from an HW info handle.
 *
 * @param[in] hwInfo The handle to get the configuration from.
 *
 * @param[out] hwVersion Receives the currently configured hardware version.
 *                       Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if hwInfo is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if hwVersion is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_HwInfoV1_t.
 */
Qairt_Status_t QairtLpaiBackend_HwInfo_getHwVersion(QairtLpaiBackend_HwInfoHandle_t hwInfo,
                                                    QairtLpaiBackend_HwVersion_t* hwVersion);

//=============================================================================
// Public Functions — RPC Polling Config
//=============================================================================

/**
 * @brief Create an LPAI backend RPC polling configuration handle.
 *
 * @param[out] rpcPolling Receives the newly created handle on success.
 *                        Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if rpcPolling is NULL.
 *         QAIRT_COMMON_ERROR_MEM_ALLOC if allocation fails.
 *
 * @note Use corresponding API through QairtLpaiBackend_RpcPollingConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_RpcPollingConfig_create(
    QairtLpaiBackend_RpcPollingConfigHandle_t* rpcPolling);

/**
 * @brief Free an LPAI backend RPC polling configuration handle.
 *
 * @param[in] rpcPolling The handle to free.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if rpcPolling is NULL or not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_RpcPollingConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_RpcPollingConfig_free(
    QairtLpaiBackend_RpcPollingConfigHandle_t rpcPolling);

/**
 * @brief Set the enable state on an RPC polling configuration handle.
 *
 * @param[in] rpcPolling The handle to set the configuration on.
 * @param[in] enable     Zero disables all RPC polling; nonzero enables it.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if rpcPolling is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_RpcPollingConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_RpcPollingConfig_setEnable(
    QairtLpaiBackend_RpcPollingConfigHandle_t rpcPolling, uint32_t enable);

/**
 * @brief Get the enable state from an RPC polling configuration handle.
 *
 * @param[in]  rpcPolling The handle to get the configuration from.
 * @param[out] enable     Receives the currently configured enable state, or
 *                        QNN_LPAI_BACKEND_RPC_POLLING_ENABLE_DEFAULT if setEnable() was never
 *                        called on this handle. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if rpcPolling is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if enable is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_RpcPollingConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_RpcPollingConfig_getEnable(
    QairtLpaiBackend_RpcPollingConfigHandle_t rpcPolling, uint32_t* enable);

/**
 * @brief Set the DSP polling timeout, in microseconds, on an RPC polling configuration handle.
 *
 * @param[in] rpcPolling         The handle to set the configuration on.
 * @param[in] dspPollingTimeoutUs Duration the DSP remains active polling for new inference
 *                                requests after being woken up, in microseconds.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if rpcPolling is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_RpcPollingConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_RpcPollingConfig_setDSPPollingTimeoutUs(
    QairtLpaiBackend_RpcPollingConfigHandle_t rpcPolling, uint32_t dspPollingTimeoutUs);

/**
 * @brief Get the DSP polling timeout, in microseconds, from an RPC polling configuration handle.
 *
 * @param[in]  rpcPolling          The handle to get the configuration from.
 * @param[out] dspPollingTimeoutUs Receives the currently configured DSP polling timeout, in
 *                                 microseconds, or QNN_LPAI_BACKEND_RPC_POLLING_DSP_TIMEOUT_DEFAULT_US
 *                                 if setDSPPollingTimeoutUs() was never called on this handle.
 *                                 Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if rpcPolling is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if dspPollingTimeoutUs is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_RpcPollingConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_RpcPollingConfig_getDSPPollingTimeoutUs(
    QairtLpaiBackend_RpcPollingConfigHandle_t rpcPolling, uint32_t* dspPollingTimeoutUs);

/**
 * @brief Set the HLOS wakeup lead time, in microseconds, on an RPC polling configuration handle.
 *
 * @param[in] rpcPolling            The handle to set the configuration on.
 * @param[in] hlosWakeupLeadTimeUs  How far in advance of estimated inference completion HLOS
 *                                  is woken up, in microseconds.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if rpcPolling is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_RpcPollingConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_RpcPollingConfig_setHLOSWakeupLeadTimeUs(
    QairtLpaiBackend_RpcPollingConfigHandle_t rpcPolling, uint32_t hlosWakeupLeadTimeUs);

/**
 * @brief Get the HLOS wakeup lead time, in microseconds, from an RPC polling configuration
 *        handle.
 *
 * @param[in]  rpcPolling           The handle to get the configuration from.
 * @param[out] hlosWakeupLeadTimeUs Receives the currently configured HLOS wakeup lead time, in
 *                                  microseconds, or
 *                                  QNN_LPAI_BACKEND_RPC_POLLING_HLOS_WAKEUP_LEAD_TIME_DEFAULT_US
 *                                  if setHLOSWakeupLeadTimeUs() was never called on this handle.
 *                                  Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if rpcPolling is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if hlosWakeupLeadTimeUs is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_RpcPollingConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_RpcPollingConfig_getHLOSWakeupLeadTimeUs(
    QairtLpaiBackend_RpcPollingConfigHandle_t rpcPolling, uint32_t* hlosWakeupLeadTimeUs);

/**
 * @brief Set the HLOS polling timeout, in microseconds, on an RPC polling configuration handle.
 *
 * @param[in] rpcPolling           The handle to set the configuration on.
 * @param[in] hlosPollingTimeoutUs Maximum duration HLOS polls for the inference result once
 *                                 polling starts, in microseconds.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if rpcPolling is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_RpcPollingConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_RpcPollingConfig_setHLOSPollingTimeoutUs(
    QairtLpaiBackend_RpcPollingConfigHandle_t rpcPolling, uint32_t hlosPollingTimeoutUs);

/**
 * @brief Get the HLOS polling timeout, in microseconds, from an RPC polling configuration
 *        handle.
 *
 * @param[in]  rpcPolling           The handle to get the configuration from.
 * @param[out] hlosPollingTimeoutUs Receives the currently configured HLOS polling timeout, in
 *                                  microseconds, or
 *                                  QNN_LPAI_BACKEND_RPC_POLLING_HLOS_TIMEOUT_DEFAULT_US if
 *                                  setHLOSPollingTimeoutUs() was never called on this handle.
 *                                  Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if rpcPolling is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if hlosPollingTimeoutUs is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_RpcPollingConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_RpcPollingConfig_getHLOSPollingTimeoutUs(
    QairtLpaiBackend_RpcPollingConfigHandle_t rpcPolling, uint32_t* hlosPollingTimeoutUs);

//=============================================================================
// Public Functions — Backend Custom Config
//=============================================================================

/**
 * @brief Create an LPAI backend custom configuration handle.
 *
 * @param[out] customConfig Receives the newly created handle on success.
 *                          Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if customConfig is NULL.
 *         QAIRT_COMMON_ERROR_MEM_ALLOC if allocation fails.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_create(
    QairtLpaiBackend_CustomConfigHandle_t* customConfig);

/**
 * @brief Free an LPAI backend custom configuration handle.
 *
 * @param[in] customConfig The handle to free.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is NULL or not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_free(
    QairtLpaiBackend_CustomConfigHandle_t customConfig);

/**
 * @brief Associate an HW info handle with a backend custom config.
 *
 *        The HW info handle is copied by value into the custom config.
 *        The caller retains ownership of hwInfo and may free it after
 *        this call.
 *
 * @param[in] customConfig The handle to set the configuration on.
 *
 * @param[in] hwInfo A valid, populated HW info handle.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if either handle is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_setHwInfo(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, QairtLpaiBackend_HwInfoHandle_t hwInfo);

/**
 * @brief Get a non-owning reference to the HW info stored in this custom config.
 *
 *        The returned handle is a non-owning reference valid for the lifetime
 *        of customConfig.  Do not free it.
 *
 * @param[in] customConfig The handle to get the configuration from.
 *
 * @param[out] hwInfo Receives a non-owning handle to the stored HW info.
 *                    Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if hwInfo is NULL or if
 *         setHwInfo() has not been called on this handle.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_getHwInfo(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, QairtLpaiBackend_HwInfoHandle_t* hwInfo);

/**
 * @brief Associate an RPC polling config handle with a backend custom config.
 *
 *        The RPC polling handle is copied by value into the custom config.
 *        The caller retains ownership of rpcPolling and may free it after
 *        this call.
 *
 * @param[in] customConfig The handle to set the configuration on.
 *
 * @param[in] rpcPolling A valid, populated RPC polling config handle.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if either handle is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_setRpcPolling(
    QairtLpaiBackend_CustomConfigHandle_t customConfig,
    QairtLpaiBackend_RpcPollingConfigHandle_t rpcPolling);

/**
 * @brief Get a non-owning reference to the RPC polling config stored in this custom config.
 *
 *        The returned handle is a non-owning reference valid for the lifetime
 *        of customConfig.  Do not free it.
 *
 * @param[in] customConfig The handle to get the configuration from.
 *
 * @param[out] rpcPolling Receives a non-owning handle to the stored RPC polling config.
 *                        Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if rpcPolling is NULL or if
 *         setRpcPolling() has not been called on this handle.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_getRpcPolling(
    QairtLpaiBackend_CustomConfigHandle_t customConfig,
    QairtLpaiBackend_RpcPollingConfigHandle_t* rpcPolling);

/**
 * @brief Set the backend-global AIX dump debug flag for the LPAI backend.
 *
 * @param[in] customConfig The handle to set the configuration on.
 *
 * @param[in] aixDump Non-zero to enable AIX dump, zero to disable it.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_setAixDump(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, uint8_t aixDump);

/**
 * @brief Get the backend-global AIX dump debug flag from the LPAI backend custom config.
 *
 * @param[in] customConfig The handle to get the configuration from.
 *
 * @param[out] aixDump Receives the AIX dump flag. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if aixDump is NULL or if
 *         setAixDump() has not been called on this handle.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_getAixDump(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, uint8_t* aixDump);

/**
 * @brief Set the backend-global force-NHWC debug flag for the LPAI backend.
 *
 * @param[in] customConfig The handle to set the configuration on.
 *
 * @param[in] forceNhwc Non-zero to force NHWC layout, zero to disable it.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_setForceNhwc(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, uint8_t forceNhwc);

/**
 * @brief Get the backend-global force-NHWC debug flag from the LPAI backend custom config.
 *
 * @param[in] customConfig The handle to get the configuration from.
 *
 * @param[out] forceNhwc Receives the force-NHWC flag. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if forceNhwc is NULL or if
 *         setForceNhwc() has not been called on this handle.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_getForceNhwc(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, uint8_t* forceNhwc);

/**
 * @brief Set the backend-global compute-unit right-shift debug flag for the LPAI backend.
 *
 * @param[in] customConfig The handle to set the configuration on.
 *
 * @param[in] enableCuRShift Non-zero to enable compute-unit right-shift, zero to disable it.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_setEnableCuRShift(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, uint8_t enableCuRShift);

/**
 * @brief Get the backend-global compute-unit right-shift debug flag from the LPAI backend
 *        custom config.
 *
 * @param[in] customConfig The handle to get the configuration from.
 *
 * @param[out] enableCuRShift Receives the compute-unit right-shift flag. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if enableCuRShift is NULL or if
 *         setEnableCuRShift() has not been called on this handle.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_getEnableCuRShift(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, uint8_t* enableCuRShift);

/**
 * @brief Set the backend-global framer optimization debug flag for the LPAI backend.
 *
 * @param[in] customConfig The handle to set the configuration on.
 *
 * @param[in] enableFramerOpt Non-zero to enable framer optimization, zero to disable it.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_setEnableFramerOpt(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, uint8_t enableFramerOpt);

/**
 * @brief Get the backend-global framer optimization debug flag from the LPAI backend custom
 *        config.
 *
 * @param[in] customConfig The handle to get the configuration from.
 *
 * @param[out] enableFramerOpt Receives the framer optimization flag. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if enableFramerOpt is NULL or if
 *         setEnableFramerOpt() has not been called on this handle.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_getEnableFramerOpt(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, uint8_t* enableFramerOpt);

/**
 * @brief Set the backend-global element broadcast layout search debug flag for the LPAI
 *        backend.
 *
 * @param[in] customConfig The handle to set the configuration on.
 *
 * @param[in] searchElemBcastLayouts Non-zero to search element broadcast layouts, zero to
 *                                   disable it.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_setSearchElemBcastLayouts(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, uint8_t searchElemBcastLayouts);

/**
 * @brief Get the backend-global element broadcast layout search debug flag from the LPAI
 *        backend custom config.
 *
 * @param[in] customConfig The handle to get the configuration from.
 *
 * @param[out] searchElemBcastLayouts Receives the element broadcast layout search flag.
 *                                    Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_COMMON_ERROR_INVALID_HANDLE if customConfig is not valid.
 *         QAIRT_COMMON_ERROR_INVALID_ARGUMENT if searchElemBcastLayouts is NULL or if
 *         setSearchElemBcastLayouts() has not been called on this handle.
 *
 * @note Use corresponding API through QairtLpaiBackend_CustomConfigV1_t.
 */
Qairt_Status_t QairtLpaiBackend_CustomConfig_getSearchElemBcastLayouts(
    QairtLpaiBackend_CustomConfigHandle_t customConfig, uint8_t* searchElemBcastLayouts);

//=============================================================================
// AlignmentReq Functions
//=============================================================================

/**
 * @brief Free a buffer alignment requirement handle.
 *
 * @param[in] alignReq The handle to free.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if alignReq is NULL or not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_AlignmentReqV1_t.
 */
Qairt_Status_t QairtLpaiBackend_AlignmentReq_free(QairtLpaiBackend_AlignmentReqHandle_t alignReq);

/**
 * @brief Get the required start-address alignment in bytes.
 *
 * @param[in]  alignReq           A handle returned by
 *                                QairtLpaiBackend_Properties_getAlignmentReq().
 * @param[out] startAddrAlignment Receives the start-address alignment in bytes.
 *                                Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if alignReq is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if startAddrAlignment is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_AlignmentReqV1_t.
 */
Qairt_Status_t QairtLpaiBackend_AlignmentReq_getStartAddrAlignment(
    QairtLpaiBackend_AlignmentReqHandle_t alignReq, uint32_t* startAddrAlignment);

/**
 * @brief Get the required buffer size alignment in bytes.
 *
 * @param[in]  alignReq       A handle returned by
 *                            QairtLpaiBackend_Properties_getAlignmentReq().
 * @param[out] sizeAlignment  Receives the size alignment in bytes.
 *                            Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if alignReq is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if sizeAlignment is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_AlignmentReqV1_t.
 */
Qairt_Status_t QairtLpaiBackend_AlignmentReq_getSizeAlignment(
    QairtLpaiBackend_AlignmentReqHandle_t alignReq, uint32_t* sizeAlignment);

//=============================================================================
// eNPUCoreInfo Functions
//=============================================================================

/**
 * @brief Free an eNPU core info handle.
 *
 * @param[in] coreInfo The handle to free.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if coreInfo is NULL or not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_eNPUCoreInfoV1_t.
 */
Qairt_Status_t QairtLpaiBackend_eNPUCoreInfo_free(QairtLpaiBackend_eNPUCoreInfoHandle_t coreInfo);

/**
 * @brief Get the zero-based index of an eNPU core.
 *
 * @param[in]  coreInfo   A handle returned by
 *                        QairtLpaiBackend_Properties_getEnpuCoreInfo().
 * @param[out] coreIndex  Receives the zero-based core index. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if coreInfo is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if coreIndex is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_eNPUCoreInfoV1_t.
 */
Qairt_Status_t QairtLpaiBackend_eNPUCoreInfo_getCoreIndex(
    QairtLpaiBackend_eNPUCoreInfoHandle_t coreInfo, uint32_t* coreIndex);

/**
 * @brief Get the MAC count of an eNPU core.
 *
 * @param[in]  coreInfo  A handle returned by
 *                       QairtLpaiBackend_Properties_getEnpuCoreInfo().
 * @param[out] macCnt    Receives the number of MACs for this core.
 *                       Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if coreInfo is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if macCnt is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_eNPUCoreInfoV1_t.
 */
Qairt_Status_t QairtLpaiBackend_eNPUCoreInfo_getMacCnt(
    QairtLpaiBackend_eNPUCoreInfoHandle_t coreInfo, uint32_t* macCnt);

/**
 * @brief Get the hardware version of an eNPU core.
 *
 * @param[in]  coreInfo  A handle returned by
 *                       QairtLpaiBackend_Properties_getEnpuCoreInfo().
 * @param[out] version   Receives the hardware version of this core.
 *                       Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if coreInfo is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if version is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_eNPUCoreInfoV1_t.
 */
Qairt_Status_t QairtLpaiBackend_eNPUCoreInfo_getVersion(
    QairtLpaiBackend_eNPUCoreInfoHandle_t coreInfo, uint32_t* version);

//=============================================================================
// eNPUCoreUtilData Functions
//=============================================================================

/**
 * @brief Free an eNPU core utilization data handle.
 *
 * @param[in] coreUtilData The handle to free.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if coreUtilData is NULL or not valid.
 *
 * @note Use corresponding API through QairtLpaiBackend_eNPUCoreUtilDataV1_t.
 */
Qairt_Status_t QairtLpaiBackend_eNPUCoreUtilData_free(
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t coreUtilData);

/**
 * @brief Get the sample timestamp of an eNPU core utilization snapshot.
 *
 * @param[in]  coreUtilData A handle returned by
 *                          QairtLpaiBackend_Properties_getEnpuCoreUtilData().
 * @param[out] sampleTs     Receives the absolute time of the sample, in microseconds since
 *                          system reset. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if coreUtilData is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if sampleTs is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_eNPUCoreUtilDataV1_t.
 */
Qairt_Status_t QairtLpaiBackend_eNPUCoreUtilData_getSampleTs(
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t coreUtilData, uint64_t* sampleTs);

/**
 * @brief Get the accumulated eNPU core cycles of a utilization snapshot.
 *
 * @param[in]  coreUtilData A handle returned by
 *                          QairtLpaiBackend_Properties_getEnpuCoreUtilData().
 * @param[out] cycles       Receives the total eNPU core cycles spent on ML work for this
 *                          core since measurement was enabled. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if coreUtilData is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if cycles is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_eNPUCoreUtilDataV1_t.
 */
Qairt_Status_t QairtLpaiBackend_eNPUCoreUtilData_getCycles(
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t coreUtilData, uint64_t* cycles);

/**
 * @brief Get the eNPU core clock frequency of a utilization snapshot.
 *
 * @param[in]  coreUtilData A handle returned by
 *                          QairtLpaiBackend_Properties_getEnpuCoreUtilData().
 * @param[out] clockKhz     Receives the current eNPU core clock frequency, in kHz.
 *                          Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if coreUtilData is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if clockKhz is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_eNPUCoreUtilDataV1_t.
 */
Qairt_Status_t QairtLpaiBackend_eNPUCoreUtilData_getClockKhz(
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t coreUtilData, uint32_t* clockKhz);

/**
 * @brief Get the voted DDR bandwidth of a utilization snapshot.
 *
 * @param[in]  coreUtilData A handle returned by
 *                          QairtLpaiBackend_Properties_getEnpuCoreUtilData().
 * @param[out] ddrBwKbps    Receives the total DDR bandwidth currently voted by all clients,
 *                          in kbps. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if coreUtilData is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if ddrBwKbps is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_eNPUCoreUtilDataV1_t.
 */
Qairt_Status_t QairtLpaiBackend_eNPUCoreUtilData_getDdrBwKbps(
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t coreUtilData, uint32_t* ddrBwKbps);

//=============================================================================
// Backend Property Getters
//=============================================================================

/**
 * @brief Get buffer alignment requirements from the LPAI backend.
 *
 * @param[in]  backendHandle  A handle to a created backend.
 * @param[out] alignReq          Receives a newly allocated alignment requirement
 *                               handle. The caller owns this handle and must free
 *                               it via QairtLpaiBackend_AlignmentReq_free().
 *                               Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if backendHandle is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if alignReq is NULL.
 *         QAIRT_COMMON_ERROR_MEM_ALLOC if allocation fails.
 *
 * @note Use corresponding API through QairtLpaiBackend_PropertiesV1_t.
 */
Qairt_Status_t QairtLpaiBackend_Properties_getAlignmentReq(
    QairtBackend_Handle_t backendHandle, QairtLpaiBackend_AlignmentReqHandle_t* alignReq);

/**
 * @brief Query whether the compiled binary must be kept persistent in memory.
 *
 *        When non-zero, the binary buffer passed to context-from-binary APIs
 *        must remain valid for the lifetime of the context.
 *
 * @param[in]  backendHandle           A handle to a created backend.
 * @param[out] requirePersistentBinary Receives 1 if persistence is required,
 *                                     0 otherwise. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if backendHandle is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if requirePersistentBinary is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_PropertiesV1_t.
 */
Qairt_Status_t QairtLpaiBackend_Properties_getRequirePersistentBinary(
    QairtBackend_Handle_t backendHandle, uint8_t* requirePersistentBinary);

/**
 * @brief Get the number of eNPU cores available on this backend.
 *
 * @param[in]  backendHandle  A handle to a created backend.
 * @param[out] coreNum           Receives the number of available eNPU cores.
 *                               Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if backendHandle is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if coreNum is NULL.
 *
 * @note Use corresponding API through QairtLpaiBackend_PropertiesV1_t.
 */
Qairt_Status_t QairtLpaiBackend_Properties_getEnpuCoreNum(QairtBackend_Handle_t backendHandle,
                                                          uint32_t* coreNum);

/**
 * @brief Get the current clock frequency of a specific eNPU core.
 *
 * @param[in]  backendHandle  A handle to a created backend.
 * @param[in]  coreIndex      Zero-based index of the core to query. An out-of-range
 *                            index is rejected by the platform.
 * @param[out] clkFreqHz      Receives the current clock frequency of the
 *                            specified core in Hz. Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if backendHandle is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if clkFreqHz is NULL.
 *         QAIRT_COMMON_ERROR_NOT_SUPPORTED if coreIndex is out of range.
 *
 * @note Use corresponding API through QairtLpaiBackend_PropertiesV1_t.
 */
Qairt_Status_t QairtLpaiBackend_Properties_getEnpuClkFreq(
    QairtBackend_Handle_t backendHandle, uint32_t coreIndex, uint32_t* clkFreqHz);

/**
 * @brief Get detailed info for a specific eNPU core.
 *
 * @param[in]  backendHandle  A handle to a created backend.
 * @param[in]  coreIndex      Zero-based index of the core to query. An out-of-range
 *                            index is rejected by the platform.
 * @param[out] coreInfo          Receives a newly allocated core info handle.
 *                               The caller owns this handle and must free it via
 *                               QairtLpaiBackend_eNPUCoreInfo_free().
 *                               Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if backendHandle is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if coreInfo is NULL.
 *         QAIRT_COMMON_ERROR_NOT_SUPPORTED if coreIndex is out of range.
 *         QAIRT_COMMON_ERROR_MEM_ALLOC if allocation fails.
 *
 * @note Use corresponding API through QairtLpaiBackend_PropertiesV1_t.
 */
Qairt_Status_t QairtLpaiBackend_Properties_getEnpuCoreInfo(
    QairtBackend_Handle_t backendHandle,
    uint32_t coreIndex,
    QairtLpaiBackend_eNPUCoreInfoHandle_t* coreInfo);

/**
 * @brief Enable or disable eNPU core utilization measurement (DEBUG).
 *
 *        Enabling should be done immediately after creating the backend handle, before any
 *        context or graph is created. Once enabled, per-core snapshots can be read with
 *        QairtLpaiBackend_Properties_getEnpuCoreUtilData(). This is a DEBUG feature for
 *        performance analysis only and must not be used in production code paths.
 *
 * @param[in] backendHandle A handle to a created backend.
 * @param[in] enable        Non-zero to enable measurement, zero to disable it.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if backendHandle is not valid.
 *         QAIRT_COMMON_ERROR_NOT_SUPPORTED if the feature is not supported by the platform.
 *
 * @note The call is silently ignored (still returns QAIRT_SUCCESS) if the feature is already
 *       in the requested state. Use corresponding API through QairtLpaiBackend_PropertiesV1_t.
 */
Qairt_Status_t QairtLpaiBackend_Properties_setEnpuCoreUtilEnabled(
    QairtBackend_Handle_t backendHandle, uint8_t enable);

/**
 * @brief Read an eNPU core utilization snapshot for a specific core.
 *
 *        Measurement must have been enabled first via
 *        QairtLpaiBackend_Properties_setEnpuCoreUtilEnabled(). This is a DEBUG feature for
 *        performance analysis only and must not be used in production code paths.
 *
 * @param[in]  backendHandle A handle to a created backend.
 * @param[in]  coreIndex     Zero-based index of the core to query. Must be less than the
 *                           value returned by QairtLpaiBackend_Properties_getEnpuCoreNum().
 * @param[out] coreUtilData  Receives a newly allocated core utilization data handle. The
 *                           caller owns this handle and must free it via
 *                           QairtLpaiBackend_eNPUCoreUtilData_free(). Must not be NULL.
 *
 * @return QAIRT_SUCCESS on success.
 *         QAIRT_BACKEND_ERROR_INVALID_HANDLE if backendHandle is not valid.
 *         QAIRT_BACKEND_ERROR_INVALID_ARGUMENT if coreUtilData is NULL.
 *         QAIRT_COMMON_ERROR_NOT_SUPPORTED if measurement is not enabled or the feature is
 *         not supported by the platform.
 *         QAIRT_COMMON_ERROR_MEM_ALLOC if allocation fails.
 *
 * @note Use corresponding API through QairtLpaiBackend_PropertiesV1_t.
 */
Qairt_Status_t QairtLpaiBackend_Properties_getEnpuCoreUtilData(
    QairtBackend_Handle_t backendHandle,
    uint32_t coreIndex,
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t* coreUtilData);

//=============================================================================
// Function Pointer Typedefs — HW Info
//=============================================================================

typedef Qairt_Status_t (*QairtLpaiBackend_HwInfo_CreateFn_t)(QairtLpaiBackend_HwInfoHandle_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_HwInfo_FreeFn_t)(QairtLpaiBackend_HwInfoHandle_t);

/* Target */
typedef Qairt_Status_t (*QairtLpaiBackend_HwInfo_SetTargetFn_t)(QairtLpaiBackend_HwInfoHandle_t,
                                                                QairtLpaiBackend_Target_t);
typedef Qairt_Status_t (*QairtLpaiBackend_HwInfo_GetTargetFn_t)(QairtLpaiBackend_HwInfoHandle_t,
                                                                QairtLpaiBackend_Target_t*);

/* HW Version */
typedef Qairt_Status_t (*QairtLpaiBackend_HwInfo_SetHwVersionFn_t)(QairtLpaiBackend_HwInfoHandle_t,
                                                                   QairtLpaiBackend_HwVersion_t);
typedef Qairt_Status_t (*QairtLpaiBackend_HwInfo_GetHwVersionFn_t)(QairtLpaiBackend_HwInfoHandle_t,
                                                                   QairtLpaiBackend_HwVersion_t*);

//=============================================================================
// Function Pointer Typedefs — RPC Polling Config
//=============================================================================

typedef Qairt_Status_t (*QairtLpaiBackend_RpcPollingConfig_CreateFn_t)(
    QairtLpaiBackend_RpcPollingConfigHandle_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_RpcPollingConfig_FreeFn_t)(
    QairtLpaiBackend_RpcPollingConfigHandle_t);

/* Enable */
typedef Qairt_Status_t (*QairtLpaiBackend_RpcPollingConfig_SetEnableFn_t)(
    QairtLpaiBackend_RpcPollingConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtLpaiBackend_RpcPollingConfig_GetEnableFn_t)(
    QairtLpaiBackend_RpcPollingConfigHandle_t, uint32_t*);

/* DSP Polling Timeout */
typedef Qairt_Status_t (*QairtLpaiBackend_RpcPollingConfig_SetDSPPollingTimeoutUsFn_t)(
    QairtLpaiBackend_RpcPollingConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtLpaiBackend_RpcPollingConfig_GetDSPPollingTimeoutUsFn_t)(
    QairtLpaiBackend_RpcPollingConfigHandle_t, uint32_t*);

/* HLOS Wakeup Lead Time */
typedef Qairt_Status_t (*QairtLpaiBackend_RpcPollingConfig_SetHLOSWakeupLeadTimeUsFn_t)(
    QairtLpaiBackend_RpcPollingConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtLpaiBackend_RpcPollingConfig_GetHLOSWakeupLeadTimeUsFn_t)(
    QairtLpaiBackend_RpcPollingConfigHandle_t, uint32_t*);

/* HLOS Polling Timeout */
typedef Qairt_Status_t (*QairtLpaiBackend_RpcPollingConfig_SetHLOSPollingTimeoutUsFn_t)(
    QairtLpaiBackend_RpcPollingConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtLpaiBackend_RpcPollingConfig_GetHLOSPollingTimeoutUsFn_t)(
    QairtLpaiBackend_RpcPollingConfigHandle_t, uint32_t*);

//=============================================================================
// Function Pointer Typedefs — Backend Custom Config
//=============================================================================

typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_CreateFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_FreeFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t);

/* HW Info */
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_SetHwInfoFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, QairtLpaiBackend_HwInfoHandle_t);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_GetHwInfoFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, QairtLpaiBackend_HwInfoHandle_t*);

/* RPC Polling Config */
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_SetRpcPollingFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, QairtLpaiBackend_RpcPollingConfigHandle_t);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_GetRpcPollingFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, QairtLpaiBackend_RpcPollingConfigHandle_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_SetAixDumpFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, uint8_t);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_GetAixDumpFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, uint8_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_SetForceNhwcFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, uint8_t);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_GetForceNhwcFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, uint8_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_SetEnableCuRShiftFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, uint8_t);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_GetEnableCuRShiftFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, uint8_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_SetEnableFramerOptFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, uint8_t);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_GetEnableFramerOptFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, uint8_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_SetSearchElemBcastLayoutsFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, uint8_t);
typedef Qairt_Status_t (*QairtLpaiBackend_CustomConfig_GetSearchElemBcastLayoutsFn_t)(
    QairtLpaiBackend_CustomConfigHandle_t, uint8_t*);

//=============================================================================
// Function Pointer Typedefs — Backend Properties
//=============================================================================

typedef Qairt_Status_t (*QairtLpaiBackend_AlignmentReq_FreeFn_t)(
    QairtLpaiBackend_AlignmentReqHandle_t);
typedef Qairt_Status_t (*QairtLpaiBackend_AlignmentReq_GetStartAddrAlignmentFn_t)(
    QairtLpaiBackend_AlignmentReqHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_AlignmentReq_GetSizeAlignmentFn_t)(
    QairtLpaiBackend_AlignmentReqHandle_t, uint32_t*);

typedef Qairt_Status_t (*QairtLpaiBackend_eNPUCoreInfo_FreeFn_t)(
    QairtLpaiBackend_eNPUCoreInfoHandle_t);
typedef Qairt_Status_t (*QairtLpaiBackend_eNPUCoreInfo_GetCoreIndexFn_t)(
    QairtLpaiBackend_eNPUCoreInfoHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_eNPUCoreInfo_GetMacCntFn_t)(
    QairtLpaiBackend_eNPUCoreInfoHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_eNPUCoreInfo_GetVersionFn_t)(
    QairtLpaiBackend_eNPUCoreInfoHandle_t, uint32_t*);

typedef Qairt_Status_t (*QairtLpaiBackend_eNPUCoreUtilData_FreeFn_t)(
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t);
typedef Qairt_Status_t (*QairtLpaiBackend_eNPUCoreUtilData_GetSampleTsFn_t)(
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_eNPUCoreUtilData_GetCyclesFn_t)(
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_eNPUCoreUtilData_GetClockKhzFn_t)(
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_eNPUCoreUtilData_GetDdrBwKbpsFn_t)(
    QairtLpaiBackend_eNPUCoreUtilDataHandle_t, uint32_t*);

typedef Qairt_Status_t (*QairtLpaiBackend_Properties_GetAlignmentReqFn_t)(
    QairtBackend_Handle_t, QairtLpaiBackend_AlignmentReqHandle_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_Properties_GetRequirePersistentBinaryFn_t)(
    QairtBackend_Handle_t, uint8_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_Properties_GetEnpuCoreNumFn_t)(QairtBackend_Handle_t,
                                                                         uint32_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_Properties_GetEnpuCoreInfoFn_t)(
    QairtBackend_Handle_t, uint32_t, QairtLpaiBackend_eNPUCoreInfoHandle_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_Properties_SetEnpuCoreUtilEnabledFn_t)(
    QairtBackend_Handle_t, uint8_t);
typedef Qairt_Status_t (*QairtLpaiBackend_Properties_GetEnpuCoreUtilDataFn_t)(
    QairtBackend_Handle_t, uint32_t, QairtLpaiBackend_eNPUCoreUtilDataHandle_t*);
typedef Qairt_Status_t (*QairtLpaiBackend_Properties_GetEnpuClkFreqFn_t)(
    QairtBackend_Handle_t, uint32_t, uint32_t*);

//=============================================================================
// Public Interfaces
//=============================================================================

/**
 * @struct QairtLpaiBackend_HwInfoV1_t
 * @brief  Interface struct for LPAI backend HW info (V1).
 *         Retrieved via interface ID QAIRT_LPAI_BACKEND_HW_INFO_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtLpaiBackend_HwInfo_CreateFn_t create;
  QairtLpaiBackend_HwInfo_FreeFn_t free;
  QairtLpaiBackend_HwInfo_SetTargetFn_t setTarget;
  QairtLpaiBackend_HwInfo_GetTargetFn_t getTarget;
  QairtLpaiBackend_HwInfo_SetHwVersionFn_t setHwVersion;
  QairtLpaiBackend_HwInfo_GetHwVersionFn_t getHwVersion;
} QairtLpaiBackend_HwInfoV1_t;

/**
 * @struct QairtLpaiBackend_CustomConfigV1_t
 * @brief  Interface struct for LPAI backend custom configuration (V1).
 *         Retrieved via interface ID QAIRT_LPAI_BACKEND_CUSTOM_CONFIG_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtLpaiBackend_CustomConfig_CreateFn_t create;
  QairtLpaiBackend_CustomConfig_FreeFn_t free;
  QairtLpaiBackend_CustomConfig_SetHwInfoFn_t setHwInfo;
  QairtLpaiBackend_CustomConfig_GetHwInfoFn_t getHwInfo;
  QairtLpaiBackend_CustomConfig_SetRpcPollingFn_t setRpcPolling;
  QairtLpaiBackend_CustomConfig_GetRpcPollingFn_t getRpcPolling;
  QairtLpaiBackend_CustomConfig_SetAixDumpFn_t setAixDump;
  QairtLpaiBackend_CustomConfig_GetAixDumpFn_t getAixDump;
  QairtLpaiBackend_CustomConfig_SetForceNhwcFn_t setForceNhwc;
  QairtLpaiBackend_CustomConfig_GetForceNhwcFn_t getForceNhwc;
  QairtLpaiBackend_CustomConfig_SetEnableCuRShiftFn_t setEnableCuRShift;
  QairtLpaiBackend_CustomConfig_GetEnableCuRShiftFn_t getEnableCuRShift;
  QairtLpaiBackend_CustomConfig_SetEnableFramerOptFn_t setEnableFramerOpt;
  QairtLpaiBackend_CustomConfig_GetEnableFramerOptFn_t getEnableFramerOpt;
  QairtLpaiBackend_CustomConfig_SetSearchElemBcastLayoutsFn_t setSearchElemBcastLayouts;
  QairtLpaiBackend_CustomConfig_GetSearchElemBcastLayoutsFn_t getSearchElemBcastLayouts;
} QairtLpaiBackend_CustomConfigV1_t;

/**
 * @struct QairtLpaiBackend_RpcPollingConfigV1_t
 * @brief  Interface struct for LPAI backend RPC polling configuration (V1).
 *         Retrieved via interface ID QAIRT_LPAI_BACKEND_RPC_POLLING_CONFIG_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtLpaiBackend_RpcPollingConfig_CreateFn_t create;
  QairtLpaiBackend_RpcPollingConfig_FreeFn_t free;
  QairtLpaiBackend_RpcPollingConfig_SetEnableFn_t setEnable;
  QairtLpaiBackend_RpcPollingConfig_GetEnableFn_t getEnable;
  QairtLpaiBackend_RpcPollingConfig_SetDSPPollingTimeoutUsFn_t setDSPPollingTimeoutUs;
  QairtLpaiBackend_RpcPollingConfig_GetDSPPollingTimeoutUsFn_t getDSPPollingTimeoutUs;
  QairtLpaiBackend_RpcPollingConfig_SetHLOSWakeupLeadTimeUsFn_t setHLOSWakeupLeadTimeUs;
  QairtLpaiBackend_RpcPollingConfig_GetHLOSWakeupLeadTimeUsFn_t getHLOSWakeupLeadTimeUs;
  QairtLpaiBackend_RpcPollingConfig_SetHLOSPollingTimeoutUsFn_t setHLOSPollingTimeoutUs;
  QairtLpaiBackend_RpcPollingConfig_GetHLOSPollingTimeoutUsFn_t getHLOSPollingTimeoutUs;
} QairtLpaiBackend_RpcPollingConfigV1_t;

/**
 * @struct QairtLpaiBackend_AlignmentReqV1_t
 * @brief  Interface struct for LPAI backend buffer alignment requirements (V1).
 *         Retrieved via interface ID QAIRT_LPAI_BACKEND_ALIGNMENT_REQ_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtLpaiBackend_AlignmentReq_FreeFn_t free;
  QairtLpaiBackend_AlignmentReq_GetStartAddrAlignmentFn_t getStartAddrAlignment;
  QairtLpaiBackend_AlignmentReq_GetSizeAlignmentFn_t getSizeAlignment;
} QairtLpaiBackend_AlignmentReqV1_t;

/**
 * @struct QairtLpaiBackend_eNPUCoreInfoV1_t
 * @brief  Interface struct for LPAI eNPU core information (V1).
 *         Retrieved via interface ID QAIRT_LPAI_BACKEND_ENPU_CORE_INFO_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtLpaiBackend_eNPUCoreInfo_FreeFn_t free;
  QairtLpaiBackend_eNPUCoreInfo_GetCoreIndexFn_t getCoreIndex;
  QairtLpaiBackend_eNPUCoreInfo_GetMacCntFn_t getMacCnt;
  QairtLpaiBackend_eNPUCoreInfo_GetVersionFn_t getVersion;
} QairtLpaiBackend_eNPUCoreInfoV1_t;

/**
 * @struct QairtLpaiBackend_eNPUCoreUtilDataV1_t
 * @brief  Interface struct for LPAI eNPU core utilization data (V1).
 *         Retrieved via interface ID QAIRT_LPAI_BACKEND_ENPU_CORE_UTIL_DATA_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtLpaiBackend_eNPUCoreUtilData_FreeFn_t free;
  QairtLpaiBackend_eNPUCoreUtilData_GetSampleTsFn_t getSampleTs;
  QairtLpaiBackend_eNPUCoreUtilData_GetCyclesFn_t getCycles;
  QairtLpaiBackend_eNPUCoreUtilData_GetClockKhzFn_t getClockKhz;
  QairtLpaiBackend_eNPUCoreUtilData_GetDdrBwKbpsFn_t getDdrBwKbps;
} QairtLpaiBackend_eNPUCoreUtilDataV1_t;

/**
 * @struct QairtLpaiBackend_PropertiesV1_t
 * @brief  Interface struct for LPAI-specific backend properties (V1).
 *         Retrieved via interface ID QAIRT_LPAI_BACKEND_PROPERTIES_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtLpaiBackend_Properties_GetAlignmentReqFn_t getAlignmentReq;
  QairtLpaiBackend_Properties_GetRequirePersistentBinaryFn_t getRequirePersistentBinary;
  QairtLpaiBackend_Properties_GetEnpuCoreNumFn_t getEnpuCoreNum;
  QairtLpaiBackend_Properties_GetEnpuCoreInfoFn_t getEnpuCoreInfo;
  QairtLpaiBackend_Properties_SetEnpuCoreUtilEnabledFn_t setEnpuCoreUtilEnabled;
  QairtLpaiBackend_Properties_GetEnpuCoreUtilDataFn_t getEnpuCoreUtilData;
  QairtLpaiBackend_Properties_GetEnpuClkFreqFn_t getEnpuClkFreq;
} QairtLpaiBackend_PropertiesV1_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAIRT_LPAI_BACKEND_H
