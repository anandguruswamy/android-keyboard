//==============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

/**
 * @file
 * @brief   DSP Performance Infrastructure component API
 *
 *          Provides an interface to control runtime performance and
 *          power settings (DCVS, voltage corners, sleep latency, RPC polling,
 *          HMX timeout, VTCM, HVX thread count) of the DSP accelerator.
 *
 *          The perf-infrastructure handle is obtained from the opaque device
 *          infrastructure handle returned by
 *          QairtDevice_InfrastructureProvider_getInfrastructure, via
 *          QairtDspDevice_getPerfInfrastructure. Support is optional and is
 *          advertised through QAIRT_CAPABILITY_DEVICE_SUPPORT_INFRASTRUCTURE.
 */

#ifndef QAIRT_DSP_PERF_INFRASTRUCTURE_H
#define QAIRT_DSP_PERF_INFRASTRUCTURE_H

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

/// Interface IDs for the DSP perf-infrastructure sub-objects and the top-level
/// perf-infrastructure interface.
#define QAIRT_MIN_ID_DSP_PERF (QAIRT_MIN_ID_DSP + 300)

#define QAIRT_DSP_PERF_INFRASTRUCTURE_ID     (QAIRT_MIN_ID_DSP_PERF + 0)
#define QAIRT_DSP_PERF_POWER_CONFIG_ID       (QAIRT_MIN_ID_DSP_PERF + 1)
#define QAIRT_DSP_PERF_POWER_CONFIG_LIST_ID  (QAIRT_MIN_ID_DSP_PERF + 2)
#define QAIRT_DSP_PERF_MEMORY_CONFIG_ID      (QAIRT_MIN_ID_DSP_PERF + 3)
#define QAIRT_DSP_PERF_MEMORY_CONFIG_LIST_ID (QAIRT_MIN_ID_DSP_PERF + 4)
#define QAIRT_DSP_PERF_THREAD_CONFIG_ID      (QAIRT_MIN_ID_DSP_PERF + 5)
#define QAIRT_DSP_PERF_THREAD_CONFIG_LIST_ID (QAIRT_MIN_ID_DSP_PERF + 6)

// Define ranges for DSP perf-infrastructure specific error codes
#define QAIRT_MIN_ERROR_DSP_PERF (QAIRT_MIN_ERROR_DSP + 3000)
#define QAIRT_MAX_ERROR_DSP_PERF (QAIRT_MIN_ERROR_DSP + 3999)

/// Maximum RPC polling time allowed, in microseconds.
#define QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIG_MAX_RPC_POLLING_TIME 9999

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Data Types
//=============================================================================

/**
 * @brief Opaque handle to a DSP perf-infrastructure object.
 *
 *        Obtained via QairtDspDevice_getPerfInfrastructure and released with
 *        QairtDspPerfInfrastructure_free. The underlying backend infrastructure
 *        it references is backend-owned and is not freed by
 *        QairtDspPerfInfrastructure_free.
 */
typedef struct _QairtDspPerfInfrastructure_Handle_t* QairtDspPerfInfrastructure_Handle_t;

/// Opaque handle to a single power-configuration entry (one option at a time).
typedef struct _QairtDspPerfPowerConfigHandle_t* QairtDspPerfPowerConfigHandle_t;
/// Opaque handle to an ordered collection of power-configuration entries.
typedef struct _QairtDspPerfPowerConfigListHandle_t* QairtDspPerfPowerConfigListHandle_t;
/// Opaque handle to a single memory-configuration entry (one option at a time).
typedef struct _QairtDspPerfMemoryConfigHandle_t* QairtDspPerfMemoryConfigHandle_t;
/// Opaque handle to an ordered collection of memory-configuration entries.
typedef struct _QairtDspPerfMemoryConfigListHandle_t* QairtDspPerfMemoryConfigListHandle_t;
/// Opaque handle to a single thread-configuration entry (one option at a time).
typedef struct _QairtDspPerfThreadConfigHandle_t* QairtDspPerfThreadConfigHandle_t;
/// Opaque handle to an ordered collection of thread-configuration entries.
typedef struct _QairtDspPerfThreadConfigListHandle_t* QairtDspPerfThreadConfigListHandle_t;

/**
 * @brief QAIRT DSP PerfInfrastructure API result / error codes.
 */
typedef enum {
  QAIRT_DSP_PERF_INFRASTRUCTURE_MIN_ERROR = QAIRT_MIN_ERROR_DSP_PERF,
  ////////////////////////////////////////

  /// DSP perf-infrastructure operation succeeded.
  QAIRT_DSP_PERF_INFRASTRUCTURE_NO_ERROR = QAIRT_SUCCESS,
  /// A handle argument is not valid.
  QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_INVALID_HANDLE = QAIRT_MIN_ERROR_DSP_PERF + 0,
  /// An argument to the API is invalid.
  QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_INVALID_INPUT = QAIRT_MIN_ERROR_DSP_PERF + 1,
  /// A configuration option is not supported.
  QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_UNSUPPORTED_CONFIG = QAIRT_MIN_ERROR_DSP_PERF + 2,
  /// Transport error communicating with the accelerator.
  QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_TRANSPORT = QAIRT_MIN_ERROR_DSP_PERF + 3,
  /// A get call was made for an option that is not the one currently set.
  QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_OPTION_MISMATCH = QAIRT_MIN_ERROR_DSP_PERF + 4,
  /// Feature or API is not supported on this target.
  QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_UNSUPPORTED = QAIRT_COMMON_ERROR_NOT_SUPPORTED,
  /// General memory allocation failure.
  QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_MEM_ALLOC = QAIRT_COMMON_ERROR_MEM_ALLOC,
  /// General error not covered by a more specific code.
  QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_FAILED = QAIRT_COMMON_ERROR_GENERAL,

  ////////////////////////////////////////
  QAIRT_DSP_PERF_INFRASTRUCTURE_MAX_ERROR = QAIRT_MAX_ERROR_DSP_PERF,
  // Unused, present to ensure 32 bits.
  QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_UNDEFINED = 0x7FFFFFFF
} QairtDspPerfInfrastructure_Error_t;

/**
 * @brief Voltage corners requestable by the client to influence the DCVS
 *        voting scheme.
 */
typedef enum {
  QAIRT_DSP_DCVS_VOLTAGE_CORNER_DISABLE             = 0x10,
  QAIRT_DSP_DCVS_VOLTAGE_VCORNER_MIN_VOLTAGE_CORNER = 0x20,
  QAIRT_DSP_DCVS_VOLTAGE_VCORNER_SVS2               = 0x30,
  QAIRT_DSP_DCVS_VOLTAGE_VCORNER_SVS                = 0x40,
  QAIRT_DSP_DCVS_VOLTAGE_VCORNER_SVS_PLUS           = 0x50,
  QAIRT_DSP_DCVS_VOLTAGE_VCORNER_NOM                = 0x60,
  QAIRT_DSP_DCVS_VOLTAGE_VCORNER_NOM_PLUS           = 0x70,
  QAIRT_DSP_DCVS_VOLTAGE_VCORNER_TURBO              = 0x80,
  QAIRT_DSP_DCVS_VOLTAGE_VCORNER_TURBO_PLUS         = 0x90,
  QAIRT_DSP_DCVS_VOLTAGE_VCORNER_MAX_VOLTAGE_CORNER = 0xA0,
  QAIRT_DSP_DCVS_VOLTAGE_VCORNER_UNKNOWN            = 0x7FFFFFFF
} QairtDspPerfInfrastructure_VoltageCorner_t;

/**
 * @brief Power modes to influence DCVS behavior.
 */
typedef enum {
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_ADJUST_UP_DOWN               = 0x1,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_ADJUST_ONLY_UP               = 0x2,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_POWER_SAVER_MODE             = 0x4,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_POWER_SAVER_AGGRESSIVE_MODE  = 0x8,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_PERFORMANCE_MODE             = 0x10,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_DUTY_CYCLE_MODE              = 0x20,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWERMODE_UNKNOWN                      = 0x7FFFFFFF
} QairtDspPerfInfrastructure_PowerMode_t;

/**
 * @brief Power configuration option selecting which value is active on a
 *        QairtDspPerfPowerConfigHandle_t.
 */
typedef enum {
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_ENABLE           = 1,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_SLEEP_LATENCY         = 2,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_SLEEP_DISABLE         = 3,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_POWER_MODE       = 4,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_VOLTAGE_CORNER   = 5,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_BUS_VOLTAGE_CORNER    = 6,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_CORE_VOLTAGE_CORNER   = 7,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_CONTROL_LATENCY   = 9,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_POLLING_TIME      = 10,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_HMX_TIMEOUT_INTERVAL_US = 11,
  QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_UNKNOWN               = 0x7FFFFFFF
} QairtDspPerfInfrastructure_PowerConfigOption_t;

/**
 * @brief Memory configuration option selecting which value is active on a
 *        QairtDspPerfMemoryConfigHandle_t.
 */
typedef enum {
  QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_GROW_SIZE          = 1,
  QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_VTCM_SIZE          = 2,
  QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_VTCM_USAGE_FACTOR  = 3,
  QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_UNKNOWN            = 0x7FFFFFFF
} QairtDspPerfInfrastructure_MemoryConfigOption_t;

/**
 * @brief Thread configuration option selecting which value is active on a
 *        QairtDspPerfThreadConfigHandle_t.
 */
typedef enum {
  QAIRT_DSP_PERF_INFRASTRUCTURE_THREAD_CONFIGOPTION_NUMBER_OF_HVX_THREADS = 1,
  QAIRT_DSP_PERF_INFRASTRUCTURE_THREAD_CONFIGOPTION_UNKNOWN               = 0x7FFFFFFF
} QairtDspPerfInfrastructure_ThreadConfigOption_t;

/**
 * @brief VTCM usage configuration for on-target libraries.
 */
typedef enum {
  QAIRT_DSP_PERF_INFRASTRUCTURE_VTCM_USE_FULL    = 1,
  QAIRT_DSP_PERF_INFRASTRUCTURE_VTCM_USE_MIN     = 2,
  QAIRT_DSP_PERF_INFRASTRUCTURE_VTCM_USE_UNKNOWN = 0x7FFFFFFF
} QairtDspPerfInfrastructure_VtcmUsageFactor_t;

//=============================================================================
// Power Configuration (one option active at a time)
//=============================================================================

/**
 * @brief Create a power-configuration handle. No option is active until one
 *        of the setters below is called.
 */
Qairt_Status_t QairtDspPerfPowerConfig_create(QairtDspPerfPowerConfigHandle_t* config);
Qairt_Status_t QairtDspPerfPowerConfig_free(QairtDspPerfPowerConfigHandle_t config);

/// Get which option is currently active on this handle.
Qairt_Status_t QairtDspPerfPowerConfig_getOption(
    QairtDspPerfPowerConfigHandle_t config, QairtDspPerfInfrastructure_PowerConfigOption_t* option);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_ENABLE.
Qairt_Status_t QairtDspPerfPowerConfig_setDcvsEnable(QairtDspPerfPowerConfigHandle_t config,
                                                     uint8_t dcvsEnable);
/// Valid only if getOption reports DCVS_ENABLE.
Qairt_Status_t QairtDspPerfPowerConfig_getDcvsEnable(QairtDspPerfPowerConfigHandle_t config,
                                                     uint8_t* dcvsEnable);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_SLEEP_LATENCY.
Qairt_Status_t QairtDspPerfPowerConfig_setSleepLatencyUs(QairtDspPerfPowerConfigHandle_t config,
                                                         uint32_t sleepLatencyUs);
/// Valid only if getOption reports SLEEP_LATENCY.
Qairt_Status_t QairtDspPerfPowerConfig_getSleepLatencyUs(QairtDspPerfPowerConfigHandle_t config,
                                                         uint32_t* sleepLatencyUs);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_SLEEP_DISABLE.
Qairt_Status_t QairtDspPerfPowerConfig_setSleepDisable(QairtDspPerfPowerConfigHandle_t config,
                                                       uint8_t sleepDisable);
/// Valid only if getOption reports SLEEP_DISABLE.
Qairt_Status_t QairtDspPerfPowerConfig_getSleepDisable(QairtDspPerfPowerConfigHandle_t config,
                                                       uint8_t* sleepDisable);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_POWER_MODE.
Qairt_Status_t QairtDspPerfPowerConfig_setDcvsPowerMode(
    QairtDspPerfPowerConfigHandle_t config, QairtDspPerfInfrastructure_PowerMode_t powerMode);
/// Valid only if getOption reports DCVS_POWER_MODE.
Qairt_Status_t QairtDspPerfPowerConfig_getDcvsPowerMode(
    QairtDspPerfPowerConfigHandle_t config, QairtDspPerfInfrastructure_PowerMode_t* powerMode);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_VOLTAGE_CORNER.
Qairt_Status_t QairtDspPerfPowerConfig_setDcvsVoltageCorners(
    QairtDspPerfPowerConfigHandle_t config,
    QairtDspPerfInfrastructure_VoltageCorner_t min,
    QairtDspPerfInfrastructure_VoltageCorner_t target,
    QairtDspPerfInfrastructure_VoltageCorner_t max);
/// Valid only if getOption reports DCVS_VOLTAGE_CORNER.
Qairt_Status_t QairtDspPerfPowerConfig_getDcvsVoltageCorners(
    QairtDspPerfPowerConfigHandle_t config,
    QairtDspPerfInfrastructure_VoltageCorner_t* min,
    QairtDspPerfInfrastructure_VoltageCorner_t* target,
    QairtDspPerfInfrastructure_VoltageCorner_t* max);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_BUS_VOLTAGE_CORNER.
Qairt_Status_t QairtDspPerfPowerConfig_setBusVoltageCorners(
    QairtDspPerfPowerConfigHandle_t config,
    QairtDspPerfInfrastructure_VoltageCorner_t min,
    QairtDspPerfInfrastructure_VoltageCorner_t target,
    QairtDspPerfInfrastructure_VoltageCorner_t max);
/// Valid only if getOption reports BUS_VOLTAGE_CORNER.
Qairt_Status_t QairtDspPerfPowerConfig_getBusVoltageCorners(
    QairtDspPerfPowerConfigHandle_t config,
    QairtDspPerfInfrastructure_VoltageCorner_t* min,
    QairtDspPerfInfrastructure_VoltageCorner_t* target,
    QairtDspPerfInfrastructure_VoltageCorner_t* max);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_CORE_VOLTAGE_CORNER.
Qairt_Status_t QairtDspPerfPowerConfig_setCoreVoltageCorners(
    QairtDspPerfPowerConfigHandle_t config,
    QairtDspPerfInfrastructure_VoltageCorner_t min,
    QairtDspPerfInfrastructure_VoltageCorner_t target,
    QairtDspPerfInfrastructure_VoltageCorner_t max);
/// Valid only if getOption reports CORE_VOLTAGE_CORNER.
Qairt_Status_t QairtDspPerfPowerConfig_getCoreVoltageCorners(
    QairtDspPerfPowerConfigHandle_t config,
    QairtDspPerfInfrastructure_VoltageCorner_t* min,
    QairtDspPerfInfrastructure_VoltageCorner_t* target,
    QairtDspPerfInfrastructure_VoltageCorner_t* max);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_CONTROL_LATENCY.
Qairt_Status_t QairtDspPerfPowerConfig_setRpcControlLatencyUs(
    QairtDspPerfPowerConfigHandle_t config, uint32_t rpcControlLatencyUs);
Qairt_Status_t QairtDspPerfPowerConfig_getRpcControlLatencyUs(
    QairtDspPerfPowerConfigHandle_t config, uint32_t* rpcControlLatencyUs);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_POLLING_TIME.
/// Supported on V69+; max value is
/// QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIG_MAX_RPC_POLLING_TIME.
Qairt_Status_t QairtDspPerfPowerConfig_setRpcPollingTimeUs(QairtDspPerfPowerConfigHandle_t config,
                                                           uint32_t rpcPollingTimeUs);
Qairt_Status_t QairtDspPerfPowerConfig_getRpcPollingTimeUs(QairtDspPerfPowerConfigHandle_t config,
                                                           uint32_t* rpcPollingTimeUs);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_HMX_TIMEOUT_INTERVAL_US.
Qairt_Status_t QairtDspPerfPowerConfig_setHmxTimeoutIntervalUs(
    QairtDspPerfPowerConfigHandle_t config, uint32_t hmxTimeoutIntervalUs);
Qairt_Status_t QairtDspPerfPowerConfig_getHmxTimeoutIntervalUs(
    QairtDspPerfPowerConfigHandle_t config, uint32_t* hmxTimeoutIntervalUs);

//=============================================================================
// Power Configuration List
//=============================================================================

/// Create an empty, ordered collection of power-configuration entries.
Qairt_Status_t QairtDspPerfPowerConfigList_create(QairtDspPerfPowerConfigListHandle_t* list);
Qairt_Status_t QairtDspPerfPowerConfigList_free(QairtDspPerfPowerConfigListHandle_t list);

/// Append a copy of powerConfig's current value to the list. The caller
/// retains ownership of powerConfig and may free or reuse it afterward.
Qairt_Status_t QairtDspPerfPowerConfigList_addPowerConfig(
    QairtDspPerfPowerConfigListHandle_t list, QairtDspPerfPowerConfigHandle_t powerConfig);

Qairt_Status_t QairtDspPerfPowerConfigList_getNumPowerConfigs(
    QairtDspPerfPowerConfigListHandle_t list, uint32_t* numPowerConfigs);

/// Get a non-owning handle to the entry at index; do not free it. Valid
/// until list is freed or cleared.
Qairt_Status_t QairtDspPerfPowerConfigList_getPowerConfigAt(
    QairtDspPerfPowerConfigListHandle_t list,
    uint32_t index,
    QairtDspPerfPowerConfigHandle_t* powerConfig);

Qairt_Status_t QairtDspPerfPowerConfigList_clear(QairtDspPerfPowerConfigListHandle_t list);

//=============================================================================
// Memory Configuration (one option active at a time)
//=============================================================================

Qairt_Status_t QairtDspPerfMemoryConfig_create(QairtDspPerfMemoryConfigHandle_t* config);
Qairt_Status_t QairtDspPerfMemoryConfig_free(QairtDspPerfMemoryConfigHandle_t config);

Qairt_Status_t QairtDspPerfMemoryConfig_getOption(
    QairtDspPerfMemoryConfigHandle_t config,
    QairtDspPerfInfrastructure_MemoryConfigOption_t* option);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_GROW_SIZE. Sets
/// the minimum size (bytes) by which the user heap grows when exhausted.
/// Expected to be set once per backend; has a process-wide impact.
Qairt_Status_t QairtDspPerfMemoryConfig_setGrowSizeBytes(QairtDspPerfMemoryConfigHandle_t config,
                                                         uint32_t growSizeBytes);
Qairt_Status_t QairtDspPerfMemoryConfig_getGrowSizeBytes(QairtDspPerfMemoryConfigHandle_t config,
                                                         uint32_t* growSizeBytes);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_VTCM_SIZE. Only
/// applicable to offline graph preparation (no physical device present).
Qairt_Status_t QairtDspPerfMemoryConfig_setVtcmSizeMB(QairtDspPerfMemoryConfigHandle_t config,
                                                      uint32_t vtcmSizeMB);
Qairt_Status_t QairtDspPerfMemoryConfig_getVtcmSizeMB(QairtDspPerfMemoryConfigHandle_t config,
                                                      uint32_t* vtcmSizeMB);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_VTCM_USAGE_FACTOR.
/// Only applicable on-target.
Qairt_Status_t QairtDspPerfMemoryConfig_setVtcmUsageFactor(
    QairtDspPerfMemoryConfigHandle_t config, QairtDspPerfInfrastructure_VtcmUsageFactor_t usage);
Qairt_Status_t QairtDspPerfMemoryConfig_getVtcmUsageFactor(
    QairtDspPerfMemoryConfigHandle_t config, QairtDspPerfInfrastructure_VtcmUsageFactor_t* usage);

//=============================================================================
// Memory Configuration List
//=============================================================================

Qairt_Status_t QairtDspPerfMemoryConfigList_create(QairtDspPerfMemoryConfigListHandle_t* list);
Qairt_Status_t QairtDspPerfMemoryConfigList_free(QairtDspPerfMemoryConfigListHandle_t list);

Qairt_Status_t QairtDspPerfMemoryConfigList_addMemoryConfig(
    QairtDspPerfMemoryConfigListHandle_t list, QairtDspPerfMemoryConfigHandle_t memoryConfig);

Qairt_Status_t QairtDspPerfMemoryConfigList_getNumMemoryConfigs(
    QairtDspPerfMemoryConfigListHandle_t list, uint32_t* numMemoryConfigs);

/// Get a non-owning handle to the entry at index; do not free it. Valid
/// until list is freed or cleared.
Qairt_Status_t QairtDspPerfMemoryConfigList_getMemoryConfigAt(
    QairtDspPerfMemoryConfigListHandle_t list,
    uint32_t index,
    QairtDspPerfMemoryConfigHandle_t* memoryConfig);

Qairt_Status_t QairtDspPerfMemoryConfigList_clear(QairtDspPerfMemoryConfigListHandle_t list);

//=============================================================================
// Thread Configuration (one option active at a time)
//=============================================================================

Qairt_Status_t QairtDspPerfThreadConfig_create(QairtDspPerfThreadConfigHandle_t* config);
Qairt_Status_t QairtDspPerfThreadConfig_free(QairtDspPerfThreadConfigHandle_t config);

Qairt_Status_t QairtDspPerfThreadConfig_getOption(
    QairtDspPerfThreadConfigHandle_t config,
    QairtDspPerfInfrastructure_ThreadConfigOption_t* option);

/// Sets option QAIRT_DSP_PERF_INFRASTRUCTURE_THREAD_CONFIGOPTION_NUMBER_OF_HVX_THREADS.
Qairt_Status_t QairtDspPerfThreadConfig_setNumHvxThreads(QairtDspPerfThreadConfigHandle_t config,
                                                         uint32_t numHvxThreads);
Qairt_Status_t QairtDspPerfThreadConfig_getNumHvxThreads(QairtDspPerfThreadConfigHandle_t config,
                                                         uint32_t* numHvxThreads);

//=============================================================================
// Thread Configuration List
//=============================================================================

Qairt_Status_t QairtDspPerfThreadConfigList_create(QairtDspPerfThreadConfigListHandle_t* list);
Qairt_Status_t QairtDspPerfThreadConfigList_free(QairtDspPerfThreadConfigListHandle_t list);

Qairt_Status_t QairtDspPerfThreadConfigList_addThreadConfig(
    QairtDspPerfThreadConfigListHandle_t list, QairtDspPerfThreadConfigHandle_t threadConfig);

Qairt_Status_t QairtDspPerfThreadConfigList_getNumThreadConfigs(
    QairtDspPerfThreadConfigListHandle_t list, uint32_t* numThreadConfigs);

/// Get a non-owning handle to the entry at index; do not free it. Valid
/// until list is freed or cleared.
Qairt_Status_t QairtDspPerfThreadConfigList_getThreadConfigAt(
    QairtDspPerfThreadConfigListHandle_t list,
    uint32_t index,
    QairtDspPerfThreadConfigHandle_t* threadConfig);

Qairt_Status_t QairtDspPerfThreadConfigList_clear(QairtDspPerfThreadConfigListHandle_t list);

//=============================================================================
// Perf Infrastructure
//=============================================================================

/**
 * @brief Resolve a DSP perf-infrastructure handle from a device infrastructure
 *        handle.
 *
 * @param[in]  deviceInfraHandle Device infrastructure handle obtained from
 *                               QairtDevice_InfrastructureProvider_getInfrastructure.
 * @param[out] perfInfraHandle   Pointer to receive the perf-infrastructure handle.
 *                               Release with QairtDspPerfInfrastructure_free.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: handle resolved.
 *         - QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_INVALID_INPUT: perfInfraHandle is NULL.
 *         - QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_INVALID_HANDLE: deviceInfraHandle invalid.
 *         - QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_UNSUPPORTED: backend does not
 *           expose a perf infrastructure.
 *         - QAIRT_DSP_PERF_INFRASTRUCTURE_ERROR_MEM_ALLOC: allocation failed.
 */
Qairt_Status_t QairtDspDevice_getPerfInfrastructure(
    QairtDevice_InfrastructureHandle_t deviceInfraHandle,
    QairtDspPerfInfrastructure_Handle_t* perfInfraHandle);

/**
 * @brief Free a DSP perf-infrastructure handle. Does not free the backend-owned
 *        infrastructure it references.
 */
Qairt_Status_t QairtDspPerfInfrastructure_free(QairtDspPerfInfrastructure_Handle_t perfInfraHandle);

/**
 * @brief Create a power configuration id, used to set performance modes.
 *        Destroy with QairtDspPerfInfrastructure_destroyPowerConfigId.
 */
Qairt_Status_t QairtDspPerfInfrastructure_createPowerConfigId(
    QairtDspPerfInfrastructure_Handle_t perfInfraHandle, uint32_t* powerConfigId);

/**
 * @brief Destroy a power configuration id.
 */
Qairt_Status_t QairtDspPerfInfrastructure_destroyPowerConfigId(
    QairtDspPerfInfrastructure_Handle_t perfInfraHandle, uint32_t powerConfigId);

/**
 * @brief Set system power configuration to enable different performance modes.
 *
 * @param[in] perfInfraHandle Perf-infrastructure handle.
 * @param[in] powerConfigId   Power config id from createPowerConfigId. A value of
 *                            0 implies a NULL power client id.
 * @param[in] configList      Handle to a list of power-configuration entries.
 *                            NULL indicates no config options.
 */
Qairt_Status_t QairtDspPerfInfrastructure_setPowerConfig(
    QairtDspPerfInfrastructure_Handle_t perfInfraHandle,
    uint32_t powerConfigId,
    QairtDspPerfPowerConfigListHandle_t configList);

/**
 * @brief Set system memory configuration.
 *
 * @param[in] perfInfraHandle Perf-infrastructure handle.
 * @param[in] configList      Handle to a list of memory-configuration entries.
 *                            NULL indicates no config options.
 */
Qairt_Status_t QairtDspPerfInfrastructure_setMemoryConfig(
    QairtDspPerfInfrastructure_Handle_t perfInfraHandle,
    QairtDspPerfMemoryConfigListHandle_t configList);

/**
 * @brief Set thread configuration. Must be called after QairtBackend
 *        initialization and before context/graph creation.
 *
 * @param[in] perfInfraHandle Perf-infrastructure handle.
 * @param[in] configList      Handle to a list of thread-configuration entries.
 *                            NULL indicates no config options.
 */
Qairt_Status_t QairtDspPerfInfrastructure_setThreadConfig(
    QairtDspPerfInfrastructure_Handle_t perfInfraHandle,
    QairtDspPerfThreadConfigListHandle_t configList);

//=============================================================================
// Function Pointer Typedefs
//=============================================================================

/* Power config (tagged slot) */
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_CreateFn_t)(QairtDspPerfPowerConfigHandle_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_FreeFn_t)(QairtDspPerfPowerConfigHandle_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetOptionFn_t)(
    QairtDspPerfPowerConfigHandle_t, QairtDspPerfInfrastructure_PowerConfigOption_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_SetDcvsEnableFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint8_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetDcvsEnableFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint8_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_SetSleepLatencyUsFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetSleepLatencyUsFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_SetSleepDisableFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint8_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetSleepDisableFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint8_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_SetDcvsPowerModeFn_t)(
    QairtDspPerfPowerConfigHandle_t, QairtDspPerfInfrastructure_PowerMode_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetDcvsPowerModeFn_t)(
    QairtDspPerfPowerConfigHandle_t, QairtDspPerfInfrastructure_PowerMode_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_SetDcvsVoltageCornersFn_t)(
    QairtDspPerfPowerConfigHandle_t,
    QairtDspPerfInfrastructure_VoltageCorner_t,
    QairtDspPerfInfrastructure_VoltageCorner_t,
    QairtDspPerfInfrastructure_VoltageCorner_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetDcvsVoltageCornersFn_t)(
    QairtDspPerfPowerConfigHandle_t,
    QairtDspPerfInfrastructure_VoltageCorner_t*,
    QairtDspPerfInfrastructure_VoltageCorner_t*,
    QairtDspPerfInfrastructure_VoltageCorner_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_SetBusVoltageCornersFn_t)(
    QairtDspPerfPowerConfigHandle_t,
    QairtDspPerfInfrastructure_VoltageCorner_t,
    QairtDspPerfInfrastructure_VoltageCorner_t,
    QairtDspPerfInfrastructure_VoltageCorner_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetBusVoltageCornersFn_t)(
    QairtDspPerfPowerConfigHandle_t,
    QairtDspPerfInfrastructure_VoltageCorner_t*,
    QairtDspPerfInfrastructure_VoltageCorner_t*,
    QairtDspPerfInfrastructure_VoltageCorner_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_SetCoreVoltageCornersFn_t)(
    QairtDspPerfPowerConfigHandle_t,
    QairtDspPerfInfrastructure_VoltageCorner_t,
    QairtDspPerfInfrastructure_VoltageCorner_t,
    QairtDspPerfInfrastructure_VoltageCorner_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetCoreVoltageCornersFn_t)(
    QairtDspPerfPowerConfigHandle_t,
    QairtDspPerfInfrastructure_VoltageCorner_t*,
    QairtDspPerfInfrastructure_VoltageCorner_t*,
    QairtDspPerfInfrastructure_VoltageCorner_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_SetRpcControlLatencyUsFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetRpcControlLatencyUsFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_SetRpcPollingTimeUsFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetRpcPollingTimeUsFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_SetHmxTimeoutIntervalUsFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfig_GetHmxTimeoutIntervalUsFn_t)(
    QairtDspPerfPowerConfigHandle_t, uint32_t*);

/* Power config list */
typedef Qairt_Status_t (*QairtDspPerfPowerConfigList_CreateFn_t)(
    QairtDspPerfPowerConfigListHandle_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfigList_FreeFn_t)(QairtDspPerfPowerConfigListHandle_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfigList_AddPowerConfigFn_t)(
    QairtDspPerfPowerConfigListHandle_t, QairtDspPerfPowerConfigHandle_t);
typedef Qairt_Status_t (*QairtDspPerfPowerConfigList_GetNumPowerConfigsFn_t)(
    QairtDspPerfPowerConfigListHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfigList_GetPowerConfigAtFn_t)(
    QairtDspPerfPowerConfigListHandle_t, uint32_t, QairtDspPerfPowerConfigHandle_t*);
typedef Qairt_Status_t (*QairtDspPerfPowerConfigList_ClearFn_t)(QairtDspPerfPowerConfigListHandle_t);

/* Memory config (tagged slot) */
typedef Qairt_Status_t (*QairtDspPerfMemoryConfig_CreateFn_t)(QairtDspPerfMemoryConfigHandle_t*);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfig_FreeFn_t)(QairtDspPerfMemoryConfigHandle_t);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfig_GetOptionFn_t)(
    QairtDspPerfMemoryConfigHandle_t, QairtDspPerfInfrastructure_MemoryConfigOption_t*);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfig_SetGrowSizeBytesFn_t)(
    QairtDspPerfMemoryConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfig_GetGrowSizeBytesFn_t)(
    QairtDspPerfMemoryConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfig_SetVtcmSizeMBFn_t)(
    QairtDspPerfMemoryConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfig_GetVtcmSizeMBFn_t)(
    QairtDspPerfMemoryConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfig_SetVtcmUsageFactorFn_t)(
    QairtDspPerfMemoryConfigHandle_t, QairtDspPerfInfrastructure_VtcmUsageFactor_t);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfig_GetVtcmUsageFactorFn_t)(
    QairtDspPerfMemoryConfigHandle_t, QairtDspPerfInfrastructure_VtcmUsageFactor_t*);

/* Memory config list */
typedef Qairt_Status_t (*QairtDspPerfMemoryConfigList_CreateFn_t)(
    QairtDspPerfMemoryConfigListHandle_t*);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfigList_FreeFn_t)(
    QairtDspPerfMemoryConfigListHandle_t);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfigList_AddMemoryConfigFn_t)(
    QairtDspPerfMemoryConfigListHandle_t, QairtDspPerfMemoryConfigHandle_t);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfigList_GetNumMemoryConfigsFn_t)(
    QairtDspPerfMemoryConfigListHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfigList_GetMemoryConfigAtFn_t)(
    QairtDspPerfMemoryConfigListHandle_t, uint32_t, QairtDspPerfMemoryConfigHandle_t*);
typedef Qairt_Status_t (*QairtDspPerfMemoryConfigList_ClearFn_t)(
    QairtDspPerfMemoryConfigListHandle_t);

/* Thread config (tagged slot) */
typedef Qairt_Status_t (*QairtDspPerfThreadConfig_CreateFn_t)(QairtDspPerfThreadConfigHandle_t*);
typedef Qairt_Status_t (*QairtDspPerfThreadConfig_FreeFn_t)(QairtDspPerfThreadConfigHandle_t);
typedef Qairt_Status_t (*QairtDspPerfThreadConfig_GetOptionFn_t)(
    QairtDspPerfThreadConfigHandle_t, QairtDspPerfInfrastructure_ThreadConfigOption_t*);
typedef Qairt_Status_t (*QairtDspPerfThreadConfig_SetNumHvxThreadsFn_t)(
    QairtDspPerfThreadConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtDspPerfThreadConfig_GetNumHvxThreadsFn_t)(
    QairtDspPerfThreadConfigHandle_t, uint32_t*);

/* Thread config list */
typedef Qairt_Status_t (*QairtDspPerfThreadConfigList_CreateFn_t)(
    QairtDspPerfThreadConfigListHandle_t*);
typedef Qairt_Status_t (*QairtDspPerfThreadConfigList_FreeFn_t)(
    QairtDspPerfThreadConfigListHandle_t);
typedef Qairt_Status_t (*QairtDspPerfThreadConfigList_AddThreadConfigFn_t)(
    QairtDspPerfThreadConfigListHandle_t, QairtDspPerfThreadConfigHandle_t);
typedef Qairt_Status_t (*QairtDspPerfThreadConfigList_GetNumThreadConfigsFn_t)(
    QairtDspPerfThreadConfigListHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtDspPerfThreadConfigList_GetThreadConfigAtFn_t)(
    QairtDspPerfThreadConfigListHandle_t, uint32_t, QairtDspPerfThreadConfigHandle_t*);
typedef Qairt_Status_t (*QairtDspPerfThreadConfigList_ClearFn_t)(
    QairtDspPerfThreadConfigListHandle_t);

/* Perf infrastructure */
typedef Qairt_Status_t (*QairtDspDevice_GetPerfInfrastructureFn_t)(
    QairtDevice_InfrastructureHandle_t, QairtDspPerfInfrastructure_Handle_t*);
typedef Qairt_Status_t (*QairtDspPerfInfrastructure_FreeFn_t)(QairtDspPerfInfrastructure_Handle_t);
typedef Qairt_Status_t (*QairtDspPerfInfrastructure_CreatePowerConfigIdFn_t)(
    QairtDspPerfInfrastructure_Handle_t, uint32_t*);
typedef Qairt_Status_t (*QairtDspPerfInfrastructure_DestroyPowerConfigIdFn_t)(
    QairtDspPerfInfrastructure_Handle_t, uint32_t);
typedef Qairt_Status_t (*QairtDspPerfInfrastructure_SetPowerConfigFn_t)(
    QairtDspPerfInfrastructure_Handle_t, uint32_t, QairtDspPerfPowerConfigListHandle_t);
typedef Qairt_Status_t (*QairtDspPerfInfrastructure_SetMemoryConfigFn_t)(
    QairtDspPerfInfrastructure_Handle_t, QairtDspPerfMemoryConfigListHandle_t);
typedef Qairt_Status_t (*QairtDspPerfInfrastructure_SetThreadConfigFn_t)(
    QairtDspPerfInfrastructure_Handle_t, QairtDspPerfThreadConfigListHandle_t);

//=============================================================================
// Public Interfaces
//=============================================================================

/// V1 interface for a power-configuration slot. Retrieved via QAIRT_DSP_PERF_POWER_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtDspPerfPowerConfig_CreateFn_t create;
  QairtDspPerfPowerConfig_FreeFn_t free;
  QairtDspPerfPowerConfig_GetOptionFn_t getOption;
  QairtDspPerfPowerConfig_SetDcvsEnableFn_t setDcvsEnable;
  QairtDspPerfPowerConfig_GetDcvsEnableFn_t getDcvsEnable;
  QairtDspPerfPowerConfig_SetSleepLatencyUsFn_t setSleepLatencyUs;
  QairtDspPerfPowerConfig_GetSleepLatencyUsFn_t getSleepLatencyUs;
  QairtDspPerfPowerConfig_SetSleepDisableFn_t setSleepDisable;
  QairtDspPerfPowerConfig_GetSleepDisableFn_t getSleepDisable;
  QairtDspPerfPowerConfig_SetDcvsPowerModeFn_t setDcvsPowerMode;
  QairtDspPerfPowerConfig_GetDcvsPowerModeFn_t getDcvsPowerMode;
  QairtDspPerfPowerConfig_SetDcvsVoltageCornersFn_t setDcvsVoltageCorners;
  QairtDspPerfPowerConfig_GetDcvsVoltageCornersFn_t getDcvsVoltageCorners;
  QairtDspPerfPowerConfig_SetBusVoltageCornersFn_t setBusVoltageCorners;
  QairtDspPerfPowerConfig_GetBusVoltageCornersFn_t getBusVoltageCorners;
  QairtDspPerfPowerConfig_SetCoreVoltageCornersFn_t setCoreVoltageCorners;
  QairtDspPerfPowerConfig_GetCoreVoltageCornersFn_t getCoreVoltageCorners;
  QairtDspPerfPowerConfig_SetRpcControlLatencyUsFn_t setRpcControlLatencyUs;
  QairtDspPerfPowerConfig_GetRpcControlLatencyUsFn_t getRpcControlLatencyUs;
  QairtDspPerfPowerConfig_SetRpcPollingTimeUsFn_t setRpcPollingTimeUs;
  QairtDspPerfPowerConfig_GetRpcPollingTimeUsFn_t getRpcPollingTimeUs;
  QairtDspPerfPowerConfig_SetHmxTimeoutIntervalUsFn_t setHmxTimeoutIntervalUs;
  QairtDspPerfPowerConfig_GetHmxTimeoutIntervalUsFn_t getHmxTimeoutIntervalUs;
} QairtDspPerfPowerConfigV1_t;

/// V1 interface for a power-configuration list. Retrieved via
/// QAIRT_DSP_PERF_POWER_CONFIG_LIST_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtDspPerfPowerConfigList_CreateFn_t create;
  QairtDspPerfPowerConfigList_FreeFn_t free;
  QairtDspPerfPowerConfigList_AddPowerConfigFn_t addPowerConfig;
  QairtDspPerfPowerConfigList_GetNumPowerConfigsFn_t getNumPowerConfigs;
  QairtDspPerfPowerConfigList_GetPowerConfigAtFn_t getPowerConfigAt;
  QairtDspPerfPowerConfigList_ClearFn_t clear;
} QairtDspPerfPowerConfigListV1_t;

/// V1 interface for a memory-configuration slot. Retrieved via QAIRT_DSP_PERF_MEMORY_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtDspPerfMemoryConfig_CreateFn_t create;
  QairtDspPerfMemoryConfig_FreeFn_t free;
  QairtDspPerfMemoryConfig_GetOptionFn_t getOption;
  QairtDspPerfMemoryConfig_SetGrowSizeBytesFn_t setGrowSizeBytes;
  QairtDspPerfMemoryConfig_GetGrowSizeBytesFn_t getGrowSizeBytes;
  QairtDspPerfMemoryConfig_SetVtcmSizeMBFn_t setVtcmSizeMB;
  QairtDspPerfMemoryConfig_GetVtcmSizeMBFn_t getVtcmSizeMB;
  QairtDspPerfMemoryConfig_SetVtcmUsageFactorFn_t setVtcmUsageFactor;
  QairtDspPerfMemoryConfig_GetVtcmUsageFactorFn_t getVtcmUsageFactor;
} QairtDspPerfMemoryConfigV1_t;

/// V1 interface for a memory-configuration list. Retrieved via
/// QAIRT_DSP_PERF_MEMORY_CONFIG_LIST_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtDspPerfMemoryConfigList_CreateFn_t create;
  QairtDspPerfMemoryConfigList_FreeFn_t free;
  QairtDspPerfMemoryConfigList_AddMemoryConfigFn_t addMemoryConfig;
  QairtDspPerfMemoryConfigList_GetNumMemoryConfigsFn_t getNumMemoryConfigs;
  QairtDspPerfMemoryConfigList_GetMemoryConfigAtFn_t getMemoryConfigAt;
  QairtDspPerfMemoryConfigList_ClearFn_t clear;
} QairtDspPerfMemoryConfigListV1_t;

/// V1 interface for a thread-configuration slot. Retrieved via QAIRT_DSP_PERF_THREAD_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtDspPerfThreadConfig_CreateFn_t create;
  QairtDspPerfThreadConfig_FreeFn_t free;
  QairtDspPerfThreadConfig_GetOptionFn_t getOption;
  QairtDspPerfThreadConfig_SetNumHvxThreadsFn_t setNumHvxThreads;
  QairtDspPerfThreadConfig_GetNumHvxThreadsFn_t getNumHvxThreads;
} QairtDspPerfThreadConfigV1_t;

/// V1 interface for a thread-configuration list. Retrieved via
/// QAIRT_DSP_PERF_THREAD_CONFIG_LIST_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtDspPerfThreadConfigList_CreateFn_t create;
  QairtDspPerfThreadConfigList_FreeFn_t free;
  QairtDspPerfThreadConfigList_AddThreadConfigFn_t addThreadConfig;
  QairtDspPerfThreadConfigList_GetNumThreadConfigsFn_t getNumThreadConfigs;
  QairtDspPerfThreadConfigList_GetThreadConfigAtFn_t getThreadConfigAt;
  QairtDspPerfThreadConfigList_ClearFn_t clear;
} QairtDspPerfThreadConfigListV1_t;

/**
 * @struct QairtDspPerfInfrastructureV1_t
 * @brief V1 interface for the QAIRT DSP perf infrastructure. Retrieved via the
 *        id QAIRT_DSP_PERF_INFRASTRUCTURE_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtDspDevice_GetPerfInfrastructureFn_t getPerfInfrastructure;
  QairtDspPerfInfrastructure_FreeFn_t free;
  QairtDspPerfInfrastructure_CreatePowerConfigIdFn_t createPowerConfigId;
  QairtDspPerfInfrastructure_DestroyPowerConfigIdFn_t destroyPowerConfigId;
  QairtDspPerfInfrastructure_SetPowerConfigFn_t setPowerConfig;
  QairtDspPerfInfrastructure_SetMemoryConfigFn_t setMemoryConfig;
  QairtDspPerfInfrastructure_SetThreadConfigFn_t setThreadConfig;
} QairtDspPerfInfrastructureV1_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAIRT_DSP_PERF_INFRASTRUCTURE_H
