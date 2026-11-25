//==============================================================================
//
//  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
//  All rights reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

/**
 * @file
 * @brief  C++ wrapper for the QAIRT System Lifecycle API.
 *
 *         The System Lifecycle object exposes process-global (handle-less)
 *         backend lifetime management for the gate-keeping sharing feature. A
 *         SystemApi must be created before constructing a SystemLifecycle
 *         object.
 */

#pragma once

#include "QairtSystem/QairtSystemLifecycle.h"
#include "QairtSystemApi.hpp"

namespace qairt {

/**
 * @brief Wrapper for the QAIRT System Lifecycle interface.
 *
 *        Obtained via SystemApi::make<SystemLifecycle>(). Handle-less: its
 *        operations act on process-global state, not a per-instance handle.
 */
class SystemLifecycle : public ApiType<SystemLifecycle, QairtSystemLifecycle_V1_t, SystemApiTable> {
  friend SystemApi;

 public:
  SystemLifecycle() noexcept                             = default;
  SystemLifecycle(const SystemLifecycle&)                = delete;
  SystemLifecycle(SystemLifecycle&&) noexcept            = default;
  SystemLifecycle& operator=(const SystemLifecycle&)     = delete;
  SystemLifecycle& operator=(SystemLifecycle&&) noexcept = default;

  /**
   * @brief Disable process-wide deferred cleanup and release all deferred backend holds.
   *
   *        Call after all Qairt instances have been destroyed to release backends pinned via
   *        setDeferredBackendCleanup(). Backends still referenced by a live Qairt instance survive
   *        until that instance drops its final reference. Future shared backends use normal cleanup
   *        unless deferred cleanup is enabled again.
   *
   * @throws qairt::Exception if the underlying CAPI call fails.
   *
   * @see QairtSystemLifecycle_releaseGlobalBackends
   */
  void releaseGlobalBackends() {
    callNoHandleAndConfirm<&interface_type::releaseGlobalBackends>();
  }

 private:
  explicit SystemLifecycle(const std::shared_ptr<SystemApiTable>& apiTable) : ApiType(apiTable) {}
};

}  // namespace qairt
