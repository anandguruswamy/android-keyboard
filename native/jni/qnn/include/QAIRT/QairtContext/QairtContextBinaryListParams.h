//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================

/**
 * @file
 * @brief   Context Binary List Params component API
 *
 *          A header which contains the QAIRT context binary list params component, used to
 *          describe a single context's inputs for QairtContext_createFromBinaryListAsync,
 *          and the interfaces it implements.
 */

#ifndef QAISW_QAIRTCONTEXTBINARYLISTPARAMS_H
#define QAISW_QAIRTCONTEXTBINARYLISTPARAMS_H

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
extern "C" {
#else
#include <stddef.h>
#include <stdint.h>
#endif

#include "QairtContextBinary.h"
#include "QairtContextCommon.h"
#include "QairtContextConfig.h"
#include "QairtGraph/QairtGraph.h"
#include "QairtProfile/QairtProfile.h"

//=============================================================================
// Macros
//=============================================================================

/// Interface id for the QAIRT Context Binary List Notify Callback V1 interface.
#define QAIRT_CONTEXT_BINARY_LIST_NOTIFY_CALLBACK_V1_ID QAIRT_MIN_ID_CONTEXT + 7

/// Interface id for the QAIRT Context Binary List Raw Buffer Callback V1 interface.
#define QAIRT_CONTEXT_BINARY_LIST_RAW_BUFFER_CALLBACK_V1_ID QAIRT_MIN_ID_CONTEXT + 8

/// Interface id for the QAIRT Context Binary List Dma Buffer Callback V1 interface.
#define QAIRT_CONTEXT_BINARY_LIST_DMA_BUFFER_CALLBACK_V1_ID QAIRT_MIN_ID_CONTEXT + 9

/// Interface id for the QAIRT Context Binary Data Request V1 interface.
#define QAIRT_CONTEXT_BINARY_DATA_REQUEST_V1_ID QAIRT_MIN_ID_CONTEXT + 10

/// Interface id for the QAIRT Context Binary Raw Data Response V1 interface.
#define QAIRT_CONTEXT_BINARY_RAW_DATA_RESPONSE_V1_ID QAIRT_MIN_ID_CONTEXT + 11

/// Interface id for the QAIRT Context Binary Dma Data Response V1 interface.
#define QAIRT_CONTEXT_BINARY_DMA_DATA_RESPONSE_V1_ID QAIRT_MIN_ID_CONTEXT + 12

/// Interface id for the QAIRT Context Binary Raw Data Mem V1 interface.
#define QAIRT_CONTEXT_BINARY_RAW_DATA_MEM_V1_ID QAIRT_MIN_ID_CONTEXT + 13

/// Interface id for the QAIRT Context Binary Dma Data Mem V1 interface.
#define QAIRT_CONTEXT_BINARY_DMA_DATA_MEM_V1_ID QAIRT_MIN_ID_CONTEXT + 14

/// Sentinel invalid file descriptor for a dma buffer, mirrors QNN_MEM_INVALID_FD.
#define QAIRT_CONTEXT_MEM_INVALID_FD -1

//=============================================================================
// Data Types
//=============================================================================

/**
 * @brief   Context Binary List Params handle used to interface with Context Binary List Params APIs
 *
 *          The handle itself is created and freed by the client.
 */
typedef struct _QairtContext_BinaryListParamsHandle_t* QairtContext_BinaryListParamsHandle_t;

/**
 * @brief   Context Binary List Notify Callback handle used to interface with Context Binary
 *          List Notify Callback APIs
 *
 *          The handle itself is created and freed by the client.
 */
typedef struct _QairtContext_BinaryListNotifyCallbackHandle_t*
    QairtContext_BinaryListNotifyCallbackHandle_t;

/**
 * @brief   Context Binary List Raw Buffer Callback handle used to interface with Context Binary
 *          List Raw Buffer Callback APIs
 *
 *          The handle itself is created and freed by the client.
 */
typedef struct _QairtContext_BinaryListRawBufferCallbackHandle_t*
    QairtContext_BinaryListRawBufferCallbackHandle_t;

/**
 * @brief   Context Binary List Dma Buffer Callback handle used to interface with Context Binary
 *          List Dma Buffer Callback APIs
 *
 *          The handle itself is created and freed by the client.
 */
typedef struct _QairtContext_BinaryListDmaBufferCallbackHandle_t*
    QairtContext_BinaryListDmaBufferCallbackHandle_t;

/**
 * @brief   Context Binary Data Request handle used to interface with Context Binary Data
 *          Request APIs
 *
 *          The handle itself is created and freed by the client.
 */
typedef struct _QairtContext_BinaryDataRequestHandle_t* QairtContext_BinaryDataRequestHandle_t;

/**
 * @brief   Context Binary Raw Data Response handle used to interface with Context Binary Raw
 *          Data Response APIs
 *
 *          The handle itself is created and freed by the client.
 */
typedef struct _QairtContext_BinaryRawDataResponseHandle_t*
    QairtContext_BinaryRawDataResponseHandle_t;

/**
 * @brief   Context Binary Dma Data Response handle used to interface with Context Binary Dma
 *          Data Response APIs
 *
 *          The handle itself is created and freed by the client.
 */
typedef struct _QairtContext_BinaryDmaDataResponseHandle_t*
    QairtContext_BinaryDmaDataResponseHandle_t;

/**
 * @brief   Context Binary Raw Data Mem handle used to interface with Context Binary Raw Data
 *          Mem APIs
 *
 *          The handle itself is created and freed by the client.
 */
typedef struct _QairtContext_BinaryRawDataMemHandle_t* QairtContext_BinaryRawDataMemHandle_t;

/**
 * @brief   Context Binary Dma Data Mem handle used to interface with Context Binary Dma Data
 *          Mem APIs
 *
 *          The handle itself is created and freed by the client.
 */
typedef struct _QairtContext_BinaryDmaDataMemHandle_t* QairtContext_BinaryDmaDataMemHandle_t;

/**
 * @brief A client-defined callback function to provide context binary data in a raw buffer.
 *        This callback is used for allocating external raw buffers and loading data.
 *
 * @param[in] request A handle containing the request for context binary data.
 *
 * @param[out] response A handle to be filled with the response to the request.
 *
 * @param[in] userData Client-supplied data object which may be used to identify
 *                      which context's instance this callback applies to.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: no error is encountered
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: memory allocation error
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: one of the arguments to the API is invalid/NULL
 */
typedef Qairt_Status_t (*QairtContext_BinaryRawDataProviderFn_t)(
    QairtContext_BinaryDataRequestHandle_t request,
    QairtContext_BinaryRawDataResponseHandle_t response,
    void* userData);

/**
 * @brief A client-defined callback function to release context binary data in a raw buffer.
 *
 * @param[in] mem A handle holding properties of the raw buffer.
 *
 * @param[in] userData Client-supplied data object which may be used to identify
 *                      which context's instance this callback applies to.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: no error is encountered
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: memory allocation error
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: one of the arguments to the API is invalid/NULL
 */
typedef Qairt_Status_t (*QairtContext_BinaryRawDataReleaseFn_t)(
    QairtContext_BinaryRawDataMemHandle_t mem, void* userData);

/**
 * @brief A client-defined callback function to provide context binary data in a dma buffer.
 *        This callback is used for allocating external dma buffers and loading data.
 *
 * @param[in] request A handle containing the request for context binary data.
 *
 * @param[out] response A handle to be filled with the response to the request.
 *
 * @param[in] userData Client-supplied data object which may be used to identify
 *                      which context's instance this callback applies to.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: no error is encountered
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: memory allocation error
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: one of the arguments to the API is invalid/NULL
 */
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataProviderFn_t)(
    QairtContext_BinaryDataRequestHandle_t request,
    QairtContext_BinaryDmaDataResponseHandle_t response,
    void* userData);

/**
 * @brief A client-defined callback function to release context binary data in a dma buffer.
 *
 * @param[in] mem A handle holding properties of the dma buffer.
 *
 * @param[in] userData Client-supplied data object which may be used to identify
 *                      which context's instance this callback applies to.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: no error is encountered
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: memory allocation error
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: one of the arguments to the API is invalid/NULL
 */
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataReleaseFn_t)(
    QairtContext_BinaryDmaDataMemHandle_t mem, void* userData);

/**
 * @brief Notify type delivered to the callback registered via
 *        QairtContext_BinaryListParams_setNotify.
 */
typedef enum {
  QAIRT_CONTEXT_NOTIFY_TYPE_GRAPH_INIT   = 0,
  QAIRT_CONTEXT_NOTIFY_TYPE_CONTEXT_INIT = 1,
  QAIRT_CONTEXT_NOTIFY_TYPE_UNDEFINED    = 0x7FFFFFF
} QairtContext_CreateFromBinaryAsyncNotifyType_t;

// Forward declarations for handle types used in callbacks.
typedef struct _QairtContext_Handle_t* QairtContext_Handle_t;

/**
 * @brief Callback invoked by QairtContext_createFromBinaryListAsync for each graph
 *        initialised and once when the overall context is ready.
 *
 * @param[in] contextHandle Handle to the context being created (valid only for
 *                          QAIRT_CONTEXT_NOTIFY_TYPE_CONTEXT_INIT notifications)
 * @param[in] graphHandle   Handle to the graph (valid only for QAIRT_CONTEXT_NOTIFY_TYPE_GRAPH_INIT
 *                          notifications).
 * @param[in] graphName     Name of the graph (valid only for QAIRT_CONTEXT_NOTIFY_TYPE_GRAPH_INIT
 *                          notifications).
 * @param[in] notifyType    Whether this is a per-graph or per-context notification.
 * @param[in] userData      User-supplied data pointer registered via
 *                          QairtContext_BinaryListParams_setNotify.
 * @param[in] status        QAIRT_SUCCESS or an error code for this notification.
 */
typedef void (*QairtContext_CreateFromBinaryListAsyncNotifyFn_t)(
    QairtContext_Handle_t contextHandle,
    QairtGraph_Handle_t graphHandle,
    const char* graphName,
    QairtContext_CreateFromBinaryAsyncNotifyType_t notifyType,
    void* userData,
    Qairt_Status_t status);

//=============================================================================
// Public Functions
//=============================================================================

/**
 * @brief Create a context binary list params handle, used to describe a single context's
 *        inputs for QairtContext_createFromBinaryListAsync.
 *
 * @param[out] paramsHandle A handle to the created context binary list params.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Context binary list params was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _paramsHandle_ is NULL.
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory allocation error while
 *           creating context binary list params.
 *
 * @note Use corresponding API through QairtContext_BinaryListParamsV1_t.
 */
Qairt_Status_t QairtContext_BinaryListParams_create(
    QairtContext_BinaryListParamsHandle_t* paramsHandle);

/**
 * @brief Free a context binary list params handle and associated resources.
 *
 * @param[in] paramsHandle A handle to a created context binary list params.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Context binary list params was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _paramsHandle_ is not a
 *           valid handle.
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory deallocation error.
 *
 * @note Use corresponding API through QairtContext_BinaryListParamsV1_t.
 */
Qairt_Status_t QairtContext_BinaryListParams_free(
    QairtContext_BinaryListParamsHandle_t paramsHandle);

/**
 * @brief Set the per-context configuration to use when creating this context.
 *
 * @param[in] paramsHandle A handle to a created context binary list params.
 *
 * @param[in] configHandle A handle to a created context configuration; may be NULL
 *                          for default configuration.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Config was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _paramsHandle_ is not a
 *           valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryListParamsV1_t.
 */
Qairt_Status_t QairtContext_BinaryListParams_setConfig(
    QairtContext_BinaryListParamsHandle_t paramsHandle, QairtContext_ConfigHandle_t configHandle);

/**
 * @brief Set the binary buffer to deserialize this context from.
 *
 * @param[in] paramsHandle A handle to a created context binary list params.
 *
 * @param[in] binaryBufferHandle A handle to a created binary buffer containing the
 *                                serialized context.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Binary buffer was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _paramsHandle_ is not a
 *           valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryListParamsV1_t.
 */
Qairt_Status_t QairtContext_BinaryListParams_setBinaryBuffer(
    QairtContext_BinaryListParamsHandle_t paramsHandle,
    QairtContext_BinaryBufferHandle_t binaryBufferHandle);

/**
 * @brief Set the profile handle used to collect metrics while creating this context.
 *
 * @param[in] paramsHandle A handle to a created context binary list params.
 *
 * @param[in] profileHandle A handle to a created profile; may be NULL to disable
 *                           profiling.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Profile was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _paramsHandle_ is not a
 *           valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryListParamsV1_t.
 */
Qairt_Status_t QairtContext_BinaryListParams_setProfile(
    QairtContext_BinaryListParamsHandle_t paramsHandle, QairtProfile_Handle_t profileHandle);

/**
 * @brief Set the notify callback and its associated cookie for this context.
 *
 * @param[in] paramsHandle A handle to a created context binary list params.
 *
 * @param[in] callbackHandle A handle to a created notify callback (see
 *                            QairtContext_BinaryListNotifyCallback_create); must not be NULL.
 *
 * @param[in] userData User-supplied data pointer passed back to the callback on
 *                     every invocation (may be NULL).
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Notify callback was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _paramsHandle_ or _callbackHandle_
 *           is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: the callback wrapped by
 *           _callbackHandle_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryListParamsV1_t.
 */
Qairt_Status_t QairtContext_BinaryListParams_setNotify(
    QairtContext_BinaryListParamsHandle_t paramsHandle,
    QairtContext_BinaryListNotifyCallbackHandle_t callbackHandle,
    void* userData);

/**
 * @brief Create a handle to a notify callback object, used to register a callback
 *        on a QairtContext_BinaryListParams via QairtContext_BinaryListParams_setNotify. Mirrors
 *        QairtLog_Callback_create.
 *
 * @param[in] callback User-supplied callback invoked as a context and its graphs
 *                      become available; must not be NULL.
 *
 * @param[out] callbackHandle The created callback handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Callback was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _callback_ or _callbackHandle_
 *           is NULL.
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory allocation error.
 *
 * @note Use corresponding API through QairtContext_BinaryListNotifyCallbackV1_t.
 */
Qairt_Status_t QairtContext_BinaryListNotifyCallback_create(
    QairtContext_CreateFromBinaryListAsyncNotifyFn_t callback,
    QairtContext_BinaryListNotifyCallbackHandle_t* callbackHandle);

/**
 * @brief Free a notify callback handle.
 *
 * @param[in] callbackHandle A handle to a created notify callback.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Callback was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _callbackHandle_ is not a valid
 *           handle.
 *
 * @note Use corresponding API through QairtContext_BinaryListNotifyCallbackV1_t.
 */
Qairt_Status_t QairtContext_BinaryListNotifyCallback_free(
    QairtContext_BinaryListNotifyCallbackHandle_t callbackHandle);

/**
 * @brief Register a raw-buffer callback (data provider + release) on this context binary list
 *        params. Setting a buffer callback makes this context initialize via the V2 params path.
 *
 * @param[in] paramsHandle   A handle to a created context binary list params.
 * @param[in] callbackHandle A handle to a created raw buffer callback (see
 *                           QairtContext_BinaryListRawBufferCallback_create); must not be NULL.
 * @param[in] userData       User-supplied data pointer passed back to the callbacks (may be NULL).
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Callback was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _paramsHandle_ or _callbackHandle_ is invalid.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: the callback wrapped by _callbackHandle_ is
 *           missing a provider or release function.
 *
 * @note Use corresponding API through QairtContext_BinaryListParamsV1_t.
 */
Qairt_Status_t QairtContext_BinaryListParams_setRawBufferCallback(
    QairtContext_BinaryListParamsHandle_t paramsHandle,
    QairtContext_BinaryListRawBufferCallbackHandle_t callbackHandle,
    void* userData);

/**
 * @brief Register a dma-buffer callback (data provider + release) on this context binary list
 *        params. Setting a buffer callback makes this context initialize via the V2 params path.
 *
 * @param[in] paramsHandle   A handle to a created context binary list params.
 * @param[in] callbackHandle A handle to a created dma buffer callback (see
 *                           QairtContext_BinaryListDmaBufferCallback_create); must not be NULL.
 * @param[in] userData       User-supplied data pointer passed back to the callbacks (may be NULL).
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Callback was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _paramsHandle_ or _callbackHandle_ is invalid.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: the callback wrapped by _callbackHandle_ is
 *           missing a provider or release function.
 *
 * @note Use corresponding API through QairtContext_BinaryListParamsV1_t.
 */
Qairt_Status_t QairtContext_BinaryListParams_setDmaBufferCallback(
    QairtContext_BinaryListParamsHandle_t paramsHandle,
    QairtContext_BinaryListDmaBufferCallbackHandle_t callbackHandle,
    void* userData);

/**
 * @brief Create a raw buffer callback handle, used to register a data provider + release pair on a
 *        QairtContext_BinaryListParams via QairtContext_BinaryListParams_setRawBufferCallback.
 *
 * @param[in]  provideFn     Callback invoked to provide context binary data; must not be NULL.
 * @param[in]  releaseFn     Callback invoked to release context binary data; must not be NULL.
 * @param[out] callbackHandle The created callback handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Callback was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _provideFn_, _releaseFn_ or _callbackHandle_
 *           is NULL.
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory allocation error.
 *
 * @note Use corresponding API through QairtContext_BinaryListRawBufferCallbackV1_t.
 */
Qairt_Status_t QairtContext_BinaryListRawBufferCallback_create(
    QairtContext_BinaryRawDataProviderFn_t provideFn,
    QairtContext_BinaryRawDataReleaseFn_t releaseFn,
    QairtContext_BinaryListRawBufferCallbackHandle_t* callbackHandle);

/**
 * @brief Free a raw buffer callback handle.
 *
 * @param[in] callbackHandle A handle to a created raw buffer callback.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Callback was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _callbackHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryListRawBufferCallbackV1_t.
 */
Qairt_Status_t QairtContext_BinaryListRawBufferCallback_free(
    QairtContext_BinaryListRawBufferCallbackHandle_t callbackHandle);

/**
 * @brief Create a dma buffer callback handle, used to register a data provider + release pair on a
 *        QairtContext_BinaryListParams via QairtContext_BinaryListParams_setDmaBufferCallback.
 *
 * @param[in]  provideFn     Callback invoked to provide context binary data; must not be NULL.
 * @param[in]  releaseFn     Callback invoked to release context binary data; must not be NULL.
 * @param[out] callbackHandle The created callback handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Callback was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _provideFn_, _releaseFn_ or _callbackHandle_
 *           is NULL.
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory allocation error.
 *
 * @note Use corresponding API through QairtContext_BinaryListDmaBufferCallbackV1_t.
 */
Qairt_Status_t QairtContext_BinaryListDmaBufferCallback_create(
    QairtContext_BinaryDmaDataProviderFn_t provideFn,
    QairtContext_BinaryDmaDataReleaseFn_t releaseFn,
    QairtContext_BinaryListDmaBufferCallbackHandle_t* callbackHandle);

/**
 * @brief Free a dma buffer callback handle.
 *
 * @param[in] callbackHandle A handle to a created dma buffer callback.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Callback was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _callbackHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryListDmaBufferCallbackV1_t.
 */
Qairt_Status_t QairtContext_BinaryListDmaBufferCallback_free(
    QairtContext_BinaryListDmaBufferCallbackHandle_t callbackHandle);

/**
 * @brief Create a context binary data request handle.
 *
 * @param[out] requestHandle A handle to the created request.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Request was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _requestHandle_ is NULL.
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory allocation error while creating the request.
 *
 * @note Use corresponding API through QairtContext_BinaryDataRequestV1_t.
 */
Qairt_Status_t QairtContext_BinaryDataRequest_create(
    QairtContext_BinaryDataRequestHandle_t* requestHandle);

/**
 * @brief Free a context binary data request handle and associated resources.
 *
 * @param[in] requestHandle A handle to a created context binary data request.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Request was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _requestHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDataRequestV1_t.
 */
Qairt_Status_t QairtContext_BinaryDataRequest_free(
    QairtContext_BinaryDataRequestHandle_t requestHandle);

/**
 * @brief Get the offset of the requested data relative to the start of the context binary.
 *
 * @param[in] requestHandle A handle to a created context binary data request.
 *
 * @param[out] offset Pointer to retrieve the offset value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Offset was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _requestHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _offset_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryDataRequestV1_t.
 */
Qairt_Status_t QairtContext_BinaryDataRequest_getOffset(
    QairtContext_BinaryDataRequestHandle_t requestHandle, uint64_t* offset);

/**
 * @brief Set the offset of the requested data relative to the start of the context binary.
 *
 * @param[in] requestHandle A handle to a created context binary data request.
 *
 * @param[in] offset The offset value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Offset was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _requestHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDataRequestV1_t.
 */
Qairt_Status_t QairtContext_BinaryDataRequest_setOffset(
    QairtContext_BinaryDataRequestHandle_t requestHandle, uint64_t offset);

/**
 * @brief Get the size of the requested data.
 *
 * @param[in] requestHandle A handle to a created context binary data request.
 *
 * @param[out] size Pointer to retrieve the size value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Size was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _requestHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _size_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryDataRequestV1_t.
 */
Qairt_Status_t QairtContext_BinaryDataRequest_getSize(
    QairtContext_BinaryDataRequestHandle_t requestHandle, uint64_t* size);

/**
 * @brief Set the size of the requested data.
 *
 * @param[in] requestHandle A handle to a created context binary data request.
 *
 * @param[in] size The size value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Size was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _requestHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDataRequestV1_t.
 */
Qairt_Status_t QairtContext_BinaryDataRequest_setSize(
    QairtContext_BinaryDataRequestHandle_t requestHandle, uint64_t size);

/**
 * @brief Get whether the requested buffer will be used for backend mapping.
 *
 * @param[in] requestHandle A handle to a created context binary data request.
 *
 * @param[out] isBackendMappingNeeded Pointer to retrieve the flag value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Flag was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _requestHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _isBackendMappingNeeded_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryDataRequestV1_t.
 */
Qairt_Status_t QairtContext_BinaryDataRequest_getIsBackendMappingNeeded(
    QairtContext_BinaryDataRequestHandle_t requestHandle, uint8_t* isBackendMappingNeeded);

/**
 * @brief Set whether the requested buffer will be used for backend mapping.
 *
 * @param[in] requestHandle A handle to a created context binary data request.
 *
 * @param[in] isBackendMappingNeeded The flag value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Flag was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _requestHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDataRequestV1_t.
 */
Qairt_Status_t QairtContext_BinaryDataRequest_setIsBackendMappingNeeded(
    QairtContext_BinaryDataRequestHandle_t requestHandle, uint8_t isBackendMappingNeeded);

/**
 * @brief Create a context binary raw data response handle, used by a raw-buffer data-provider
 *        callback to describe the buffer it supplied.
 *
 * @param[out] responseHandle A handle to the created response.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Response was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _responseHandle_ is NULL.
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory allocation error while creating the response.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataResponse_create(
    QairtContext_BinaryRawDataResponseHandle_t* responseHandle);

/**
 * @brief Free a context binary raw data response handle and associated resources.
 *
 * @param[in] responseHandle A handle to a created context binary raw data response.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Response was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataResponse_free(
    QairtContext_BinaryRawDataResponseHandle_t responseHandle);

/**
 * @brief Get the pointer to the supplied raw buffer.
 *
 * @param[in] responseHandle A handle to a created context binary raw data response.
 *
 * @param[out] memPtr Pointer to retrieve the raw buffer pointer.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Pointer was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _memPtr_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataResponse_getMemPtr(
    QairtContext_BinaryRawDataResponseHandle_t responseHandle, void** memPtr);

/**
 * @brief Set the pointer to the supplied raw buffer.
 *
 * @param[in] responseHandle A handle to a created context binary raw data response.
 *
 * @param[in] memPtr The raw buffer pointer to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Pointer was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataResponse_setMemPtr(
    QairtContext_BinaryRawDataResponseHandle_t responseHandle, void* memPtr);

/**
 * @brief Get the actual offset of the requested data relative to the start of the external buffer.
 *
 * @param[in] responseHandle A handle to a created context binary raw data response.
 *
 * @param[out] dataStartOffset Pointer to retrieve the offset value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Offset was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _dataStartOffset_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataResponse_getDataStartOffset(
    QairtContext_BinaryRawDataResponseHandle_t responseHandle, uint64_t* dataStartOffset);

/**
 * @brief Set the actual offset of the requested data relative to the start of the external buffer.
 *
 * @param[in] responseHandle A handle to a created context binary raw data response.
 *
 * @param[in] dataStartOffset The offset value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Offset was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataResponse_setDataStartOffset(
    QairtContext_BinaryRawDataResponseHandle_t responseHandle, uint64_t dataStartOffset);

/**
 * @brief Get the aligned size of the requested data.
 *
 * @param[in] responseHandle A handle to a created context binary raw data response.
 *
 * @param[out] alignedSize Pointer to retrieve the aligned size value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Aligned size was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _alignedSize_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataResponse_getAlignedSize(
    QairtContext_BinaryRawDataResponseHandle_t responseHandle, uint64_t* alignedSize);

/**
 * @brief Set the aligned size of the requested data.
 *
 * @param[in] responseHandle A handle to a created context binary raw data response.
 *
 * @param[in] alignedSize The aligned size value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Aligned size was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataResponse_setAlignedSize(
    QairtContext_BinaryRawDataResponseHandle_t responseHandle, uint64_t alignedSize);

/**
 * @brief Create a context binary dma data response handle, used by a dma-buffer data-provider
 *        callback to describe the buffer it supplied.
 *
 * @param[out] responseHandle A handle to the created response.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Response was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _responseHandle_ is NULL.
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory allocation error while creating the response.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataResponse_create(
    QairtContext_BinaryDmaDataResponseHandle_t* responseHandle);

/**
 * @brief Free a context binary dma data response handle and associated resources.
 *
 * @param[in] responseHandle A handle to a created context binary dma data response.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Response was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataResponse_free(
    QairtContext_BinaryDmaDataResponseHandle_t responseHandle);

/**
 * @brief Get the file descriptor for the supplied dma buffer.
 *
 * @param[in] responseHandle A handle to a created context binary dma data response.
 *
 * @param[out] fd Pointer to retrieve the file descriptor value
 *                 (QAIRT_CONTEXT_MEM_INVALID_FD if unset).
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: File descriptor was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _fd_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataResponse_getFd(
    QairtContext_BinaryDmaDataResponseHandle_t responseHandle, int32_t* fd);

/**
 * @brief Set the file descriptor for the supplied dma buffer.
 *
 * @param[in] responseHandle A handle to a created context binary dma data response.
 *
 * @param[in] fd The file descriptor value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: File descriptor was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataResponse_setFd(
    QairtContext_BinaryDmaDataResponseHandle_t responseHandle, int32_t fd);

/**
 * @brief Get the data pointer for the supplied dma buffer.
 *
 * @param[in] responseHandle A handle to a created context binary dma data response.
 *
 * @param[out] data Pointer to retrieve the data pointer.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Data pointer was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _data_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataResponse_getData(
    QairtContext_BinaryDmaDataResponseHandle_t responseHandle, void** data);

/**
 * @brief Set the data pointer for the supplied dma buffer.
 *
 * @param[in] responseHandle A handle to a created context binary dma data response.
 *
 * @param[in] data The data pointer to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Data pointer was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataResponse_setData(
    QairtContext_BinaryDmaDataResponseHandle_t responseHandle, void* data);

/**
 * @brief Get the actual offset of the requested data relative to the start of the external buffer.
 *
 * @param[in] responseHandle A handle to a created context binary dma data response.
 *
 * @param[out] dataStartOffset Pointer to retrieve the offset value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Offset was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _dataStartOffset_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataResponse_getDataStartOffset(
    QairtContext_BinaryDmaDataResponseHandle_t responseHandle, uint64_t* dataStartOffset);

/**
 * @brief Set the actual offset of the requested data relative to the start of the external buffer.
 *
 * @param[in] responseHandle A handle to a created context binary dma data response.
 *
 * @param[in] dataStartOffset The offset value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Offset was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataResponse_setDataStartOffset(
    QairtContext_BinaryDmaDataResponseHandle_t responseHandle, uint64_t dataStartOffset);

/**
 * @brief Get the aligned size of the requested data.
 *
 * @param[in] responseHandle A handle to a created context binary dma data response.
 *
 * @param[out] alignedSize Pointer to retrieve the aligned size value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Aligned size was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _alignedSize_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataResponse_getAlignedSize(
    QairtContext_BinaryDmaDataResponseHandle_t responseHandle, uint64_t* alignedSize);

/**
 * @brief Set the aligned size of the requested data.
 *
 * @param[in] responseHandle A handle to a created context binary dma data response.
 *
 * @param[in] alignedSize The aligned size value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Aligned size was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _responseHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataResponseV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataResponse_setAlignedSize(
    QairtContext_BinaryDmaDataResponseHandle_t responseHandle, uint64_t alignedSize);

/**
 * @brief Create a context binary raw data mem handle, describing a previously provided raw buffer
 *        to be released.
 *
 * @param[out] memHandle A handle to the created mem descriptor.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Mem descriptor was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _memHandle_ is NULL.
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory allocation error while creating the descriptor.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataMem_create(
    QairtContext_BinaryRawDataMemHandle_t* memHandle);

/**
 * @brief Free a context binary raw data mem handle and associated resources.
 *
 * @param[in] memHandle A handle to a created context binary raw data mem descriptor.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Mem descriptor was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataMem_free(
    QairtContext_BinaryRawDataMemHandle_t memHandle);

/**
 * @brief Get the pointer to the raw buffer to be released.
 *
 * @param[in] memHandle A handle to a created context binary raw data mem descriptor.
 *
 * @param[out] memPtr Pointer to retrieve the raw buffer pointer.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Pointer was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _memPtr_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataMem_getMemPtr(
    QairtContext_BinaryRawDataMemHandle_t memHandle, void** memPtr);

/**
 * @brief Set the pointer to the raw buffer to be released.
 *
 * @param[in] memHandle A handle to a created context binary raw data mem descriptor.
 *
 * @param[in] memPtr The raw buffer pointer to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Pointer was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataMem_setMemPtr(
    QairtContext_BinaryRawDataMemHandle_t memHandle, void* memPtr);

/**
 * @brief Get the size of the raw buffer to be released.
 *
 * @param[in] memHandle A handle to a created context binary raw data mem descriptor.
 *
 * @param[out] memSize Pointer to retrieve the size value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Size was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _memSize_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataMem_getMemSize(
    QairtContext_BinaryRawDataMemHandle_t memHandle, uint64_t* memSize);

/**
 * @brief Set the size of the raw buffer to be released.
 *
 * @param[in] memHandle A handle to a created context binary raw data mem descriptor.
 *
 * @param[in] memSize The size value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Size was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryRawDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryRawDataMem_setMemSize(
    QairtContext_BinaryRawDataMemHandle_t memHandle, uint64_t memSize);

/**
 * @brief Create a context binary dma data mem handle, describing a previously provided dma buffer
 *        to be released.
 *
 * @param[out] memHandle A handle to the created mem descriptor.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Mem descriptor was successfully created.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _memHandle_ is NULL.
 *         - QAIRT_CONTEXT_ERROR_MEM_ALLOC: Memory allocation error while creating the descriptor.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataMem_create(
    QairtContext_BinaryDmaDataMemHandle_t* memHandle);

/**
 * @brief Free a context binary dma data mem handle and associated resources.
 *
 * @param[in] memHandle A handle to a created context binary dma data mem descriptor.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Mem descriptor was successfully freed.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataMem_free(
    QairtContext_BinaryDmaDataMemHandle_t memHandle);

/**
 * @brief Get the file descriptor for the dma buffer to be released.
 *
 * @param[in] memHandle A handle to a created context binary dma data mem descriptor.
 *
 * @param[out] fd Pointer to retrieve the file descriptor value
 *                 (QAIRT_CONTEXT_MEM_INVALID_FD if unset).
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: File descriptor was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _fd_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataMem_getFd(
    QairtContext_BinaryDmaDataMemHandle_t memHandle, int32_t* fd);

/**
 * @brief Set the file descriptor for the dma buffer to be released.
 *
 * @param[in] memHandle A handle to a created context binary dma data mem descriptor.
 *
 * @param[in] fd The file descriptor value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: File descriptor was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataMem_setFd(
    QairtContext_BinaryDmaDataMemHandle_t memHandle, int32_t fd);

/**
 * @brief Get the data pointer for the dma buffer to be released.
 *
 * @param[in] memHandle A handle to a created context binary dma data mem descriptor.
 *
 * @param[out] data Pointer to retrieve the data pointer.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Data pointer was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _data_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataMem_getData(
    QairtContext_BinaryDmaDataMemHandle_t memHandle, void** data);

/**
 * @brief Set the data pointer for the dma buffer to be released.
 *
 * @param[in] memHandle A handle to a created context binary dma data mem descriptor.
 *
 * @param[in] data The data pointer to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Data pointer was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataMem_setData(
    QairtContext_BinaryDmaDataMemHandle_t memHandle, void* data);

/**
 * @brief Get the size of the dma buffer to be released.
 *
 * @param[in] memHandle A handle to a created context binary dma data mem descriptor.
 *
 * @param[out] memSize Pointer to retrieve the size value.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Size was successfully retrieved.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *         - QAIRT_CONTEXT_ERROR_INVALID_ARGUMENT: _memSize_ is NULL.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataMem_getMemSize(
    QairtContext_BinaryDmaDataMemHandle_t memHandle, uint64_t* memSize);

/**
 * @brief Set the size of the dma buffer to be released.
 *
 * @param[in] memHandle A handle to a created context binary dma data mem descriptor.
 *
 * @param[in] memSize The size value to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Size was successfully set.
 *         - QAIRT_CONTEXT_ERROR_INVALID_HANDLE: _memHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtContext_BinaryDmaDataMemV1_t.
 */
Qairt_Status_t QairtContext_BinaryDmaDataMem_setMemSize(
    QairtContext_BinaryDmaDataMemHandle_t memHandle, uint64_t memSize);

//=============================================================================
// Public Interfaces
//=============================================================================

typedef Qairt_Status_t (*QairtContext_BinaryListParams_CreateFn_t)(
    QairtContext_BinaryListParamsHandle_t*);

typedef Qairt_Status_t (*QairtContext_BinaryListParams_FreeFn_t)(
    QairtContext_BinaryListParamsHandle_t);

typedef Qairt_Status_t (*QairtContext_BinaryListParams_SetConfigFn_t)(
    QairtContext_BinaryListParamsHandle_t, QairtContext_ConfigHandle_t);

typedef Qairt_Status_t (*QairtContext_BinaryListParams_SetBinaryBufferFn_t)(
    QairtContext_BinaryListParamsHandle_t, QairtContext_BinaryBufferHandle_t);

typedef Qairt_Status_t (*QairtContext_BinaryListParams_SetProfileFn_t)(
    QairtContext_BinaryListParamsHandle_t, QairtProfile_Handle_t);

typedef Qairt_Status_t (*QairtContext_BinaryListParams_SetNotifyFn_t)(
    QairtContext_BinaryListParamsHandle_t,
    QairtContext_BinaryListNotifyCallbackHandle_t,
    void*);

typedef Qairt_Status_t (*QairtContext_BinaryListParams_SetRawBufferCallbackFn_t)(
    QairtContext_BinaryListParamsHandle_t,
    QairtContext_BinaryListRawBufferCallbackHandle_t,
    void*);

typedef Qairt_Status_t (*QairtContext_BinaryListParams_SetDmaBufferCallbackFn_t)(
    QairtContext_BinaryListParamsHandle_t,
    QairtContext_BinaryListDmaBufferCallbackHandle_t,
    void*);

typedef Qairt_Status_t (*QairtContext_BinaryListNotifyCallback_CreateFn_t)(
    QairtContext_CreateFromBinaryListAsyncNotifyFn_t,
    QairtContext_BinaryListNotifyCallbackHandle_t*);

typedef Qairt_Status_t (*QairtContext_BinaryListNotifyCallback_FreeFn_t)(
    QairtContext_BinaryListNotifyCallbackHandle_t);

typedef Qairt_Status_t (*QairtContext_BinaryListRawBufferCallback_CreateFn_t)(
    QairtContext_BinaryRawDataProviderFn_t,
    QairtContext_BinaryRawDataReleaseFn_t,
    QairtContext_BinaryListRawBufferCallbackHandle_t*);

typedef Qairt_Status_t (*QairtContext_BinaryListRawBufferCallback_FreeFn_t)(
    QairtContext_BinaryListRawBufferCallbackHandle_t);

typedef Qairt_Status_t (*QairtContext_BinaryListDmaBufferCallback_CreateFn_t)(
    QairtContext_BinaryDmaDataProviderFn_t,
    QairtContext_BinaryDmaDataReleaseFn_t,
    QairtContext_BinaryListDmaBufferCallbackHandle_t*);

typedef Qairt_Status_t (*QairtContext_BinaryListDmaBufferCallback_FreeFn_t)(
    QairtContext_BinaryListDmaBufferCallbackHandle_t);

/**
 * @struct QairtContext_BinaryListParamsV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Context Binary List Params.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtContext_BinaryListParams_CreateFn_t create;
  QairtContext_BinaryListParams_FreeFn_t free;
  QairtContext_BinaryListParams_SetConfigFn_t setConfig;
  QairtContext_BinaryListParams_SetBinaryBufferFn_t setBinaryBuffer;
  QairtContext_BinaryListParams_SetProfileFn_t setProfile;
  QairtContext_BinaryListParams_SetNotifyFn_t setNotify;
  QairtContext_BinaryListParams_SetRawBufferCallbackFn_t setRawBufferCallback;
  QairtContext_BinaryListParams_SetDmaBufferCallbackFn_t setDmaBufferCallback;
} QairtContext_BinaryListParamsV1_t;

/**
 * @struct QairtContext_BinaryListNotifyCallbackV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Context Binary List Notify Callback. This interface can be retrieved through
 *        the id QAIRT_CONTEXT_BINARY_LIST_NOTIFY_CALLBACK_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtContext_BinaryListNotifyCallback_CreateFn_t create;
  QairtContext_BinaryListNotifyCallback_FreeFn_t free;
} QairtContext_BinaryListNotifyCallbackV1_t;

/**
 * @struct QairtContext_BinaryListRawBufferCallbackV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Context Binary List Raw Buffer Callback. This interface can be retrieved through
 *        the id QAIRT_CONTEXT_BINARY_LIST_RAW_BUFFER_CALLBACK_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtContext_BinaryListRawBufferCallback_CreateFn_t create;
  QairtContext_BinaryListRawBufferCallback_FreeFn_t free;
} QairtContext_BinaryListRawBufferCallbackV1_t;

/**
 * @struct QairtContext_BinaryListDmaBufferCallbackV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Context Binary List Dma Buffer Callback. This interface can be retrieved through
 *        the id QAIRT_CONTEXT_BINARY_LIST_DMA_BUFFER_CALLBACK_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtContext_BinaryListDmaBufferCallback_CreateFn_t create;
  QairtContext_BinaryListDmaBufferCallback_FreeFn_t free;
} QairtContext_BinaryListDmaBufferCallbackV1_t;

/// Data Request
typedef Qairt_Status_t (*QairtContext_BinaryDataRequest_CreateFn_t)(
    QairtContext_BinaryDataRequestHandle_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDataRequest_FreeFn_t)(
    QairtContext_BinaryDataRequestHandle_t);
typedef Qairt_Status_t (*QairtContext_BinaryDataRequest_GetOffsetFn_t)(
    QairtContext_BinaryDataRequestHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDataRequest_SetOffsetFn_t)(
    QairtContext_BinaryDataRequestHandle_t, uint64_t);
typedef Qairt_Status_t (*QairtContext_BinaryDataRequest_GetSizeFn_t)(
    QairtContext_BinaryDataRequestHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDataRequest_SetSizeFn_t)(
    QairtContext_BinaryDataRequestHandle_t, uint64_t);
typedef Qairt_Status_t (*QairtContext_BinaryDataRequest_GetIsBackendMappingNeededFn_t)(
    QairtContext_BinaryDataRequestHandle_t, uint8_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDataRequest_SetIsBackendMappingNeededFn_t)(
    QairtContext_BinaryDataRequestHandle_t, uint8_t);

/// Raw Data Response
typedef Qairt_Status_t (*QairtContext_BinaryRawDataResponse_CreateFn_t)(
    QairtContext_BinaryRawDataResponseHandle_t*);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataResponse_FreeFn_t)(
    QairtContext_BinaryRawDataResponseHandle_t);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataResponse_GetMemPtrFn_t)(
    QairtContext_BinaryRawDataResponseHandle_t, void**);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataResponse_SetMemPtrFn_t)(
    QairtContext_BinaryRawDataResponseHandle_t, void*);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataResponse_GetDataStartOffsetFn_t)(
    QairtContext_BinaryRawDataResponseHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataResponse_SetDataStartOffsetFn_t)(
    QairtContext_BinaryRawDataResponseHandle_t, uint64_t);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataResponse_GetAlignedSizeFn_t)(
    QairtContext_BinaryRawDataResponseHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataResponse_SetAlignedSizeFn_t)(
    QairtContext_BinaryRawDataResponseHandle_t, uint64_t);

/// Dma Data Response
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataResponse_CreateFn_t)(
    QairtContext_BinaryDmaDataResponseHandle_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataResponse_FreeFn_t)(
    QairtContext_BinaryDmaDataResponseHandle_t);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataResponse_GetFdFn_t)(
    QairtContext_BinaryDmaDataResponseHandle_t, int32_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataResponse_SetFdFn_t)(
    QairtContext_BinaryDmaDataResponseHandle_t, int32_t);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataResponse_GetDataFn_t)(
    QairtContext_BinaryDmaDataResponseHandle_t, void**);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataResponse_SetDataFn_t)(
    QairtContext_BinaryDmaDataResponseHandle_t, void*);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataResponse_GetDataStartOffsetFn_t)(
    QairtContext_BinaryDmaDataResponseHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataResponse_SetDataStartOffsetFn_t)(
    QairtContext_BinaryDmaDataResponseHandle_t, uint64_t);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataResponse_GetAlignedSizeFn_t)(
    QairtContext_BinaryDmaDataResponseHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataResponse_SetAlignedSizeFn_t)(
    QairtContext_BinaryDmaDataResponseHandle_t, uint64_t);

/// Raw Data Mem
typedef Qairt_Status_t (*QairtContext_BinaryRawDataMem_CreateFn_t)(
    QairtContext_BinaryRawDataMemHandle_t*);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataMem_FreeFn_t)(
    QairtContext_BinaryRawDataMemHandle_t);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataMem_GetMemPtrFn_t)(
    QairtContext_BinaryRawDataMemHandle_t, void**);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataMem_SetMemPtrFn_t)(
    QairtContext_BinaryRawDataMemHandle_t, void*);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataMem_GetMemSizeFn_t)(
    QairtContext_BinaryRawDataMemHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtContext_BinaryRawDataMem_SetMemSizeFn_t)(
    QairtContext_BinaryRawDataMemHandle_t, uint64_t);

/// Dma Data Mem
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataMem_CreateFn_t)(
    QairtContext_BinaryDmaDataMemHandle_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataMem_FreeFn_t)(
    QairtContext_BinaryDmaDataMemHandle_t);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataMem_GetFdFn_t)(
    QairtContext_BinaryDmaDataMemHandle_t, int32_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataMem_SetFdFn_t)(
    QairtContext_BinaryDmaDataMemHandle_t, int32_t);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataMem_GetDataFn_t)(
    QairtContext_BinaryDmaDataMemHandle_t, void**);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataMem_SetDataFn_t)(
    QairtContext_BinaryDmaDataMemHandle_t, void*);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataMem_GetMemSizeFn_t)(
    QairtContext_BinaryDmaDataMemHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtContext_BinaryDmaDataMem_SetMemSizeFn_t)(
    QairtContext_BinaryDmaDataMemHandle_t, uint64_t);

/**
 * @struct QairtContext_BinaryDataRequestV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Context Binary Data Request. This interface can be retrieved through
 *        the id QAIRT_CONTEXT_BINARY_DATA_REQUEST_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtContext_BinaryDataRequest_CreateFn_t create;
  QairtContext_BinaryDataRequest_FreeFn_t free;
  QairtContext_BinaryDataRequest_GetOffsetFn_t getOffset;
  QairtContext_BinaryDataRequest_SetOffsetFn_t setOffset;
  QairtContext_BinaryDataRequest_GetSizeFn_t getSize;
  QairtContext_BinaryDataRequest_SetSizeFn_t setSize;
  QairtContext_BinaryDataRequest_GetIsBackendMappingNeededFn_t getIsBackendMappingNeeded;
  QairtContext_BinaryDataRequest_SetIsBackendMappingNeededFn_t setIsBackendMappingNeeded;
} QairtContext_BinaryDataRequestV1_t;

/**
 * @struct QairtContext_BinaryRawDataResponseV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Context Binary Raw Data Response. This interface can be retrieved through
 *        the id QAIRT_CONTEXT_BINARY_RAW_DATA_RESPONSE_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtContext_BinaryRawDataResponse_CreateFn_t create;
  QairtContext_BinaryRawDataResponse_FreeFn_t free;
  QairtContext_BinaryRawDataResponse_GetMemPtrFn_t getMemPtr;
  QairtContext_BinaryRawDataResponse_SetMemPtrFn_t setMemPtr;
  QairtContext_BinaryRawDataResponse_GetDataStartOffsetFn_t getDataStartOffset;
  QairtContext_BinaryRawDataResponse_SetDataStartOffsetFn_t setDataStartOffset;
  QairtContext_BinaryRawDataResponse_GetAlignedSizeFn_t getAlignedSize;
  QairtContext_BinaryRawDataResponse_SetAlignedSizeFn_t setAlignedSize;
} QairtContext_BinaryRawDataResponseV1_t;

/**
 * @struct QairtContext_BinaryDmaDataResponseV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Context Binary Dma Data Response. This interface can be retrieved through
 *        the id QAIRT_CONTEXT_BINARY_DMA_DATA_RESPONSE_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtContext_BinaryDmaDataResponse_CreateFn_t create;
  QairtContext_BinaryDmaDataResponse_FreeFn_t free;
  QairtContext_BinaryDmaDataResponse_GetFdFn_t getFd;
  QairtContext_BinaryDmaDataResponse_SetFdFn_t setFd;
  QairtContext_BinaryDmaDataResponse_GetDataFn_t getData;
  QairtContext_BinaryDmaDataResponse_SetDataFn_t setData;
  QairtContext_BinaryDmaDataResponse_GetDataStartOffsetFn_t getDataStartOffset;
  QairtContext_BinaryDmaDataResponse_SetDataStartOffsetFn_t setDataStartOffset;
  QairtContext_BinaryDmaDataResponse_GetAlignedSizeFn_t getAlignedSize;
  QairtContext_BinaryDmaDataResponse_SetAlignedSizeFn_t setAlignedSize;
} QairtContext_BinaryDmaDataResponseV1_t;

/**
 * @struct QairtContext_BinaryRawDataMemV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Context Binary Raw Data Mem. This interface can be retrieved through
 *        the id QAIRT_CONTEXT_BINARY_RAW_DATA_MEM_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtContext_BinaryRawDataMem_CreateFn_t create;
  QairtContext_BinaryRawDataMem_FreeFn_t free;
  QairtContext_BinaryRawDataMem_GetMemPtrFn_t getMemPtr;
  QairtContext_BinaryRawDataMem_SetMemPtrFn_t setMemPtr;
  QairtContext_BinaryRawDataMem_GetMemSizeFn_t getMemSize;
  QairtContext_BinaryRawDataMem_SetMemSizeFn_t setMemSize;
} QairtContext_BinaryRawDataMemV1_t;

/**
 * @struct QairtContext_BinaryDmaDataMemV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Context Binary Dma Data Mem. This interface can be retrieved through
 *        the id QAIRT_CONTEXT_BINARY_DMA_DATA_MEM_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtContext_BinaryDmaDataMem_CreateFn_t create;
  QairtContext_BinaryDmaDataMem_FreeFn_t free;
  QairtContext_BinaryDmaDataMem_GetFdFn_t getFd;
  QairtContext_BinaryDmaDataMem_SetFdFn_t setFd;
  QairtContext_BinaryDmaDataMem_GetDataFn_t getData;
  QairtContext_BinaryDmaDataMem_SetDataFn_t setData;
  QairtContext_BinaryDmaDataMem_GetMemSizeFn_t getMemSize;
  QairtContext_BinaryDmaDataMem_SetMemSizeFn_t setMemSize;
} QairtContext_BinaryDmaDataMemV1_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAISW_QAIRTCONTEXTBINARYLISTPARAMS_H
