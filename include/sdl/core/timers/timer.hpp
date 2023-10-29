#pragma once

#include <atomic>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <utility>

#include <SDL2/SDL_timer.h>

#include <sdl/core/basics/error.hpp>

namespace sdl::time
{
class timer
{
public:
  timer           (const std::uint32_t milliseconds, std::function<void()> callback, const bool repeat = false, const bool start_now = true)
  : duration_(milliseconds)
  , callback_(std::move(callback))
  , repeat_  (repeat)
  {
    if (start_now)
      start();
  }
  timer           (const timer&  that) = delete;
  timer           (      timer&& temp) noexcept
  : duration_(temp.duration_.load())
  , callback_(std::move(temp.callback_))
  , repeat_  (temp.repeat_  .load())
  , id_      (temp.id_)
  {
    temp.id_ = 0;
  }
 ~timer           ()
  {
    stop ();
  }
  timer& operator=(const timer&  that) = delete;
  timer& operator=(      timer&& temp) noexcept
  {
    if (this != &temp)
    {
      duration_ = temp.duration_.load();
      callback_ = std::move(temp.callback_);
      repeat_   = temp.repeat_  .load();
      id_       = temp.id_;

      temp.id_  = 0;
    }
    return *this;
  }

  void                         start       ()
  {
    id_ = SDL_AddTimer(
      duration_,
      [ ] (const std::uint32_t last_duration, void* user_data)
      {
        const auto _this = static_cast<timer*>(user_data);
        _this->callback_();
        return _this->repeat_ ? _this->duration_.load() : 0;
      }, this);

#ifdef SDL_USE_EXCEPTIONS
    if (!id_)
      throw std::runtime_error(get_error());
#endif
  }
  void                         stop        ()
  {
    if (!id_)
      return;

    SDL_RemoveTimer(id_);
    id_ = 0;
  }
  void                         reset       ()
  {
    stop ();
    start();
  }

  void                         set_duration(const std::uint32_t   milliseconds)
  {
    duration_ = milliseconds;
  }
  // Warning: This function is not thread safe. Ensure the timer is stopped by calling stop() in prior.
  void                         set_callback(std::function<void()> callback)
  {
    callback_ = std::move(callback);
  }
  void                         set_repeat  (const bool            repeat)
  {
    repeat_   = repeat;
  }

  [[nodiscard]]
  std::uint32_t                duration    () const
  {
    return duration_;
  }
  [[nodiscard]]
  const std::function<void()>& callback    () const
  {
    return callback_;
  }
  [[nodiscard]]
  bool                         repeat      () const
  {
    return repeat_;
  }

  [[nodiscard]]
  std::int32_t                 id          () const
  {
    return id_;
  }

private:
  // Parameters (accessible and mutable).
  std::atomic_uint32_t  duration_ {};
  std::function<void()> callback_ {};
  std::atomic_bool      repeat_   {};

  // Book-keeping variables (accessible but not mutable).
  std::int32_t          id_       {};
};
}