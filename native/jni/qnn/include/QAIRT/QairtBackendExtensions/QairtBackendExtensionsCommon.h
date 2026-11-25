//==============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All Rights Reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

/**
 * @file
 * @brief   QAIRT Backend Extensions Common API component
 *
 *          A header which contains common types shared by QAIRT backend 
 *          extensions components.
 */

#ifndef QAIRT_BACKEND_EXT_COMMON_H
#define QAIRT_BACKEND_EXT_COMMON_H

#include "QairtCommon/QairtCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Error Codes
//=============================================================================

#define QAIRT_BACKEND_EXT_COMMON_MIN_ERROR  QAIRT_MIN_ERROR_BACKEND_EXT
#define QAIRT_BACKEND_EXT_COMMON_MAX_ERROR  QAIRT_MAX_ERROR_BACKEND_EXT
#define QAIRT_BACKEND_EXT_ALLOCATOR_MIN_ERROR QAIRT_MIN_ERROR_BACKEND_EXT + 1000
#define QAIRT_BACKEND_EXT_ALLOCATOR_MAX_ERROR QAIRT_MIN_ERROR_BACKEND_EXT + 1999

/**
 * @brief QAIRT Backend Extensions API result / error codes.
 */
typedef enum {
  /// QAIRT Backend Extensions success
  QAIRT_BACKEND_EXT_NO_ERROR = QAIRT_SUCCESS,
  /// There is an API component that is not supported yet.
  QAIRT_BACKEND_EXT_ERROR_UNSUPPORTED_FEATURE = QAIRT_COMMON_ERROR_NOT_SUPPORTED,
  /// QAIRT Backend Extensions invalid handle
  QAIRT_BACKEND_EXT_ERROR_INVALID_HANDLE = QAIRT_COMMON_ERROR_INVALID_HANDLE,
  /// One or more arguments to a Backend Extensions API is/are NULL/invalid.
  QAIRT_BACKEND_EXT_ERROR_INVALID_ARGUMENT = QAIRT_COMMON_ERROR_INVALID_ARGUMENT,
  /// API called before required call to initialize
  QAIRT_BACKEND_EXT_ERROR_UNINITIALIZED = 50000,
  ////////////////////////////////////
  QAIRT_BACKEND_EXT_ERROR_MAX_ERROR = QAIRT_MAX_ERROR_BACKEND_EXT,
  // Unused, present to ensure 32 bits.
  QAIRT_BACKEND_EXT_ERROR_UNDEFINED = 0x7FFFFFFF
} QairtBackendExt_Error_t;

//=============================================================================
// Macros
//=============================================================================

// QAIRT Backend Extensions library interface provider name
#define QAIRT_BACKEND_EXT_INTERFACE_PROVIDER_NAME "BACKEND_EXT_QTI_AISW"

// Macro controlling visibility of QAIRT_BACKEND_EXT API
#ifndef QAIRT_BACKEND_EXT_API
#define QAIRT_BACKEND_EXT_API
#endif

// Provide values to use for API version.
#define QAIRT_BACKEND_EXT_API_VERSION_MAJOR 0
#define QAIRT_BACKEND_EXT_API_VERSION_MINOR 1
#define QAIRT_BACKEND_EXT_API_VERSION_PATCH 0


//=============================================================================
// Data Types
//=============================================================================

//=============================================================================
// Public Functions
//=============================================================================

/**
 * @brief Retrieve a backend extensions interface by its unique identifier.
 *
 * This is the entry-point symbol exported by the QAIRT Backend Extensions 
 * library (libQairt<backend>BackendExtensions.so).
 *
 * @param[in] id The unique identifier for the interface to retrieve.
 * @param[out] interfaceOut A pointer to be populated with the requested interface.
 * @return Error code:
 *         - QAIRT_SUCCESS: Interface was successfully retrieved.
 *         - QAIRT_INTERFACE_ERROR_NOT_SUPPORTED: The requested interface
 *           ID is not supported.
 *         - QAIRT_COMMON_ERROR_INVALID_ARGUMENT: _interfaceOut_ is NULL.
 **/
#ifndef QAIRT_BACKEND_EXT_INTERFACE
#define QAIRT_BACKEND_EXT_INTERFACE QAIRT_INTERFACE
#endif

QAIRT_BACKEND_EXT_INTERFACE
Qairt_Status_t QairtBackendExtInterface_getInterface(uint64_t id, void** interfaceOut);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // QAIRT_BACKEND_EXT_COMMON_H
