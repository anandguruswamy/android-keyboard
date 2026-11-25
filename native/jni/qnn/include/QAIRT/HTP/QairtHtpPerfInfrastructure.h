//==============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

/**
 * @file
 * @brief   HTP Performance Infrastructure component API
 *
 *          Provides an interface to control runtime performance and
 *          power settings (DCVS, voltage corners, sleep latency, RPC polling,
 *          HMX, DDR, CENG, memory grow-size) of the HTP accelerator.
 *
 *          The perf-infrastructure handle is obtained from the opaque device
 *          infrastructure handle returned by
 *          QairtDevice_InfrastructureProvider_getInfrastructure, via
 *          QairtHtpDevice_getPerfInfrastructure. Support is optional and is
 *          advertised through QAIRT_CAPABILITY_DEVICE_SUPPORT_INFRASTRUCTURE.
 */

#ifndef QAIRT_HTP_PERF_INFRASTRUCTURE_H
#define QAIRT_HTP_PERF_INFRASTRUCTURE_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#include "QairtCommon/QairtCommon.h"
#include "QairtDevice/QairtDeviceInfrastructure.h"
#include "QairtHtpDevice.h"

//=============================================================================
// Macros
//=============================================================================

/// Interface IDs for the HTP perf-infrastructure sub-objects and the top-level
/// perf-infrastructure interface.
#define QAIRT_MIN_ID_HTP_PERF (QAIRT_MIN_ID_HTP + 300)

#define QAIRT_HTP_PERF_INFRASTRUCTURE_ID          (QAIRT_MIN_ID_HTP_PERF + 0)
#define QAIRT_HTP_PERF_DCVS_V3_CONFIG_ID          (QAIRT_MIN_ID_HTP_PERF + 1)
#define QAIRT_HTP_PERF_HMX_V2_CONFIG_ID           (QAIRT_MIN_ID_HTP_PERF + 2)
#define QAIRT_HTP_PERF_CENG_CONFIG_ID             (QAIRT_MIN_ID_HTP_PERF + 3)
#define QAIRT_HTP_PERF_DCVS_V3_EXP_CONFIG_ID      (QAIRT_MIN_ID_HTP_PERF + 4)
#define QAIRT_HTP_PERF_CENG_EXP_CONFIG_ID         (QAIRT_MIN_ID_HTP_PERF + 5)
#define QAIRT_HTP_PERF_POWER_CONFIG_ID            (QAIRT_MIN_ID_HTP_PERF + 6)
#define QAIRT_HTP_PERF_POWER_CONFIG_LIST_ID       (QAIRT_MIN_ID_HTP_PERF + 7)
#define QAIRT_HTP_PERF_MEMORY_CONFIG_ID           (QAIRT_MIN_ID_HTP_PERF + 8)
#define QAIRT_HTP_PERF_MEMORY_CONFIG_LIST_ID      (QAIRT_MIN_ID_HTP_PERF + 9)

// Define ranges for HTP perf-infrastructure specific error codes
#define QAIRT_MIN_ERROR_HTP_PERF (QAIRT_MIN_ERROR_HTP + 3000)
#define QAIRT_MAX_ERROR_HTP_PERF (QAIRT_MIN_ERROR_HTP + 3999)

// Maximum RPC polling time allowed, in microseconds.
#define QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIG_MAX_RPC_POLLING_TIME 9999

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Data Types
//=============================================================================

/**
 * @brief Opaque handle to an HTP perf-infrastructure object.
 *
 *        Obtained via QairtHtpDevice_getPerfInfrastructure and released with
 *        QairtHtpPerfInfrastructure_free. The underlying backend infrastructure
 *        it references is backend-owned and is not freed by
 *        QairtHtpPerfInfrastructure_free.
 */
typedef struct _QairtHtpPerfInfrastructure_Handle_t* QairtHtpPerfInfrastructure_Handle_t;

/// Opaque handle to a DcvsV3 configuration value.
typedef struct _QairtHtpPerfDcvsV3ConfigHandle_t* QairtHtpPerfDcvsV3ConfigHandle_t;
/// Opaque handle to an HmxV2 configuration value.
typedef struct _QairtHtpPerfHmxV2ConfigHandle_t* QairtHtpPerfHmxV2ConfigHandle_t;
/// Opaque handle to a CENG configuration value.
typedef struct _QairtHtpPerfCengConfigHandle_t* QairtHtpPerfCengConfigHandle_t;
/// Opaque handle to a DcvsV3 expanded-corner configuration value.
typedef struct _QairtHtpPerfDcvsV3ExpConfigHandle_t* QairtHtpPerfDcvsV3ExpConfigHandle_t;
/// Opaque handle to a CENG expanded-corner configuration value.
typedef struct _QairtHtpPerfCengExpConfigHandle_t* QairtHtpPerfCengExpConfigHandle_t;
/// Opaque handle to a single power-configuration entry (one option at a time).
typedef struct _QairtHtpPerfPowerConfigHandle_t* QairtHtpPerfPowerConfigHandle_t;
/// Opaque handle to an ordered collection of power-configuration entries.
typedef struct _QairtHtpPerfPowerConfigListHandle_t* QairtHtpPerfPowerConfigListHandle_t;
/// Opaque handle to a single memory-configuration entry (one option at a time).
typedef struct _QairtHtpPerfMemoryConfigHandle_t* QairtHtpPerfMemoryConfigHandle_t;
/// Opaque handle to an ordered collection of memory-configuration entries.
typedef struct _QairtHtpPerfMemoryConfigListHandle_t* QairtHtpPerfMemoryConfigListHandle_t;

/**
 * @brief QAIRT HTP PerfInfrastructure API result / error codes.
 */
typedef enum {
  QAIRT_HTP_PERF_INFRASTRUCTURE_MIN_ERROR = QAIRT_MIN_ERROR_HTP_PERF,
  ////////////////////////////////////////

  /// HTP perf-infrastructure operation succeeded.
  QAIRT_HTP_PERF_INFRASTRUCTURE_NO_ERROR = QAIRT_SUCCESS,
  /// A handle argument is not valid.
  QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_INVALID_HANDLE = QAIRT_MIN_ERROR_HTP_PERF + 0,
  /// An argument to the API is invalid.
  QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_INVALID_INPUT = QAIRT_MIN_ERROR_HTP_PERF + 1,
  /// A configuration option is not supported.
  QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_UNSUPPORTED_CONFIG = QAIRT_MIN_ERROR_HTP_PERF + 2,
  /// Transport error communicating with the accelerator.
  QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_TRANSPORT = QAIRT_MIN_ERROR_HTP_PERF + 3,
  /// A get call was made for an option that is not the one currently set.
  QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_OPTION_MISMATCH = QAIRT_MIN_ERROR_HTP_PERF + 4,
  /// Feature or API is not supported on this target.
  QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_UNSUPPORTED = QAIRT_COMMON_ERROR_NOT_SUPPORTED,
  /// General memory allocation failure.
  QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_MEM_ALLOC = QAIRT_COMMON_ERROR_MEM_ALLOC,
  /// General error not covered by a more specific code.
  QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_FAILED = QAIRT_COMMON_ERROR_GENERAL,

  ////////////////////////////////////////
  QAIRT_HTP_PERF_INFRASTRUCTURE_MAX_ERROR = QAIRT_MAX_ERROR_HTP_PERF,
  // Unused, present to ensure 32 bits.
  QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_UNDEFINED = 0x7FFFFFFF
} QairtHtpPerfInfrastructure_Error_t;

/**
 * @brief Perf modes to specify clock frequency level within a target voltage
 *        corner.
 */
typedef enum {
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH     = 0,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_LOW      = 1,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D1  = 2,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D2  = 3,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D3  = 4,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D4  = 5,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D5  = 6,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D6  = 7,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D7  = 8,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D8  = 9,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D9  = 10,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D10 = 11,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D11 = 12,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D12 = 13,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D13 = 14,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D14 = 15,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_HIGH_D15 = 16,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_MAX      = 0xFF,
  QAIRT_HTP_PERF_INFRASTRUCTURE_CLK_PERF_UNKNOWN  = 0x7FFFFFFF
} QairtHtpPerfInfrastructure_ClkPerfMode_t;

/**
 * @brief Voltage corners requestable by the client to influence the DCVS
 *        voting scheme.
 */
typedef enum {
  QAIRT_DCVS_VOLTAGE_CORNER_DISABLE             = 0x10,
  QAIRT_DCVS_VOLTAGE_VCORNER_MIN_VOLTAGE_CORNER = 0x20,
  QAIRT_DCVS_VOLTAGE_VCORNER_SVS2               = 0x30,
  QAIRT_DCVS_VOLTAGE_VCORNER_SVS                = 0x40,
  QAIRT_DCVS_VOLTAGE_VCORNER_SVS_PLUS           = 0x50,
  QAIRT_DCVS_VOLTAGE_VCORNER_NOM                = 0x60,
  QAIRT_DCVS_VOLTAGE_VCORNER_NOM_PLUS           = 0x70,
  QAIRT_DCVS_VOLTAGE_VCORNER_TURBO              = 0x80,
  QAIRT_DCVS_VOLTAGE_VCORNER_TURBO_PLUS         = 0x90,
  QAIRT_DCVS_VOLTAGE_VCORNER_TURBO_L2           = 0x92,
  QAIRT_DCVS_VOLTAGE_VCORNER_TURBO_L3           = 0x93,
  QAIRT_DCVS_VOLTAGE_VCORNER_TURBO_L4           = 0x94,
  QAIRT_DCVS_VOLTAGE_VCORNER_TURBO_L5           = 0x95,
  QAIRT_DCVS_VOLTAGE_VCORNER_MAX_VOLTAGE_CORNER = 0xA0,
  QAIRT_DCVS_VOLTAGE_VCORNER_UNKNOWN            = 0x7FFFFFFF
} QairtHtpPerfInfrastructure_VoltageCorner_t;

/**
 * @brief Expanded voltage corners (2-byte range, 0x100+) for fine-grained
 *        power/perf control.
 */
typedef enum {
  QAIRT_DCVS_EXP_VCORNER_DISABLE            = 0,
  QAIRT_DCVS_EXP_VCORNER_MIN                = 0x100,
  QAIRT_DCVS_EXP_VCORNER_RET                = 0x110,
  QAIRT_DCVS_EXP_VCORNER_MIN_SVS            = 0x130,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D3_0       = 0x131,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D3         = 0x132,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D2_1       = 0x133,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D2         = 0x134,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D1_2       = 0x135,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D1_1       = 0x136,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D1_0       = 0x137,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D1         = 0x138,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D0_1       = 0x13A,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D0_0       = 0x13B,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_D0         = 0x13C,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS            = 0x140,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_P1         = 0x148,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_L0         = 0x14C,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_L1         = 0x150,
  QAIRT_DCVS_EXP_VCORNER_LOW_SVS_L2         = 0x160,
  QAIRT_DCVS_EXP_VCORNER_SVS                = 0x180,
  QAIRT_DCVS_EXP_VCORNER_SVS_L0             = 0x190,
  QAIRT_DCVS_EXP_VCORNER_SVS_L0_1           = 0x191,
  QAIRT_DCVS_EXP_VCORNER_SVS_L0_2           = 0x192,
  QAIRT_DCVS_EXP_VCORNER_SVS_L1             = 0x1C0,
  QAIRT_DCVS_EXP_VCORNER_SVS_L2             = 0x1E0,
  QAIRT_DCVS_EXP_VCORNER_SVS_L2_0           = 0x1E1,
  QAIRT_DCVS_EXP_VCORNER_NOM                = 0x200,
  QAIRT_DCVS_EXP_VCORNER_NOM_L0             = 0x220,
  QAIRT_DCVS_EXP_VCORNER_NOM_L0_0           = 0x221,
  QAIRT_DCVS_EXP_VCORNER_NOM_L1             = 0x240,
  QAIRT_DCVS_EXP_VCORNER_NOM_L2             = 0x250,
  QAIRT_DCVS_EXP_VCORNER_NOM_L3             = 0x260,
  QAIRT_DCVS_EXP_VCORNER_TUR                = 0x280,
  QAIRT_DCVS_EXP_VCORNER_TUR_L0             = 0x290,
  QAIRT_DCVS_EXP_VCORNER_TUR_L1             = 0x2A0,
  QAIRT_DCVS_EXP_VCORNER_TUR_L1_0           = 0x2A1,
  QAIRT_DCVS_EXP_VCORNER_TUR_L1_1           = 0x2A2,
  QAIRT_DCVS_EXP_VCORNER_TUR_L1_2           = 0x2A3,
  QAIRT_DCVS_EXP_VCORNER_TUR_L2             = 0x2B0,
  QAIRT_DCVS_EXP_VCORNER_TUR_L3             = 0x2C0,
  QAIRT_DCVS_EXP_VCORNER_TUR_L3_0           = 0x2C1,
  QAIRT_DCVS_EXP_VCORNER_TUR_L3_1           = 0x2C2,
  QAIRT_DCVS_EXP_VCORNER_TUR_L3_2           = 0x2C3,
  QAIRT_DCVS_EXP_VCORNER_TUR_L4             = 0x2C4,
  QAIRT_DCVS_EXP_VCORNER_TUR_L5             = 0x2C8,
  QAIRT_DCVS_EXP_VCORNER_SUPER_TURBO        = 0x2D0,
  QAIRT_DCVS_EXP_VCORNER_SUPER_TURBO_NO_CPR = 0x2E0,
  QAIRT_DCVS_EXP_VCORNER_DEFINED_MAX        = 0x2E0,
  QAIRT_DCVS_EXP_VCORNER_MAX                = 0xFFFF,
  QAIRT_DCVS_EXP_VCORNER_UNKNOWN            = 0x7FFFFFFF
} QairtHtpPerfInfrastructure_ExpVoltageCorner_t;

/**
 * @brief Power modes to influence DCVS behavior.
 */
typedef enum {
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_ADJUST_UP_DOWN             = 0x1,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_ADJUST_ONLY_UP            = 0x2,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_POWER_SAVER_MODE          = 0x4,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_POWER_SAVER_AGGRESSIVE_MODE = 0x8,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_PERFORMANCE_MODE          = 0x10,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_DUTY_CYCLE_MODE           = 0x20,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWERMODE_UNKNOWN                   = 0x7FFFFFFF
} QairtHtpPerfInfrastructure_PowerMode_t;

/**
 * @brief Power configuration option selecting which value is active on a
 *        QairtHtpPerfPowerConfigHandle_t.
 */
typedef enum {
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_V3               = 1,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_CONTROL_LATENCY   = 2,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_POLLING_TIME      = 3,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_HMX_TIMEOUT_INTERVAL_US = 4,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_HMX_V2                = 5,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_ADAPTIVE_POLLING_TIME = 6,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DDR_PERF_MODE         = 7,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_CENG                  = 8,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_V3_EXP           = 9,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_CENG_EXP              = 10,
  QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_UNKNOWN              = 0x7FFFFFFF
} QairtHtpPerfInfrastructure_PowerConfigOption_t;

/**
 * @brief Memory configuration option selecting which value is active on a
 *        QairtHtpPerfMemoryConfigHandle_t.
 */
typedef enum {
  QAIRT_HTP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_GROW_SIZE = 1,
  QAIRT_HTP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_UNKNOWN   = 0x7FFFFFFF
} QairtHtpPerfInfrastructure_MemoryConfigOption_t;

//=============================================================================
// DcvsV3 Configuration
//=============================================================================
/// Select bus and core operating corners separately.

Qairt_Status_t QairtHtpPerfDcvsV3Config_create(QairtHtpPerfDcvsV3ConfigHandle_t* config);
Qairt_Status_t QairtHtpPerfDcvsV3Config_free(QairtHtpPerfDcvsV3ConfigHandle_t config);

Qairt_Status_t QairtHtpPerfDcvsV3Config_setContextId(QairtHtpPerfDcvsV3ConfigHandle_t config,
                                                     uint32_t contextId);
Qairt_Status_t QairtHtpPerfDcvsV3Config_getContextId(QairtHtpPerfDcvsV3ConfigHandle_t config,
                                                     uint32_t* contextId);

Qairt_Status_t QairtHtpPerfDcvsV3Config_setDcvsEnable(QairtHtpPerfDcvsV3ConfigHandle_t config,
                                                       uint8_t setDcvsEnable,
                                                       uint8_t dcvsEnable);
Qairt_Status_t QairtHtpPerfDcvsV3Config_getDcvsEnable(QairtHtpPerfDcvsV3ConfigHandle_t config,
                                                       uint8_t* setDcvsEnable,
                                                       uint8_t* dcvsEnable);

Qairt_Status_t QairtHtpPerfDcvsV3Config_setPowerMode(
    QairtHtpPerfDcvsV3ConfigHandle_t config, QairtHtpPerfInfrastructure_PowerMode_t powerMode);
Qairt_Status_t QairtHtpPerfDcvsV3Config_getPowerMode(
    QairtHtpPerfDcvsV3ConfigHandle_t config, QairtHtpPerfInfrastructure_PowerMode_t* powerMode);

Qairt_Status_t QairtHtpPerfDcvsV3Config_setSleepLatency(QairtHtpPerfDcvsV3ConfigHandle_t config,
                                                        uint8_t setSleepLatency,
                                                        uint32_t sleepLatencyUs);
Qairt_Status_t QairtHtpPerfDcvsV3Config_getSleepLatency(QairtHtpPerfDcvsV3ConfigHandle_t config,
                                                        uint8_t* setSleepLatency,
                                                        uint32_t* sleepLatencyUs);

Qairt_Status_t QairtHtpPerfDcvsV3Config_setSleepDisable(QairtHtpPerfDcvsV3ConfigHandle_t config,
                                                        uint8_t setSleepDisable,
                                                        uint8_t sleepDisable);
Qairt_Status_t QairtHtpPerfDcvsV3Config_getSleepDisable(QairtHtpPerfDcvsV3ConfigHandle_t config,
                                                        uint8_t* setSleepDisable,
                                                        uint8_t* sleepDisable);

Qairt_Status_t QairtHtpPerfDcvsV3Config_setBusVoltageCorners(
    QairtHtpPerfDcvsV3ConfigHandle_t config,
    uint8_t setBusParams,
    QairtHtpPerfInfrastructure_VoltageCorner_t busVoltageCornerMin,
    QairtHtpPerfInfrastructure_VoltageCorner_t busVoltageCornerTarget,
    QairtHtpPerfInfrastructure_VoltageCorner_t busVoltageCornerMax);
Qairt_Status_t QairtHtpPerfDcvsV3Config_getBusVoltageCorners(
    QairtHtpPerfDcvsV3ConfigHandle_t config,
    uint8_t* setBusParams,
    QairtHtpPerfInfrastructure_VoltageCorner_t* busVoltageCornerMin,
    QairtHtpPerfInfrastructure_VoltageCorner_t* busVoltageCornerTarget,
    QairtHtpPerfInfrastructure_VoltageCorner_t* busVoltageCornerMax);

Qairt_Status_t QairtHtpPerfDcvsV3Config_setCoreVoltageCorners(
    QairtHtpPerfDcvsV3ConfigHandle_t config,
    uint8_t setCoreParams,
    QairtHtpPerfInfrastructure_VoltageCorner_t coreVoltageCornerMin,
    QairtHtpPerfInfrastructure_VoltageCorner_t coreVoltageCornerTarget,
    QairtHtpPerfInfrastructure_VoltageCorner_t coreVoltageCornerMax);
Qairt_Status_t QairtHtpPerfDcvsV3Config_getCoreVoltageCorners(
    QairtHtpPerfDcvsV3ConfigHandle_t config,
    uint8_t* setCoreParams,
    QairtHtpPerfInfrastructure_VoltageCorner_t* coreVoltageCornerMin,
    QairtHtpPerfInfrastructure_VoltageCorner_t* coreVoltageCornerTarget,
    QairtHtpPerfInfrastructure_VoltageCorner_t* coreVoltageCornerMax);

//=============================================================================
// HmxV2 Configuration
//=============================================================================
/// Select HMX corner separately. If hmxPickDefault is non-zero all voltage
/// corner params are ignored.

Qairt_Status_t QairtHtpPerfHmxV2Config_create(QairtHtpPerfHmxV2ConfigHandle_t* config);
Qairt_Status_t QairtHtpPerfHmxV2Config_free(QairtHtpPerfHmxV2ConfigHandle_t config);

Qairt_Status_t QairtHtpPerfHmxV2Config_setPickDefault(QairtHtpPerfHmxV2ConfigHandle_t config,
                                                      uint8_t hmxPickDefault);
Qairt_Status_t QairtHtpPerfHmxV2Config_getPickDefault(QairtHtpPerfHmxV2ConfigHandle_t config,
                                                      uint8_t* hmxPickDefault);

Qairt_Status_t QairtHtpPerfHmxV2Config_setVoltageCorners(
    QairtHtpPerfHmxV2ConfigHandle_t config,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t hmxVoltageCornerMin,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t hmxVoltageCornerTarget,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t hmxVoltageCornerMax);
Qairt_Status_t QairtHtpPerfHmxV2Config_getVoltageCorners(
    QairtHtpPerfHmxV2ConfigHandle_t config,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* hmxVoltageCornerMin,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* hmxVoltageCornerTarget,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* hmxVoltageCornerMax);

Qairt_Status_t QairtHtpPerfHmxV2Config_setPerfMode(
    QairtHtpPerfHmxV2ConfigHandle_t config, QairtHtpPerfInfrastructure_ClkPerfMode_t hmxPerfMode);
Qairt_Status_t QairtHtpPerfHmxV2Config_getPerfMode(
    QairtHtpPerfHmxV2ConfigHandle_t config, QairtHtpPerfInfrastructure_ClkPerfMode_t* hmxPerfMode);

//=============================================================================
// CENG Configuration
//=============================================================================
/// Select CENG voltage corners for frequency voting.

Qairt_Status_t QairtHtpPerfCengConfig_create(QairtHtpPerfCengConfigHandle_t* config);
Qairt_Status_t QairtHtpPerfCengConfig_free(QairtHtpPerfCengConfigHandle_t config);

Qairt_Status_t QairtHtpPerfCengConfig_setVoltageCorners(
    QairtHtpPerfCengConfigHandle_t config,
    QairtHtpPerfInfrastructure_VoltageCorner_t cengVoltageCornerMin,
    QairtHtpPerfInfrastructure_VoltageCorner_t cengVoltageCornerTarget,
    QairtHtpPerfInfrastructure_VoltageCorner_t cengVoltageCornerMax);
Qairt_Status_t QairtHtpPerfCengConfig_getVoltageCorners(
    QairtHtpPerfCengConfigHandle_t config,
    QairtHtpPerfInfrastructure_VoltageCorner_t* cengVoltageCornerMin,
    QairtHtpPerfInfrastructure_VoltageCorner_t* cengVoltageCornerTarget,
    QairtHtpPerfInfrastructure_VoltageCorner_t* cengVoltageCornerMax);

Qairt_Status_t QairtHtpPerfCengConfig_setPerfMode(
    QairtHtpPerfCengConfigHandle_t config, QairtHtpPerfInfrastructure_ClkPerfMode_t cengPerfMode);
Qairt_Status_t QairtHtpPerfCengConfig_getPerfMode(
    QairtHtpPerfCengConfigHandle_t config, QairtHtpPerfInfrastructure_ClkPerfMode_t* cengPerfMode);

//=============================================================================
// DcvsV3 Expanded-Corner Configuration
//=============================================================================
/// Same semantics as DcvsV3, using expanded voltage corners. DCVS is
/// implicitly enabled. Requires CDSP expanded-corner support (V85+).

Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_create(QairtHtpPerfDcvsV3ExpConfigHandle_t* config);
Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_free(QairtHtpPerfDcvsV3ExpConfigHandle_t config);

Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_setContextId(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config, uint32_t contextId);
Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_getContextId(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config, uint32_t* contextId);

Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_setPowerMode(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config,
    QairtHtpPerfInfrastructure_PowerMode_t powerMode);
Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_getPowerMode(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config,
    QairtHtpPerfInfrastructure_PowerMode_t* powerMode);

Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_setSleepLatency(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config, uint8_t setSleepLatency, uint32_t sleepLatencyUs);
Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_getSleepLatency(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config,
    uint8_t* setSleepLatency,
    uint32_t* sleepLatencyUs);

Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_setSleepDisable(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config, uint8_t setSleepDisable, uint8_t sleepDisable);
Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_getSleepDisable(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config, uint8_t* setSleepDisable, uint8_t* sleepDisable);

Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_setBusVoltageCorners(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config,
    uint8_t setBusParams,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t busVoltageCornerMin,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t busVoltageCornerTarget,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t busVoltageCornerMax);
Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_getBusVoltageCorners(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config,
    uint8_t* setBusParams,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* busVoltageCornerMin,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* busVoltageCornerTarget,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* busVoltageCornerMax);

Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_setCoreVoltageCorners(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config,
    uint8_t setCoreParams,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t coreVoltageCornerMin,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t coreVoltageCornerTarget,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t coreVoltageCornerMax);
Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_getCoreVoltageCorners(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config,
    uint8_t* setCoreParams,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* coreVoltageCornerMin,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* coreVoltageCornerTarget,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* coreVoltageCornerMax);

Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_setPerfModes(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config,
    QairtHtpPerfInfrastructure_ClkPerfMode_t corePerfMode,
    QairtHtpPerfInfrastructure_ClkPerfMode_t busPerfMode);
Qairt_Status_t QairtHtpPerfDcvsV3ExpConfig_getPerfModes(
    QairtHtpPerfDcvsV3ExpConfigHandle_t config,
    QairtHtpPerfInfrastructure_ClkPerfMode_t* corePerfMode,
    QairtHtpPerfInfrastructure_ClkPerfMode_t* busPerfMode);

//=============================================================================
// CENG Expanded-Corner Configuration
//=============================================================================

Qairt_Status_t QairtHtpPerfCengExpConfig_create(QairtHtpPerfCengExpConfigHandle_t* config);
Qairt_Status_t QairtHtpPerfCengExpConfig_free(QairtHtpPerfCengExpConfigHandle_t config);

Qairt_Status_t QairtHtpPerfCengExpConfig_setVoltageCorners(
    QairtHtpPerfCengExpConfigHandle_t config,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t cengVoltageCornerMin,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t cengVoltageCornerTarget,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t cengVoltageCornerMax);
Qairt_Status_t QairtHtpPerfCengExpConfig_getVoltageCorners(
    QairtHtpPerfCengExpConfigHandle_t config,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* cengVoltageCornerMin,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* cengVoltageCornerTarget,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t* cengVoltageCornerMax);

Qairt_Status_t QairtHtpPerfCengExpConfig_setPerfMode(
    QairtHtpPerfCengExpConfigHandle_t config,
    QairtHtpPerfInfrastructure_ClkPerfMode_t cengPerfMode);
Qairt_Status_t QairtHtpPerfCengExpConfig_getPerfMode(
    QairtHtpPerfCengExpConfigHandle_t config,
    QairtHtpPerfInfrastructure_ClkPerfMode_t* cengPerfMode);

//=============================================================================
// Power Configuration (one option active at a time)
//=============================================================================

/**
 * @brief Create a power-configuration handle. No option is active until one
 *        of the setters below is called.
 */
Qairt_Status_t QairtHtpPerfPowerConfig_create(QairtHtpPerfPowerConfigHandle_t* config);
Qairt_Status_t QairtHtpPerfPowerConfig_free(QairtHtpPerfPowerConfigHandle_t config);

/// Get which option is currently active on this handle.
Qairt_Status_t QairtHtpPerfPowerConfig_getOption(
    QairtHtpPerfPowerConfigHandle_t config, QairtHtpPerfInfrastructure_PowerConfigOption_t* option);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_V3. Does
/// not take ownership of dcvsV3Config; the caller may free it after this call.
Qairt_Status_t QairtHtpPerfPowerConfig_setDcvsV3(QairtHtpPerfPowerConfigHandle_t config,
                                                 QairtHtpPerfDcvsV3ConfigHandle_t dcvsV3Config);
/// Valid only if getOption reports DCVS_V3. Returns a non-owning handle into
/// config; do not free it. Valid until config is freed or re-set to a
/// different option.
Qairt_Status_t QairtHtpPerfPowerConfig_getDcvsV3(QairtHtpPerfPowerConfigHandle_t config,
                                                 QairtHtpPerfDcvsV3ConfigHandle_t* dcvsV3Config);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_HMX_V2.
Qairt_Status_t QairtHtpPerfPowerConfig_setHmxV2(QairtHtpPerfPowerConfigHandle_t config,
                                                QairtHtpPerfHmxV2ConfigHandle_t hmxV2Config);
/// Valid only if getOption reports HMX_V2. Returns a non-owning handle into
/// config; do not free it. Valid until config is freed or re-set to a
/// different option.
Qairt_Status_t QairtHtpPerfPowerConfig_getHmxV2(QairtHtpPerfPowerConfigHandle_t config,
                                                QairtHtpPerfHmxV2ConfigHandle_t* hmxV2Config);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_CENG.
Qairt_Status_t QairtHtpPerfPowerConfig_setCeng(QairtHtpPerfPowerConfigHandle_t config,
                                               QairtHtpPerfCengConfigHandle_t cengConfig);
/// Valid only if getOption reports CENG. Returns a non-owning handle into
/// config; do not free it. Valid until config is freed or re-set to a
/// different option.
Qairt_Status_t QairtHtpPerfPowerConfig_getCeng(QairtHtpPerfPowerConfigHandle_t config,
                                               QairtHtpPerfCengConfigHandle_t* cengConfig);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DCVS_V3_EXP.
Qairt_Status_t QairtHtpPerfPowerConfig_setDcvsV3Exp(
    QairtHtpPerfPowerConfigHandle_t config, QairtHtpPerfDcvsV3ExpConfigHandle_t dcvsV3ExpConfig);
/// Valid only if getOption reports DCVS_V3_EXP. Returns a non-owning handle
/// into config; do not free it. Valid until config is freed or re-set to a
/// different option.
Qairt_Status_t QairtHtpPerfPowerConfig_getDcvsV3Exp(
    QairtHtpPerfPowerConfigHandle_t config, QairtHtpPerfDcvsV3ExpConfigHandle_t* dcvsV3ExpConfig);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_CENG_EXP.
Qairt_Status_t QairtHtpPerfPowerConfig_setCengExp(QairtHtpPerfPowerConfigHandle_t config,
                                                  QairtHtpPerfCengExpConfigHandle_t cengExpConfig);
/// Valid only if getOption reports CENG_EXP. Returns a non-owning handle
/// into config; do not free it. Valid until config is freed or re-set to a
/// different option.
Qairt_Status_t QairtHtpPerfPowerConfig_getCengExp(
    QairtHtpPerfPowerConfigHandle_t config, QairtHtpPerfCengExpConfigHandle_t* cengExpConfig);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_CONTROL_LATENCY.
Qairt_Status_t QairtHtpPerfPowerConfig_setRpcControlLatency(
    QairtHtpPerfPowerConfigHandle_t config, uint32_t rpcControlLatencyUs);
Qairt_Status_t QairtHtpPerfPowerConfig_getRpcControlLatency(
    QairtHtpPerfPowerConfigHandle_t config, uint32_t* rpcControlLatencyUs);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_RPC_POLLING_TIME.
Qairt_Status_t QairtHtpPerfPowerConfig_setRpcPollingTime(QairtHtpPerfPowerConfigHandle_t config,
                                                         uint32_t rpcPollingTimeUs);
Qairt_Status_t QairtHtpPerfPowerConfig_getRpcPollingTime(QairtHtpPerfPowerConfigHandle_t config,
                                                         uint32_t* rpcPollingTimeUs);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_HMX_TIMEOUT_INTERVAL_US.
Qairt_Status_t QairtHtpPerfPowerConfig_setHmxTimeoutIntervalUs(
    QairtHtpPerfPowerConfigHandle_t config, uint32_t hmxTimeoutIntervalUs);
Qairt_Status_t QairtHtpPerfPowerConfig_getHmxTimeoutIntervalUs(
    QairtHtpPerfPowerConfigHandle_t config, uint32_t* hmxTimeoutIntervalUs);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_ADAPTIVE_POLLING_TIME.
Qairt_Status_t QairtHtpPerfPowerConfig_setAdaptivePollingTime(
    QairtHtpPerfPowerConfigHandle_t config, uint32_t adaptivePollingTimeUs);
Qairt_Status_t QairtHtpPerfPowerConfig_getAdaptivePollingTime(
    QairtHtpPerfPowerConfigHandle_t config, uint32_t* adaptivePollingTimeUs);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_POWER_CONFIGOPTION_DDR_PERF_MODE.
Qairt_Status_t QairtHtpPerfPowerConfig_setDdrPerfMode(QairtHtpPerfPowerConfigHandle_t config,
                                                      uint8_t ddrPerfModeEnable);
Qairt_Status_t QairtHtpPerfPowerConfig_getDdrPerfMode(QairtHtpPerfPowerConfigHandle_t config,
                                                      uint8_t* ddrPerfModeEnable);

//=============================================================================
// Power Configuration List
//=============================================================================

/// Create an empty, ordered collection of power-configuration entries.
Qairt_Status_t QairtHtpPerfPowerConfigList_create(QairtHtpPerfPowerConfigListHandle_t* list);
Qairt_Status_t QairtHtpPerfPowerConfigList_free(QairtHtpPerfPowerConfigListHandle_t list);

/// Append a copy of powerConfig's current value to the list. The caller
/// retains ownership of powerConfig and may free or reuse it afterward.
Qairt_Status_t QairtHtpPerfPowerConfigList_addPowerConfig(
    QairtHtpPerfPowerConfigListHandle_t list, QairtHtpPerfPowerConfigHandle_t powerConfig);

Qairt_Status_t QairtHtpPerfPowerConfigList_getNumPowerConfigs(
    QairtHtpPerfPowerConfigListHandle_t list, uint32_t* numPowerConfigs);

/// Get a non-owning handle to the entry at index; do not free it. Valid
/// until list is freed or cleared.
Qairt_Status_t QairtHtpPerfPowerConfigList_getPowerConfigAt(
    QairtHtpPerfPowerConfigListHandle_t list,
    uint32_t index,
    QairtHtpPerfPowerConfigHandle_t* powerConfig);

Qairt_Status_t QairtHtpPerfPowerConfigList_clear(QairtHtpPerfPowerConfigListHandle_t list);

//=============================================================================
// Memory Configuration (one option active at a time)
//=============================================================================

Qairt_Status_t QairtHtpPerfMemoryConfig_create(QairtHtpPerfMemoryConfigHandle_t* config);
Qairt_Status_t QairtHtpPerfMemoryConfig_free(QairtHtpPerfMemoryConfigHandle_t config);

Qairt_Status_t QairtHtpPerfMemoryConfig_getOption(
    QairtHtpPerfMemoryConfigHandle_t config,
    QairtHtpPerfInfrastructure_MemoryConfigOption_t* option);

/// Sets option QAIRT_HTP_PERF_INFRASTRUCTURE_MEMORY_CONFIGOPTION_GROW_SIZE.
Qairt_Status_t QairtHtpPerfMemoryConfig_setGrowSize(QairtHtpPerfMemoryConfigHandle_t config,
                                                    uint32_t growSizeBytes);
Qairt_Status_t QairtHtpPerfMemoryConfig_getGrowSize(QairtHtpPerfMemoryConfigHandle_t config,
                                                    uint32_t* growSizeBytes);

//=============================================================================
// Memory Configuration List
//=============================================================================

Qairt_Status_t QairtHtpPerfMemoryConfigList_create(QairtHtpPerfMemoryConfigListHandle_t* list);
Qairt_Status_t QairtHtpPerfMemoryConfigList_free(QairtHtpPerfMemoryConfigListHandle_t list);

Qairt_Status_t QairtHtpPerfMemoryConfigList_addMemoryConfig(
    QairtHtpPerfMemoryConfigListHandle_t list, QairtHtpPerfMemoryConfigHandle_t memoryConfig);

Qairt_Status_t QairtHtpPerfMemoryConfigList_getNumMemoryConfigs(
    QairtHtpPerfMemoryConfigListHandle_t list, uint32_t* numMemoryConfigs);

/// Get a non-owning handle to the entry at index; do not free it. Valid
/// until list is freed or cleared.
Qairt_Status_t QairtHtpPerfMemoryConfigList_getMemoryConfigAt(
    QairtHtpPerfMemoryConfigListHandle_t list,
    uint32_t index,
    QairtHtpPerfMemoryConfigHandle_t* memoryConfig);

Qairt_Status_t QairtHtpPerfMemoryConfigList_clear(QairtHtpPerfMemoryConfigListHandle_t list);

//=============================================================================
// Perf Infrastructure
//=============================================================================

/**
 * @brief Resolve an HTP perf-infrastructure handle from a device infrastructure
 *        handle.
 *
 * @param[in]  deviceInfraHandle Device infrastructure handle obtained from
 *                               QairtDevice_InfrastructureProvider_getInfrastructure.
 * @param[out] perfInfraHandle   Pointer to receive the perf-infrastructure handle.
 *                               Release with QairtHtpPerfInfrastructure_free.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: handle resolved.
 *         - QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_INVALID_INPUT: perfInfraHandle is NULL.
 *         - QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_INVALID_HANDLE: deviceInfraHandle invalid.
 *         - QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_UNSUPPORTED: backend does not
 *           expose a perf infrastructure.
 *         - QAIRT_HTP_PERF_INFRASTRUCTURE_ERROR_MEM_ALLOC: allocation failed.
 */
Qairt_Status_t QairtHtpDevice_getPerfInfrastructure(
    QairtDevice_InfrastructureHandle_t deviceInfraHandle,
    QairtHtpPerfInfrastructure_Handle_t* perfInfraHandle);

/**
 * @brief Free an HTP perf-infrastructure handle. Does not free the backend-owned
 *        infrastructure it references.
 */
Qairt_Status_t QairtHtpPerfInfrastructure_free(
    QairtHtpPerfInfrastructure_Handle_t perfInfraHandle);

/**
 * @brief Create a power configuration id, used to set performance modes.
 *        Destroy with QairtHtpPerfInfrastructure_destroyPowerConfigId.
 */
Qairt_Status_t QairtHtpPerfInfrastructure_createPowerConfigId(
    QairtHtpPerfInfrastructure_Handle_t perfInfraHandle,
    uint32_t deviceId,
    uint32_t coreId,
    uint32_t* powerConfigId);

/**
 * @brief Destroy a power configuration id.
 */
Qairt_Status_t QairtHtpPerfInfrastructure_destroyPowerConfigId(
    QairtHtpPerfInfrastructure_Handle_t perfInfraHandle, uint32_t powerConfigId);

/**
 * @brief Set system power configuration to enable different performance modes.
 *
 * @param[in] perfInfraHandle Perf-infrastructure handle.
 * @param[in] powerConfigId   Power config id from createPowerConfigId. A value of
 *                            0 implies a NULL power client id.
 * @param[in] configList      Handle to a list of power-configuration entries.
 *                            NULL indicates no config options.
 */
Qairt_Status_t QairtHtpPerfInfrastructure_setPowerConfig(
    QairtHtpPerfInfrastructure_Handle_t perfInfraHandle,
    uint32_t powerConfigId,
    QairtHtpPerfPowerConfigListHandle_t configList);

/**
 * @brief Set system memory configuration on a specific device/core.
 *
 * @param[in] perfInfraHandle Perf-infrastructure handle.
 * @param[in] deviceId        Hardware device on which the config applies.
 * @param[in] coreId          Core/NSP on which the config applies.
 * @param[in] configList      Handle to a list of memory-configuration entries.
 *                            NULL indicates no config options.
 */
Qairt_Status_t QairtHtpPerfInfrastructure_setMemoryConfig(
    QairtHtpPerfInfrastructure_Handle_t perfInfraHandle,
    uint32_t deviceId,
    uint32_t coreId,
    QairtHtpPerfMemoryConfigListHandle_t configList);

//=============================================================================
// Function Pointer Typedefs
//=============================================================================

/* DcvsV3 config */
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_CreateFn_t)(QairtHtpPerfDcvsV3ConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_FreeFn_t)(QairtHtpPerfDcvsV3ConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_SetContextIdFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_GetContextIdFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_SetDcvsEnableFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t, uint8_t, uint8_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_GetDcvsEnableFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t, uint8_t*, uint8_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_SetPowerModeFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t, QairtHtpPerfInfrastructure_PowerMode_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_GetPowerModeFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t, QairtHtpPerfInfrastructure_PowerMode_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_SetSleepLatencyFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t, uint8_t, uint32_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_GetSleepLatencyFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t, uint8_t*, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_SetSleepDisableFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t, uint8_t, uint8_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_GetSleepDisableFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t, uint8_t*, uint8_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_SetBusVoltageCornersFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t,
    uint8_t,
    QairtHtpPerfInfrastructure_VoltageCorner_t,
    QairtHtpPerfInfrastructure_VoltageCorner_t,
    QairtHtpPerfInfrastructure_VoltageCorner_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_GetBusVoltageCornersFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t,
    uint8_t*,
    QairtHtpPerfInfrastructure_VoltageCorner_t*,
    QairtHtpPerfInfrastructure_VoltageCorner_t*,
    QairtHtpPerfInfrastructure_VoltageCorner_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_SetCoreVoltageCornersFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t,
    uint8_t,
    QairtHtpPerfInfrastructure_VoltageCorner_t,
    QairtHtpPerfInfrastructure_VoltageCorner_t,
    QairtHtpPerfInfrastructure_VoltageCorner_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3Config_GetCoreVoltageCornersFn_t)(
    QairtHtpPerfDcvsV3ConfigHandle_t,
    uint8_t*,
    QairtHtpPerfInfrastructure_VoltageCorner_t*,
    QairtHtpPerfInfrastructure_VoltageCorner_t*,
    QairtHtpPerfInfrastructure_VoltageCorner_t*);

/* HmxV2 config */
typedef Qairt_Status_t (*QairtHtpPerfHmxV2Config_CreateFn_t)(QairtHtpPerfHmxV2ConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfHmxV2Config_FreeFn_t)(QairtHtpPerfHmxV2ConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfHmxV2Config_SetPickDefaultFn_t)(
    QairtHtpPerfHmxV2ConfigHandle_t, uint8_t);
typedef Qairt_Status_t (*QairtHtpPerfHmxV2Config_GetPickDefaultFn_t)(
    QairtHtpPerfHmxV2ConfigHandle_t, uint8_t*);
typedef Qairt_Status_t (*QairtHtpPerfHmxV2Config_SetVoltageCornersFn_t)(
    QairtHtpPerfHmxV2ConfigHandle_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t);
typedef Qairt_Status_t (*QairtHtpPerfHmxV2Config_GetVoltageCornersFn_t)(
    QairtHtpPerfHmxV2ConfigHandle_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*);
typedef Qairt_Status_t (*QairtHtpPerfHmxV2Config_SetPerfModeFn_t)(
    QairtHtpPerfHmxV2ConfigHandle_t, QairtHtpPerfInfrastructure_ClkPerfMode_t);
typedef Qairt_Status_t (*QairtHtpPerfHmxV2Config_GetPerfModeFn_t)(
    QairtHtpPerfHmxV2ConfigHandle_t, QairtHtpPerfInfrastructure_ClkPerfMode_t*);

/* CENG config */
typedef Qairt_Status_t (*QairtHtpPerfCengConfig_CreateFn_t)(QairtHtpPerfCengConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfCengConfig_FreeFn_t)(QairtHtpPerfCengConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfCengConfig_SetVoltageCornersFn_t)(
    QairtHtpPerfCengConfigHandle_t,
    QairtHtpPerfInfrastructure_VoltageCorner_t,
    QairtHtpPerfInfrastructure_VoltageCorner_t,
    QairtHtpPerfInfrastructure_VoltageCorner_t);
typedef Qairt_Status_t (*QairtHtpPerfCengConfig_GetVoltageCornersFn_t)(
    QairtHtpPerfCengConfigHandle_t,
    QairtHtpPerfInfrastructure_VoltageCorner_t*,
    QairtHtpPerfInfrastructure_VoltageCorner_t*,
    QairtHtpPerfInfrastructure_VoltageCorner_t*);
typedef Qairt_Status_t (*QairtHtpPerfCengConfig_SetPerfModeFn_t)(
    QairtHtpPerfCengConfigHandle_t, QairtHtpPerfInfrastructure_ClkPerfMode_t);
typedef Qairt_Status_t (*QairtHtpPerfCengConfig_GetPerfModeFn_t)(
    QairtHtpPerfCengConfigHandle_t, QairtHtpPerfInfrastructure_ClkPerfMode_t*);

/* DcvsV3Exp config */
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_CreateFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_FreeFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_SetContextIdFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_GetContextIdFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_SetPowerModeFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t, QairtHtpPerfInfrastructure_PowerMode_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_GetPowerModeFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t, QairtHtpPerfInfrastructure_PowerMode_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_SetSleepLatencyFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t, uint8_t, uint32_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_GetSleepLatencyFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t, uint8_t*, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_SetSleepDisableFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t, uint8_t, uint8_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_GetSleepDisableFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t, uint8_t*, uint8_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_SetBusVoltageCornersFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t,
    uint8_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_GetBusVoltageCornersFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t,
    uint8_t*,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_SetCoreVoltageCornersFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t,
    uint8_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_GetCoreVoltageCornersFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t,
    uint8_t*,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_SetPerfModesFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t,
    QairtHtpPerfInfrastructure_ClkPerfMode_t,
    QairtHtpPerfInfrastructure_ClkPerfMode_t);
typedef Qairt_Status_t (*QairtHtpPerfDcvsV3ExpConfig_GetPerfModesFn_t)(
    QairtHtpPerfDcvsV3ExpConfigHandle_t,
    QairtHtpPerfInfrastructure_ClkPerfMode_t*,
    QairtHtpPerfInfrastructure_ClkPerfMode_t*);

/* CENG Exp config */
typedef Qairt_Status_t (*QairtHtpPerfCengExpConfig_CreateFn_t)(
    QairtHtpPerfCengExpConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfCengExpConfig_FreeFn_t)(QairtHtpPerfCengExpConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfCengExpConfig_SetVoltageCornersFn_t)(
    QairtHtpPerfCengExpConfigHandle_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t);
typedef Qairt_Status_t (*QairtHtpPerfCengExpConfig_GetVoltageCornersFn_t)(
    QairtHtpPerfCengExpConfigHandle_t,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*,
    QairtHtpPerfInfrastructure_ExpVoltageCorner_t*);
typedef Qairt_Status_t (*QairtHtpPerfCengExpConfig_SetPerfModeFn_t)(
    QairtHtpPerfCengExpConfigHandle_t, QairtHtpPerfInfrastructure_ClkPerfMode_t);
typedef Qairt_Status_t (*QairtHtpPerfCengExpConfig_GetPerfModeFn_t)(
    QairtHtpPerfCengExpConfigHandle_t, QairtHtpPerfInfrastructure_ClkPerfMode_t*);

/* Power config (tagged slot) */
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_CreateFn_t)(QairtHtpPerfPowerConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_FreeFn_t)(QairtHtpPerfPowerConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetOptionFn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfInfrastructure_PowerConfigOption_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_SetDcvsV3Fn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfDcvsV3ConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetDcvsV3Fn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfDcvsV3ConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_SetHmxV2Fn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfHmxV2ConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetHmxV2Fn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfHmxV2ConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_SetCengFn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfCengConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetCengFn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfCengConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_SetDcvsV3ExpFn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfDcvsV3ExpConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetDcvsV3ExpFn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfDcvsV3ExpConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_SetCengExpFn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfCengExpConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetCengExpFn_t)(
    QairtHtpPerfPowerConfigHandle_t, QairtHtpPerfCengExpConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_SetRpcControlLatencyFn_t)(
    QairtHtpPerfPowerConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetRpcControlLatencyFn_t)(
    QairtHtpPerfPowerConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_SetRpcPollingTimeFn_t)(
    QairtHtpPerfPowerConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetRpcPollingTimeFn_t)(
    QairtHtpPerfPowerConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_SetHmxTimeoutIntervalUsFn_t)(
    QairtHtpPerfPowerConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetHmxTimeoutIntervalUsFn_t)(
    QairtHtpPerfPowerConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_SetAdaptivePollingTimeFn_t)(
    QairtHtpPerfPowerConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetAdaptivePollingTimeFn_t)(
    QairtHtpPerfPowerConfigHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_SetDdrPerfModeFn_t)(
    QairtHtpPerfPowerConfigHandle_t, uint8_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfig_GetDdrPerfModeFn_t)(
    QairtHtpPerfPowerConfigHandle_t, uint8_t*);

/* Power config list */
typedef Qairt_Status_t (*QairtHtpPerfPowerConfigList_CreateFn_t)(
    QairtHtpPerfPowerConfigListHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfigList_FreeFn_t)(
    QairtHtpPerfPowerConfigListHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfigList_AddPowerConfigFn_t)(
    QairtHtpPerfPowerConfigListHandle_t, QairtHtpPerfPowerConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfigList_GetNumPowerConfigsFn_t)(
    QairtHtpPerfPowerConfigListHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfigList_GetPowerConfigAtFn_t)(
    QairtHtpPerfPowerConfigListHandle_t, uint32_t, QairtHtpPerfPowerConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfPowerConfigList_ClearFn_t)(
    QairtHtpPerfPowerConfigListHandle_t);

/* Memory config (tagged slot) */
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfig_CreateFn_t)(QairtHtpPerfMemoryConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfig_FreeFn_t)(QairtHtpPerfMemoryConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfig_GetOptionFn_t)(
    QairtHtpPerfMemoryConfigHandle_t, QairtHtpPerfInfrastructure_MemoryConfigOption_t*);
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfig_SetGrowSizeFn_t)(
    QairtHtpPerfMemoryConfigHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfig_GetGrowSizeFn_t)(
    QairtHtpPerfMemoryConfigHandle_t, uint32_t*);

/* Memory config list */
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfigList_CreateFn_t)(
    QairtHtpPerfMemoryConfigListHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfigList_FreeFn_t)(
    QairtHtpPerfMemoryConfigListHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfigList_AddMemoryConfigFn_t)(
    QairtHtpPerfMemoryConfigListHandle_t, QairtHtpPerfMemoryConfigHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfigList_GetNumMemoryConfigsFn_t)(
    QairtHtpPerfMemoryConfigListHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfigList_GetMemoryConfigAtFn_t)(
    QairtHtpPerfMemoryConfigListHandle_t, uint32_t, QairtHtpPerfMemoryConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpPerfMemoryConfigList_ClearFn_t)(
    QairtHtpPerfMemoryConfigListHandle_t);

/* Perf infrastructure */
typedef Qairt_Status_t (*QairtHtpDevice_GetPerfInfrastructureFn_t)(
    QairtDevice_InfrastructureHandle_t, QairtHtpPerfInfrastructure_Handle_t*);
typedef Qairt_Status_t (*QairtHtpPerfInfrastructure_FreeFn_t)(
    QairtHtpPerfInfrastructure_Handle_t);
typedef Qairt_Status_t (*QairtHtpPerfInfrastructure_CreatePowerConfigIdFn_t)(
    QairtHtpPerfInfrastructure_Handle_t, uint32_t, uint32_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpPerfInfrastructure_DestroyPowerConfigIdFn_t)(
    QairtHtpPerfInfrastructure_Handle_t, uint32_t);
typedef Qairt_Status_t (*QairtHtpPerfInfrastructure_SetPowerConfigFn_t)(
    QairtHtpPerfInfrastructure_Handle_t, uint32_t, QairtHtpPerfPowerConfigListHandle_t);
typedef Qairt_Status_t (*QairtHtpPerfInfrastructure_SetMemoryConfigFn_t)(
    QairtHtpPerfInfrastructure_Handle_t, uint32_t, uint32_t, QairtHtpPerfMemoryConfigListHandle_t);

//=============================================================================
// Public Interfaces
//=============================================================================

/// V1 interface for a DcvsV3 configuration. Retrieved via QAIRT_HTP_PERF_DCVS_V3_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpPerfDcvsV3Config_CreateFn_t create;
  QairtHtpPerfDcvsV3Config_FreeFn_t free;
  QairtHtpPerfDcvsV3Config_SetContextIdFn_t setContextId;
  QairtHtpPerfDcvsV3Config_GetContextIdFn_t getContextId;
  QairtHtpPerfDcvsV3Config_SetDcvsEnableFn_t setDcvsEnable;
  QairtHtpPerfDcvsV3Config_GetDcvsEnableFn_t getDcvsEnable;
  QairtHtpPerfDcvsV3Config_SetPowerModeFn_t setPowerMode;
  QairtHtpPerfDcvsV3Config_GetPowerModeFn_t getPowerMode;
  QairtHtpPerfDcvsV3Config_SetSleepLatencyFn_t setSleepLatency;
  QairtHtpPerfDcvsV3Config_GetSleepLatencyFn_t getSleepLatency;
  QairtHtpPerfDcvsV3Config_SetSleepDisableFn_t setSleepDisable;
  QairtHtpPerfDcvsV3Config_GetSleepDisableFn_t getSleepDisable;
  QairtHtpPerfDcvsV3Config_SetBusVoltageCornersFn_t setBusVoltageCorners;
  QairtHtpPerfDcvsV3Config_GetBusVoltageCornersFn_t getBusVoltageCorners;
  QairtHtpPerfDcvsV3Config_SetCoreVoltageCornersFn_t setCoreVoltageCorners;
  QairtHtpPerfDcvsV3Config_GetCoreVoltageCornersFn_t getCoreVoltageCorners;
} QairtHtpPerfDcvsV3ConfigV1_t;

/// V1 interface for an HmxV2 configuration. Retrieved via QAIRT_HTP_PERF_HMX_V2_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpPerfHmxV2Config_CreateFn_t create;
  QairtHtpPerfHmxV2Config_FreeFn_t free;
  QairtHtpPerfHmxV2Config_SetPickDefaultFn_t setPickDefault;
  QairtHtpPerfHmxV2Config_GetPickDefaultFn_t getPickDefault;
  QairtHtpPerfHmxV2Config_SetVoltageCornersFn_t setVoltageCorners;
  QairtHtpPerfHmxV2Config_GetVoltageCornersFn_t getVoltageCorners;
  QairtHtpPerfHmxV2Config_SetPerfModeFn_t setPerfMode;
  QairtHtpPerfHmxV2Config_GetPerfModeFn_t getPerfMode;
} QairtHtpPerfHmxV2ConfigV1_t;

/// V1 interface for a CENG configuration. Retrieved via QAIRT_HTP_PERF_CENG_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpPerfCengConfig_CreateFn_t create;
  QairtHtpPerfCengConfig_FreeFn_t free;
  QairtHtpPerfCengConfig_SetVoltageCornersFn_t setVoltageCorners;
  QairtHtpPerfCengConfig_GetVoltageCornersFn_t getVoltageCorners;
  QairtHtpPerfCengConfig_SetPerfModeFn_t setPerfMode;
  QairtHtpPerfCengConfig_GetPerfModeFn_t getPerfMode;
} QairtHtpPerfCengConfigV1_t;

/// V1 interface for a DcvsV3Exp configuration. Retrieved via QAIRT_HTP_PERF_DCVS_V3_EXP_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpPerfDcvsV3ExpConfig_CreateFn_t create;
  QairtHtpPerfDcvsV3ExpConfig_FreeFn_t free;
  QairtHtpPerfDcvsV3ExpConfig_SetContextIdFn_t setContextId;
  QairtHtpPerfDcvsV3ExpConfig_GetContextIdFn_t getContextId;
  QairtHtpPerfDcvsV3ExpConfig_SetPowerModeFn_t setPowerMode;
  QairtHtpPerfDcvsV3ExpConfig_GetPowerModeFn_t getPowerMode;
  QairtHtpPerfDcvsV3ExpConfig_SetSleepLatencyFn_t setSleepLatency;
  QairtHtpPerfDcvsV3ExpConfig_GetSleepLatencyFn_t getSleepLatency;
  QairtHtpPerfDcvsV3ExpConfig_SetSleepDisableFn_t setSleepDisable;
  QairtHtpPerfDcvsV3ExpConfig_GetSleepDisableFn_t getSleepDisable;
  QairtHtpPerfDcvsV3ExpConfig_SetBusVoltageCornersFn_t setBusVoltageCorners;
  QairtHtpPerfDcvsV3ExpConfig_GetBusVoltageCornersFn_t getBusVoltageCorners;
  QairtHtpPerfDcvsV3ExpConfig_SetCoreVoltageCornersFn_t setCoreVoltageCorners;
  QairtHtpPerfDcvsV3ExpConfig_GetCoreVoltageCornersFn_t getCoreVoltageCorners;
  QairtHtpPerfDcvsV3ExpConfig_SetPerfModesFn_t setPerfModes;
  QairtHtpPerfDcvsV3ExpConfig_GetPerfModesFn_t getPerfModes;
} QairtHtpPerfDcvsV3ExpConfigV1_t;

/// V1 interface for a CENG expanded-corner configuration. Retrieved via
/// QAIRT_HTP_PERF_CENG_EXP_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpPerfCengExpConfig_CreateFn_t create;
  QairtHtpPerfCengExpConfig_FreeFn_t free;
  QairtHtpPerfCengExpConfig_SetVoltageCornersFn_t setVoltageCorners;
  QairtHtpPerfCengExpConfig_GetVoltageCornersFn_t getVoltageCorners;
  QairtHtpPerfCengExpConfig_SetPerfModeFn_t setPerfMode;
  QairtHtpPerfCengExpConfig_GetPerfModeFn_t getPerfMode;
} QairtHtpPerfCengExpConfigV1_t;

/// V1 interface for a power-configuration slot. Retrieved via QAIRT_HTP_PERF_POWER_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpPerfPowerConfig_CreateFn_t create;
  QairtHtpPerfPowerConfig_FreeFn_t free;
  QairtHtpPerfPowerConfig_GetOptionFn_t getOption;
  QairtHtpPerfPowerConfig_SetDcvsV3Fn_t setDcvsV3;
  QairtHtpPerfPowerConfig_GetDcvsV3Fn_t getDcvsV3;
  QairtHtpPerfPowerConfig_SetHmxV2Fn_t setHmxV2;
  QairtHtpPerfPowerConfig_GetHmxV2Fn_t getHmxV2;
  QairtHtpPerfPowerConfig_SetCengFn_t setCeng;
  QairtHtpPerfPowerConfig_GetCengFn_t getCeng;
  QairtHtpPerfPowerConfig_SetDcvsV3ExpFn_t setDcvsV3Exp;
  QairtHtpPerfPowerConfig_GetDcvsV3ExpFn_t getDcvsV3Exp;
  QairtHtpPerfPowerConfig_SetCengExpFn_t setCengExp;
  QairtHtpPerfPowerConfig_GetCengExpFn_t getCengExp;
  QairtHtpPerfPowerConfig_SetRpcControlLatencyFn_t setRpcControlLatency;
  QairtHtpPerfPowerConfig_GetRpcControlLatencyFn_t getRpcControlLatency;
  QairtHtpPerfPowerConfig_SetRpcPollingTimeFn_t setRpcPollingTime;
  QairtHtpPerfPowerConfig_GetRpcPollingTimeFn_t getRpcPollingTime;
  QairtHtpPerfPowerConfig_SetHmxTimeoutIntervalUsFn_t setHmxTimeoutIntervalUs;
  QairtHtpPerfPowerConfig_GetHmxTimeoutIntervalUsFn_t getHmxTimeoutIntervalUs;
  QairtHtpPerfPowerConfig_SetAdaptivePollingTimeFn_t setAdaptivePollingTime;
  QairtHtpPerfPowerConfig_GetAdaptivePollingTimeFn_t getAdaptivePollingTime;
  QairtHtpPerfPowerConfig_SetDdrPerfModeFn_t setDdrPerfMode;
  QairtHtpPerfPowerConfig_GetDdrPerfModeFn_t getDdrPerfMode;
} QairtHtpPerfPowerConfigV1_t;

/// V1 interface for a power-configuration list. Retrieved via QAIRT_HTP_PERF_POWER_CONFIG_LIST_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpPerfPowerConfigList_CreateFn_t create;
  QairtHtpPerfPowerConfigList_FreeFn_t free;
  QairtHtpPerfPowerConfigList_AddPowerConfigFn_t addPowerConfig;
  QairtHtpPerfPowerConfigList_GetNumPowerConfigsFn_t getNumPowerConfigs;
  QairtHtpPerfPowerConfigList_GetPowerConfigAtFn_t getPowerConfigAt;
  QairtHtpPerfPowerConfigList_ClearFn_t clear;
} QairtHtpPerfPowerConfigListV1_t;

/// V1 interface for a memory-configuration slot. Retrieved via QAIRT_HTP_PERF_MEMORY_CONFIG_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpPerfMemoryConfig_CreateFn_t create;
  QairtHtpPerfMemoryConfig_FreeFn_t free;
  QairtHtpPerfMemoryConfig_GetOptionFn_t getOption;
  QairtHtpPerfMemoryConfig_SetGrowSizeFn_t setGrowSize;
  QairtHtpPerfMemoryConfig_GetGrowSizeFn_t getGrowSize;
} QairtHtpPerfMemoryConfigV1_t;

/// V1 interface for a memory-configuration list. Retrieved via QAIRT_HTP_PERF_MEMORY_CONFIG_LIST_ID.
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpPerfMemoryConfigList_CreateFn_t create;
  QairtHtpPerfMemoryConfigList_FreeFn_t free;
  QairtHtpPerfMemoryConfigList_AddMemoryConfigFn_t addMemoryConfig;
  QairtHtpPerfMemoryConfigList_GetNumMemoryConfigsFn_t getNumMemoryConfigs;
  QairtHtpPerfMemoryConfigList_GetMemoryConfigAtFn_t getMemoryConfigAt;
  QairtHtpPerfMemoryConfigList_ClearFn_t clear;
} QairtHtpPerfMemoryConfigListV1_t;

/**
 * @struct QairtHtpPerfInfrastructureV1_t
 * @brief V1 interface for the QAIRT HTP perf infrastructure. Retrieved via the
 *        id QAIRT_HTP_PERF_INFRASTRUCTURE_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpDevice_GetPerfInfrastructureFn_t getPerfInfrastructure;
  QairtHtpPerfInfrastructure_FreeFn_t free;
  QairtHtpPerfInfrastructure_CreatePowerConfigIdFn_t createPowerConfigId;
  QairtHtpPerfInfrastructure_DestroyPowerConfigIdFn_t destroyPowerConfigId;
  QairtHtpPerfInfrastructure_SetPowerConfigFn_t setPowerConfig;
  QairtHtpPerfInfrastructure_SetMemoryConfigFn_t setMemoryConfig;
} QairtHtpPerfInfrastructureV1_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAIRT_HTP_PERF_INFRASTRUCTURE_H
