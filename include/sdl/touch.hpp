#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

#include <SDL_touch.h>

#include <sdl/error.hpp>
#include <sdl/gesture.hpp>
#include <sdl/rwops.hpp>

namespace sdl
{
enum class touch_device_type
{
  //invalid         = SDL_TOUCH_DEVICE_INVALID          ,
  direct            = SDL_TOUCH_DEVICE_DIRECT           ,
  indirect_absolute = SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE,
  indirect_relative = SDL_TOUCH_DEVICE_INDIRECT_RELATIVE
};

using finger = SDL_Finger;

inline constexpr std::uint32_t touch_mouse_id = SDL_TOUCH_MOUSEID;
inline constexpr std::int32_t  mouse_touch_id = SDL_MOUSE_TOUCHID;

[[nodiscard]]
inline std::int32_t                                  get_num_touch_devices() noexcept
{
  return SDL_GetNumTouchDevices();
}
[[nodiscard]]
inline std::expected<std::int64_t     , std::string> get_touch_device     (const std::int32_t index) noexcept
{
  const auto result = SDL_GetTouchDevice(index);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<std::string      , std::string> get_touch_device_name(const std::int32_t index) noexcept
{
  const auto result = SDL_GetTouchName(index); // Does not require SDL_free.
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<touch_device_type, std::string> get_touch_device_type(const std::int64_t id) noexcept
{
  const auto result = SDL_GetTouchDeviceType(id);
  if (result == SDL_TOUCH_DEVICE_INVALID)
    return std::unexpected(get_error());
  return static_cast<touch_device_type>(result);
}
[[nodiscard]]
inline std::expected<std::int32_t     , std::string> get_num_touch_fingers(const std::int64_t id) noexcept
{
  const auto result = SDL_GetNumTouchFingers(id);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline finger*                                       get_touch_finger     (const std::int64_t id, const std::int32_t index) noexcept
{
  return SDL_GetTouchFinger(id, index);
}

// Conveniences.

class touch_device
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_touch_device(std::int32_t)` to handle errors.
  explicit touch_device  (const std::int32_t   index)
  : index_(index), native_(get_touch_device(index_).value_or(0))
  {

  }
  touch_device           (const touch_device&  that) = default;
  touch_device           (      touch_device&& temp) = default;
 ~touch_device           ()                          = default;
  touch_device& operator=(const touch_device&  that) = default;
  touch_device& operator=(      touch_device&& temp) = default;

  [[nodiscard]]
  std::expected<std::string      , std::string> name                 () const
  {
    return get_touch_device_name(index_);
  }
  [[nodiscard]]
  std::expected<touch_device_type, std::string> type                 () const
  {
    return get_touch_device_type(native_);
  }
  [[nodiscard]]
  std::expected<std::int32_t     , std::string> finger_count         () const
  {
    return get_num_touch_fingers(native_);
  }
  [[nodiscard]]
  sdl::finger*                                  finger               (const std::int32_t finger_index = 0) const
  {
    return get_touch_finger(native_, finger_index);
  }
  [[nodiscard]]
  std::vector<sdl::finger*>                     fingers              () const
  {
    const auto count = finger_count();
    if (!count)
      return {};

    std::vector<sdl::finger*> result;
    result.reserve(count.value());
    for (auto i = 0; i < count.value(); ++i)
      result.emplace_back(finger(i));
    return result;
  }

  // Gesture conveniences.

  [[deprecated]]
  std::expected<void             , std::string> record_gesture       () const
  {
    return sdl::record_gesture(native_);
  }
  [[deprecated, nodiscard]]
  std::expected<std::int32_t     , std::string> load_dollar_templates(const rw_ops& source) const
  {
    return sdl::load_dollar_templates(native_, source);
  }

  // Accessors.

  [[nodiscard]]
  std::int32_t                                  index                () const
  {
    return index_;
  }
  [[nodiscard]]
  std::int64_t                                  native               () const
  {
    return native_;
  }

private:
  std::int32_t index_  {};
  std::int64_t native_ {};
};

[[nodiscard]]
inline std::expected<touch_device, std::string>      make_touch_device    (const std::int32_t index)
{
  touch_device result(index);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::vector<touch_device>                     get_touch_devices    ()
{
  const auto count = get_num_touch_devices();

  std::vector<touch_device> result;
  result.reserve(static_cast<std::size_t>(count));
  for (auto i = 0; i < count; ++i)
    result.emplace_back(i);
  return result;
}
}