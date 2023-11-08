#pragma once

#include <chrono>
#include <cstdint>
#include <expected>
#include <string>

#include <SDL_mutex.h>

#include <sdl/error.hpp>

namespace sdl
{
inline constexpr auto mutex_timed_out = SDL_MUTEX_TIMEDOUT;
inline constexpr auto mutex_max_wait  = SDL_MUTEX_MAXWAIT ;

using native_mutex     = SDL_mutex;
using native_semaphore = SDL_semaphore;
using native_cond      = SDL_cond;

// Bad practice: You should use `std::mutex` instead.
[[nodiscard]]
inline std::expected<native_mutex*     , std::string> create_mutex           ()
{
  auto result = SDL_CreateMutex();
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::mutex` instead.
inline void                                           destroy_mutex          (native_mutex* mutex)
{
  SDL_DestroyMutex(mutex);
}
// Bad practice: You should use `std::mutex` instead.
inline std::expected<void              , std::string> lock_mutex             (native_mutex* mutex)
{
  if (SDL_LockMutex   (mutex) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::mutex` instead.
inline std::expected<void              , std::string> try_lock_mutex         (native_mutex* mutex)
{
  if (SDL_TryLockMutex(mutex) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::mutex` instead.
inline std::expected<void              , std::string> unlock_mutex           (native_mutex* mutex)
{
  if (SDL_UnlockMutex (mutex) < 0)
    return std::unexpected(get_error());
  return {};
}

// Bad practice: You should use `std::counting_semaphore` instead.
[[nodiscard]]
inline std::expected<native_semaphore* , std::string> create_semaphore       (const std::uint32_t initial_value)
{
  auto result = SDL_CreateSemaphore(initial_value);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::counting_semaphore` instead.
inline void                                           destroy_semaphore      (native_semaphore* semaphore)
{
  SDL_DestroySemaphore(semaphore);
}
// Bad practice: You should use `std::counting_semaphore` instead.
inline std::expected<void              , std::string> sem_wait               (native_semaphore* semaphore)
{
  if (SDL_SemWait(semaphore) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::counting_semaphore` instead.
inline std::expected<void              , std::string> sem_try_wait           (native_semaphore* semaphore)
{
  if (SDL_SemTryWait(semaphore) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::counting_semaphore` instead.
inline std::expected<void              , std::string> sem_wait_timeout       (native_semaphore* semaphore, const std::chrono::milliseconds timeout)
{
  if (SDL_SemWaitTimeout(semaphore, static_cast<std::uint32_t>(timeout.count())) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::counting_semaphore` instead.
inline std::expected<void              , std::string> sem_post               (native_semaphore* semaphore)
{
  if (SDL_SemPost(semaphore) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::counting_semaphore` instead.
[[nodiscard]]
inline std::uint32_t                                  sem_value              (native_semaphore* semaphore)
{
  return SDL_SemValue(semaphore);
}

// Bad practice: You should use `std::condition_variable` instead.
[[nodiscard]]
inline std::expected<native_cond*      , std::string> create_cond            ()
{
  auto result = SDL_CreateCond();
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::condition_variable` instead.
inline void                                           destroy_cond           (native_cond* cond)
{
  SDL_DestroyCond(cond);
}
// Bad practice: You should use `std::condition_variable` instead.
inline std::expected<void              , std::string> cond_signal            (native_cond* cond)
{
  if (SDL_CondSignal(cond) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::condition_variable` instead.
inline std::expected<void              , std::string> cond_broadcast         (native_cond* cond)
{
  if (SDL_CondBroadcast(cond) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::condition_variable` instead.
inline std::expected<void              , std::string> cond_wait              (native_cond* cond, native_mutex* mutex)
{
  if (SDL_CondWait(cond, mutex) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::condition_variable` instead.
inline std::expected<void              , std::string> cond_wait_timeout      (native_cond* cond, native_mutex* mutex, const std::chrono::milliseconds timeout)
{
  if (SDL_CondWaitTimeout(cond, mutex, static_cast<std::uint32_t>(timeout.count())) < 0)
    return std::unexpected(get_error());
  return {};
}

// Conveniences.

// Bad practice: You should use `std::mutex` instead.
class mutex
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_mutex()` to handle errors.
  mutex           ()
  : native_(create_mutex().value_or(nullptr))
  {
    
  }
  mutex           (const mutex&  that) = delete;
  mutex           (      mutex&& temp) noexcept
  : native_(temp.native_)
  {
    temp.native_ = nullptr;
  }
 ~mutex           ()
  {
    if (native_)
      destroy_mutex(native_);
  }
  mutex& operator=(const mutex&  that) = delete;
  mutex& operator=(      mutex&& temp) noexcept
  {
    if (this != &temp)
    {
      if (native_)
        destroy_mutex(native_);

      native_      = temp.native_;

      temp.native_ = nullptr;
    }
    return *this;
  }

  std::expected<void, std::string> lock           () const
  {
    return lock_mutex    (native_);
  }
  std::expected<void, std::string> try_lock       () const
  {
    return try_lock_mutex(native_);
  }
  std::expected<void, std::string> unlock         () const
  {
    return unlock_mutex  (native_);
  }

  [[nodiscard]]
  native_mutex*                    native         () const
  {
    return native_;
  }

private:
  native_mutex* native_ {};
};

// Bad practice: You should use `std::counting_semaphore` instead.
class semaphore
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_semaphore(std::uint32_t)` to handle errors.
  explicit semaphore  (const std::uint32_t initial_value)
  : native_(create_semaphore(initial_value).value_or(nullptr))
  {
    
  }
  semaphore           (const semaphore&  that) = delete;
  semaphore           (      semaphore&& temp) noexcept
  : native_(temp.native_)
  {
    temp.native_ = nullptr;
  }
 ~semaphore           ()
  {
    if (native_)
      destroy_semaphore(native_);
  }
  semaphore& operator=(const semaphore&  that) = delete;
  semaphore& operator=(      semaphore&& temp) noexcept
  {
    if (this != &temp)
    {
      if (native_)
        destroy_semaphore(native_);

      native_      = temp.native_;

      temp.native_ = nullptr;
    }
    return *this;
  }

  std::expected<void, std::string> acquire        () const
  {
    return sem_wait        (native_);
  }
  std::expected<void, std::string> try_acquire    () const
  {
    return sem_try_wait    (native_);
  }
  std::expected<void, std::string> try_acquire_for(const std::chrono::milliseconds timeout) const
  {
    return sem_wait_timeout(native_, timeout);
  }
  std::expected<void, std::string> release        () const
  {
    return sem_post        (native_);
  }

  [[nodiscard]]
  std::uint32_t                    value          () const
  {
    return sem_value       (native_);
  }

  [[nodiscard]]
  native_semaphore*                native         () const
  {
    return native_;
  }

private:
  native_semaphore* native_ {};
};

// Bad practice: You should use `std::condition_variable` instead.
class condition_variable
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_condition_variable()` to handle errors.
  condition_variable           ()
  : native_(create_cond().value_or(nullptr))
  {
    
  }
  condition_variable           (const condition_variable&  that) = delete;
  condition_variable           (      condition_variable&& temp) noexcept
  : native_(temp.native_)
  {
    temp.native_ = nullptr;
  }
 ~condition_variable           ()
  {
    if (native_)
      destroy_cond(native_);
  }
  condition_variable& operator=(const condition_variable&  that) = delete;
  condition_variable& operator=(      condition_variable&& temp) noexcept
  {
    if (this != &temp)
    {
      if (native_)
        destroy_cond(native_);

      native_      = temp.native_;

      temp.native_ = nullptr;
    }
    return *this;
  }

  std::expected<void, std::string> notify_one     () const
  {
    return cond_signal      (native_);
  }
  std::expected<void, std::string> notify_all     () const
  {
    return cond_broadcast   (native_);
  }
  std::expected<void, std::string> wait           (const sdl::mutex& mutex) const
  {
    return cond_wait        (native_, mutex.native());
  }
  std::expected<void, std::string> wait_for       (const sdl::mutex& mutex, const std::chrono::milliseconds timeout) const
  {
    return cond_wait_timeout(native_, mutex.native(), timeout);
  }

  [[nodiscard]]
  native_cond*                     native         () const
  {
    return native_;
  }

private:
  native_cond* native_ {};
};

// Bad practice: You should use `std::mutex` instead.
[[nodiscard]]
inline std::expected<mutex             , std::string> make_mutex             ()
{
  mutex result;
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::counting_semaphore` instead.
[[nodiscard]]
inline std::expected<semaphore         , std::string> make_semaphore         (const std::uint32_t initial_value)
{
  semaphore result(initial_value);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::condition_variable` instead.
[[nodiscard]]
inline std::expected<condition_variable, std::string> make_condition_variable()
{
  condition_variable result;
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
}