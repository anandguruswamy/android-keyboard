//==============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

/**
 * @file
 * @brief   LPAI Performance Infrastructure component API
 *
 *          Provides an interface to control runtime eNPU power
 *          level of the LPAI accelerator.
 *
 *          The perf-infrastructure handle is obtained from the opaque device
 *          infrastructure handle returned by
 *          QairtDevice_InfrastructureProvider_getInfrastructure, via
 *          QairtLpaiDevice_getPerfInfrastructure. Support is optional and is
 *          advertised through QAIRT_CAPABILITY_DEVICE_SUPPORT_INFRASTRUCTURE.
 */

#ifndef QAIRT_LPAI_PERF_INFRASTRUCTURE_H
#define QAIRT_LPAI_PERF_INFRASTRUCTURE_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#include "QairtCommon/QairtCommon.h"
#include "QairtDevice/QairtDeviceInfrastructure.h"

//=============================================================================
// Macros
//=============================================================================

/// Interface IDs for the LPAI perf-infrastructure sub-objects and the
/// top-level perf-infrastructure interface.
#define QAIRT_MIN_ID_LPAI_PERF (QAIRT_MIN_ID_LPAI + 3000)

#define QAIRT_LPAI_PERF_INFRASTRUCTURE_ID (QAIRT_MIN_ID_LPAI_PERF + 0)
#define QAIRT_LPAI_PERF_CONFIG_ID         (QAIRT_MIN_ID_LPAI_PERF + 1)
#define QAIRT_LPAI_PERF_CONFIG_LIST_ID    (QAIRT_MIN_ID_LPAI_PERF + 2)

// Define ranges for LPAI perf-infrastructure specific error codes
#define QAIRT_MIN_ERROR_LPAI_PERF (QAIRT_MIN_ERROR_LPAI + 3000)
#define QAIRT_MAX_ERROR_LPAI_PERF (QAIRT_MIN_ERROR_LPAI + 3999)

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Data Types
//=============================================================================

/**
 * @brief Opaque handle to an LPAI perf-infrastructure object.
 *
 *        Obtained via QairtLpaiDevice_getPerfInfrastructure and released with
 *        QairtLpaiPerfInfrastructure_free. The underlying backend
 *        infrastructure it references is backend-owned and is not freed by
 *        QairtLpaiPerfInfrastructure_free.
 */
typedef struct _QairtLpaiPerfInfrastructure_Handle_t* QairtLpaiPerfInfrastructure_Handle_t;

/// Opaque handle to a single perf-configuration entry (one option at a time).
typedef struct _QairtLpaiPerfConfigHandle_t* QairtLpaiPerfConfigHandle_t;
/// Opaque handle to an ordered collection of perf-configuration entries.
typedef struct _QairtLpaiPerfConfigListHandle_t* QairtLpaiPerfConfigListHandle_t;

/**
 * @brief QAIRT LPAI PerfInfrastructure API result / error codes.
 */
typedef enum {
  QAIRT_LPAI_PERF_INFRASTRUCTURE_MIN_ERROR = QAIRT_MIN_ERROR_LPAI_PERF,
  ////////////////////////////////////////

  /// LPAI perf-infrastructure operation succeeded.
  QAIRT_LPAI_PERF_INFRASTRUCTURE_NO_ERROR = QAIRT_SUCCESS,
  /// A handle argument is not valid.
  QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_INVALID_HANDLE = QAIRT_MIN_ERROR_LPAI_PERF + 0,
  /// An argument to the API is invalid.
  QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_INVALID_INPUT = QAIRT_MIN_ERROR_LPAI_PERF + 1,
  /// A configuration option is not supported.
  QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_UNSUPPORTED_CONFIG = QAIRT_MIN_ERROR_LPAI_PERF + 2,
  /// Transport error communicating with the accelerator.
  QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_TRANSPORT = QAIRT_MIN_ERROR_LPAI_PERF + 3,
  /// A get call was made for an option that is not the one currently set.
  QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_OPTION_MISMATCH = QAIRT_MIN_ERROR_LPAI_PERF + 4,
  /// Feature or API is not supported on this target.
  QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_UNSUPPORTED = QAIRT_COMMON_ERROR_NOT_SUPPORTED,
  /// General memory allocation failure.
  QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_MEM_ALLOC = QAIRT_COMMON_ERROR_MEM_ALLOC,
  /// General error not covered by a more specific code.
  QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_FAILED = QAIRT_COMMON_ERROR_GENERAL,

  ////////////////////////////////////////
  QAIRT_LPAI_PERF_INFRASTRUCTURE_MAX_ERROR = QAIRT_MAX_ERROR_LPAI_PERF,
  // Unused, present to ensure 32 bits.
  QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_UNDEFINED = 0x7FFFFFFF
} QairtLpaiPerfInfrastructure_Error_t;

/**
 * @brief Performance modes requestable by the client to influence eNPU clock
 *        level.
 */
typedef enum {
  QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_DEFAULT = 0,
  QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_BURST   = 1,
  QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_TURBO   = 2,
  QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_NOMINAL = 3,
  QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_SVS_L1  = 4,
  QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_SVS     = 5,
  QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_LOWSVS  = 6,
  QAIRT_LPAI_PERF_INFRASTRUCTURE_PERFMODE_UNKNOWN = 0x7FFFFFFF
} QairtLpaiPerfInfrastructure_PerformanceMode_t;

/**
 * @brief Perf configuration option selecting which value is active on a
 *        QairtLpaiPerfConfigHandle_t.
 */
typedef enum {
  /// config enum implies the usage of the eNPU power-level fields, requesting
  /// the default (nominal) DRAM bandwidth.
  QAIRT_LPAI_PERF_CONFIGOPTION_ENPU_POWER_LEVEL = 1,
  /// config enum implies the usage of the eNPU power-level fields, requesting
  /// maximum DRAM bandwidth alongside the eNPU clock level.
  QAIRT_LPAI_PERF_CONFIGOPTION_ENPU_WITH_DRAM_POWER_LEVEL = 2,
  /// UNKNOWN config option which must not be used.
  QAIRT_LPAI_PERF_CONFIGOPTION_UNKNOWN = 0x7FFFFFFF
} QairtLpaiPerfInfrastructure_ConfigOption_t;

//=============================================================================
// Perf Configuration (one option active at a time)
//=============================================================================

/**
 * @brief Create a perf-configuration handle. No option is active until one
 *        of the setters below is called.
 */
Qairt_Status_t QairtLpaiPerfConfig_create(QairtLpaiPerfConfigHandle_t* config);
Qairt_Status_t QairtLpaiPerfConfig_free(QairtLpaiPerfConfigHandle_t config);

/// Get which option is currently active on this handle.
Qairt_Status_t QairtLpaiPerfConfig_getOption(
    QairtLpaiPerfConfigHandle_t config, QairtLpaiPerfInfrastructure_ConfigOption_t* option);

/**
 * @brief Set the eNPU performance mode.
 *
 * Sets option QAIRT_LPAI_PERF_CONFIGOPTION_ENPU_POWER_LEVEL, or
 * QAIRT_LPAI_PERF_CONFIGOPTION_ENPU_WITH_DRAM_POWER_LEVEL if maxDdrBandwidth
 * is non-zero.
 *
 * @param[in] config           Handle to the perf-configuration object.
 * @param[in] performanceMode  Requested eNPU performance mode.
 * @param[in] maxDdrBandwidth  Non-zero to additionally request maximum DRAM
 *                             bandwidth alongside the eNPU clock level.
 */
Qairt_Status_t QairtLpaiPerfConfig_setEnpuPowerLevel(
    QairtLpaiPerfConfigHandle_t config,
    QairtLpaiPerfInfrastructure_PerformanceMode_t performanceMode,
    uint8_t maxDdrBandwidth);

/// Valid only if getOption reports ENPU_POWER_LEVEL or
/// ENPU_WITH_DRAM_POWER_LEVEL.
Qairt_Status_t QairtLpaiPerfConfig_getEnpuPowerLevel(
    QairtLpaiPerfConfigHandle_t config,
    QairtLpaiPerfInfrastructure_PerformanceMode_t* performanceMode,
    uint8_t* maxDdrBandwidth);

//=============================================================================
// Perf Configuration List
//=============================================================================

/// Create an empty, ordered collection of perf-configuration entries.
Qairt_Status_t QairtLpaiPerfConfigList_create(QairtLpaiPerfConfigListHandle_t* list);
Qairt_Status_t QairtLpaiPerfConfigList_free(QairtLpaiPerfConfigListHandle_t list);

/// Append a copy of config's current value to the list. The caller retains
/// ownership of config and may free or reuse it afterward.
Qairt_Status_t QairtLpaiPerfConfigList_addConfig(QairtLpaiPerfConfigListHandle_t list,
                                                 QairtLpaiPerfConfigHandle_t config);

Qairt_Status_t QairtLpaiPerfConfigList_getNumConfigs(QairtLpaiPerfConfigListHandle_t list,
                                                     uint32_t* numConfigs);

/// Get a non-owning handle to the entry at index; do not free it. Valid
/// until list is freed or cleared.
Qairt_Status_t QairtLpaiPerfConfigList_getConfigAt(QairtLpaiPerfConfigListHandle_t list,
                                                   uint32_t index,
                                                   QairtLpaiPerfConfigHandle_t* config);

Qairt_Status_t QairtLpaiPerfConfigList_clear(QairtLpaiPerfConfigListHandle_t list);

//=============================================================================
// Perf Infrastructure
//=============================================================================

/**
 * @brief Resolve an LPAI perf-infrastructure handle from a device
 *        infrastructure handle.
 *
 * @param[in]  deviceInfraHandle Device infrastructure handle obtained from
 *                               QairtDevice_InfrastructureProvider_getInfrastructure.
 * @param[out] perfInfraHandle   Pointer to receive the perf-infrastructure handle.
 *                               Release with QairtLpaiPerfInfrastructure_free.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: handle resolved.
 *         - QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_INVALID_INPUT: perfInfraHandle is NULL.
 *         - QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_INVALID_HANDLE: deviceInfraHandle invalid.
 *         - QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_UNSUPPORTED: backend does not
 *           expose a perf infrastructure.
 *         - QAIRT_LPAI_PERF_INFRASTRUCTURE_ERROR_MEM_ALLOC: allocation failed.
 */
Qairt_Status_t QairtLpaiDevice_getPerfInfrastructure(
    QairtDevice_InfrastructureHandle_t deviceInfraHandle,
    QairtLpaiPerfInfrastructure_Handle_t* perfInfraHandle);

/**
 * @brief Free an LPAI perf-infrastructure handle. Does not free the
 *        backend-owned infrastructure it references.
 */
Qairt_Status_t QairtLpaiPerfInfrastructure_free(
    QairtLpaiPerfInfrastructure_Handle_t perfInfraHandle);

/**
 * @brief Set eNPU power configuration to enable different performance modes.
 *
 * @param[in] perfInfraHandle Perf-infrastructure handle.
 * @param[in] hwVersion       Hardware version the configuration applies to.
 * @param[in] configList      Handle to a list of perf-configuration entries.
 *                            NULL indicates no config options.
 */
Qairt_Status_t QairtLpaiPerfInfrastructure_setPowerConfig(
    QairtLpaiPerfInfrastructure_Handle_t perfInfraHandle,
    uint32_t hwVersion,
    QairtLpaiPerfConfigListHandle_t configList);

//=============================================================================
// Function Pointer Typedefs
//=============================================================================

/* Perf config (tagged slot) */
typedef Qairt_Status_t (*QairtLpaiPerfConfig_CreateFn_t)(QairtLpaiPerfConfigHandle_t*);
typedef Qairt_Status_t (*QairtLpaiPerfConfig_FreeFn_t)(QairtLpaiPerfConfigHandle_t);
typedef Qairt_Status_t (*QairtLpaiPerfConfig_GetOptionFn_t)(
    QairtLpaiPerfConfigHandle_t, QairtLpaiPerfInfrastructure_ConfigOption_t*);
typedef Qairt_Status_t (*QairtLpaiPerfConfig_SetEnpuPowerLevelFn_t)(
    QairtLpaiPerfConfigHandle_t, QairtLpaiPerfInfrastructure_PerformanceMode_t, uint8_t);
typedef Qairt_Status_t (*QairtLpaiPerfConfig_GetEnpuPowerLevelFn_t)(
    QairtLpaiPerfConfigHandle_t, QairtLpaiPerfInfrastructure_PerformanceMode_t*, uint8_t*);

/* Perf config list */
typedef Qairt_Status_t (*QairtLpaiPerfConfigList_CreateFn_t)(QairtLpaiPerfConfigListHandle_t*);
typedef Qairt_Status_t (*QairtLpaiPerfConfigList_FreeFn_t)(QairtLpaiPerfConfigListHandle_t);
typedef Qairt_Status_t (*QairtLpaiPerfConfigList_AddConfigFn_t)(QairtLpaiPerfConfigListHandle_t,
                                                                QairtLpaiPerfConfigHandle_t);
typedef Qairt_Status_t (*QairtLpaiPerfConfigList_GetNumConfigsFn_t)(
    QairtLpaiPerfConfigListHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtLpaiPerfConfigList_GetConfigAtFn_t)(QairtLpaiPerfConfigListHandle_t,
                                                                  uint32_t,
                                                                  QairtLpaiPerfConfigHandle_t*);
typedef Qairt_Status_t (*QairtLpaiPerfConfigList_ClearFn_t)(QairtLpaiPerfConfigListHandle_t);

/* Perf infrastructure */
typedef Qairt_Status_t (*QairtLpaiDevice_GetPerfInfrastructureFn_t)(
    QairtDevice_InfrastructureHandle_t, QairtLpaiPerfInfrastructure_Handle_t*);
typedef Qairt_Status_t (*QairtLpaiPerfInfrastructure_FreeFn_t)(
    QairtLpaiPerfInfrastructure_Handle_t);
typedef Qairt_Status_t (*QairtLpaiPerfInfrastructure_SetPowerConfigFn_t)(
    QairtLpaiPerfInfrastructure_Handle_t, uint32_t, QairtLpaiPerfConfigListHandle_t);

//=============================================================================
// Public Interfaces
//=============================================================================

/// V1 interface for a perf-configuration slot. Retrieved via QAIRT_LPAI_PERF_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtLpaiPerfConfig_CreateFn_t create;
  QairtLpaiPerfConfig_FreeFn_t free;
  QairtLpaiPerfConfig_GetOptionFn_t getOption;
  QairtLpaiPerfConfig_SetEnpuPowerLevelFn_t setEnpuPowerLevel;
  QairtLpaiPerfConfig_GetEnpuPowerLevelFn_t getEnpuPowerLevel;
} QairtLpaiPerfConfigV1_t;

/// V1 interface for a perf-configuration list. Retrieved via QAIRT_LPAI_PERF_CONFIG_LIST_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtLpaiPerfConfigList_CreateFn_t create;
  QairtLpaiPerfConfigList_FreeFn_t free;
  QairtLpaiPerfConfigList_AddConfigFn_t addConfig;
  QairtLpaiPerfConfigList_GetNumConfigsFn_t getNumConfigs;
  QairtLpaiPerfConfigList_GetConfigAtFn_t getConfigAt;
  QairtLpaiPerfConfigList_ClearFn_t clear;
} QairtLpaiPerfConfigListV1_t;

/**
 * @struct QairtLpaiPerfInfrastructureV1_t
 * @brief V1 interface for the QAIRT LPAI perf infrastructure. Retrieved via
 *        the id QAIRT_LPAI_PERF_INFRASTRUCTURE_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtLpaiDevice_GetPerfInfrastructureFn_t getPerfInfrastructure;
  QairtLpaiPerfInfrastructure_FreeFn_t free;
  QairtLpaiPerfInfrastructure_SetPowerConfigFn_t setPowerConfig;
} QairtLpaiPerfInfrastructureV1_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAIRT_LPAI_PERF_INFRASTRUCTURE_H
