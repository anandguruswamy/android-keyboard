//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================

#ifndef QAIRT_HTP_CONTEXT_H
#define QAIRT_HTP_CONTEXT_H

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

#include "QairtContext/QairtContext.h"
#include "QairtContext/QairtContextConfig.h"

//=============================================================================
// Macros
//=============================================================================
#define QAIRT_MIN_ID_HTP_CONTEXT (QAIRT_MIN_ID_HTP + 0)

#define QAIRT_HTP_CONTEXT_GROUP_REGISTRATION_ID         (QAIRT_MIN_ID_HTP_CONTEXT + 0)
#define QAIRT_HTP_CONTEXT_CUSTOM_CONFIG_ID              (QAIRT_MIN_ID_HTP_CONTEXT + 1)
#define QAIRT_HTP_CONTEXT_PROPERTIES_ID                 (QAIRT_MIN_ID_HTP_CONTEXT + 2)
#define QAIRT_HTP_CONTEXT_GRAPH_SPLIT_ID                (QAIRT_MIN_ID_HTP_CONTEXT + 3)
#define QAIRT_HTP_CONTEXT_LORA_PRELOAD_GROUP_ID         (QAIRT_MIN_ID_HTP_CONTEXT + 4)
#define QAIRT_HTP_CONTEXT_LORA_PRELOAD_BUFFER_GROUPS_ID (QAIRT_MIN_ID_HTP_CONTEXT + 5)

// Define ranges for HTP context-specific error codes
#define QAIRT_MIN_ERROR_HTP_CONTEXT (QAIRT_MIN_ERROR_HTP + 0)
#define QAIRT_MAX_ERROR_HTP_CONTEXT (QAIRT_MIN_ERROR_HTP + 999)

// Auto mode for number of prepare threads for QairtHtpContext_CustomConfig_setNumPrepareThreads.
#define QAIRT_HTP_CONTEXT_NUM_PREPARE_THREADS_AUTO UINT32_MAX

//=============================================================================
// Data Types
//=============================================================================
typedef struct _QairtHtpContext_GroupRegistrationHandle_t*
    QairtHtpContext_GroupRegistrationHandle_t;
typedef QairtContext_CustomConfigHandle_t QairtHtpContext_CustomConfigHandle_t;
typedef struct _QairtHtpContext_GraphSplitHandle_t* QairtHtpContext_GraphSplitHandle_t;
typedef struct _QairtHtpContext_LoraPreloadGroupHandle_t*
    QairtHtpContext_LoraPreloadGroupHandle_t;
typedef struct _QairtHtpContext_LoraPreloadBufferGroupsHandle_t*
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t;

// This enum is supported only with the QairtContext_createFromBinaryListAsync API, when
// shareResources is true; otherwise, it is ignored. This enumeration allows users to specify how
// graphs are going to be executed, providing QAIRT with hints for optimizing memory.
typedef enum {
  // Default value if no user input is provided.
  // This type is used for sequential graph execution, optimizing both VA and memory.
  QAIRT_HTP_CONTEXT_RESOURCE_OPTIMIZATION_SEQUENTIAL_WITH_VA_OPTIMIZATION =
      0,  // SEQUENTIAL_WITH_VA_OPTIMIZATION,
  // This type is used for sequential graph execution, optimizing memory.
  QAIRT_HTP_CONTEXT_RESOURCE_OPTIMIZATION_SEQUENTIAL_WITHOUT_VA_OPTIMIZATION =
      1,  // SEQUENTIAL_WITHOUT_VA_OPTIMIZATION,
  // This type is used for concurrent resource sharing, optimizing memory by sharing
  // resources across contexts with the same priority level.
  QAIRT_HTP_CONTEXT_RESOURCE_OPTIMIZATION_CONCURRENT_OPTIMIZATION = 2,  // CONCURRENT_OPTIMIZATION,
  QAIRT_HTP_CONTEXT_RESOURCE_OPTIMIZATION_UNDEFINED               = 0x7fffffff
} QairtHtpContext_ShareResourcesOptimizationType_t;

/**
 * @brief QAIRT HTP Context API result / error codes.
 */
typedef enum {
  QAIRT_HTP_CONTEXT_MIN_ERROR = QAIRT_MIN_ERROR_HTP_CONTEXT,
  ////////////////////////////////////////

  /// HTP context operation succeeded.
  QAIRT_HTP_CONTEXT_NO_ERROR = QAIRT_SUCCESS,
  /// General memory allocation failure.
  QAIRT_HTP_CONTEXT_ERROR_MEM_ALLOC = QAIRT_COMMON_ERROR_MEM_ALLOC,
  /// Feature or API is not supported.
  QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE = QAIRT_COMMON_ERROR_NOT_SUPPORTED,
  /// General error not covered by a more specific code.
  QAIRT_HTP_CONTEXT_ERROR_GENERAL = QAIRT_COMMON_ERROR_GENERAL,
  /// An argument to the API is invalid.
  QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT = QAIRT_MIN_ERROR_HTP_CONTEXT + 0,
  /// A handle argument is not valid.
  QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE = QAIRT_MIN_ERROR_HTP_CONTEXT + 1,
  /// A configuration value is invalid or incompatible.
  QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG = QAIRT_MIN_ERROR_HTP_CONTEXT + 2,

  ////////////////////////////////////////
  QAIRT_HTP_CONTEXT_MAX_ERROR = QAIRT_MAX_ERROR_HTP_CONTEXT,
  // Unused, present to ensure 32 bits.
  QAIRT_HTP_CONTEXT_ERROR_UNDEFINED = 0x7FFFFFFF
} QairtHtpContext_Error_t;

//=============================================================================
// Public Functions
//=============================================================================

#ifdef __cplusplus
extern "C" {
#endif

/// HTP Group Registration
/**
 * @brief Create a group registration handle used to manage HTP program groups.
 *
 * @param[out] groupRegistration A handle to the created group registration object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Group registration was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * groupRegistration is NULL).
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Failure in allocating memory when creating the group
 * registration.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: The optional group registration feature is not
 * supported.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_GroupRegistrationV1_t.
 */
Qairt_Status_t QairtHtpContext_GroupRegistration_create(
    QairtHtpContext_GroupRegistrationHandle_t* groupRegistration);
/**
 * @brief Free a previously created group registration handle and associated resources.
 *
 * @param[in] groupRegistration A handle to a group registration. Passing an invalid handle
 *                              results in an error.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Group registration was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g., NULL
 * handle).
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: groupRegistration is not a valid handle.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_GroupRegistrationV1_t.
 */
Qairt_Status_t QairtHtpContext_GroupRegistration_free(
    QairtHtpContext_GroupRegistrationHandle_t groupRegistration);

/**
 * @brief Set the first group handle for an HTP group registration.
 *
 * Associates the first group in the registration sequence with the provided context handle.
 *
 * @param[in] groupRegistration A handle to a group registration.
 *
 * @param[in] context A handle to a context to associate as the first group in the registration.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: First group handle was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: groupRegistration or context is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: The requested operation is not supported.
 *         - QAIRT_CONTEXT_ERROR_INVALID_CONFIG: One or more configuration values are invalid.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_GroupRegistrationV1_t.
 */
Qairt_Status_t QairtHtpContext_GroupRegistration_setFirstGroupHandle(
    QairtHtpContext_GroupRegistrationHandle_t groupRegistration, QairtContext_Handle_t context);

/**
 * @brief Get the first group handle associated with an HTP group registration.
 *
 * Retrieves the context handle that was previously set as the first group for the specified
 * group registration.
 *
 * @param[in]  groupRegistration A handle to a group registration.
 *
 * @param[out] context A pointer to receive the context handle associated as the first group.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: First group handle was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g., context
 * is NULL).
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: groupRegistration is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: The requested operation is not supported.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_GroupRegistrationV1_t.
 */
Qairt_Status_t QairtHtpContext_GroupRegistration_getFirstGroupHandle(
    QairtHtpContext_GroupRegistrationHandle_t groupRegistration, QairtContext_Handle_t* context);

/**
 * @brief Set the maximum spill-fill buffer size for an HTP group registration.
 *
 * Configures the maximum spill-fill buffer size (in bytes) that may be required by
 * programs within the registered group.
 *
 * @param[in] groupRegistration A handle to a group registration.
 *
 * @param[in] spillFill The maximum spill-fill buffer size in bytes.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: groupRegistration is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: The requested operation is not supported.
 *         - QAIRT_CONTEXT_ERROR_INVALID_CONFIG: The provided spillFill value is not valid.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_GroupRegistrationV1_t.
 */
Qairt_Status_t QairtHtpContext_GroupRegistration_setMaxSpillFill(
    QairtHtpContext_GroupRegistrationHandle_t groupRegistration, uint64_t spillFill);
/**
 * @brief Get the maximum spill-fill buffer size for an HTP group registration.
 *
 * Retrieves the currently configured maximum spill-fill buffer size (in bytes) for
 * the specified group registration.
 *
 * @param[in]  groupRegistration A handle to a group registration.
 *
 * @param[out] spillFill A pointer to receive the maximum spill-fill buffer size in bytes.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g., spillFill
 * is NULL).
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: groupRegistration is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: The requested operation is not supported.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_GroupRegistrationV1_t.
 */
Qairt_Status_t QairtHtpContext_GroupRegistration_getMaxSpillFill(
    QairtHtpContext_GroupRegistrationHandle_t groupRegistration, uint64_t* spillFill);

/// HTP Context Configuration

/**
 * @brief Create a custom configuration handle for HTP context operations.
 *
 * Allocates and initializes a custom configuration object that can be used to
 * override default context settings for HTP programs.
 *
 * @param[out] customConfig A pointer to receive the created custom configuration handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Custom configuration handle was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * customConfig is NULL).
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory allocation failed during creation.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: The requested operation is not supported.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_create(
    QairtHtpContext_CustomConfigHandle_t* customConfig);

/**
 * @brief Free a previously created custom configuration handle and associated resources.
 *
 * Releases all resources associated with the custom configuration handle. On success,
 * the handle pointed to by customConfig will be set to NULL.
 *
 * @param[in,out] customConfig A pointer to the custom configuration handle to free.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Custom configuration handle was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * customConfig is NULL).
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: The handle referenced by customConfig is not valid
 * or already freed.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_free(QairtHtpContext_CustomConfigHandle_t customConfig);

// Weight Sharing
/**
 * @brief Enable or disable weight sharing for the HTP context custom configuration.
 *
 * Configures whether weight sharing is enabled when preparing or executing graphs
 * within the context. When enabled, compatible graphs can share constant sections
 * (weights) to reduce memory footprint.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] enableWeightSharing Non-zero to enable weight sharing; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided value is not valid for this option.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setEnableWeightSharing(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t enableWeightSharing);
/**
 * @brief Get the weight sharing enablement setting from the HTP context custom configuration.
 *
 * Retrieves whether weight sharing is currently enabled for the context configuration.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] enableWeightSharing Pointer to receive the non-zero/zero value indicating
 *                                 whether weight sharing is enabled.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * enableWeightSharing is NULL).
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getEnableWeightSharing(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* enableWeightSharing);

// Group Registration
/**
 * @brief Set the group registration to associate with this custom configuration.
 *
 * Associates a group registration handle used to register multiple contexts into
 * a group for shared spill-fill buffer management.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] groupRegistration A valid group registration handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Handle was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig or groupRegistration is not a
 * valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided handle is incompatible with current
 * configuration.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setGroupRegistration(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    QairtHtpContext_GroupRegistrationHandle_t groupRegistration);
/**
 * @brief Get the group registration associated with this custom configuration.
 *
 * Retrieves the currently associated group registration handle, if any.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] groupRegistration Pointer to receive the group registration handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Handle was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * groupRegistration is NULL).
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getGroupRegistration(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    QairtHtpContext_GroupRegistrationHandle_t* groupRegistration);

// FileReadMemoryBudget
/**
 * @brief Set the file read memory budget (in MB) for deserializing context binaries.
 *
 * Provides a hint to the backend to read and copy constant sections of the context
 * binary in chunks up to the specified number of megabytes, potentially reducing
 * peak host memory usage during deserialization.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] fileReadMemoryBudgetInMb The read budget in megabytes. Implementations may
 *                                     interpret 0 as disabled and enforce upper bounds.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Budget was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_CONTEXT_ERROR_INVALID_CONFIG: Provided budget value is out of allowed range.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setFileReadMemoryBudget(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint64_t fileReadMemoryBudgetInMb);
/**
 * @brief Get the file read memory budget (in MB) used for context binary deserialization.
 *
 * Retrieves the currently configured read budget that guides chunked loading of
 * constant sections during deserialization.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] fileReadMemoryBudgetInMb Pointer to receive the budget in megabytes.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Budget was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * fileReadMemoryBudgetInMb is NULL).
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getFileReadMemoryBudget(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint64_t* fileReadMemoryBudgetInMb);

// DSP Memory Profiling Enabled
/**
 * @brief Enable or disable DSP memory profiling.
 *
 * Controls whether DSP memory profiling is enabled for the session, which may
 * incur additional overhead to collect memory usage metrics.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] enableDspMemoryProfiling Non-zero to enable profiling; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided value is not valid for this option.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setEnableDspMemoryProfiling(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t enableDspMemoryProfiling);
/**
 * @brief Get the DSP memory profiling enablement setting.
 *
 * Retrieves whether DSP memory profiling is currently enabled for the context configuration.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] enableDspMemoryProfiling Pointer to receive the non-zero/zero value indicating
 *                                      whether profiling is enabled.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * enableDspMemoryProfiling is NULL).
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getEnableDspMemoryProfiling(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* enableDspMemoryProfiling);

// Share Resources
/**
 * @brief Enable or disable resource sharing across eligible graphs/contexts.
 *
 * Configures whether resources (e.g., buffers) may be shared where supported to
 * optimize memory usage under certain execution models.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] shareResources Non-zero to enable sharing; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided value is not valid for this option.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setShareResources(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t shareResources);
/**
 * @brief Get the resource sharing enablement setting.
 *
 * Retrieves whether resource sharing is currently enabled for the context configuration.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] shareResources Pointer to receive the non-zero/zero value indicating
 *                            whether sharing is enabled.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * shareResources is NULL).
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getShareResources(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* shareResources);

// IO Mem Estimation
/**
 * @brief Set the I/O memory estimation mode.
 *
 * Configures whether I/O memory estimation heuristics are enabled to help predict
 * memory requirements prior to execution.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] ioMemEstimation Non-zero to enable estimation; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided value is not valid for this option.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setIoMemEstimation(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t ioMemEstimation);
/**
 * @brief Get the I/O memory estimation mode.
 *
 * Retrieves whether I/O memory estimation is currently enabled for the context configuration.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] ioMemEstimation Pointer to receive the non-zero/zero value indicating
 *                             whether estimation is enabled.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * ioMemEstimation is NULL).
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getIoMemEstimation(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* ioMemEstimation);

// Is Prepare Only
/**
 * @brief Set prepare-only mode for context operations.
 *
 * Configures whether the context should only perform preparation (without executing),
 * which may be useful for ahead-of-time preparation workflows.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] isPrepareOnly Non-zero to enable prepare-only; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided value is not valid for this option.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setIsPrepareOnly(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t isPrepareOnly);
/**
 * @brief Get prepare-only mode for context operations.
 *
 * Retrieves whether prepare-only mode is currently enabled for the context configuration.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] isPrepareOnly Pointer to receive the non-zero/zero value indicating
 *                           whether prepare-only is enabled.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * isPrepareOnly is NULL).
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getIsPrepareOnly(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* isPrepareOnly);

// Init Acceleration
/**
 * @brief Enable or disable initialization acceleration.
 *
 * Controls whether an optimization path is used during initialization to accelerate
 * setup at potential trade-offs depending on backend support.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] initAcceleration Non-zero to enable; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided value is not valid for this option.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setInitAcceleration(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t initAcceleration);
/**
 * @brief Get the initialization acceleration setting.
 *
 * Retrieves whether initialization acceleration is currently enabled.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] initAcceleration Pointer to receive the non-zero/zero value indicating
 *                              whether the feature is enabled.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * initAcceleration is NULL).
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note The function name indicates "set", but the signature returns a value via pointer.
 *       This API retrieves the current setting.
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getInitAcceleration(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* initAcceleration);

// Skip Binary Section Validation
/**
 * @brief Skip validation on binary sections during context operations.
 *
 * Configures whether certain binary section validation steps should be skipped.
 * Disabling validation may reduce initialization time but should be used with caution.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] skipValidationOnBinarySection Non-zero to skip validation; zero to perform validation.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided value is not valid for this option.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setSkipBinarySectionValidation(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t skipValidationOnBinarySection);
/**
 * @brief Get the binary section validation skip setting.
 *
 * Retrieves whether binary section validation is currently being skipped.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] skipValidationOnBinarySection Pointer to receive the non-zero/zero value indicating
 *                                           whether validation is skipped.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * skipValidationOnBinarySection is NULL).
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getSkipBinarySectionValidation(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* skipValidationOnBinarySection);

// Share Resource Optimization Type
/**
 * @brief Set the optimization type for resource sharing.
 *
 * Selects the optimization strategy for sharing resources across eligible graphs/contexts.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] shareResourcesOptimizationType Optimization type selector.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided type is not supported or out of range.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setShareResourcesOptimizationType(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    QairtHtpContext_ShareResourcesOptimizationType_t shareResourcesOptimizationType);
/**
 * @brief Get the optimization type used for resource sharing.
 *
 * Retrieves the currently configured resource sharing optimization strategy.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] shareResourcesOptimizationType Pointer to receive the optimization type.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * shareResourcesOptimizationType is NULL).
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getShareResourcesOptimizationType(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    QairtHtpContext_ShareResourcesOptimizationType_t* shareResourcesOptimizationType);

// Use Extended UDMA
/**
 * @brief Enable or disable use of extended UDMA.
 *
 * Configures whether extended UDMA capabilities are used when supported by the target.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] useExtendedUdma Non-zero to enable; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided value is not valid for this option.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setUseExtendedUdma(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t useExtendedUdma);
/**
 * @brief Get the extended UDMA setting.
 *
 * Retrieves whether extended UDMA is currently enabled.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] useExtendedUdma Pointer to receive the non-zero/zero value indicating
 *                             whether extended UDMA is enabled.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * useExtendedUdma is NULL).
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getUseExtendedUdma(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* useExtendedUdma);

// ConcurrentGroupRegistration
/**
 * @brief Set the concurrent group registration to associate with this custom configuration.
 *
 * Associates a group registration handle used to coordinate shared resources or
 * sequencing across multiple contexts/graphs as supported.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] groupRegistration A valid group registration handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Handle was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig or groupRegistration is not a
 * valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided handle is incompatible with current
 * configuration.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setConcurrentGroupRegistration(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    QairtHtpContext_GroupRegistrationHandle_t groupRegistration);
/**
 * @brief Get the concurrent group registration associated with this custom configuration.
 *
 * Retrieves the currently associated group registration handle, if any.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] groupRegistration Pointer to receive the group registration handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Handle was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * groupRegistration is NULL).
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getConcurrentGroupRegistration(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    QairtHtpContext_GroupRegistrationHandle_t* groupRegistration);

// LoRA Weight Sharing Enabled
/**
 * @brief Enable or disable LoRA-specific weight sharing behavior.
 *
 * Configures whether LoRA (Low-Rank Adaptation) weight sharing is enabled when supported,
 * allowing additional memory savings for LoRA-based models.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] loraWeightSharingEnabled Non-zero to enable; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_CONFIG: Provided value is not valid for this option or
 * unsupported with current config.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setLoraWeightSharingEnabled(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t loraWeightSharingEnabled);
/**
 * @brief Get the LoRA weight sharing enablement setting.
 *
 * Retrieves whether LoRA-specific weight sharing is currently enabled.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] loraWeightSharingEnabled Pointer to receive the non-zero/zero value indicating
 *                                      whether LoRA weight sharing is enabled.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid (e.g.,
 * loraWeightSharingEnabled is NULL).
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION: SSR occurrence (successful recovery).
 *         - QAIRT_COMMON_ERROR_SYSTEM_COMMUNICATION_FATAL: SSR occurrence (unsuccessful recovery).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getLoraWeightSharingEnabled(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* loraWeightSharingEnabled);

// LoRA Weight Sharing RAM Preload Enabled
/**
 * @brief Enable or disable LoRA weight sharing RAM preload.
 *
 * When enabled, each graph maintains its own replaceable weight blob (preloaded into RAM)
 * instead of using a single shared blob. Only meaningful when LoRA weight sharing is enabled.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] loraWeightSharingRamPreloadEnabled Non-zero to enable; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setLoraWeightSharingRamPreloadEnabled(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t loraWeightSharingRamPreloadEnabled);
/**
 * @brief Get the LoRA weight sharing RAM preload enablement setting.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] loraWeightSharingRamPreloadEnabled Pointer to receive the non-zero/zero value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: loraWeightSharingRamPreloadEnabled is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getLoraWeightSharingRamPreloadEnabled(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    uint8_t* loraWeightSharingRamPreloadEnabled);

/**
 * @brief Set LoRA preload buffer groups.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] loraPreloadBufferGroups Handle to LoRA preload buffer groups. The data is copied
 *                                    into the custom configuration handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: loraPreloadBufferGroups is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setLoraPreloadBufferGroups(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t loraPreloadBufferGroups);

/**
 * @brief Get LoRA preload buffer groups.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] loraPreloadBufferGroups Pointer to receive the configured LoRA preload buffer groups.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: loraPreloadBufferGroups is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getLoraPreloadBufferGroups(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t* loraPreloadBufferGroups);

/**
 * @brief Create a LoRA preload group.
 *
 * Creates an opaque handle used to hold the graph names that share one LoRA preload buffer.
 *
 * @param[out] loraPreloadGroup Pointer to receive the created LoRA preload group handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: LoRA preload group was successfully created.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: loraPreloadGroup is NULL.
 *         - QAIRT_HTP_CONTEXT_ERROR_MEM_ALLOC: Failed to allocate the group object.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadGroupV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadGroup_create(
    QairtHtpContext_LoraPreloadGroupHandle_t* loraPreloadGroup);

/**
 * @brief Free a LoRA preload group.
 *
 * Releases a group handle created by QairtHtpContext_LoraPreloadGroup_create.
 *
 * @param[in] loraPreloadGroup LoRA preload group handle to free.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: LoRA preload group was successfully freed.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: loraPreloadGroup is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadGroupV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadGroup_free(
    QairtHtpContext_LoraPreloadGroupHandle_t loraPreloadGroup);

/**
 * @brief Set graph names for a LoRA preload group.
 *
 * Replaces the current graph-name list on the group. The graph-name strings are copied into the
 * group handle.
 *
 * @param[in] loraPreloadGroup LoRA preload group handle to update.
 * @param[in] graphNames Array of graph-name C strings. May be NULL only when numGraphs is 0.
 * @param[in] numGraphs Number of graph-name entries in graphNames.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Graph names were successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: loraPreloadGroup is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: graphNames or one of its entries is NULL
 *           while numGraphs is non-zero.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadGroupV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadGroup_setGraphNames(
    QairtHtpContext_LoraPreloadGroupHandle_t loraPreloadGroup,
    const char** graphNames,
    uint32_t numGraphs);

/**
 * @brief Get the number of graph names in a LoRA preload group.
 *
 * @param[in]  loraPreloadGroup LoRA preload group handle to query.
 * @param[out] numGraphs Pointer to receive the number of graph names.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Graph count was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: loraPreloadGroup is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: numGraphs is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadGroupV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadGroup_getNumGraphs(
    QairtHtpContext_LoraPreloadGroupHandle_t loraPreloadGroup, uint32_t* numGraphs);

/**
 * @brief Get a graph name from a LoRA preload group by index.
 *
 * The returned string is owned by the group handle and remains valid until the group is modified
 * or freed.
 *
 * @param[in]  loraPreloadGroup LoRA preload group handle to query.
 * @param[in]  index Zero-based graph-name index.
 * @param[out] graphName Pointer to receive the graph-name C string.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Graph name was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: loraPreloadGroup is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: graphName is NULL or index is out of range.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadGroupV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadGroup_getGraphNameAt(
    QairtHtpContext_LoraPreloadGroupHandle_t loraPreloadGroup,
    uint32_t index,
    const char** graphName);

/**
 * @brief Create a LoRA preload buffer groups collection.
 *
 * Creates an opaque collection handle used to store LoRA preload groups.
 *
 * @param[out] loraPreloadBufferGroups Pointer to receive the created collection handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Collection was successfully created.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: loraPreloadBufferGroups is NULL.
 *         - QAIRT_HTP_CONTEXT_ERROR_MEM_ALLOC: Failed to allocate the collection object.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadBufferGroupsV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadBufferGroups_create(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t* loraPreloadBufferGroups);

/**
 * @brief Free a LoRA preload buffer groups collection.
 *
 * Releases a collection handle created by QairtHtpContext_LoraPreloadBufferGroups_create.
 *
 * @param[in] loraPreloadBufferGroups LoRA preload buffer groups handle to free.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Collection was successfully freed.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: loraPreloadBufferGroups is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadBufferGroupsV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadBufferGroups_free(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t loraPreloadBufferGroups);

/**
 * @brief Add a LoRA preload group to a buffer groups collection.
 *
 * Appends a copy of the provided group to the collection. The caller may modify or free
 * loraPreloadGroup after this call.
 *
 * @param[in] loraPreloadBufferGroups LoRA preload buffer groups collection to update.
 * @param[in] loraPreloadGroup LoRA preload group to copy into the collection.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Group was successfully added.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: One or more handles are invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_MEM_ALLOC: Failed to allocate the copied group object.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadBufferGroupsV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadBufferGroups_addGroup(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t loraPreloadBufferGroups,
    QairtHtpContext_LoraPreloadGroupHandle_t loraPreloadGroup);

/**
 * @brief Get the number of LoRA preload groups in a collection.
 *
 * @param[in]  loraPreloadBufferGroups LoRA preload buffer groups collection to query.
 * @param[out] numGroups Pointer to receive the number of groups.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Group count was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: loraPreloadBufferGroups is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: numGroups is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadBufferGroupsV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadBufferGroups_getNumGroups(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t loraPreloadBufferGroups,
    uint32_t* numGroups);

/**
 * @brief Get a LoRA preload group from a collection by index.
 *
 * Returns a non-owning group handle owned by the collection. The returned handle remains valid
 * until the collection is modified, cleared, or freed.
 *
 * @param[in]  loraPreloadBufferGroups LoRA preload buffer groups collection to query.
 * @param[in]  index Zero-based group index.
 * @param[out] loraPreloadGroup Pointer to receive the non-owning group handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Group handle was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: loraPreloadBufferGroups is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: loraPreloadGroup is NULL or index is out
 *           of range.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadBufferGroupsV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadBufferGroups_getGroupAt(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t loraPreloadBufferGroups,
    uint32_t index,
    QairtHtpContext_LoraPreloadGroupHandle_t* loraPreloadGroup);

/**
 * @brief Clear all LoRA preload groups from a collection.
 *
 * Removes all groups currently stored in the collection.
 *
 * @param[in] loraPreloadBufferGroups LoRA preload buffer groups collection to clear.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Collection was successfully cleared.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: loraPreloadBufferGroups is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_LoraPreloadBufferGroupsV1_t.
 */
Qairt_Status_t QairtHtpContext_LoraPreloadBufferGroups_clear(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t loraPreloadBufferGroups);

// Reused IO Limit
/**
 * @brief Set the reused I/O buffer limit (in MB) for context binary creation.
 *
 * Optimizes I/O memory management during context creation; for binary-list creation this is
 * a group-level property shared by all contexts in the group.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] reusedIoLimitMb The limit in megabytes.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setReusedIoLimitMb(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint64_t reusedIoLimitMb);
/**
 * @brief Get the reused I/O buffer limit (in MB).
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] reusedIoLimitMb Pointer to receive the limit in megabytes.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: reusedIoLimitMb is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getReusedIoLimitMb(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint64_t* reusedIoLimitMb);

// Reference Weight Sharing Enabled
/**
 * @brief Enable or disable reference weight sharing across context binaries targeting
 *        different SoCs. Only supported via the DLC workflow.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] referenceWeightSharingEnabled Non-zero to enable; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Option was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setReferenceWeightSharingEnabled(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t referenceWeightSharingEnabled);
/**
 * @brief Get the reference weight sharing enablement setting.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] referenceWeightSharingEnabled Pointer to receive the non-zero/zero value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: referenceWeightSharingEnabled is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getReferenceWeightSharingEnabled(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* referenceWeightSharingEnabled);

// Concurrent Deserialization Patch
/**
 * @brief Set the file descriptor for the backward-compatible concurrent deserialization
 *        patch file.
 *
 * The caller is responsible for opening (and, on context teardown, closing) the file;
 * this API only stores the descriptor to be forwarded to the backend.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] patchFd An open, read/write file descriptor for the patch file.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setConcurrentDeserializationPatchFd(
    QairtHtpContext_CustomConfigHandle_t customConfig, int32_t patchFd);
/**
 * @brief Get the concurrent deserialization patch file descriptor.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] patchFd Pointer to receive the file descriptor.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: patchFd is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getConcurrentDeserializationPatchFd(
    QairtHtpContext_CustomConfigHandle_t customConfig, int32_t* patchFd);

// Context Properties
/**
 * @brief Get the buffer start alignment from HTP context properties.
 *
 * @param[in] contextHandle A handle to a finalized context.
 * @param[out] value The buffer start alignment value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: _contextHandle_ is not valid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: _value_ is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_PropertiesV1_t.
 */
Qairt_Status_t QairtHtpContext_Properties_getBufferStartAlignment(
    QairtContext_Handle_t contextHandle, uint64_t* value);

/**
 * @brief Get the spill-fill buffer size from HTP context properties.
 *
 * @param[in] contextHandle A handle to a finalized context.
 * @param[out] value The spill-fill buffer size in bytes.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: _contextHandle_ is not valid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: _value_ is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_PropertiesV1_t.
 */
Qairt_Status_t QairtHtpContext_Properties_getSpillfillBufferSize(
    QairtContext_Handle_t contextHandle, uint64_t* value);

/**
 * @brief Get the weights buffer size from HTP context properties.
 *
 * @param[in] contextHandle A handle to a finalized context.
 * @param[out] value The weights buffer size in bytes.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: _contextHandle_ is not valid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: _value_ is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_PropertiesV1_t.
 */
Qairt_Status_t QairtHtpContext_Properties_getWeightsBufferSize(
    QairtContext_Handle_t contextHandle, uint64_t* value);

/**
 * @brief Get the VTCM backup buffer size from HTP context properties.
 *
 * @param[in] contextHandle A handle to a finalized context.
 * @param[out] value The VTCM backup buffer size in bytes.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: _contextHandle_ is not valid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: _value_ is NULL.
 *
 * @note Use corresponding API through QairtHtpContext_PropertiesV1_t.
 */
Qairt_Status_t QairtHtpContext_Properties_getVtcmBackupBufferSize(
    QairtContext_Handle_t contextHandle, uint64_t* value);

// Graph Split
/**
 * @brief Create a graph split handle used to configure graph-splitting behavior.
 *
 * @param[out] graphSplit A pointer to receive the created graph split handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Graph split handle was successfully created.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_MEM_ALLOC: Failure in allocating memory.
 *
 * @note Use corresponding API through QairtHtpContext_GraphSplitV1_t.
 */
Qairt_Status_t QairtHtpContext_GraphSplit_create(QairtHtpContext_GraphSplitHandle_t* graphSplit);
/**
 * @brief Free a previously created graph split handle and associated resources.
 *
 * @param[in] graphSplit A handle to a graph split object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Graph split handle was successfully freed.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: graphSplit is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_GraphSplitV1_t.
 */
Qairt_Status_t QairtHtpContext_GraphSplit_free(QairtHtpContext_GraphSplitHandle_t graphSplit);

/**
 * @brief Enable or disable graph splitting.
 *
 * When enabled, the graph is split based on its structure and each part is compiled
 * as an independent subgraph.
 *
 * @param[in] graphSplit A handle to a graph split object.
 * @param[in] graphSplittingEnabled Non-zero to enable graph splitting; zero to disable.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: graphSplit is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_GraphSplitV1_t.
 */
Qairt_Status_t QairtHtpContext_GraphSplit_setGraphSplittingEnabled(
    QairtHtpContext_GraphSplitHandle_t graphSplit, uint8_t graphSplittingEnabled);
/**
 * @brief Get the graph splitting enablement setting.
 *
 * @param[in]  graphSplit A handle to a graph split object.
 * @param[out] graphSplittingEnabled Pointer to receive the non-zero/zero value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: graphSplittingEnabled is NULL.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: graphSplit is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_GraphSplitV1_t.
 */
Qairt_Status_t QairtHtpContext_GraphSplit_getGraphSplittingEnabled(
    QairtHtpContext_GraphSplitHandle_t graphSplit, uint8_t* graphSplittingEnabled);

// Graph Split (CustomConfig association)
/**
 * @brief Set the graph split configuration to associate with this custom configuration.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] graphSplit A valid graph split handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Handle was successfully set.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig or graphSplit is not a valid
 * handle.
 *         - QAIRT_HTP_CONTEXT_ERROR_UNSUPPORTED_FEATURE: Operation is not supported on the target.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setGraphSplit(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    QairtHtpContext_GraphSplitHandle_t graphSplit);
/**
 * @brief Get the graph split configuration associated with this custom configuration.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] graphSplit Pointer to receive the graph split handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Handle was successfully retrieved.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_ARGUMENT: At least one argument is invalid.
 *         - QAIRT_HTP_CONTEXT_ERROR_INVALID_HANDLE: customConfig is not a valid handle.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getGraphSplit(
    QairtHtpContext_CustomConfigHandle_t customConfig,
    QairtHtpContext_GraphSplitHandle_t* graphSplit);
/**
 * @brief Set graph splitting enabled directly on a custom configuration.
 *
 * @param[in] customConfig Handle to the custom configuration object.
 * @param[in] graphSplittingEnabled 1 to enable graph splitting, 0 to disable.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setGraphSplittingEnabled(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t graphSplittingEnabled);
/**
 * @brief Get the graph splitting enabled flag set directly on a custom configuration.
 *
 * @param[in]  customConfig Handle to the custom configuration object.
 * @param[out] graphSplittingEnabled Pointer to receive 1 (enabled) or 0 (disabled).
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getGraphSplittingEnabled(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint8_t* graphSplittingEnabled);
/**
 * @brief Set the number of prepare threads on a custom configuration.
 *
 * @param[in] customConfig     Handle to the custom configuration object.
 * @param[in] numPrepareThreads Worker thread count. Pass
 *                             QAIRT_HTP_CONTEXT_NUM_PREPARE_THREADS_AUTO for auto mode.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_setNumPrepareThreads(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint32_t numPrepareThreads);
/**
 * @brief Get the number of prepare threads set on a custom configuration.
 *
 * @param[in]  customConfig      Handle to the custom configuration object.
 * @param[out] numPrepareThreads Pointer to receive the configured worker thread count.
 *
 * @note Use corresponding API through QairtHtpContext_CustomConfigV1_t.
 */
Qairt_Status_t QairtHtpContext_CustomConfig_getNumPrepareThreads(
    QairtHtpContext_CustomConfigHandle_t customConfig, uint32_t* numPrepareThreads);

typedef Qairt_Status_t (*QairtHtpContext_GroupRegistration_CreateFn_t)(
    QairtHtpContext_GroupRegistrationHandle_t*);
typedef Qairt_Status_t (*QairtHtpContext_GroupRegistration_FreeFn_t)(
    QairtHtpContext_GroupRegistrationHandle_t);
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_CreateFn_t)(
    QairtHtpContext_CustomConfigHandle_t*);
typedef Qairt_Status_t (*QairtHtpContext_GroupRegistration_SetFirstGroupHandleFn_t)(
    QairtHtpContext_GroupRegistrationHandle_t, QairtContext_Handle_t);

typedef Qairt_Status_t (*QairtHtpContext_GroupRegistration_GetFirstGroupHandleFn_t)(
    QairtHtpContext_GroupRegistrationHandle_t, QairtContext_Handle_t*);

typedef Qairt_Status_t (*QairtHtpContext_GroupRegistration_SetMaxSpillFillFn_t)(
    QairtHtpContext_GroupRegistrationHandle_t, uint64_t);

typedef Qairt_Status_t (*QairtHtpContext_GroupRegistration_GetMaxSpillFillFn_t)(
    QairtHtpContext_GroupRegistrationHandle_t, uint64_t*);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_FreeFn_t)(
    QairtHtpContext_CustomConfigHandle_t);

/* Weight Sharing */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetEnableWeightSharingFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetEnableWeightSharingFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* Group Registration */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetGroupRegistrationFn_t)(
    QairtHtpContext_CustomConfigHandle_t, QairtHtpContext_GroupRegistrationHandle_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetGroupRegistrationFn_t)(
    QairtHtpContext_CustomConfigHandle_t, QairtHtpContext_GroupRegistrationHandle_t*);

/* File Read Memory Budget */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetFileReadMemoryBudgetFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint64_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetFileReadMemoryBudgetFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint64_t*);

/* DSP Memory Profiling Enabled */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetEnableDspMemoryProfilingFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetEnableDspMemoryProfilingFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* Share Resources */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetShareResourcesFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetShareResourcesFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* IO Mem Estimation */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetIoMemEstimationFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetIoMemEstimationFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* Is Prepare Only */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetIsPrepareOnlyFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetIsPrepareOnlyFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* Init Acceleration */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetInitAccelerationFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetInitAccelerationFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* Skip Binary Section Validation */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetSkipBinarySectionValidationFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetSkipBinarySectionValidationFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* Share Resource Optimization Type */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetShareResourcesOptimizationTypeFn_t)(
    QairtHtpContext_CustomConfigHandle_t, QairtHtpContext_ShareResourcesOptimizationType_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetShareResourcesOptimizationTypeFn_t)(
    QairtHtpContext_CustomConfigHandle_t, QairtHtpContext_ShareResourcesOptimizationType_t*);

/* Use Extended UDMA */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetUseExtendedUdmaFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetUseExtendedUdmaFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* Concurrent Group Registration */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetConcurrentGroupRegistrationFn_t)(
    QairtHtpContext_CustomConfigHandle_t, QairtHtpContext_GroupRegistrationHandle_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetConcurrentGroupRegistrationFn_t)(
    QairtHtpContext_CustomConfigHandle_t, QairtHtpContext_GroupRegistrationHandle_t*);

/* LoRA Weight Sharing Enabled */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetLoraWeightSharingEnabledFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetLoraWeightSharingEnabledFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* LoRA Weight Sharing RAM Preload Enabled */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetLoraWeightSharingRamPreloadEnabledFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetLoraWeightSharingRamPreloadEnabledFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* LoRA Preload Buffer Groups */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetLoraPreloadBufferGroupsFn_t)(
    QairtHtpContext_CustomConfigHandle_t, QairtHtpContext_LoraPreloadBufferGroupsHandle_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetLoraPreloadBufferGroupsFn_t)(
    QairtHtpContext_CustomConfigHandle_t, QairtHtpContext_LoraPreloadBufferGroupsHandle_t*);

typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadGroup_CreateFn_t)(
    QairtHtpContext_LoraPreloadGroupHandle_t*);
typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadGroup_FreeFn_t)(
    QairtHtpContext_LoraPreloadGroupHandle_t);
typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadGroup_SetGraphNamesFn_t)(
    QairtHtpContext_LoraPreloadGroupHandle_t, const char**, uint32_t);
typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadGroup_GetNumGraphsFn_t)(
    QairtHtpContext_LoraPreloadGroupHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadGroup_GetGraphNameAtFn_t)(
    QairtHtpContext_LoraPreloadGroupHandle_t, uint32_t, const char**);

typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadBufferGroups_CreateFn_t)(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t*);
typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadBufferGroups_FreeFn_t)(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t);
typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadBufferGroups_AddGroupFn_t)(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t, QairtHtpContext_LoraPreloadGroupHandle_t);
typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadBufferGroups_GetNumGroupsFn_t)(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadBufferGroups_GetGroupAtFn_t)(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t,
    uint32_t,
    QairtHtpContext_LoraPreloadGroupHandle_t*);
typedef Qairt_Status_t (*QairtHtpContext_LoraPreloadBufferGroups_ClearFn_t)(
    QairtHtpContext_LoraPreloadBufferGroupsHandle_t);

/* Reused IO Limit */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetReusedIoLimitMbFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint64_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetReusedIoLimitMbFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint64_t*);

/* Reference Weight Sharing Enabled */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetReferenceWeightSharingEnabledFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetReferenceWeightSharingEnabledFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

/* Concurrent Deserialization Patch */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetConcurrentDeserializationPatchFdFn_t)(
    QairtHtpContext_CustomConfigHandle_t, int32_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetConcurrentDeserializationPatchFdFn_t)(
    QairtHtpContext_CustomConfigHandle_t, int32_t*);

/* Context Properties */
typedef Qairt_Status_t (*QairtHtpContext_Properties_GetBufferStartAlignmentFn_t)(
    QairtContext_Handle_t, uint64_t*);

typedef Qairt_Status_t (*QairtHtpContext_Properties_GetSpillfillBufferSizeFn_t)(
    QairtContext_Handle_t, uint64_t*);

typedef Qairt_Status_t (*QairtHtpContext_Properties_GetWeightsBufferSizeFn_t)(
    QairtContext_Handle_t, uint64_t*);

typedef Qairt_Status_t (*QairtHtpContext_Properties_GetVtcmBackupBufferSizeFn_t)(
    QairtContext_Handle_t, uint64_t*);

/* Graph Split */
typedef Qairt_Status_t (*QairtHtpContext_GraphSplit_CreateFn_t)(
    QairtHtpContext_GraphSplitHandle_t*);

typedef Qairt_Status_t (*QairtHtpContext_GraphSplit_FreeFn_t)(QairtHtpContext_GraphSplitHandle_t);

typedef Qairt_Status_t (*QairtHtpContext_GraphSplit_SetGraphSplittingEnabledFn_t)(
    QairtHtpContext_GraphSplitHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_GraphSplit_GetGraphSplittingEnabledFn_t)(
    QairtHtpContext_GraphSplitHandle_t, uint8_t*);

/* Graph Split (CustomConfig association) */
typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetGraphSplitFn_t)(
    QairtHtpContext_CustomConfigHandle_t, QairtHtpContext_GraphSplitHandle_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetGraphSplitFn_t)(
    QairtHtpContext_CustomConfigHandle_t, QairtHtpContext_GraphSplitHandle_t*);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetGraphSplittingEnabledFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetGraphSplittingEnabledFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint8_t*);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_SetNumPrepareThreadsFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint32_t);

typedef Qairt_Status_t (*QairtHtpContext_CustomConfig_GetNumPrepareThreadsFn_t)(
    QairtHtpContext_CustomConfigHandle_t, uint32_t*);

//=============================================================================
// Public Interfaces
//=============================================================================

typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpContext_GroupRegistration_CreateFn_t create;
  QairtHtpContext_GroupRegistration_FreeFn_t free;
  QairtHtpContext_GroupRegistration_SetFirstGroupHandleFn_t setFirstGroupHandle;
  QairtHtpContext_GroupRegistration_GetFirstGroupHandleFn_t getFirstGroupHandle;
  QairtHtpContext_GroupRegistration_SetMaxSpillFillFn_t setMaxSpillFill;
  QairtHtpContext_GroupRegistration_GetMaxSpillFillFn_t getMaxSpillFill;
} QairtHtpContext_GroupRegistrationV1_t;

typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpContext_CustomConfig_CreateFn_t create;
  QairtHtpContext_CustomConfig_FreeFn_t free;
  QairtHtpContext_CustomConfig_SetEnableWeightSharingFn_t setEnableWeightSharing;
  QairtHtpContext_CustomConfig_GetEnableWeightSharingFn_t getEnableWeightSharing;
  QairtHtpContext_CustomConfig_SetGroupRegistrationFn_t setGroupRegistration;
  QairtHtpContext_CustomConfig_GetGroupRegistrationFn_t getGroupRegistration;
  QairtHtpContext_CustomConfig_SetFileReadMemoryBudgetFn_t setFileReadMemoryBudget;
  QairtHtpContext_CustomConfig_GetFileReadMemoryBudgetFn_t getFileReadMemoryBudget;
  QairtHtpContext_CustomConfig_SetEnableDspMemoryProfilingFn_t setEnableDspMemoryProfiling;
  QairtHtpContext_CustomConfig_GetEnableDspMemoryProfilingFn_t getEnableDspMemoryProfiling;
  QairtHtpContext_CustomConfig_SetShareResourcesFn_t setShareResources;
  QairtHtpContext_CustomConfig_GetShareResourcesFn_t getShareResources;
  QairtHtpContext_CustomConfig_SetIoMemEstimationFn_t setIoMemEstimation;
  QairtHtpContext_CustomConfig_GetIoMemEstimationFn_t getIoMemEstimation;
  QairtHtpContext_CustomConfig_SetIsPrepareOnlyFn_t setIsPrepareOnly;
  QairtHtpContext_CustomConfig_GetIsPrepareOnlyFn_t getIsPrepareOnly;
  QairtHtpContext_CustomConfig_SetInitAccelerationFn_t setInitAcceleration;
  QairtHtpContext_CustomConfig_GetInitAccelerationFn_t getInitAcceleration;
  QairtHtpContext_CustomConfig_SetSkipBinarySectionValidationFn_t setSkipBinarySectionValidation;
  QairtHtpContext_CustomConfig_GetSkipBinarySectionValidationFn_t getSkipBinarySectionValidation;
  QairtHtpContext_CustomConfig_SetShareResourcesOptimizationTypeFn_t
      setShareResourcesOptimizationType;
  QairtHtpContext_CustomConfig_GetShareResourcesOptimizationTypeFn_t
      getShareResourcesOptimizationType;
  QairtHtpContext_CustomConfig_SetUseExtendedUdmaFn_t setUseExtendedUdma;
  QairtHtpContext_CustomConfig_GetUseExtendedUdmaFn_t getUseExtendedUdma;
  QairtHtpContext_CustomConfig_SetConcurrentGroupRegistrationFn_t setConcurrentGroupRegistration;
  QairtHtpContext_CustomConfig_GetConcurrentGroupRegistrationFn_t getConcurrentGroupRegistration;
  QairtHtpContext_CustomConfig_SetLoraWeightSharingEnabledFn_t setLoraWeightSharingEnabled;
  QairtHtpContext_CustomConfig_GetLoraWeightSharingEnabledFn_t getLoraWeightSharingEnabled;
  QairtHtpContext_CustomConfig_SetGraphSplitFn_t setGraphSplit;
  QairtHtpContext_CustomConfig_GetGraphSplitFn_t getGraphSplit;
  QairtHtpContext_CustomConfig_SetGraphSplittingEnabledFn_t setGraphSplittingEnabled;
  QairtHtpContext_CustomConfig_GetGraphSplittingEnabledFn_t getGraphSplittingEnabled;
  QairtHtpContext_CustomConfig_SetNumPrepareThreadsFn_t setNumPrepareThreads;
  QairtHtpContext_CustomConfig_GetNumPrepareThreadsFn_t getNumPrepareThreads;
  QairtHtpContext_CustomConfig_SetLoraWeightSharingRamPreloadEnabledFn_t
      setLoraWeightSharingRamPreloadEnabled;
  QairtHtpContext_CustomConfig_GetLoraWeightSharingRamPreloadEnabledFn_t
      getLoraWeightSharingRamPreloadEnabled;
  QairtHtpContext_CustomConfig_SetReusedIoLimitMbFn_t setReusedIoLimitMb;
  QairtHtpContext_CustomConfig_GetReusedIoLimitMbFn_t getReusedIoLimitMb;
  QairtHtpContext_CustomConfig_SetReferenceWeightSharingEnabledFn_t
      setReferenceWeightSharingEnabled;
  QairtHtpContext_CustomConfig_GetReferenceWeightSharingEnabledFn_t
      getReferenceWeightSharingEnabled;
  QairtHtpContext_CustomConfig_SetConcurrentDeserializationPatchFdFn_t
      setConcurrentDeserializationPatchFd;
  QairtHtpContext_CustomConfig_GetConcurrentDeserializationPatchFdFn_t
      getConcurrentDeserializationPatchFd;
  QairtHtpContext_CustomConfig_SetLoraPreloadBufferGroupsFn_t setLoraPreloadBufferGroups;
  QairtHtpContext_CustomConfig_GetLoraPreloadBufferGroupsFn_t getLoraPreloadBufferGroups;
} QairtHtpContext_CustomConfigV1_t;

typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpContext_LoraPreloadGroup_CreateFn_t create;
  QairtHtpContext_LoraPreloadGroup_FreeFn_t free;
  QairtHtpContext_LoraPreloadGroup_SetGraphNamesFn_t setGraphNames;
  QairtHtpContext_LoraPreloadGroup_GetNumGraphsFn_t getNumGraphs;
  QairtHtpContext_LoraPreloadGroup_GetGraphNameAtFn_t getGraphNameAt;
} QairtHtpContext_LoraPreloadGroupV1_t;

typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpContext_LoraPreloadBufferGroups_CreateFn_t create;
  QairtHtpContext_LoraPreloadBufferGroups_FreeFn_t free;
  QairtHtpContext_LoraPreloadBufferGroups_AddGroupFn_t addGroup;
  QairtHtpContext_LoraPreloadBufferGroups_GetNumGroupsFn_t getNumGroups;
  QairtHtpContext_LoraPreloadBufferGroups_GetGroupAtFn_t getGroupAt;
  QairtHtpContext_LoraPreloadBufferGroups_ClearFn_t clear;
} QairtHtpContext_LoraPreloadBufferGroupsV1_t;

typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpContext_GraphSplit_CreateFn_t create;
  QairtHtpContext_GraphSplit_FreeFn_t free;
  QairtHtpContext_GraphSplit_SetGraphSplittingEnabledFn_t setGraphSplittingEnabled;
  QairtHtpContext_GraphSplit_GetGraphSplittingEnabledFn_t getGraphSplittingEnabled;
} QairtHtpContext_GraphSplitV1_t;

/**
 * @struct QairtHtpContext_PropertiesV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT HTP Context Properties.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtHtpContext_Properties_GetBufferStartAlignmentFn_t getBufferStartAlignment;
  QairtHtpContext_Properties_GetSpillfillBufferSizeFn_t getSpillfillBufferSize;
  QairtHtpContext_Properties_GetWeightsBufferSizeFn_t getWeightsBufferSize;
  QairtHtpContext_Properties_GetVtcmBackupBufferSizeFn_t getVtcmBackupBufferSize;
} QairtHtpContext_PropertiesV1_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
