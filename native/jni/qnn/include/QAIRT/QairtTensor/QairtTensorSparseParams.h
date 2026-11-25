//=============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//=============================================================================

/**
 * @file
 * @brief   Tensor Sparse Params component API
 *
 *          A header which contains the QAIRT Tensor Sparse Parameters component
 *          and the interfaces it implements. This component provides the sparse
 *          tensor layout configuration used when a tensor's data format is
 *          set to sparse (QAIRT_TENSOR_DATA_FORMAT_SPARSE).
 *
 *          The sparse layout is expressed as a two-level handle hierarchy:
 *            - QairtTensor_SparseParamsHandle_t  — top-level sparse params handle
 *              (holds the layout type and a reference to the layout handle)
 *            - QairtTensor_SparseParamsHybridCooHandle_t — hybrid COO layout handle
 *              (holds numSpecifiedElements and numSparseDimensions)
 *
 *          Additional layout types may be added in future API versions by
 *          introducing new layout handle types alongside the existing ones.
 */

#ifndef QAISW_QAIRTTENSORSPARSEPARAMS_H
#define QAISW_QAIRTTENSORSPARSEPARAMS_H

#include "QairtTensorCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Data Types
//=============================================================================

/**
 * @brief Opaque handle to a sparse parameters object.
 *
 *        The handle is created and freed by the client via
 *        QairtTensor_SparseParams_create() / QairtTensor_SparseParams_free().
 */
typedef struct _QairtTensor_SparseParamsHandle_t* QairtTensor_SparseParamsHandle_t;

/**
 * @brief Opaque handle to a hybrid COO sparse layout object.
 *
 *        The handle is created and freed by the client via
 *        QairtTensor_SparseParams_HybridCoo_create() /
 *        QairtTensor_SparseParams_HybridCoo_free().
 */
typedef struct _QairtTensor_SparseParams_HybridCooHandle_t*
    QairtTensor_SparseParams_HybridCooHandle_t;

/**
 * @brief Sparse layout type enumeration.
 *
 *        Used to identify which layout handle is active on a
 *        QairtTensor_SparseParamsHandle_t.
 */
typedef enum {
  /// Hybrid coordinate list sparse tensor layout.
  QAIRT_TENSOR_SPARSE_LAYOUT_HYBRID_COO = 0,
  // Unused, present to ensure 32 bits.
  QAIRT_TENSOR_SPARSE_LAYOUT_UNDEFINED  = 0x7FFFFFFF
} QairtTensor_SparseParams_LayoutType_t;

//=============================================================================
// QairtTensor_SparseParams_HybridCoo Functions
//=============================================================================

/**
 * @brief Create a hybrid COO sparse layout handle.
 *
 * @param[out] hybridCooHandle A handle to the created hybrid COO layout.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Handle was successfully created.
 *         - QAIRT_TENSOR_ERROR_INVALID_ARGUMENT: _hybridCooHandle_ is NULL.
 *         - QAIRT_TENSOR_ERROR_MEM_ALLOC: Memory allocation error.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsHybridCooV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_HybridCoo_create(
    QairtTensor_SparseParams_HybridCooHandle_t* hybridCooHandle);

/**
 * @brief Free a hybrid COO sparse layout handle.
 *
 * @param[in] hybridCooHandle A handle to a created hybrid COO layout.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Handle was successfully freed.
 *         - QAIRT_TENSOR_ERROR_INVALID_HANDLE: _hybridCooHandle_ is not a
 *           valid handle.
 *         - QAIRT_TENSOR_ERROR_MEM_ALLOC: Memory deallocation error.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsHybridCooV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_HybridCoo_free(
    QairtTensor_SparseParams_HybridCooHandle_t hybridCooHandle);

/**
 * @brief Set the number of specified elements on a hybrid COO layout.
 *
 *        This value is treated as the maximum number of specified (non-zero)
 *        elements when creating a tensor.
 *
 * @param[in] hybridCooHandle      A handle to a created hybrid COO layout.
 * @param[in] numSpecifiedElements The number of specified elements to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully set.
 *         - QAIRT_TENSOR_ERROR_INVALID_HANDLE: _hybridCooHandle_ is not a
 *           valid handle.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsHybridCooV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_HybridCoo_setNumSpecifiedElements(
    QairtTensor_SparseParams_HybridCooHandle_t hybridCooHandle, uint32_t numSpecifiedElements);

/**
 * @brief Get the number of specified elements from a hybrid COO layout.
 *
 * @param[in]  hybridCooHandle      A handle to a created hybrid COO layout.
 * @param[out] numSpecifiedElements Receives the number of specified elements.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_TENSOR_ERROR_INVALID_HANDLE: _hybridCooHandle_ is not a
 *           valid handle.
 *         - QAIRT_TENSOR_ERROR_INVALID_ARGUMENT: _numSpecifiedElements_ is NULL.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsHybridCooV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_HybridCoo_getNumSpecifiedElements(
    QairtTensor_SparseParams_HybridCooHandle_t hybridCooHandle, uint32_t* numSpecifiedElements);

/**
 * @brief Set the number of sparse dimensions on a hybrid COO layout.
 *
 *        The size of the index can range from 1 to the rank of the tensor.
 *        This feature allows for partially sparse tensors.
 *
 * @param[in] hybridCooHandle     A handle to a created hybrid COO layout.
 * @param[in] numSparseDimensions The number of sparse dimensions to set.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully set.
 *         - QAIRT_TENSOR_ERROR_INVALID_HANDLE: _hybridCooHandle_ is not a
 *           valid handle.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsHybridCooV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_HybridCoo_setNumSparseDimensions(
    QairtTensor_SparseParams_HybridCooHandle_t hybridCooHandle, uint32_t numSparseDimensions);

/**
 * @brief Get the number of sparse dimensions from a hybrid COO layout.
 *
 * @param[in]  hybridCooHandle     A handle to a created hybrid COO layout.
 * @param[out] numSparseDimensions Receives the number of sparse dimensions.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Value was successfully retrieved.
 *         - QAIRT_TENSOR_ERROR_INVALID_HANDLE: _hybridCooHandle_ is not a
 *           valid handle.
 *         - QAIRT_TENSOR_ERROR_INVALID_ARGUMENT: _numSparseDimensions_ is NULL.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsHybridCooV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_HybridCoo_getNumSparseDimensions(
    QairtTensor_SparseParams_HybridCooHandle_t hybridCooHandle, uint32_t* numSparseDimensions);

//=============================================================================
// QairtTensor_SparseParams Functions
//=============================================================================

/**
 * @brief Create a sparse params handle.
 *
 * @param[out] sparseParamsHandle A handle to the created sparse params.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Sparse params was successfully created.
 *         - QAIRT_TENSOR_ERROR_INVALID_ARGUMENT: _sparseParamsHandle_ is NULL.
 *         - QAIRT_TENSOR_ERROR_MEM_ALLOC: Memory allocation error.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_create(
    QairtTensor_SparseParamsHandle_t* sparseParamsHandle);

/**
 * @brief Free a sparse params handle.
 *
 * @param[in] sparseParamsHandle A handle to a created sparse params.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Sparse params was successfully freed.
 *         - QAIRT_TENSOR_ERROR_INVALID_HANDLE: _sparseParamsHandle_ is not a
 *           valid handle.
 *         - QAIRT_TENSOR_ERROR_MEM_ALLOC: Memory deallocation error.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_free(
    QairtTensor_SparseParamsHandle_t sparseParamsHandle);

/**
 * @brief Get the active sparse layout type from a sparse params handle.
 *
 * @param[in]  sparseParamsHandle A handle to a created sparse params.
 * @param[out] layoutType         Receives the active sparse layout type.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Layout type was successfully retrieved.
 *         - QAIRT_TENSOR_ERROR_INVALID_HANDLE: _sparseParamsHandle_ is not a
 *           valid handle.
 *         - QAIRT_TENSOR_ERROR_INVALID_ARGUMENT: _layoutType_ is NULL.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_getLayoutType(
    QairtTensor_SparseParamsHandle_t sparseParamsHandle,
    QairtTensor_SparseParams_LayoutType_t* layoutType);

/**
 * @brief Set the hybrid COO layout on a sparse params handle.
 *
 *        Sets the active layout type to QAIRT_TENSOR_SPARSE_LAYOUT_HYBRID_COO
 *        and copies the configuration from the provided hybrid COO handle into
 *        the sparse params.
 *
 * @param[in] sparseParamsHandle A handle to a created sparse params.
 * @param[in] hybridCooHandle    A handle to a configured hybrid COO layout.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hybrid COO layout was successfully set.
 *         - QAIRT_TENSOR_ERROR_INVALID_HANDLE: _sparseParamsHandle_ or
 *           _hybridCooHandle_ is not a valid handle.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_setHybridCoo(
    QairtTensor_SparseParamsHandle_t sparseParamsHandle,
    QairtTensor_SparseParams_HybridCooHandle_t hybridCooHandle);

/**
 * @brief Get the hybrid COO layout from a sparse params handle.
 *
 *        Returns a referenced handle to the hybrid COO layout stored inside
 *        the sparse params. The referenced handle should not be freed by the
 *        caller and is invalidated if the sparse params handle is modified.
 *
 * @param[in]  sparseParamsHandle A handle to a created sparse params.
 * @param[out] hybridCooHandle    Receives a referenced hybrid COO layout handle.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: Hybrid COO layout was successfully retrieved.
 *         - QAIRT_TENSOR_ERROR_INVALID_HANDLE: _sparseParamsHandle_ is not a
 *           valid handle.
 *         - QAIRT_TENSOR_ERROR_INVALID_ARGUMENT: _hybridCooHandle_ is NULL.
 *         - QAIRT_TENSOR_ERROR_UNSUPPORTED_TENSOR_PARAM: Active layout type is
 *           not QAIRT_TENSOR_SPARSE_LAYOUT_HYBRID_COO.
 *
 * @note Use corresponding API through QairtTensor_SparseParamsV1_t.
 */
Qairt_Status_t QairtTensor_SparseParams_getHybridCoo(
    QairtTensor_SparseParamsHandle_t sparseParamsHandle,
    QairtTensor_SparseParams_HybridCooHandle_t* hybridCooHandle);

//=============================================================================
// Function Pointer Typedefs — QairtTensor_SparseParams_HybridCoo
//=============================================================================

typedef Qairt_Status_t (*QairtTensor_SparseParams_HybridCoo_CreateFn_t)(
    QairtTensor_SparseParams_HybridCooHandle_t*);
typedef Qairt_Status_t (*QairtTensor_SparseParams_HybridCoo_FreeFn_t)(
    QairtTensor_SparseParams_HybridCooHandle_t);
typedef Qairt_Status_t (*QairtTensor_SparseParams_HybridCoo_SetNumSpecifiedElementsFn_t)(
    QairtTensor_SparseParams_HybridCooHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtTensor_SparseParams_HybridCoo_GetNumSpecifiedElementsFn_t)(
    QairtTensor_SparseParams_HybridCooHandle_t, uint32_t*);
typedef Qairt_Status_t (*QairtTensor_SparseParams_HybridCoo_SetNumSparseDimensionsFn_t)(
    QairtTensor_SparseParams_HybridCooHandle_t, uint32_t);
typedef Qairt_Status_t (*QairtTensor_SparseParams_HybridCoo_GetNumSparseDimensionsFn_t)(
    QairtTensor_SparseParams_HybridCooHandle_t, uint32_t*);

//=============================================================================
// Function Pointer Typedefs — QairtTensor_SparseParams
//=============================================================================

typedef Qairt_Status_t (*QairtTensor_SparseParams_CreateFn_t)(
    QairtTensor_SparseParamsHandle_t*);
typedef Qairt_Status_t (*QairtTensor_SparseParams_FreeFn_t)(
    QairtTensor_SparseParamsHandle_t);
typedef Qairt_Status_t (*QairtTensor_SparseParams_GetLayoutTypeFn_t)(
    QairtTensor_SparseParamsHandle_t, QairtTensor_SparseParams_LayoutType_t*);
typedef Qairt_Status_t (*QairtTensor_SparseParams_SetHybridCooFn_t)(
    QairtTensor_SparseParamsHandle_t, QairtTensor_SparseParams_HybridCooHandle_t);
typedef Qairt_Status_t (*QairtTensor_SparseParams_GetHybridCooFn_t)(
    QairtTensor_SparseParamsHandle_t, QairtTensor_SparseParams_HybridCooHandle_t*);

//=============================================================================
// Public Interfaces
//=============================================================================

/**
 * @struct QairtTensor_SparseParams_HybridCooV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Tensor Sparse Parameters Hybrid COO layout. This interface can
 *        be retrieved through the id QAIRT_TENSOR_SPARSE_PARAMS_HYBRID_COO_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtTensor_SparseParams_HybridCoo_CreateFn_t create;
  QairtTensor_SparseParams_HybridCoo_FreeFn_t free;
  QairtTensor_SparseParams_HybridCoo_SetNumSpecifiedElementsFn_t setNumSpecifiedElements;
  QairtTensor_SparseParams_HybridCoo_GetNumSpecifiedElementsFn_t getNumSpecifiedElements;
  QairtTensor_SparseParams_HybridCoo_SetNumSparseDimensionsFn_t setNumSparseDimensions;
  QairtTensor_SparseParams_HybridCoo_GetNumSparseDimensionsFn_t getNumSparseDimensions;
} QairtTensor_SparseParams_HybridCooV1_t;

/**
 * @struct QairtTensor_SparseParamsV1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT Tensor Sparse Parameters. This interface can be retrieved through
 *        the id QAIRT_TENSOR_SPARSE_PARAMS_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtTensor_SparseParams_CreateFn_t create;
  QairtTensor_SparseParams_FreeFn_t free;
  QairtTensor_SparseParams_GetLayoutTypeFn_t getLayoutType;
  QairtTensor_SparseParams_SetHybridCooFn_t setHybridCoo;
  QairtTensor_SparseParams_GetHybridCooFn_t getHybridCoo;
} QairtTensor_SparseParamsV1_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAISW_QAIRTTENSORSPARSEPARAMS_H
