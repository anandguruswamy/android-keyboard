//==============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================
#pragma once

/**
 * @file
 * @brief C++ wrapper for the QAIRT Backend Extensions Allocator API.
 *
 *        A BackendExtensionsAllocator manages buffers on behalf of a BackendExtensions
 *        object, including tensor buffer allocation and read/write synchronization hooks.
 */

#include <memory>
#include <vector>

#include "../QairtApi.hpp"
#include "../QairtContext.hpp"
#include "QairtBackendExtensionsApi.hpp"
#include "QairtBackendExtensions/QairtBackendExtensions.h"
#include "QairtBackendExtensions/QairtBackendExtensionsAllocator.h"

namespace qairt {


/**
 * @brief Wrapper for a QAIRT Backend Extensions Allocator handle.
 *
 *        Obtained via `BackendExtensions::getAllocator()`.
 */
class BackendExtensionsAllocator : public ApiType<BackendExtensionsAllocator,
                                          QairtBackendExt_Allocator_t,
                                          BackendExtensionsApiTable> {
  friend class Api;
  friend class BackendExtensions;
  friend class BackendExtensionsApi;
  template <typename, typename, typename>
  friend class ::qairt::ApiType;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  BackendExtensionsAllocator() noexcept                                        = default;
  BackendExtensionsAllocator(const BackendExtensionsAllocator&)                = delete;
  BackendExtensionsAllocator(BackendExtensionsAllocator&&) noexcept            = default;
  BackendExtensionsAllocator& operator=(const BackendExtensionsAllocator&)     = delete;
  BackendExtensionsAllocator& operator=(BackendExtensionsAllocator&&) noexcept = default;

  /**
   * @brief Allocate a buffer using this allocator.
   *
   * @param[in] alignment  Required byte alignment of the returned buffer.
   * @param[in] size       Size in bytes of the buffer to allocate.
   * @param[out] buffer    Populated with a pointer to the allocated buffer.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtBackendExt_Allocator_allocateBuffer
   */
  void allocateBuffer(uint64_t alignment, uint64_t size, void** buffer) {
    callAndConfirm<&interface_type::allocateBuffer>(alignment, size, buffer);
  }

  /**
   * @brief Deallocate a buffer previously allocated via `allocateBuffer()`.
   *
   * @param[in] buffer  The buffer to deallocate.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtBackendExt_Allocator_deallocateBuffer
   */
  void deallocateBuffer(void* buffer) {
    callAndConfirm<&interface_type::deallocateBuffer>(buffer);
  }

  /**
   * @brief Retrieve the underlying buffer backing a tensor allocated by this allocator.
   *
   * @param[in] tensor    The tensor whose buffer to retrieve.
   * @param[out] buffer   Populated with a pointer to the tensor's underlying buffer.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtBackendExt_Allocator_retrieveTensorBuffer
   */
  void retrieveTensorBuffer(const Tensor& tensor, void** buffer) {
    callAndConfirm<&interface_type::retrieveTensorBuffer>(tensor.getHandle(), buffer);
  }

  /**
   * @brief Allocate a buffer for a tensor and bind it to the tensor handle.
   *
   * @param[in] tensor  The tensor for which to allocate a buffer.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_Allocator_allocateTensorBuffer
   */
  void allocateTensorBuffer(const Tensor& tensor) {
    callAndConfirm<&interface_type::allocateTensorBuffer>(tensor.getHandle());
  }

  /**
   * @brief Deallocate the buffer bound to a tensor by `allocateTensorBuffer()`.
   *
   * @param[in] tensor  The tensor whose buffer to deallocate.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_Allocator_deallocateTensorBuffer
   */
  void deallocateTensorBuffer(const Tensor& tensor) {
    callAndConfirm<&interface_type::deallocateTensorBuffer>(tensor.getHandle());
  }

  /**
   * @brief Hook invoked before the client writes to a tensor's buffer, allowing this
   *        allocator to prepare the buffer for the write (e.g. cache invalidation).
   *
   * @param[in] tensor  The tensor whose buffer is about to be written to.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_Allocator_beforeWriteToTensorBuffer
   */
  void beforeWriteToTensorBuffer(const Tensor& tensor) {
    callAndConfirm<&interface_type::beforeWriteToTensorBuffer>(tensor.getHandle());
  }

  /**
   * @brief Hook invoked after the client writes to a tensor's buffer, allowing this allocator
   *        to flush or otherwise finalize the write.
   *
   * @param[in] tensor  The tensor whose buffer was written to.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_Allocator_afterWriteToTensorBuffer
   */
  void afterWriteToTensorBuffer(const Tensor& tensor) {
    callAndConfirm<&interface_type::afterWriteToTensorBuffer>(tensor.getHandle());
  }

  /**
   * @brief Hook invoked before the client reads from a tensor's buffer, allowing this
   *        allocator to synchronize the buffer's contents (e.g. cache invalidation) prior to
   *        the read.
   *
   * @param[in] tensor  The tensor whose buffer is about to be read from.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_Allocator_beforeReadFromTensorBuffer
   */
  void beforeReadFromTensorBuffer(const Tensor& tensor) {
    callAndConfirm<&interface_type::beforeReadFromTensorBuffer>(tensor.getHandle());
  }

  /**
   * @brief Hook invoked after the client reads from a tensor's buffer.
   *
   * @param[in] tensor  The tensor whose buffer was read from.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtBackendExt_Allocator_afterReadFromTensorBuffer
   */
  void afterReadFromTensorBuffer(const Tensor& tensor) {
    callAndConfirm<&interface_type::afterReadFromTensorBuffer>(tensor.getHandle());
  }



 private:
  void prepareToCross() const {}
  void updateAfterCross() const {}
  BackendExtensionsAllocator(
    const std::shared_ptr<BackendExtensionsApiTable>& apiTable,
    QairtBackendExt_Allocator_Handle_t handle)
      : ApiType(apiTable) {
    setHandle(handle);
    setNonOwning(true);
  }
};

}  // namespace qairt
