//==============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

/**
 * @file
 * @brief   System Lifecycle component API
 *
 *          A header which contains the QAIRT System Lifecycle component and the
 *          interface it implements. The System Lifecycle API is process-global
 *          (handle-less) and covers process-wide backend lifetime management for
 *          the gate-keeping backend-sharing feature.
 */

#ifndef QAISW_QAIRT_SYSTEM_LIFECYCLE_H
#define QAISW_QAIRT_SYSTEM_LIFECYCLE_H

#include "QairtSystemCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Macros
//=============================================================================

/**
 * @brief Interface ID for QairtSystemLifecycle V1
 */
#define QAIRT_SYSTEM_LIFECYCLE_V1_ID (QAIRT_MIN_ID_SYSTEM + 12)

//=============================================================================
// Public Functions
//=============================================================================

/**
 * @brief Disable process-wide deferred cleanup and release all deferred backend holds.
 *
 *        Process-global, handle-less. Call after all Qairt instances have been
 *        destroyed to release backends pinned by QairtSystemBuilder_setDeferredBackendCleanup().
 *        Backends still referenced by a live Qairt instance survive via that instance's reference
 *        and are released when its final reference is dropped. Future shared backends use normal
 *        cleanup unless deferred cleanup is enabled again.
 *
 * @return Error code:
 *         - QAIRT_SUCCESS: deferred mode was disabled and its holds were released.
 *         - QAIRT_COMMON_ERROR_NOT_SUPPORTED: the loaded library does not
 *           expose System Lifecycle (e.g. DSP/Hexagon build).
 *
 * @note Use corresponding API through QairtSystemLifecycle_V1_t.
 */
Qairt_Status_t QairtSystemLifecycle_releaseGlobalBackends(void);

//=============================================================================
// Function Pointer Types
//=============================================================================

typedef Qairt_Status_t (*QairtSystemLifecycle_ReleaseGlobalBackendsFn_t)(void);

//=============================================================================
// Public Interfaces
//=============================================================================

/**
 * @struct QairtSystemLifecycle_V1_t
 * @brief This struct contains the functions that define the V1 interface for
 *        QAIRT System Lifecycle. This interface can be retrieved through the id
 *        QAIRT_SYSTEM_LIFECYCLE_V1_ID.
 */
typedef struct {
  uint64_t size;
  Qairt_GetInterfaceFn_t getInterface;
  QairtSystemLifecycle_ReleaseGlobalBackendsFn_t releaseGlobalBackends;
} QairtSystemLifecycle_V1_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAISW_QAIRT_SYSTEM_LIFECYCLE_H
