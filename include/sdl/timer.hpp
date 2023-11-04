#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <expected>
#include <functional>
#include <string>
#include <utility>

#include <SDL_timer.h>

#include <sdl/error.hpp>

namespace sdl
{
template <typename type> [[deprecated]]
constexpr bool                                  ticks_passed             (type a, type b)
{
  return SDL_TICKS_PASSED(a, b);
}

// Bad practice: You should use `std::chrono::high_resolution_clock::now()` instead.
[[deprecated, nodiscard]]
inline std::chrono::milliseconds                get_ticks                ()
{
  return std::chrono::milliseconds(SDL_GetTicks());
}
// Bad practice: You should use `std::chrono::high_resolution_clock::now()` instead.
[[nodiscard]]
inline std::chrono::milliseconds                get_ticks_64             ()
{
  return std::chrono::milliseconds(SDL_GetTicks64());
}

[[nodiscard]]
inline std::uint64_t                            get_performance_counter  ()
{
  return SDL_GetPerformanceCounter();
}
[[nodiscard]]
inline std::uint64_t                            get_performance_frequency()
{
  return SDL_GetPerformanceFrequency();
}

// Bad practice: You should use `std::this_thread::sleep_for(const std::chrono::duration&)` instead.
inline void                                     delay                    (const std::chrono::milliseconds duration)
{
  SDL_Delay(static_cast<std::uint32_t>(duration.count()));
}

using timer_callback = std::uint32_t (*) (std::uint32_t, void*);

// This function only accepts function pointers with a signature identical to SDL_TimerCallback, as it is stateless. See the `sdl::make_timer(std::chrono::milliseconds, std::function<void()>, bool)` for a more idiomatic, stateful alternative.
inline std::expected<std::int32_t, std::string> add_timer                (const std::chrono::milliseconds interval, const timer_callback& callback)
{
  const auto result = SDL_AddTimer(static_cast<std::uint32_t>(interval.count()), callback, nullptr);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
inline bool                                     remove_timer             (const std::int32_t id)
{
  return SDL_RemoveTimer(id) == SDL_TRUE;
}

// Conveniences.

class timer
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_timer(std::chrono::milliseconds, std::function<void()>, bool)` to handle errors.
  timer           (const std::chrono::milliseconds duration, std::function<void()> callback, const bool repeat = false)
  : duration_(duration)
  , callback_(std::move(callback))
  , repeat_  (repeat)
  {
    construct_native();
  }
  timer           (const timer&  that) = delete;
  timer           (      timer&& temp) noexcept
  : duration_(temp.duration_)
  , callback_(std::move(temp.callback_))
  , repeat_  (temp.repeat_)
  {
    temp.destroy_native();
    construct_native();
  }
 ~timer           ()
  {
    destroy_native();
  }
  timer& operator=(const timer&  that) = delete;
  timer& operator=(      timer&& temp) noexcept
  {
    if (this != &temp)
    {
      destroy_native();

      duration_ = temp.duration_;
      callback_ = std::move(temp.callback_);
      repeat_   = temp.repeat_;

      temp.destroy_native();
      construct_native();
    }
    return *this;
  }

  [[nodiscard]]
  std::chrono::milliseconds    duration          () const
  {
    return duration_;
  }
  [[nodiscard]]
  const std::function<void()>& callback          () const
  {
    return callback_;
  }
  [[nodiscard]]
  bool                         repeat            () const
  {
    return repeat_;
  }

  [[nodiscard]]
  std::int32_t                 native            () const
  {
    return native_;
  }

private:
  void                         construct_native  ()
  {
    native_ = SDL_AddTimer(
      static_cast<std::uint32_t>(duration_.count()),
      [ ] (const std::uint32_t last_duration, void* user_data)
      {
        const auto _this = static_cast<timer*>(user_data);
        _this->callback_();
        return _this->repeat_ ? last_duration : 0u;
      }, 
      this); // This pointer is the reason why a reconstruction is necessary in the move constructor and assignment operator.
  }
  void                         destroy_native    () const
  {
    if (native_)
      SDL_RemoveTimer(native_);
  }

  std::chrono::milliseconds duration_ {};
  std::function<void()>     callback_ {};
  bool                      repeat_   {};
  
  std::int32_t              native_   {};
};

[[nodiscard]]
inline std::expected<timer, std::string>        make_timer               (const std::chrono::milliseconds duration, std::function<void()> callback, const bool repeat = false)
{
  timer result(duration, std::move(callback), repeat);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
}