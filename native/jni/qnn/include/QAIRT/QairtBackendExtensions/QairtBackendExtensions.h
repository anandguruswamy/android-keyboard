//==============================================================================
//
// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// All rights reserved.
// Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

/**
 * @file
 * @brief   Backend extensions component API
 *
 *          A header which contains the QAIRT Backend Extensions component and
 *          the interfaces it implements.
 */

#ifndef QAISW_QAIRTBACKENDEXT_H
#define QAISW_QAIRTBACKENDEXT_H

#include "QairtBackend/QairtBackendConfig.h"
#include "QairtBackendExtensions/QairtBackendExtensionsAllocator.h"
#include "QairtBackendExtensions/QairtBackendExtensionsCommon.h"
#include "QairtContext/QairtContextConfig.h"
#include "QairtDevice/QairtDeviceConfig.h"
#include "QairtGraph/QairtGraphConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Macros
//=============================================================================
#define QAIRT_BACKEND_EXT_ID QAIRT_MIN_ID_BACKEND_EXT

//=============================================================================
// Datatypes
//=============================================================================
/**
 * @brief QAIRT Backend Extensions Performance Profiles
 */
typedef enum {
  /// Balanced performance/power profile, biased slightly toward power savings.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LOW_BALANCED                          = 0,
  /// Balanced performance and power consumption.
  QAIRT_BACKEND_EXT_PERF_PROFILE_BALANCED                              = 1,
  /// Default performance profile chosen by the backend.
  QAIRT_BACKEND_EXT_PERF_PROFILE_DEFAULT                               = 2,
  /// Maximizes performance for a short burst of execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_HIGH_PERFORMANCE                      = 3,
  /// Maximizes performance while sustaining it over a longer duration.
  QAIRT_BACKEND_EXT_PERF_PROFILE_SUSTAINED_HIGH_PERFORMANCE            = 4,
  /// Maximizes performance for the shortest, most latency-critical bursts of
  /// execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_BURST                                 = 5,
  /// Burst profile tuned for LLM decode (token-by-token) execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_BURST                      = 6,
  /// Low-balanced profile tuned for LLM decode execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_LOW_BALANCED               = 7,
  /// Balanced profile tuned for LLM decode execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_BALANCED                   = 8,
  /// Default profile tuned for LLM decode execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_DEFAULT                    = 9,
  /// High-performance profile tuned for LLM decode execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_HIGH_PERFORMANCE           = 10,
  /// Sustained high-performance profile tuned for LLM decode execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_SUSTAINED_HIGH_PERFORMANCE = 11,
  /// Extreme power saver profile tuned for LLM decode execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_EXTREME_POWER_SAVER        = 12,
  /// Low power saver profile tuned for LLM decode execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_LOW_POWER_SAVER            = 13,
  /// Power saver profile tuned for LLM decode execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_POWER_SAVER                = 14,
  /// High power saver profile tuned for LLM decode execution.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LLM_DECODE_HIGH_POWER_SAVER           = 15,
  /// Maximizes power savings at the expense of performance.
  QAIRT_BACKEND_EXT_PERF_PROFILE_EXTREME_POWER_SAVER                   = 16,
  /// Favors power savings over performance, to a lesser degree than
  /// QAIRT_BACKEND_EXT_PERF_PROFILE_EXTREME_POWER_SAVER.
  QAIRT_BACKEND_EXT_PERF_PROFILE_LOW_POWER_SAVER                       = 17,
  /// Favors power savings over performance.
  QAIRT_BACKEND_EXT_PERF_PROFILE_POWER_SAVER                           = 18,
  /// Favors power savings over performance, to a greater degree than
  /// QAIRT_BACKEND_EXT_PERF_PROFILE_POWER_SAVER.
  QAIRT_BACKEND_EXT_PERF_PROFILE_HIGH_POWER_SAVER                      = 19,
  /// Defers to the system-wide power/performance settings rather than an
  /// explicit profile.
  QAIRT_BACKEND_EXT_PERF_PROFILE_SYSTEM_SETTINGS                       = 20,
  /// Indicates no explicit performance profile was requested by the user.
  QAIRT_BACKEND_EXT_PERF_PROFILE_NO_USER_INPUT                         = 21,
  /// A custom performance profile defined outside the predefined set.
  QAIRT_BACKEND_EXT_PERF_PROFILE_CUSTOM                                = 22,
  /// Not a valid performance profile.
  QAIRT_BACKEND_EXT_PERF_PROFILE_INVALID                               = 23,
  // Unused, present to ensure 32 bits.
  QAIRT_BACKEND_EXT_PERF_PROFILE_UNDEFINED = 0x7FFFFFFF
} QairtBackendExt_PerfProfile_t;


/**
 * @brief   Backend extensions handle used to interface with Backend Extensions
 *          APIs
 *
 *          The handle itself is created and freed by the client.
 */
typedef struct _QairtBackendExt_Handle_t* QairtBackendExt_Handle_t;

//=============================================================================
// Public Functions
//=============================================================================

/**
 * @brief Create a backend extensions handle.
 *
 * @param[out] backendExtHandle A handle to the created backend extensions
 *                               object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Backend extensions handle was successfully
 *           created.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _backendExtHandle_
 *           is NULL.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_create(QairtBackendExt_Handle_t* backendExtHandle);

/**
 * @brief Free a backend extensions handle and associated resources.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Backend extensions handle was successfully
 *           freed.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_free(QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Initialize a backend extensions object with the backend interface it
 *        extends.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] qairtBackendInterface Pointer to the QAIRT backend interface
 *                                  (e.g. QairtBackend_V1_t) that this backend
 *                                  extensions object augments.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Backend extensions object was successfully
 *           initialized.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT:
 *           _qairtBackendInterface_ is NULL.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_initialize(QairtBackendExt_Handle_t backendExtHandle,
                                          void* qairtBackendInterface);

/**
 * @brief Set the performance profile the backend extensions object should
 *        apply.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] perfProfile The desired performance profile.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Performance profile was successfully set.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _perfProfile_ is
 *           invalid.
 *         - QAIRT_BACKEND_EXT_ERROR_UNSUPPORTED_FEATURE: _perfProfile_ is
 *           not supported.
 *         - QAIRT_BACKEND_EXT_ERROR_UNINITIALIZED: Called before
 *           QairtBackendExt_initialize().
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_setPerfProfile(QairtBackendExt_Handle_t backendExtHandle,
                                              QairtBackendExt_PerfProfile_t perfProfile);


/**
 * @brief Load configuration options for the backend extensions object from a
 *        JSON string.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] jsonConfiguration A JSON-formatted configuration string.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Configuration was successfully loaded.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _jsonConfiguration_
 *           is NULL or malformed.
 *         - QAIRT_BACKEND_EXT_ERROR_UNINITIALIZED: Called before
 *           QairtBackendExt_initialize().
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_loadConfig(QairtBackendExt_Handle_t backendExtHandle,
                                          const char* jsonConfiguration);

/**
 * @brief Provide a logger for the backend extensions object to use.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] logHandle A handle to the logger. Use a NULL handle to disable
 *                       logging. QairtBackendExt does not manage the
 *                       lifecycle of the logger; it must be freed by the
 *                       caller using QairtLog_free().
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Logging was successfully set up.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ or
 *           _logHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_setupLogging(QairtBackendExt_Handle_t backendExtHandle,
                                            QairtLog_Handle_t logHandle);

/**
 * @brief Query the profiling level requested by the backend extensions
 *        object.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[out] profileLevel Populated with the requested profiling level.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Profiling level was successfully retrieved.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _profileLevel_ is
 *           NULL.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_getProfilingLevel(QairtBackendExt_Handle_t backendExtHandle,
                                                 uint32_t* profileLevel);

/**
 * @brief Hook invoked before a backend is initialized, allowing the extension
 *        to inspect or modify the backend configuration before it takes
 *        effect.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] configHandle A handle to the backend configuration about to be
 *                          applied.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ or
 *           _configHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeBackendInitialize(
    QairtBackendExt_Handle_t backendExtHandle, QairtBackend_ConfigHandle_t configHandle);

/**
 * @brief Hook invoked after a backend has finished initializing.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterBackendInitialize(
    QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before a context is created, allowing the extension to
 *        inspect or modify the context configuration before it takes effect.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] configHandle A handle to the context configuration about to be
 *                          applied.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ or
 *           _configHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeContextCreate(QairtBackendExt_Handle_t backendExtHandle,
                                                   QairtContext_ConfigHandle_t configHandle);

/**
 * @brief Hook invoked after a context has been created.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterContextCreate(QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before a set of graphs are composed within a context,
 *        allowing the extension to inspect or modify each graph's
 *        configuration before composition takes effect.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] numGraphs Number of graphs about to be composed.
 * @param[in] graphNames Array of _numGraphs_ graph names.
 * @param[in] configHandles Array of _numGraphs_ graph configuration
 *                          handles, one per entry in _graphNames_.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _graphNames_ or
 *           _configHandles_ is NULL.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeComposeGraphs(
    QairtBackendExt_Handle_t backendExtHandle,
    uint32_t numGraphs,
    const char** graphNames,
    QairtGraph_ConfigHandle_t* configHandles);

/**
 * @brief Hook invoked after a set of graphs have been composed.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterComposeGraphs(QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before a graph is finalized, allowing the extension to
 *        inspect or modify its configuration before finalization takes
 *        effect.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] graphName Name of the graph about to be finalized.
 * @param[in] graphHandle A handle to the graph about to be finalized.
 * @param[in] configHandle A handle to the configuration about to be applied
 *                          during finalization.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ or
 *           _graphHandle_ is not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _graphName_ is NULL.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeGraphFinalize(QairtBackendExt_Handle_t backendExtHandle,
                                                   const char* graphName,
                                                   QairtGraph_Handle_t graphHandle,
                                                   QairtGraph_ConfigHandle_t configHandle);

/**
 * @brief Hook invoked after a graph has been finalized.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterGraphFinalize(QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before a graph is executed, allowing the extension to
 *        inspect or modify its execution configuration before execution
 *        takes effect.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] graphName Name of the graph about to be executed.
 * @param[in] configHandle A handle to the configuration about to be applied
 *                          during execution.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _graphName_ is NULL.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeExecute(QairtBackendExt_Handle_t backendExtHandle,
                                             const char* graphName,
                                             QairtGraph_ConfigHandle_t configHandle);

/**
 * @brief Hook invoked after a graph has been executed.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterExecute(QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before one or more contexts are freed.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] numContexts Number of contexts about to be freed.
 * @param[in] contextHandle Array of _numContexts_ context handles about to
 *                          be freed.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _contextHandle_ is
 *           NULL.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeContextFree(QairtBackendExt_Handle_t backendExtHandle,
                                                 uint32_t numContexts,
                                                 QairtContext_Handle_t* contextHandle);

/**
 * @brief Hook invoked after one or more contexts have been freed.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterContextFree(QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before the backend is terminated, allowing the
 *        extension to release resources tied to the backend's lifetime.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeBackendTerminate(
    QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked after the backend has terminated.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterBackendTerminate(
    QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before a context is created from a binary, allowing
 *        the extension to inspect or modify the context configuration
 *        before it takes effect.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] configHandle A handle to the context configuration about to be
 *                          applied.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ or
 *           _configHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeCreateFromBinary(
    QairtBackendExt_Handle_t backendExtHandle, QairtContext_ConfigHandle_t configHandle);

/**
 * @brief Hook invoked after a context has been created from a binary.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterCreateFromBinary(
    QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before multiple contexts are created from a list of
 *        binaries, allowing the extension to inspect or modify each
 *        context's configuration before creation takes effect.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] numContexts Number of contexts about to be created.
 * @param[in] contextKeys Array of _numContexts_ identifying keys, one per
 *                        context.
 * @param[in] configs Array of _numContexts_ per-context configuration
 *                     handles, one per entry in _contextKeys_.
 * @param[in] commonConfig A handle to configuration shared across all
 *                         contexts in the list.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ or
 *           _commonConfig_ is not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _contextKeys_ or
 *           _configs_ is NULL.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeCreateContextsFromBinaryList(
    QairtBackendExt_Handle_t backendExtHandle,
    uint32_t numContexts,
    const char** contextKeys,
    QairtContext_ConfigHandle_t* configs,
    QairtContext_ConfigHandle_t commonConfig);

/**
 * @brief Hook invoked after all contexts in a binary list have been created.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterCreateContextsFromBinaryList(
    QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before a device is created, allowing the extension to
 *        inspect or modify the device configuration before it takes effect.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] configHandle A handle to the device configuration about to be
 *                          applied.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ or
 *           _configHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeCreateDevice(QairtBackendExt_Handle_t backendExtHandle,
                                                  QairtDevice_ConfigHandle_t configHandle);

/**
 * @brief Hook invoked after a device has been created.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterCreateDevice(QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before a device is freed.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeFreeDevice(QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked after a device has been freed.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterFreeDevice(QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before a context is activated, allowing the extension
 *        to inspect or modify the activation configuration before it takes
 *        effect.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] configHandle A handle to the configuration about to be applied
 *                          during activation.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ or
 *           _configHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeActivateContext(
    QairtBackendExt_Handle_t backendExtHandle, QairtContext_ConfigHandle_t configHandle);

/**
 * @brief Hook invoked after a context has been activated.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterActivateContext(QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked before a context is deactivated, allowing the
 *        extension to inspect or modify the deactivation configuration
 *        before it takes effect.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[in] configHandle A handle to the configuration about to be applied
 *                          during deactivation.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ or
 *           _configHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeDeactivateContext(
    QairtBackendExt_Handle_t backendExtHandle, QairtContext_ConfigHandle_t configHandle);

/**
 * @brief Hook invoked after a context has been deactivated.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterDeactivateContext(
    QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Retrieve the allocator implemented by this backend
 *        extensions object.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 * @param[out] allocator A handle to the backend extensions allocator.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Allocator was successfully retrieved.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _allocator_ is NULL.
 *         - QAIRT_BACKEND_EXT_ERROR_UNSUPPORTED_FEATURE: This backend
 *           extensions object does not implement a buffer allocator.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_getAllocator(QairtBackendExt_Handle_t backendExtHandle,
                                            QairtBackendExt_Allocator_Handle_t* allocator);

/**
 * @brief Hook invoked before a binary section is applied to a context.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_beforeContextApplyBinarySection(
    QairtBackendExt_Handle_t backendExtHandle);

/**
 * @brief Hook invoked after a binary section has been applied to a context.
 *
 * @param[in] backendExtHandle A handle to a created backend extensions
 *                              object.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _backendExtHandle_ is
 *           not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_t.
 */
Qairt_Status_t QairtBackendExt_afterContextApplyBinarySection(
    QairtBackendExt_Handle_t backendExtHandle);


typedef Qairt_Status_t (*QairtBackendExt_CreateFn_t)(QairtBackendExt_Handle_t*);

typedef Qairt_Status_t (*QairtBackendExt_FreeFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_InitializeFn_t)(QairtBackendExt_Handle_t,
                                                              void*);

typedef Qairt_Status_t (*QairtBackendExt_SetPerfProfileFn_t)(QairtBackendExt_Handle_t,
                                                                  QairtBackendExt_PerfProfile_t);

typedef Qairt_Status_t (*QairtBackendExt_LoadConfigFn_t)(QairtBackendExt_Handle_t,
                                                              const char*);

typedef Qairt_Status_t (*QairtBackendExt_SetupLoggingFn_t)(QairtBackendExt_Handle_t,
                                                                QairtLog_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_GetProfilingLevelFn_t)(QairtBackendExt_Handle_t,
                                                                     uint32_t*);

typedef Qairt_Status_t (*QairtBackendExt_BeforeBackendInitializeFn_t)(
    QairtBackendExt_Handle_t, QairtBackend_ConfigHandle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterBackendInitializeFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeContextCreateFn_t)(
    QairtBackendExt_Handle_t, QairtContext_ConfigHandle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterContextCreateFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeComposeGraphsFn_t)(
    QairtBackendExt_Handle_t, uint32_t, const char**, QairtGraph_ConfigHandle_t*);

typedef Qairt_Status_t (*QairtBackendExt_AfterComposeGraphsFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeGraphFinalizeFn_t)(QairtBackendExt_Handle_t,
                                                                  const char*,
                                                                  QairtGraph_Handle_t,
                                                                  QairtGraph_ConfigHandle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterGraphFinalizeFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeExecuteFn_t)(QairtBackendExt_Handle_t,
                                                                 const char*,
                                                                 QairtGraph_ConfigHandle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterExecuteFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeContextFreeFn_t)(QairtBackendExt_Handle_t, uint32_t, QairtContext_Handle_t*);

typedef Qairt_Status_t (*QairtBackendExt_AfterContextFreeFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeBackendTerminateFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterBackendTerminateFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeCreateFromBinaryFn_t)(
    QairtBackendExt_Handle_t, QairtContext_ConfigHandle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterCreateFromBinaryFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeCreateContextsFromBinaryListFn_t)(
    QairtBackendExt_Handle_t,
    uint32_t,
    const char**,
    QairtContext_ConfigHandle_t*,
    QairtContext_ConfigHandle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterCreateContextsFromBinaryListFn_t)(
    QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeCreateDeviceFn_t)(QairtBackendExt_Handle_t,
                                                                 QairtDevice_ConfigHandle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterCreateDeviceFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeFreeDeviceFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterFreeDeviceFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeActivateContextFn_t)(
    QairtBackendExt_Handle_t, QairtContext_ConfigHandle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterActivateContextFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_BeforeDeactivateContextFn_t)(
    QairtBackendExt_Handle_t, QairtContext_ConfigHandle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterDeactivateContextFn_t)(QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_GetAllocatorFn_t)(QairtBackendExt_Handle_t,
                                                           QairtBackendExt_Allocator_Handle_t*);

typedef Qairt_Status_t (*QairtBackendExt_BeforeContextApplyBinarySectionFn_t)(
    QairtBackendExt_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_AfterContextApplyBinarySectionFn_t)(
    QairtBackendExt_Handle_t);

//=============================================================================
// Public Interfaces
//=============================================================================
/**
 * @struct QairtBackendExt_t
 * @brief This struct contains the functions that define the interface for
 *        QAIRT Backend Extensions. This interface can be retrieved through the
 *        id QAIRT_BACKEND_EXT_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtBackendExt_CreateFn_t create;
  QairtBackendExt_FreeFn_t free;
  QairtBackendExt_InitializeFn_t initialize;
  QairtBackendExt_SetPerfProfileFn_t setPerfProfile;
  QairtBackendExt_LoadConfigFn_t loadConfig;
  QairtBackendExt_SetupLoggingFn_t setupLogging;
  QairtBackendExt_GetProfilingLevelFn_t getProfilingLevel;
  QairtBackendExt_BeforeBackendInitializeFn_t beforeBackendInitialize;
  QairtBackendExt_AfterBackendInitializeFn_t afterBackendInitialize;
  QairtBackendExt_BeforeContextCreateFn_t beforeContextCreate;
  QairtBackendExt_AfterContextCreateFn_t afterContextCreate;
  QairtBackendExt_BeforeComposeGraphsFn_t beforeComposeGraphs;
  QairtBackendExt_AfterComposeGraphsFn_t afterComposeGraphs;
  QairtBackendExt_BeforeGraphFinalizeFn_t beforeGraphFinalize;
  QairtBackendExt_AfterGraphFinalizeFn_t afterGraphFinalize;
  QairtBackendExt_BeforeExecuteFn_t beforeExecute;
  QairtBackendExt_AfterExecuteFn_t afterExecute;
  QairtBackendExt_BeforeContextFreeFn_t beforeContextFree;
  QairtBackendExt_AfterContextFreeFn_t afterContextFree;
  QairtBackendExt_BeforeBackendTerminateFn_t beforeBackendTerminate;
  QairtBackendExt_AfterBackendTerminateFn_t afterBackendTerminate;
  QairtBackendExt_BeforeCreateFromBinaryFn_t beforeCreateFromBinary;
  QairtBackendExt_AfterCreateFromBinaryFn_t afterCreateFromBinary;
  QairtBackendExt_BeforeCreateContextsFromBinaryListFn_t beforeCreateContextsFromBinaryList;
  QairtBackendExt_AfterCreateContextsFromBinaryListFn_t afterCreateContextsFromBinaryList;
  QairtBackendExt_BeforeCreateDeviceFn_t beforeCreateDevice;
  QairtBackendExt_AfterCreateDeviceFn_t afterCreateDevice;
  QairtBackendExt_BeforeFreeDeviceFn_t beforeFreeDevice;
  QairtBackendExt_AfterFreeDeviceFn_t afterFreeDevice;
  QairtBackendExt_BeforeActivateContextFn_t beforeActivateContext;
  QairtBackendExt_AfterActivateContextFn_t afterActivateContext;
  QairtBackendExt_BeforeDeactivateContextFn_t beforeDeactivateContext;
  QairtBackendExt_AfterDeactivateContextFn_t afterDeactivateContext;
  QairtBackendExt_GetAllocatorFn_t getAllocator;
  QairtBackendExt_BeforeContextApplyBinarySectionFn_t beforeContextApplyBinarySection;
  QairtBackendExt_AfterContextApplyBinarySectionFn_t afterContextApplyBinarySection;
} QairtBackendExt_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAISW_QAIRTBACKENDEXT_H
