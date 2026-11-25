//==============================================================================
//
// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// All rights reserved.
// Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

#pragma once

/**
 * @file
 * @brief C++ wrappers for QAIRT memory descriptors, shapes, and operations.
 *
 *        Provides type-safe access to memory descriptors and shapes, and
 *        operations on registered memory through MemOperations.
 *
 */

#include <limits>
#include <vector>

#include "QairtApi.hpp"
#include "QairtMem/QairtMemCommon.h"
#include "QairtMem/QairtMemCopyDescriptor.h"
#include "QairtMem/QairtMemDescriptor.h"
#include "QairtMem/QairtMemOperations.h"
#include "QairtMem/QairtMemShape.h"
#include "QairtOpConfig.hpp"

namespace qairt {

// Forward declaration for DataType enum defined in QairtTensor.hpp
enum class DataType : std::underlying_type_t<Qairt_DataType_t>;

/**
 * @brief Error codes returned by QAIRT memory operations.
 *
 * @details
 * | Enumerator          | Description                                          |
 * |---------------------|------------------------------------------------------|
 * | `NoError`           | Operation succeeded.                                 |
 * | `NotSupported`      | The requested functionality is not supported by the backend. |
 * | `InvalidArgument`   | An argument to the operation was invalid.            |
 * | `InvalidHandle`     | The provided handle is not valid.                    |
 * | `AlreadyRegistered` | The provided memory has already been registered.     |
 * | `Mapping`           | An error occurred during memory mapping.             |
 * | `InvalidShape`      | The memory shape is invalid for this backend.        |
 * | `UnsupportedMemtype`| The requested memory type is not supported by the backend. |
 * | `Undefined`         | An undefined or unknown error occurred.              |
 */
QAIRT_CPPAPI_DEFINE_ENUM(MemError,
                         QairtMem_Error_t,
                         NoError            = QAIRT_MEM_NO_ERROR,
                         NotSupported       = QAIRT_MEM_ERROR_NOT_SUPPORTED,
                         InvalidArgument    = QAIRT_MEM_ERROR_INVALID_ARGUMENT,
                         InvalidHandle      = QAIRT_MEM_ERROR_INVALID_HANDLE,
                         AlreadyRegistered  = QAIRT_MEM_ERROR_ALREADY_REGISTERED,
                         Mapping            = QAIRT_MEM_ERROR_MAPPING,
                         InvalidShape       = QAIRT_MEM_ERROR_INVALID_SHAPE,
                         UnsupportedMemtype = QAIRT_MEM_ERROR_UNSUPPORTED_MEMTYPE,
                         Undefined          = QAIRT_MEM_ERROR_UNDEFINED)

/**
 * @brief Memory types used to provide data for a QAIRT tensor.
 *
 * @details
 * | Enumerator    | Description                                         |
 * |---------------|-----------------------------------------------------|
 * | `Unsupported` | Memory type is not supported.                       |
 * | `Custom`      | Memory allocated by a custom backend mechanism.     |
 * | `DmaBuf`      | Memory allocated via the DMA-BUF subsystem.         |
 * | `Undefined`   | Memory type is undefined.                           |
 */
QAIRT_CPPAPI_DEFINE_ENUM(MemType,
                         Qairt_MemType_t,
                         Unsupported = QAIRT_UNSUPPORTED,
                         Custom      = QAIRT_MEM_TYPE_CUSTOM,
                         DmaBuf      = QAIRT_MEM_TYPE_DMA_BUF,
                         Undefined   = QAIRT_MEM_TYPE_UNDEFINED)

/**
 * @brief Direction for a QAIRT registered-memory copy operation.
 *
 * @details
 * | Enumerator     | Description                                  |
 * |----------------|----------------------------------------------|
 * | `HostToDevice` | Copy from host memory to accelerator memory. |
 * | `DeviceToHost` | Copy from accelerator memory to host memory. |
 * | `Undefined`    | Direction is unspecified or invalid.        |
 */
QAIRT_CPPAPI_DEFINE_ENUM(MemCopyDirection,
                         QairtMem_CopyDirection_t,
                         HostToDevice = QAIRT_MEM_COPY_HOST_TO_DEVICE,
                         DeviceToHost = QAIRT_MEM_COPY_DEVICE_TO_HOST,
                         Undefined    = QAIRT_MEM_COPY_UNDEFINED)

/**
 * @brief Wrapper for a QAIRT memory shape handle.
 */
class MemShape : public ApiType<MemShape, QairtMem_ShapeV1_t> {
  friend class Api;

  /// @brief Backend-specific shape configuration string.
  detail::crossable<std::string, &interface_type::getShapeConfig, &interface_type::setShapeConfig>
      m_shapeConfig;

  /// @brief Cached dimension values for this shape.
  mutable std::vector<uint32_t> m_dimensions;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  MemShape() noexcept                      = default;
  MemShape(const MemShape&)                = delete;
  MemShape(MemShape&&) noexcept            = default;
  MemShape& operator=(const MemShape&)     = delete;
  MemShape& operator=(MemShape&&) noexcept = default;

  /**
   * @brief Create a shallow copy of this shape with the same rank, dimensions, and config.
   *
   * @return A new @ref MemShape with the same rank, dimensions, and shape config.
   *
   * @throws qairt::Exception on invalid handle.
   */
  MemShape shallowCopy() const {
    MemShape toret(copy_table_tag, *this);
    toret.construct();
    toret.setDimensions(getDimensions());
    toret.setShapeConfig(getShapeConfig());

    return toret;
  }

  /**
   * @brief Get the backend-specific shape configuration string.
   *
   * @return Reference to the shape config string. Empty if none was set.
   *
   * @see QairtMem_Shape_getShapeConfig
   */
  std::string& getShapeConfig() { return m_shapeConfig.get(); }

  /**
   * @brief Get the backend-specific shape configuration string.
   *
   * @return Const reference to the shape config string. Empty if none was set.
   *
   * @see QairtMem_Shape_getShapeConfig
   */
  const std::string& getShapeConfig() const { return m_shapeConfig.get(); }

  /**
   * @brief Set the backend-specific shape configuration string.
   *
   * Refer to the backend documentation for valid values. NULL is allowed
   * in the underlying C API; pass an empty string to clear.
   *
   * @param[in] shapeConfig  Backend-specific configuration string for this shape.
   *
   * @see QairtMem_Shape_setShapeConfig
   */
  void setShapeConfig(std::string shapeConfig) { m_shapeConfig = std::move(shapeConfig); }

  /**
   * @brief Get the number of dimensions (rank) of this memory shape.
   *
   * @return Number of dimensions in this shape.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtMem_Shape_getRank
   */
  uint32_t getRank() const { return callGetter<&interface_type::getRank>(); }

  /**
   * @brief Get the size of a single dimension by index.
   *
   * @param[in] index  Zero-based dimension index. Must be less than getRank().
   *
   * @return Size of the dimension at _index_.
   *
   * @throws qairt::Exception on invalid handle or out-of-range index.
   *
   * @see QairtMem_Shape_getDimensionAt
   */
  uint32_t getDimensionAt(uint32_t index) const {
    uint32_t toret{};
    callAndConfirm<&interface_type::getDimensionAt>(index, &toret);
    return toret;
  }

  /**
   * @brief Get the full ordered list of dimension sizes for this shape.
   *
   * @return Reference to a vector of dimension sizes in order.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtMem_Shape_getDimensionAt
   */
  std::vector<uint32_t>& getDimensions() {
    const uint32_t rank = getRank();
    m_dimensions.resize(rank);
    for (uint32_t i = 0; i < rank; ++i) {
      callAndConfirm<&interface_type::getDimensionAt>(i, &(m_dimensions[i]));
    }
    return m_dimensions;
  }

  /**
   * @brief Get the full ordered list of dimension sizes for this shape.
   *
   * @return Const reference to a vector of dimension sizes in order.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtMem_Shape_getDimensionAt
   */
  const std::vector<uint32_t>& getDimensions() const {
    const uint32_t rank = getRank();
    m_dimensions.resize(rank);
    for (uint32_t i = 0; i < rank; ++i) {
      callAndConfirm<&interface_type::getDimensionAt>(i, &(m_dimensions[i]));
    }
    return m_dimensions;
  }

  /**
   * @brief Set the ordered list of dimension sizes for this shape.
   *
   * @param[in] dimensions  Ordered dimension sizes. The number of elements determines the rank.
   *
   * @see QairtMem_Shape_setDimensions
   */
  void setDimensions(std::vector<uint32_t> dimensions) { m_dimensions = std::move(dimensions); }

 private:
  // Called by Api::make<MemShape>() — allocates the underlying C handle.
  explicit MemShape(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }

  void prepareToCross() const {
    if (!m_shapeConfig.get().empty()) {
      prepareMultipleToCross(m_shapeConfig);
    }

    callSetter<&interface_type::setDimensions>(m_dimensions.data(), m_dimensions.size());
  }
  void updateAfterCross() const {
    updateMultipleAfterCross(m_shapeConfig);
  }
};

/**
 * @brief Wrapper for a QAIRT memory descriptor handle.
 */
class MemDescriptor : public ApiType<MemDescriptor, QairtMem_DescriptorV1_t> {
  friend class Api;

  /// @brief Shape associated with this memory descriptor.
  detail::crossable<detail::non_owning<MemShape>,
                    &interface_type::getMemShape,
                    &interface_type::setMemShape>
      m_memShape;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  MemDescriptor() noexcept                           = default;
  MemDescriptor(const MemDescriptor&)                = delete;
  MemDescriptor(MemDescriptor&&) noexcept            = default;
  MemDescriptor& operator=(const MemDescriptor&)     = delete;
  MemDescriptor& operator=(MemDescriptor&&) noexcept = default;

  /**
   * @brief Set the Direct Memory Access buffer (DMA-BUF) file descriptor and data pointer.
   *
   * @param[in] dmaBufFd    DMA-BUF file descriptor. Must be a valid open file
   *                        descriptor, or @c QAIRT_MEM_INVALID_FD.
   * @param[in] dmaBufData  Pointer to the memory-mapped region produced by @c mmap
   *                        on _dmaBufFd_. NULL is allowed when a DMA-BUF is not used.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtMem_Descriptor_setDmaBuf
   */
  void setDmaBuf(int32_t dmaBufFd, void* dmaBufData) {
    callSetter<&interface_type::setDmaBuf>(dmaBufFd, dmaBufData);
  }

  /**
   * @brief Set backend-specific custom info on this descriptor.
   *
   * Used to pass backend-specific memory descriptors to the backend via
   * Context::registerMem().
   * The custom info handle must remain valid until registerMem() returns.
   *
   * @param[in] customInfo  Opaque handle to backend-specific custom info.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtMem_Descriptor_setCustomInfo
   */
  void setCustomInfo(QairtMem_CustomInfoHandle_t customInfo) {
    callSetter<&interface_type::setCustomInfo>(customInfo);
  }

  /**
   * @brief Get the Direct Memory Access buffer (DMA-BUF) file descriptor and data pointer.
   *
   * @return Pair of (file descriptor, data pointer) previously set via setDmaBuf().
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtMem_Descriptor_getDmaBuf
   */
  std::pair<int32_t, void*> getDmaBuf() const {
    std::pair<int32_t, void*> toret;
    callAndConfirm<&interface_type::getDmaBuf>(&std::get<0>(toret), &std::get<1>(toret));
    return toret;
  }

  /**
   * @brief Set the memory shape for this descriptor.
   *
   * @param[in] memShape  Memory shape describing the rank and per-dimension
   *                      sizes of the buffer.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtMem_Descriptor_setMemShape
   */
  void setMemShape(const MemShape& memShape) {
    QAIRT_NO_RETURN m_memShape.setNonOwning(*this, memShape);
  }

  /**
   * @brief Get the memory shape associated with this descriptor.
   *
   * @return Reference to the memory shape for this descriptor.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtMem_Descriptor_getMemShape
   */
  MemShape& getMemShape() { return m_memShape.bindNonOwning(*this); }

  /// @see MemDescriptor::getMemShape()
  const MemShape& getMemShape() const { return m_memShape.bindNonOwning(*this); }

  /**
   * @brief Set the data type for this memory descriptor.
   *
   * @param[in] dataType  Data type for the memory described by this descriptor.
   *
   * @throws qairt::Exception on invalid handle or invalid argument.
   *
   * @see QairtMem_Descriptor_setDataType
   */
  void setDataType(qairt::DataType dataType) {
    callSetter<&interface_type::setDataType>(static_cast<Qairt_DataType_t>(dataType));
  }

  /**
   * @brief Get the data type of the memory descriptor.
   *
   * @return The data type.
   *
   * @throws qairt::Exception on invalid handle.
   *
   * @see QairtMem_Descriptor_getDataType
   */
  qairt::DataType getDataType() const {
    return static_cast<qairt::DataType>(callGetter<&interface_type::getDataType>());
  }

 private:
  // Called by Api::make<MemDescriptor>() — allocates the underlying C handle.
  explicit MemDescriptor(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }
  void prepareToCross() const { prepareMultipleToCross(m_memShape); }
  void updateAfterCross() const { updateMultipleAfterCross(m_memShape); }
};

/**
 * @brief Typed wrapper for a QAIRT memory handle produced by
 *        @ref Context::registerMem.
 *
 *        Lightweight, copyable value type that hides the raw
 *        @c QairtMem_Handle_t. Non-owning: cleanup is via
 *        @ref Context::deregisterMem, mirroring the explicit-deregister
 *        semantics of the QAIRT C API. Pass @ref getHandle() to APIs that
 *        consume the raw handle (e.g. @c TensorMemory::setMemHandle).
 */
class MemHandle {
  QairtMem_Handle_t m_handle{};

 public:
  MemHandle() noexcept                            = default;
  MemHandle(const MemHandle&) noexcept            = default;
  MemHandle(MemHandle&&) noexcept                 = default;
  MemHandle& operator=(const MemHandle&) noexcept = default;
  MemHandle& operator=(MemHandle&&) noexcept      = default;

  explicit MemHandle(QairtMem_Handle_t handle) noexcept : m_handle(handle) {}

  /// @brief Get the underlying QAIRT C handle.
  QairtMem_Handle_t getHandle() const noexcept { return m_handle; }

  /// @brief True if this wraps a non-null handle.
  explicit operator bool() const noexcept { return m_handle != nullptr; }
};

/**
 * @brief Wrapper for a QAIRT memory copy descriptor handle.
 *
 * Construct via Api::make<MemCopyDescriptor>(). Owns the descriptor, but only
 * borrows the registered memory handle set by setMemHandle(). Destroying the
 * descriptor does not deregister memory or free its backing allocations.
 * The registration must remain valid whenever this descriptor is submitted.
 */
class MemCopyDescriptor : public ApiType<MemCopyDescriptor, QairtMem_CopyDescriptorV1_t> {
  friend class Api;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  /// @brief Set the registered memory handle used by this copy request.
  void setMemHandle(const MemHandle& memHandle) {
    callSetter<&interface_type::setMemHandle>(memHandle.getHandle());
  }

  /// @brief Get the registered memory handle used by this copy request.
  MemHandle getMemHandle() const {
    return MemHandle(callGetter<&interface_type::getMemHandle>());
  }

  /// @brief Get the direction of this copy request.
  MemCopyDirection getDirection() const {
    return static_cast<MemCopyDirection>(callGetter<&interface_type::getDirection>());
  }

  /// @brief Set the direction of this copy request.
  void setDirection(MemCopyDirection direction) {
    callSetter<&interface_type::setDirection>(static_cast<QairtMem_CopyDirection_t>(direction));
  }

  /// @brief Get the byte offset at which this copy starts.
  uint64_t getOffset() const { return callGetter<&interface_type::getOffset>(); }

  /// @brief Set the byte offset at which this copy starts.
  void setOffset(uint64_t offset) { callSetter<&interface_type::setOffset>(offset); }

  /// @brief Get the number of bytes to copy.
  uint64_t getSize() const { return callGetter<&interface_type::getSize>(); }

  /// @brief Set the number of bytes to copy. Zero copies all remaining bytes.
  void setSize(uint64_t size) { callSetter<&interface_type::setSize>(size); }

 private:
  explicit MemCopyDescriptor(const std::shared_ptr<ApiTable>& apiTable) : ApiType(apiTable) {
    construct();
  }
};

/**
 * @brief Operations on registered memory, dispatched through the QAIRT memory interface.
 *
 * Construct via Api::make<MemOperations>(). This dispatcher has no C handle
 * or create/free lifecycle and is not bound to a Context. The Api must outlive
 * it; descriptors, scalars, and memory handles must be compatible with that
 * Api's backend. It does not extend the lifetime of memory registrations.
 *
 * For copro-resident graph I/O, register memory through Context::registerMem()
 * and bind it to tensors. Use copyBatch() to upload inputs and fill() for any
 * required initialization before Graph::execute(), then copyBatch() to download
 * outputs. Complete any asynchronous execution before reading or reusing its
 * memory, and deregister memory after all users have finished.
 */
class MemOperations : public ApiType<MemOperations, QairtMem_OperationsV1_t> {
  friend class Api;

 public:
  using ApiType::ApiType;
  using ApiType::operator=;

  MemOperations() noexcept                            = default;
  MemOperations(const MemOperations&)                  = delete;
  MemOperations(MemOperations&&) noexcept               = default;
  MemOperations& operator=(const MemOperations&)        = delete;
  MemOperations& operator=(MemOperations&&) noexcept    = default;

  /**
   * @brief Copy between the client and accelerator sides of registered memory.
   *
   * @param[in] descriptors Copy requests, supplied to the backend in order.
   *                         Each descriptor borrows its registered memory handle.
   *                         Offsets and sizes are in bytes; size zero copies the
   *                         remaining bytes from the offset.
   *
   * @throws qairt::Exception on invalid handles or arguments, an unsupported
   *         operation, or a batch size exceeding uint32_t. Empty batches are
   *         forwarded to the C API, which rejects them as invalid arguments.
   *
   * @note A backend copy failure may leave earlier requests completed; copies
   *       are not rolled back. This wrapper does not provide atomic batches.
   *
   * @see QairtMem_copyBatch
   */
  void copyBatch(const std::vector<MemCopyDescriptor>& descriptors) const {
    if (descriptors.size() > (std::numeric_limits<uint32_t>::max)()) {
      throw Exception("Memory copy batch exceeds uint32_t", MemError::InvalidArgument);
    }

    std::vector<QairtMem_CopyDescriptorHandle_t> descriptorHandles;
    descriptorHandles.reserve(descriptors.size());
    for (const auto& descriptor : descriptors) {
      descriptorHandles.push_back(getHandle(descriptor));
    }

    callNoHandleAndConfirm<&interface_type::copyBatch>(
        descriptorHandles.data(), static_cast<uint32_t>(descriptorHandles.size()));
  }

  /**
   * @brief Fill the accelerator side of registered memory with a scalar value.
   *
   * @param[in] memHandle Borrowed registered memory handle to fill.
   * @param[in] offsetElements Offset in elements of the scalar's data type.
   * @param[in] value Repeated scalar value; its data type determines element size.
   * @param[in] numElements Number of elements to fill. Zero writes no elements;
   *                        the request still undergoes C API validation.
   *
   * @throws qairt::Exception on invalid handles or arguments, an unsupported
   *         operation, or an unsupported scalar data type.
   *
   * @see QairtMem_fill
   */
  void fill(const MemHandle& memHandle,
            uint64_t offsetElements,
            const Scalar& value,
            uint64_t numElements) const {
    callNoHandleAndConfirm<&interface_type::fill>(
        memHandle.getHandle(), offsetElements, getHandle(value), numElements);
  }
};

}  // namespace qairt
