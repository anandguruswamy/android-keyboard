//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================

#ifndef QAISW_QAIRTMEMOPERATIONS_H
#define QAISW_QAIRTMEMOPERATIONS_H

#include "QairtCommon/QairtScalar.h"
#include "QairtMemCopyDescriptor.h"
#include "QairtMemCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Macros
//=============================================================================
#define QAIRT_MEM_OPERATIONS_V1_ID QAIRT_MIN_ID_MEM + 1
/**
 * @brief Copy data between the client and accelerator sides of registered memory handles.
 *
 * @param[in] descriptors Array of copy descriptor handles. Referenced memory
 *                        registrations must remain valid for the call.
 * @param[in] numDescriptors Number of descriptors. Must be greater than zero.
 *
 * A NULL array or zero count returns QAIRT_MEM_ERROR_INVALID_ARGUMENT.
 */
Qairt_Status_t QairtMem_copyBatch(const QairtMem_CopyDescriptorHandle_t* descriptors,
                                  uint32_t numDescriptors);

/**
 * @brief Fill the accelerator side of a registered memory handle with a scalar value.
 *
 * @param[in] memHandle Borrowed memory handle with an accelerator allocation.
 * @param[in] offsetElements Offset in elements of the scalar's data type.
 * @param[in] value Scalar handle; its data type determines the element size.
 * @param[in] numElements Number of elements to fill. Zero writes no elements;
 *                        handle validation still applies.
 */
Qairt_Status_t QairtMem_fill(QairtMem_Handle_t memHandle,
                             uint64_t offsetElements,
                             QairtScalar_Handle_t value,
                             uint64_t numElements);

typedef Qairt_Status_t (*QairtMem_CopyBatchFn_t)(
    const QairtMem_CopyDescriptorHandle_t* descriptors, uint32_t numDescriptors);
typedef Qairt_Status_t (*QairtMem_FillFn_t)(QairtMem_Handle_t memHandle,
                                            uint64_t offsetElements,
                                            QairtScalar_Handle_t value,
                                            uint64_t numElements);

//=============================================================================
// Public Interfaces
//=============================================================================
/**
 * @struct QairtMem_OperationsV1_t
 * @brief Functions for operations on registered memory handles.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtMem_CopyBatchFn_t copyBatch;
  QairtMem_FillFn_t fill;
} QairtMem_OperationsV1_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAISW_QAIRTMEMOPERATIONS_H
