//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================

#ifndef QAISW_QAIRTMEMCOPYDESCRIPTOR_H
#define QAISW_QAIRTMEMCOPYDESCRIPTOR_H

#include "QairtMemCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Macros
//=============================================================================
#define QAIRT_MEM_COPY_DESCRIPTOR_V1_ID QAIRT_MIN_ID_MEM + 3

//=============================================================================
// Data Types
//=============================================================================
typedef struct _QairtMem_CopyDescriptorHandle_t* QairtMem_CopyDescriptorHandle_t;

/**
 * @brief Direction of a memory copy performed by QairtMem_copyBatch().
 */
typedef enum {
  /// Copy from the client (host) side to the accelerator side.
  QAIRT_MEM_COPY_HOST_TO_DEVICE = 0,
  /// Copy from the accelerator side to the client (host) side.
  QAIRT_MEM_COPY_DEVICE_TO_HOST = 1,
  // Unused, present to ensure 32 bits.
  QAIRT_MEM_COPY_UNDEFINED = 0x7FFFFFFF
} QairtMem_CopyDirection_t;

//=============================================================================
// Public Functions
//=============================================================================
/**
 * @brief Creates a memory copy descriptor handle.
 */
Qairt_Status_t QairtMem_CopyDescriptor_create(
    QairtMem_CopyDescriptorHandle_t* descriptorHandle);

/**
 * @brief Frees a memory copy descriptor handle.
 */
Qairt_Status_t QairtMem_CopyDescriptor_free(
    QairtMem_CopyDescriptorHandle_t descriptorHandle);

/**
 * @brief Sets the registered memory handle used by a copy request.
 *
 * The descriptor borrows the memory handle and does not extend its registration
 * lifetime. Freeing the descriptor does not deregister the memory.
 */
Qairt_Status_t QairtMem_CopyDescriptor_setMemHandle(
    QairtMem_CopyDescriptorHandle_t descriptorHandle,
    QairtMem_Handle_t memHandle);

/**
 * @brief Gets the registered memory handle used by a copy request.
 */
Qairt_Status_t QairtMem_CopyDescriptor_getMemHandle(
    QairtMem_CopyDescriptorHandle_t descriptorHandle,
    QairtMem_Handle_t* memHandle);

/**
 * @brief Sets the direction of a copy request.
 */
Qairt_Status_t QairtMem_CopyDescriptor_setDirection(
    QairtMem_CopyDescriptorHandle_t descriptorHandle,
    QairtMem_CopyDirection_t direction);

/**
 * @brief Gets the direction of a copy request.
 */
Qairt_Status_t QairtMem_CopyDescriptor_getDirection(
    QairtMem_CopyDescriptorHandle_t descriptorHandle,
    QairtMem_CopyDirection_t* direction);

/**
 * @brief Sets the byte offset of a copy request.
 */
Qairt_Status_t QairtMem_CopyDescriptor_setOffset(
    QairtMem_CopyDescriptorHandle_t descriptorHandle,
    uint64_t offset);

/**
 * @brief Gets the byte offset of a copy request.
 */
Qairt_Status_t QairtMem_CopyDescriptor_getOffset(
    QairtMem_CopyDescriptorHandle_t descriptorHandle,
    uint64_t* offset);

/**
 * @brief Sets the number of bytes to copy. Zero copies all remaining bytes.
 */
Qairt_Status_t QairtMem_CopyDescriptor_setSize(
    QairtMem_CopyDescriptorHandle_t descriptorHandle,
    uint64_t size);

/**
 * @brief Gets the number of bytes to copy.
 */
Qairt_Status_t QairtMem_CopyDescriptor_getSize(
    QairtMem_CopyDescriptorHandle_t descriptorHandle,
    uint64_t* size);

typedef Qairt_Status_t (*QairtMem_CopyDescriptor_CreateFn_t)(
    QairtMem_CopyDescriptorHandle_t*);
typedef Qairt_Status_t (*QairtMem_CopyDescriptor_FreeFn_t)(
    QairtMem_CopyDescriptorHandle_t);
typedef Qairt_Status_t (*QairtMem_CopyDescriptor_SetMemHandleFn_t)(
    QairtMem_CopyDescriptorHandle_t, QairtMem_Handle_t);
typedef Qairt_Status_t (*QairtMem_CopyDescriptor_GetMemHandleFn_t)(
    QairtMem_CopyDescriptorHandle_t, QairtMem_Handle_t*);
typedef Qairt_Status_t (*QairtMem_CopyDescriptor_SetDirectionFn_t)(
    QairtMem_CopyDescriptorHandle_t, QairtMem_CopyDirection_t);
typedef Qairt_Status_t (*QairtMem_CopyDescriptor_GetDirectionFn_t)(
    QairtMem_CopyDescriptorHandle_t, QairtMem_CopyDirection_t*);
typedef Qairt_Status_t (*QairtMem_CopyDescriptor_SetOffsetFn_t)(
    QairtMem_CopyDescriptorHandle_t, uint64_t);
typedef Qairt_Status_t (*QairtMem_CopyDescriptor_GetOffsetFn_t)(
    QairtMem_CopyDescriptorHandle_t, uint64_t*);
typedef Qairt_Status_t (*QairtMem_CopyDescriptor_SetSizeFn_t)(
    QairtMem_CopyDescriptorHandle_t, uint64_t);
typedef Qairt_Status_t (*QairtMem_CopyDescriptor_GetSizeFn_t)(
    QairtMem_CopyDescriptorHandle_t, uint64_t*);

//=============================================================================
// Public Interfaces
//=============================================================================
/**
 * @struct QairtMem_CopyDescriptorV1_t
 * @brief Functions that define the V1 interface for QAIRT memory copy descriptors.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtMem_CopyDescriptor_CreateFn_t create;
  QairtMem_CopyDescriptor_FreeFn_t free;
  QairtMem_CopyDescriptor_SetMemHandleFn_t setMemHandle;
  QairtMem_CopyDescriptor_GetMemHandleFn_t getMemHandle;
  QairtMem_CopyDescriptor_SetDirectionFn_t setDirection;
  QairtMem_CopyDescriptor_GetDirectionFn_t getDirection;
  QairtMem_CopyDescriptor_SetOffsetFn_t setOffset;
  QairtMem_CopyDescriptor_GetOffsetFn_t getOffset;
  QairtMem_CopyDescriptor_SetSizeFn_t setSize;
  QairtMem_CopyDescriptor_GetSizeFn_t getSize;
} QairtMem_CopyDescriptorV1_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAISW_QAIRTMEMCOPYDESCRIPTOR_H
