//==============================================================================
//
// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// All Rights Reserved.
// Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

#pragma once

/**
 * @file
 * @brief C++ API for loading the QAIRT Backend Extensions library and accessing its interfaces.
 */

#include "../QairtApi.hpp"
#include "QairtBackendExtensions/QairtBackendExtensions.h"
#include "QairtBackendExtensions/QairtBackendExtensionsAllocator.h"
#include "QairtBackendExtensions/QairtBackendExtensionsCommon.h"

namespace qairt {

/**
 * @brief Interface table for the QAIRT Backend Extensions library.
 *
 *        Populated by BackendExtensionsApi on construction via
 *        QairtBackendExtensionsInterface_getInterface.
 *        All BackendExtensions-level C++ wrappers (BackendExtensions, ...) 
 *        dispatch their C API calls through this table.
 *
 * @note Instances are not constructed directly; obtain one via BackendExtensionsApi.
 */
struct BackendExtensionsApiTable {
  /// @brief Shared ownership handle for the dynamically loaded BackendExtensions library.
  std::shared_ptr<DlHandleHolder> m_dlHandle;

  template <typename T>
  static void handle_type_tag(T*);

  template <auto Value>
  struct apitable_p2m {
    static constexpr auto value = Value;
  };
  template <typename T>
  struct p2m_to_api_member : apitable_p2m<nullptr> {};

#define QAIRT_BACKEND_EXT_INTERFACE_DECLARE_MEMBER(type, handle, name) \
  static handle handle_type_tag(type*);                           \
  type name;                                                      \
  template <typename T>                                           \
  struct p2m_to_api_member<T type::*> : apitable_p2m<&BackendExtensionsApiTable::name> {};

  QAIRT_BACKEND_EXT_INTERFACE_DECLARE_MEMBER(QairtBackendExt_t,
                                             QairtBackendExt_Handle_t,
                                             backendExtensions)

  QAIRT_BACKEND_EXT_INTERFACE_DECLARE_MEMBER(QairtBackendExt_Allocator_t,
                                             QairtBackendExt_Allocator_Handle_t,
                                             backendExtensionsAllocator)

#undef QAIRT_BACKEND_EXT_INTERFACE_DECLARE_MEMBER

  template <typename T>
  struct interface_handle_type {
    using handle_type = decltype(handle_type_tag(std::declval<T*>()));
  };

  template <typename T>
  using interface_handle_type_t = typename interface_handle_type<T>::handle_type;

  template <auto P2m>
  struct p2mv_to_api_member : p2m_to_api_member<decltype(P2m)> {};

  template <auto P2m>
  inline static constexpr auto p2mv_to_api_member_v = p2mv_to_api_member<P2m>::value;

  static BackendExtensionsApiTable create(void* dlHandle,
                               decltype(QairtBackendExtInterface_getInterface) getInterfaceFn) {
    using detail::populateInterface;
    BackendExtensionsApiTable api;
    api.m_dlHandle = std::make_shared<DlHandleHolder>(dlHandle);

    api.backendExtensions =
        populateInterface<QAIRT_BACKEND_EXT_ID,
                          &QairtBackendExt_t::create,
                          &QairtBackendExt_t::free,
                          &QairtBackendExt_t::initialize,
                          &QairtBackendExt_t::setPerfProfile,
                          &QairtBackendExt_t::loadConfig,
                          &QairtBackendExt_t::setupLogging,
                          &QairtBackendExt_t::getProfilingLevel,
                          &QairtBackendExt_t::beforeBackendInitialize,
                          &QairtBackendExt_t::afterBackendInitialize,
                          &QairtBackendExt_t::beforeContextCreate,
                          &QairtBackendExt_t::afterContextCreate,
                          &QairtBackendExt_t::beforeComposeGraphs,
                          &QairtBackendExt_t::afterComposeGraphs,
                          &QairtBackendExt_t::beforeGraphFinalize,
                          &QairtBackendExt_t::afterGraphFinalize,
                          &QairtBackendExt_t::beforeExecute,
                          &QairtBackendExt_t::afterExecute,
                          &QairtBackendExt_t::beforeContextFree,
                          &QairtBackendExt_t::afterContextFree,
                          &QairtBackendExt_t::beforeBackendTerminate,
                          &QairtBackendExt_t::afterBackendTerminate,
                          &QairtBackendExt_t::beforeCreateFromBinary,
                          &QairtBackendExt_t::afterCreateFromBinary,
                          &QairtBackendExt_t::beforeCreateContextsFromBinaryList,
                          &QairtBackendExt_t::afterCreateContextsFromBinaryList,
                          &QairtBackendExt_t::beforeCreateDevice,
                          &QairtBackendExt_t::afterCreateDevice,
                          &QairtBackendExt_t::beforeFreeDevice,
                          &QairtBackendExt_t::afterFreeDevice,
                          &QairtBackendExt_t::beforeActivateContext,
                          &QairtBackendExt_t::afterActivateContext,
                          &QairtBackendExt_t::beforeDeactivateContext,
                          &QairtBackendExt_t::afterDeactivateContext,
                          &QairtBackendExt_t::getAllocator,
                          &QairtBackendExt_t::beforeContextApplyBinarySection,
                          &QairtBackendExt_t::afterContextApplyBinarySection>(
            getInterfaceFn);

    api.backendExtensionsAllocator =
        populateInterface<QAIRT_BACKEND_EXT_ALLOCATOR_ID,
                          &QairtBackendExt_Allocator_t::allocateBuffer,
                          &QairtBackendExt_Allocator_t::deallocateBuffer,
                          &QairtBackendExt_Allocator_t::retrieveTensorBuffer,
                          &QairtBackendExt_Allocator_t::allocateTensorBuffer,
                          &QairtBackendExt_Allocator_t::deallocateTensorBuffer,
                          &QairtBackendExt_Allocator_t::beforeWriteToTensorBuffer,
                          &QairtBackendExt_Allocator_t::afterWriteToTensorBuffer,
                          &QairtBackendExt_Allocator_t::beforeReadFromTensorBuffer,
                          &QairtBackendExt_Allocator_t::afterReadFromTensorBuffer>(
            getInterfaceFn);

    return api;
  }

  template <auto p2m>
  static constexpr bool ensureNotNullptr() {
    static_assert(p2m != nullptr);
    return p2m != nullptr;
  }

  template <auto P2m_InterfaceFn, typename... Args>
  auto call(Args&&... args) const {
    static_assert(ensureNotNullptr<P2m_InterfaceFn>());
    static_assert(ensureNotNullptr<p2m_to_api_member<decltype(P2m_InterfaceFn)>::value>());
    static_assert(ensureNotNullptr<p2mv_to_api_member_v<P2m_InterfaceFn>>());

    if (this->*p2mv_to_api_member_v<P2m_InterfaceFn>.*P2m_InterfaceFn == nullptr) {
      throw std::runtime_error("A function has not been wired up to the backend extensions api table.");
    }

#if defined(__clang__) //Checks if the compiler is Clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif
    return (this->*p2mv_to_api_member_v<P2m_InterfaceFn>.*
            P2m_InterfaceFn)(std::forward<Args>(args)...);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
  }

  template <auto P2m_InterfaceFn, typename... Args>
  auto getFunction() const {
    static_assert(ensureNotNullptr<P2m_InterfaceFn>());
    static_assert(ensureNotNullptr<p2m_to_api_member<decltype(P2m_InterfaceFn)>::value>());
    static_assert(ensureNotNullptr<p2mv_to_api_member_v<P2m_InterfaceFn>>());

    return this->*p2mv_to_api_member_v<P2m_InterfaceFn>.*P2m_InterfaceFn;
  }
};

/**
 * @brief Entry point for loading the QAIRT Backend Extensions library and creating backend extensions objects
 *
 *        Construct by passing a path to the backendExtensions shared library (e.g.
 *        `"libQairtHtpBackendExtensions.so"`), a static function pointer, or a
 *        StaticBackendTag for specialized lookup modes.
 *        Use make(), makeUnique(), or makeShared() to instantiate
 *        Backend Extensions objects.
 */
class BackendExtensionsApi {
  template <typename, typename, typename>
  friend class ApiType;

 public:
  /// @brief Shared pointer to the populated QAIRT Backend Extensions API interface table.
  std::shared_ptr<BackendExtensionsApiTable> m_backendExtApiTable;

  /**
   * @brief Tag type for static backend construction (RTLD_DEFAULT / compile-time linkage).
   *
   * When QAIRT_BACKEND_EXT_STATIC_LINK is defined, falls back to the directly linked
   * QairtBackendExtInterface_getInterface symbol if the RTLD_DEFAULT lookup fails.
   */
  struct StaticBackendTag {};
  static constexpr StaticBackendTag StaticBackend{};

  /**
   * @brief Construct BackendExtensionsApi by dynamically loading a backend extensions library.
   *
   * @param libSoFilePath Path to the backend extensions shared library (e.g. "libQairt<backend>BackendExtensions.so").
   * @param openFlags     dlopen flags (default: RTLD_NOW).
   */
  // Default flags: RTLD_NOW | RTLD_GLOBAL.
  // RTLD_GLOBAL ensures that symbols exported by the backend extensions library are visible
  // to subsequently dlopen'd libraries (and vice versa).
  explicit BackendExtensionsApi(const char* libSoFilePath,
                     unsigned int openFlags = dynamicloading::dl_now | dynamicloading::dl_global) {
    void* handle = dynamicloading::dlopen(libSoFilePath, openFlags);
    if (!handle) {
      std::string errMsg = "Failed to dlopen backend extensions library: " + std::string(libSoFilePath);
#ifndef _WIN32
      const char* dlErr = dlerror();
      if (dlErr) {
        errMsg += ": ";
        errMsg += dlErr;
      }
#endif
      throw std::runtime_error(errMsg);
    }

    void* getInterfaceFn = dynamicloading::dlsym(handle, "QairtBackendExtInterface_getInterface");
    if (!getInterfaceFn) {
      throw std::runtime_error(
          std::string("Failed to dlsym QairtBackendExtInterface_getInterface from: ") + libSoFilePath);
    }

    m_backendExtApiTable = std::make_shared<BackendExtensionsApiTable>(BackendExtensionsApiTable::create(
        handle, reinterpret_cast<decltype(QairtBackendExtInterface_getInterface)*>(getInterfaceFn)));
  }

  /**
   * @brief Construct BackendExtensionsApi from a statically-linked getInterface function pointer.
   *
   * Use this when the backend extensions library is linked statically into the binary.
   *
   * @param getInterfaceFn Pointer to QairtBackendExtInterface_getInterface from the backend extensions library.
   */
  explicit BackendExtensionsApi(decltype(QairtBackendExtInterface_getInterface) getInterfaceFn)
      : m_backendExtApiTable(
            std::make_shared<BackendExtensionsApiTable>(BackendExtensionsApiTable::create(nullptr, getInterfaceFn))) {}
  
  // TODO: Implement when InternalBackendExtensionsContextBridge is needed.
  // /**
  //  * @brief Tag type for constructing BackendExtensionsApi from the underlying backend
  //  * extensions library directly instead of the QAIRT Backend Extensions library
  //  * (QairtBackendExtInterface_getInterface).
  //  *
  //  * TODO is this necessary for backend ext?
  //  * This is used by the InternalBackendExtensionsContextBridge to avoid infinite recursion:
  //  * the QAIRT Backend Extensions C API implementations call the bridge, and the bridge needs
  //  * to call the underlying backend extensions library directly without going back through the
  //  * QAIRT Backend Extensions C API function pointers.
  //  */
  // struct QairtBackendExtensionsBackendTag {};
  // static constexpr QairtBackendExtensionsBackendTag QairtBackendExtensionsBackend{};

  // /**
  //  * @brief Construct BackendExtensionsApi from the underlying backend extensions interface provider.
  //  *
  //  * Looks up the backend extensions interface provider via RTLD_DEFAULT, gets the
  //  * backend extensions interface, and creates a BackendExtensionsApiTable by wrapping the
  //  * backend extensions functions in QAIRT-compatible shims.
  //  */
  // explicit BackendExtensionsApi(QairtBackendExtensionsBackendTag);

  /**
   * @brief Construct BackendExtensionsApi using RTLD_DEFAULT symbol lookup.
   *
   * Searches for QairtBackendExtInterface_getInterface in the already-loaded symbols.
   * If QAIRT_BACKEND_EXT_STATIC_LINK is defined and the lookup fails, falls back to
   * the directly linked symbol.
   */
  explicit BackendExtensionsApi(StaticBackendTag) {
    auto* getInterfaceFn = reinterpret_cast<decltype(QairtBackendExtInterface_getInterface)*>(
        dynamicloading::dlsym(dynamicloading::dl_default, "QairtBackendExtInterface_getInterface"));

#ifdef QAIRT_BACKEND_EXT_STATIC_LINK
    if (!getInterfaceFn) {
      getInterfaceFn = &QairtBackendExtInterface_getInterface;
    }
#endif

    if (!getInterfaceFn) {
      throw std::runtime_error(
          "Failed to find QairtBackendExtInterface_getInterface for backend extensions library. "
          "Ensure the backend extensions library is loaded or define QAIRT_BACKEND_EXT_STATIC_LINK.");
    }

    m_backendExtApiTable =
        std::make_shared<BackendExtensionsApiTable>(BackendExtensionsApiTable::create(nullptr, getInterfaceFn));
  }

  /**
   * @brief Construct BackendExtensionsApi from an existing, pre-populated API table.
   *
   * Use this when the BackendExtensionsApiTable has already been constructed externally
   * (e.g. by an internal bridge) and you need to wrap it without re-loading
   * the library.
   *
   * @param[in] backendExtApiTable  Shared pointer to a fully populated BackendExtensionsApiTable.
   *                             Must not be null.
   */
  explicit BackendExtensionsApi(std::shared_ptr<BackendExtensionsApiTable> backendExtApiTable)
      : m_backendExtApiTable(std::move(backendExtApiTable)) {}

  BackendExtensionsApi(const BackendExtensionsApi&)                = delete;
  BackendExtensionsApi(BackendExtensionsApi&&) noexcept            = default;
  BackendExtensionsApi& operator=(const BackendExtensionsApi&)     = delete;
  BackendExtensionsApi& operator=(BackendExtensionsApi&&) noexcept = default;

  /**
   * @brief Create a QAIRT backend extensions object bound to this BackendExtensionsApi's interface table.
   *
   * @tparam QairtClass  A class derived from ApiType.
   * @tparam Args        Constructor argument types forwarded to QairtClass.
   * @param[in] args     Constructor arguments forwarded to QairtClass.
   * @return             A newly constructed QairtClass instance.
   * @throws std::runtime_error  If a required interface function pointer is null in the table.
   */
  template <typename QairtClass, typename... Args>
  typename std::enable_if_t<detail::is_api_type_v<QairtClass>, QairtClass> make(
      Args&&... args) const {
    return QairtClass(m_backendExtApiTable, std::forward<Args>(args)...);
  }

  /**
   * @brief Wrapper which allows for a raw-pointer result type instead of a value type.
   *
   * @see BackendExtensionsApi::make(Args&&...)
   */
  template <typename QairtClass, typename... Args>
  std::enable_if_t<std::is_pointer_v<QairtClass> &&
                       detail::is_api_type_v<std::remove_pointer_t<QairtClass>>,
                   QairtClass>
  make(Args&&... args) const {
    return new std::remove_pointer_t<QairtClass>(m_backendExtApiTable, std::forward<Args>(args)...);
  }

  /**
   * @brief Create a QAIRT backend extensions object wrapped in a std::unique_ptr.
   *
   * @tparam QairtClass  A class derived from ApiType.
   * @tparam Args        Constructor argument types forwarded to QairtClass.
   * @param[in] args     Constructor arguments forwarded after the BackendExtApiTable pointer.
   * @return             std::unique_ptr<QairtClass> owning the newly constructed object.
   * @throws std::runtime_error  If a required interface function pointer is null in the table.
   */
  template <typename QairtClass, typename... Args>
  std::enable_if_t<detail::is_api_type_v<QairtClass>, std::unique_ptr<QairtClass>> makeUnique(
      Args&&... args) const {
    return std::unique_ptr<QairtClass>(
        new QairtClass(m_backendExtApiTable, std::forward<Args>(args)...));
  }

  /**
   * @brief Create a QAIRT backend extensions object wrapped in a std::shared_ptr.
   *
   * @tparam QairtClass  A class derived from ApiType.
   * @tparam Args        Constructor argument types forwarded to QairtClass.
   * @param[in] args     Constructor arguments forwarded after the BackendExtApiTable pointer.
   * @return             std::shared_ptr<QairtClass> owning the newly constructed object.
   * @throws std::runtime_error  If a required interface function pointer is null in the table.
   */
  template <typename QairtClass, typename... Args>
  std::enable_if_t<detail::is_api_type_v<QairtClass>, std::shared_ptr<QairtClass>> makeShared(
      Args&&... args) const {
    return std::shared_ptr<QairtClass>(
        new QairtClass(m_backendExtApiTable, std::forward<Args>(args)...));
  }
};

}  // namespace qairt
