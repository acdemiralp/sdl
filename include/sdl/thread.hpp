#pragma once

#include <cstdint>
#include <expected>
#include <functional>
#include <memory>
#include <string>

#include <SDL_thread.h>

#include <sdl/error.hpp>

namespace sdl
{
using native_thread           = SDL_Thread;

using thread_id               = unsigned long;
using thread_function         = std::int32_t (*) (void*);
using tls_destructor_function = void         (*) (void*);
using tls_destructor_function = void         (*) (void*);

enum class thread_priority
{
  low           = SDL_THREAD_PRIORITY_LOW,
  normal        = SDL_THREAD_PRIORITY_NORMAL,
  high          = SDL_THREAD_PRIORITY_HIGH,
  time_critical = SDL_THREAD_PRIORITY_TIME_CRITICAL
};

// Bad practice: You should use `std::thread` instead.
[[nodiscard]]
inline std::expected<native_thread*, std::string> create_thread                (const thread_function function, const std::string& name,                               void* user_data)
{
  auto result = SDL_CreateThread(function, name.c_str(), user_data);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::thread` instead.
[[nodiscard]]
inline std::expected<native_thread*, std::string> create_thread_with_stack_size(const thread_function function, const std::string& name, const std::size_t stack_size, void* user_data)
{
  auto result = SDL_CreateThreadWithStackSize(function, name.c_str(), stack_size, user_data);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::thread` instead.
[[nodiscard]]
inline std::expected<std::string   , std::string> get_thread_name              (native_thread*  thread)
{
  const auto result = SDL_GetThreadName(thread);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::thread` instead.
[[nodiscard]]
inline thread_id                                  get_thread_id                (native_thread*  thread)
{
  return SDL_GetThreadID(thread);
}
// Bad practice: You should use `std::thread` instead.
inline std::int32_t                               wait_thread                  (native_thread*  thread)
{
  std::int32_t result {};
  SDL_WaitThread(thread, &result);
  return result;
}
// Bad practice: You should use `std::thread` instead.
inline void                                       detach_thread                (native_thread*  thread)
{
  SDL_DetachThread(thread);
}
// Bad practice: You should use `std::thread` instead.
[[nodiscard]]
inline thread_id                                  get_current_thread_id        ()
{
  return SDL_ThreadID();
}
// Bad practice: You should use `std::thread` instead.
inline std::expected<void          , std::string> set_thread_priority          (thread_priority priority)
{
  if (SDL_SetThreadPriority(static_cast<SDL_ThreadPriority>(priority)) < 0)
    return std::unexpected(get_error());
  return {};
}

// Bad practice: You should use the `thread_local` keyword instead.
[[nodiscard]]
inline std::expected<std::uint32_t , std::string> tls_create                   ()
{
  auto result = SDL_TLSCreate();
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use the `thread_local` keyword instead.
[[nodiscard]]
inline std::expected<void*         , std::string> tls_get                      (const std::uint32_t tls_id)
{
  auto result = SDL_TLSGet(tls_id);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use the `thread_local` keyword instead.
inline std::expected<void          , std::string> tls_set                      (const std::uint32_t tls_id, const void* value, const tls_destructor_function destructor = nullptr)
{
  if (SDL_TLSSet(tls_id, value, destructor) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use the `thread_local` keyword instead.
inline void                                       tls_cleanup                  ()
{
  SDL_TLSCleanup();
}

// Conveniences.

// Bad practice: You should use `std::thread` instead.
class thread
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_thread(...)` to handle errors.
  thread           (const std::function<std::int32_t()>& function, const std::string& name)
  : function_(function), native_(create_thread                ([ ] (void* user_data) { return static_cast<thread*>(user_data)->function_(); }, name,             this).value_or(nullptr))
  {
    
  }
  // The constructor cannot transmit error state. You should use `sdl::make_thread(...)` to handle errors.
  thread           (const std::function<std::int32_t()>& function, const std::string& name, const std::size_t stack_size)
  : function_(function), native_(create_thread_with_stack_size([ ] (void* user_data) { return static_cast<thread*>(user_data)->function_(); }, name, stack_size, this).value_or(nullptr))
  {

  }
  thread           (const thread&  that) = delete;
  thread           (      thread&& temp) = delete;
 ~thread           ()
  {
    if (native_ && joinable())
      join();
  }
  thread& operator=(const thread&  that) = delete;
  thread& operator=(      thread&& temp) = delete;

  [[nodiscard]]
  bool                                    joinable() const
  {
    return get_thread_id(native_) != get_current_thread_id();
  }
  [[nodiscard]]
  thread_id                               get_id  () const
  {
    return get_thread_id(native_);
  }
  [[nodiscard]]
  std::expected<std::string, std::string> get_name() const
  {
    return get_thread_name(native_);
  }

  std::int32_t                            join    ()
  {
    const auto result = wait_thread(native_);
    native_ = nullptr;
    return result;
  }
  void                                    detach  ()
  {
    detach_thread(native_);
    native_ = nullptr;
  }

  [[nodiscard]]
  const std::function<std::int32_t()>&    function() const noexcept
  {
    return function_;
  }
  [[nodiscard]]
  native_thread*                          native  () const noexcept
  {
    return native_;
  }

private:
  std::function<std::int32_t()> function_ {};
  native_thread*                native_   {};
};

// Bad practice: You should use the `thread_local` keyword instead.
class thread_local_storage
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_thread_local_storage()` to handle errors.
  thread_local_storage           ()
  : native_(tls_create().value_or(0))
  {
    
  }
  thread_local_storage           (const thread_local_storage&  that) = delete;
  thread_local_storage           (      thread_local_storage&& temp) noexcept
  : native_(temp.native_)
  {
    temp.native_ = 0;
  }
 ~thread_local_storage           ()
  {
    if (native_)
      tls_cleanup();
  }
  thread_local_storage& operator=(const thread_local_storage&  that) = delete;
  thread_local_storage& operator=(      thread_local_storage&& temp) noexcept
  {
    if (this != &temp)
    {
      if (native_)
        tls_cleanup();

      native_      = temp.native_;

      temp.native_ = 0;
    }
    return *this;
  }

  template <typename type> [[nodiscard]]
  std::expected<type*, std::string>       get     () const
  {
    auto result = tls_get(native_);
    if (!result)
      return std::unexpected(result.error());
    return static_cast<type*>(result.value());
  }
  template <typename type>
  std::expected<void , std::string>       set     (const type* value, const tls_destructor_function destructor = nullptr)
  {
    return tls_set(native_, static_cast<const void*>(value), destructor);
  }

  [[nodiscard]]
  std::uint32_t                           native  () const noexcept
  {
    return native_;
  }

private:
  std::uint32_t native_ {};
};

// Bad practice: You should use `std::thread` instead.
inline std::expected<std::unique_ptr<thread>, std::string> make_thread              (const std::function<std::int32_t()>& function, const std::string& name)
{
  auto result = std::make_unique<thread>(function, name);
  if (!result->native())
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::thread` instead.
inline std::expected<std::unique_ptr<thread>, std::string> make_thread              (const std::function<std::int32_t()>& function, const std::string& name, const std::size_t stack_size)
{
  auto result = std::make_unique<thread>(function, name, stack_size);
  if (!result->native())
    return std::unexpected(get_error());
  return result;
}

// Bad practice: You should use the `thread_local` keyword instead.
inline std::expected<thread_local_storage   , std::string> make_thread_local_storage()
{
  thread_local_storage result;
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
}