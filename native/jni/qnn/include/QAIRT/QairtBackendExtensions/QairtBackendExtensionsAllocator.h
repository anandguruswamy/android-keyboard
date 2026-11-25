//==============================================================================
//
// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// All rights reserved.
// Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

/**
 * @file
 * @brief   Backend extensions allocator component API
 *
 *          A header which contains the QAIRT Backend Extensions Allocator
 *          component and the interfaces it implements.
 */

#ifndef QAISW_QAIRTBACKENDEXTALLOCATOR_H
#define QAISW_QAIRTBACKENDEXTALLOCATOR_H

#include "QairtBackend/QairtBackendConfig.h"
#include "QairtContext/QairtContextConfig.h"
#include "QairtDevice/QairtDeviceConfig.h"
#include "QairtGraph/QairtGraphConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Macros
//=============================================================================
#define QAIRT_BACKEND_EXT_ALLOCATOR_ID QAIRT_MIN_ID_BACKEND_EXT_ALLOCATOR
//=============================================================================
// Datatypes
//=============================================================================

/**
 * @brief   Backend extensions allocator handle used to interface with Backend
 *          Extensions Allocator APIs
 *
 */
typedef
struct _QairtBackendExt_Allocator_Handle_t* QairtBackendExt_Allocator_Handle_t;

//=============================================================================
// Public Functions
//=============================================================================


/**
 * @brief Allocate a buffer using the allocator.
 *
 * @param[in] allocatorHandle A handle to a backend extensions allocator.
 * @param[in] alignment Required byte alignment of the returned buffer.
 * @param[in] size Size in bytes of the buffer to allocate.
 * @param[out] buffer Populated with a pointer to the allocated buffer.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Buffer was successfully allocated.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _allocatorHandle_ is
 *           not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _size_ is 0 or
 *           _buffer_ is NULL, or the buffer could not be allocated.
 *
 * @note Use corresponding API through QairtBackendExt_Allocator_t.
 */
Qairt_Status_t QairtBackendExt_Allocator_allocateBuffer(
  QairtBackendExt_Allocator_Handle_t allocatorHandle,
  uint64_t alignment,
  uint64_t size,
  void** buffer);

/**
 * @brief Deallocate a buffer previously allocated via
 *        QairtBackendExt_Allocator_allocateBuffer().
 *
 * @param[in] allocatorHandle A handle to a backend extensions allocator.
 * @param[in] buffer The buffer to deallocate.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Buffer was successfully deallocated.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _allocatorHandle_ is
 *           not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _buffer_ is NULL or
 *           was not allocated by this allocator.
 *
 * @note Use corresponding API through QairtBackendExt_Allocator_t.
 */
Qairt_Status_t QairtBackendExt_Allocator_deallocateBuffer(
  QairtBackendExt_Allocator_Handle_t allocatorHandle,
  void* buffer);

/**
 * @brief Retrieve the underlying buffer backing a tensor allocated by this
 *        allocator.
 *
 * @param[in] allocatorHandle A handle to a backend extensions allocator.
 * @param[in] tensorHandle A handle to the tensor whose buffer to retrieve.
 * @param[out] buffer Populated with a pointer to the tensor's underlying
 *                     buffer.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Buffer was successfully retrieved.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _allocatorHandle_ or
 *           _tensorHandle_ is not a valid handle.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT: _buffer_ is NULL.
 *
 * @note Use corresponding API through QairtBackendExt_Allocator_t.
 */
Qairt_Status_t QairtBackendExt_Allocator_retrieveTensorBuffer(
  QairtBackendExt_Allocator_Handle_t allocatorHandle,
  QairtTensor_Handle_t tensorHandle,
  void** buffer);

/**
 * @brief Allocate a buffer for a tensor and bind it to the tensor handle.
 *
 * @param[in] allocatorHandle A handle to a backend extensions allocator.
 * @param[in,out] tensorHandles A handle to the tensor for which to allocate
 *                               a buffer.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Tensor buffer was successfully allocated.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _allocatorHandle_ or
 *           _tensorHandles_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_Allocator_t.
 */
Qairt_Status_t QairtBackendExt_Allocator_allocateTensorBuffer(
  QairtBackendExt_Allocator_Handle_t allocatorHandle,
  QairtTensor_Handle_t tensorHandles);

/**
 * @brief Deallocate the buffer bound to a tensor by
 *        QairtBackendExt_Allocator_allocateTensorBuffer().
 *
 * @param[in] allocatorHandle A handle to a backend extensions allocator.
 * @param[in] tensorHandle A handle to the tensor whose buffer to
 *                          deallocate.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Tensor buffer was successfully deallocated.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _allocatorHandle_ or
 *           _tensorHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_Allocator_t.
 */
Qairt_Status_t QairtBackendExt_Allocator_deallocateTensorBuffer(
  QairtBackendExt_Allocator_Handle_t allocatorHandle,
  QairtTensor_Handle_t tensorHandle);

/**
 * @brief Hook invoked before the client writes to a tensor's buffer,
 *        allowing the allocator to prepare the buffer for the write (e.g.
 *        cache invalidation).
 *
 * @param[in] allocatorHandle A handle to a backend extensions allocator.
 * @param[in] tensorHandle A handle to the tensor whose buffer is about to
 *                          be written to.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _allocatorHandle_ or
 *           _tensorHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_Allocator_t.
 */
Qairt_Status_t QairtBackendExt_Allocator_beforeWriteToTensorBuffer(
  QairtBackendExt_Allocator_Handle_t allocatorHandle,
  QairtTensor_Handle_t tensorHandle);

/**
 * @brief Hook invoked after the client writes to a tensor's buffer, allowing
 *        the allocator to flush or otherwise finalize the write.
 *
 * @param[in] allocatorHandle A handle to a backend extensions allocator.
 * @param[in] tensorHandle A handle to the tensor whose buffer was written
 *                          to.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _allocatorHandle_ or
 *           _tensorHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_Allocator_t.
 */
Qairt_Status_t QairtBackendExt_Allocator_afterWriteToTensorBuffer(
  QairtBackendExt_Allocator_Handle_t allocatorHandle,
  QairtTensor_Handle_t tensorHandle);

/**
 * @brief Hook invoked before the client reads from a tensor's buffer,
 *        allowing the allocator to synchronize the buffer's contents (e.g.
 *        cache invalidation) prior to the read.
 *
 * @param[in] allocatorHandle A handle to a backend extensions allocator.
 * @param[in] tensorHandle A handle to the tensor whose buffer is about to
 *                          be read from.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _allocatorHandle_ or
 *           _tensorHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_Allocator_t.
 */
Qairt_Status_t QairtBackendExt_Allocator_beforeReadFromTensorBuffer(
  QairtBackendExt_Allocator_Handle_t allocatorHandle,
  QairtTensor_Handle_t tensorHandle);

/**
 * @brief Hook invoked after the client reads from a tensor's buffer.
 *
 * @param[in] allocatorHandle A handle to a backend extensions allocator.
 * @param[in] tensorHandle A handle to the tensor whose buffer was read
 *                          from.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hook completed successfully.
 *         - QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE: _allocatorHandle_ or
 *           _tensorHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtBackendExt_Allocator_t.
 */
Qairt_Status_t QairtBackendExt_Allocator_afterReadFromTensorBuffer(
  QairtBackendExt_Allocator_Handle_t allocatorHandle,
  QairtTensor_Handle_t tensorHandle);

typedef Qairt_Status_t (*QairtBackendExt_Allocator_AllocateBufferFn_t)(
  QairtBackendExt_Allocator_Handle_t,
  uint64_t,
  uint64_t,
  void**);

typedef Qairt_Status_t (*QairtBackendExt_Allocator_DeallocateBufferFn_t)(
  QairtBackendExt_Allocator_Handle_t,
  void*);

typedef Qairt_Status_t (*QairtBackendExt_Allocator_RetrieveTensorBufferFn_t)(
  QairtBackendExt_Allocator_Handle_t,
  QairtTensor_Handle_t,
  void**);

typedef Qairt_Status_t (*QairtBackendExt_Allocator_AllocateTensorBufferFn_t)(
  QairtBackendExt_Allocator_Handle_t,
  QairtTensor_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_Allocator_DeallocateTensorBufferFn_t)(
  QairtBackendExt_Allocator_Handle_t,
  QairtTensor_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_Allocator_BeforeWriteToTensorBufferFn_t)(
  QairtBackendExt_Allocator_Handle_t,
  QairtTensor_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_Allocator_AfterWriteToTensorBufferFn_t)(
  QairtBackendExt_Allocator_Handle_t,
  QairtTensor_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_Allocator_BeforeReadFromTensorBufferFn_t)(
  QairtBackendExt_Allocator_Handle_t,
  QairtTensor_Handle_t);

typedef Qairt_Status_t (*QairtBackendExt_Allocator_AfterReadFromTensorBufferFn_t)(
  QairtBackendExt_Allocator_Handle_t,
  QairtTensor_Handle_t);
//=============================================================================
// Public Interfaces
//=============================================================================
/**
 * @struct QairtBackendExt_Allocator_t
 * @brief This struct contains the functions that define the interface for
 *        QAIRT Backend Extensions Allocator. This interface can be retrieved
 *        through the id QAIRT_BACKEND_EXT_ALLOCATOR_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtBackendExt_Allocator_AllocateBufferFn_t allocateBuffer;
  QairtBackendExt_Allocator_DeallocateBufferFn_t deallocateBuffer;
  QairtBackendExt_Allocator_RetrieveTensorBufferFn_t retrieveTensorBuffer;
  QairtBackendExt_Allocator_AllocateTensorBufferFn_t allocateTensorBuffer;
  QairtBackendExt_Allocator_DeallocateTensorBufferFn_t deallocateTensorBuffer;
  QairtBackendExt_Allocator_BeforeWriteToTensorBufferFn_t beforeWriteToTensorBuffer;
  QairtBackendExt_Allocator_AfterWriteToTensorBufferFn_t afterWriteToTensorBuffer;
  QairtBackendExt_Allocator_BeforeReadFromTensorBufferFn_t beforeReadFromTensorBuffer;
  QairtBackendExt_Allocator_AfterReadFromTensorBufferFn_t afterReadFromTensorBuffer;
} QairtBackendExt_Allocator_t;
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAISW_QAIRTBACKENDEXTALLOCATOR_H
