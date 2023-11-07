#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <string>
#include <type_traits>
#include <vector>

#include <SDL_events.h>
#include <SDL_mouse.h>

#include <sdl/bitset_enum.hpp>
#include <sdl/error.hpp>
#include <sdl/surface.hpp>
#include <sdl/video.hpp>

namespace sdl
{
using native_cursor = SDL_Cursor;

enum class system_cursor
{
  arrow                    = SDL_SYSTEM_CURSOR_ARROW,
  i_beam                   = SDL_SYSTEM_CURSOR_IBEAM,
  wait                     = SDL_SYSTEM_CURSOR_WAIT,
  crosshair                = SDL_SYSTEM_CURSOR_CROSSHAIR,
  wait_arrow               = SDL_SYSTEM_CURSOR_WAITARROW,
  size_northwest_southeast = SDL_SYSTEM_CURSOR_SIZENWSE,
  size_northeast_southwest = SDL_SYSTEM_CURSOR_SIZENESW,
  size_west_east           = SDL_SYSTEM_CURSOR_SIZEWE,
  size_north_south         = SDL_SYSTEM_CURSOR_SIZENS,
  size_all                 = SDL_SYSTEM_CURSOR_SIZEALL,
  no                       = SDL_SYSTEM_CURSOR_NO,
  hand                     = SDL_SYSTEM_CURSOR_HAND,
  num_system_cursors       = SDL_NUM_SYSTEM_CURSORS
};
enum class mouse_wheel_direction
{
  normal  = SDL_MOUSEWHEEL_NORMAL,
  flipped = SDL_MOUSEWHEEL_FLIPPED
};

enum class mouse_button      : std::uint32_t
{
  left   = SDL_BUTTON_LEFT  ,
  middle = SDL_BUTTON_MIDDLE,
  right  = SDL_BUTTON_RIGHT ,
  x1     = SDL_BUTTON_X1    ,
  x2     = SDL_BUTTON_X2
};
enum class mouse_button_mask : std::uint32_t
{
  left   = SDL_BUTTON_LMASK ,
  middle = SDL_BUTTON_MMASK ,
  right  = SDL_BUTTON_RMASK ,
  x1     = SDL_BUTTON_X1MASK,
  x2     = SDL_BUTTON_X2MASK
};

template <>
struct is_bitset_enum<mouse_button_mask> : std::true_type {};

struct mouse_state
{
  // Warning: Index starts from 1.
  [[nodiscard]]
  bool is_button_down(const std::uint32_t     index ) const
  {
    return button_mask & SDL_BUTTON(index);
  }
  [[nodiscard]]
  bool is_button_down(const mouse_button_mask button) const
  {
    return button_mask & static_cast<std::uint32_t>(button);
  }

  std::array<std::int32_t, 2> position   ;
  std::uint32_t               button_mask;
};

// Warning: Index starts from 1.
constexpr std::uint32_t                           get_button_mask         (const std::uint32_t index) noexcept
{
  return SDL_BUTTON(index);
}

[[nodiscard]]
inline native_window*                             get_mouse_focus         ()
{
  return SDL_GetMouseFocus();
}
[[nodiscard]]
inline mouse_state                                get_mouse_state         ()
{
  mouse_state state;
  state.button_mask = SDL_GetMouseState        (state.position.data(), &state.position[1]);
  return state;
}
[[nodiscard]]
inline mouse_state                                get_global_mouse_state  ()
{
  mouse_state state;
  state.button_mask = SDL_GetGlobalMouseState  (state.position.data(), &state.position[1]);
  return state;
}
[[nodiscard]]
inline mouse_state                                get_relative_mouse_state()
{
  mouse_state state;
  state.button_mask = SDL_GetRelativeMouseState(state.position.data(), &state.position[1]);
  return state;
}

inline void                                       warp_mouse_in_window    (native_window* window, const std::array<std::int32_t, 2>& position)
{
  SDL_WarpMouseInWindow(window, position[0], position[1]);
}

inline std::expected<void          , std::string> warp_mouse_global       (                       const std::array<std::int32_t, 2>& position)
{
  if (SDL_WarpMouseGlobal(position[0], position[1]) < 0)
    return std::unexpected(get_error());
  return {};
}
[[nodiscard]]
inline bool                                       get_relative_mouse_mode ()
{
  return static_cast<bool>(SDL_GetRelativeMouseMode());
}

inline std::expected<void          , std::string> set_relative_mouse_mode (const bool enabled)
{
  if (SDL_SetRelativeMouseMode(static_cast<SDL_bool>(enabled)) < 0)
    return std::unexpected(get_error());
  return {};
}

inline std::expected<void          , std::string> capture_mouse           (const bool enabled)
{
  if (SDL_CaptureMouse(static_cast<SDL_bool>(enabled)) < 0)
    return std::unexpected(get_error());
  return {};
}

// Width must be a multiple of 8 bits.
[[nodiscard]]
inline std::expected<native_cursor*, std::string> create_cursor           (const std::uint8_t* data, const std::uint8_t* mask, const std::array<std::int32_t, 2>& size, const std::array<std::int32_t, 2>& offset)
{
  auto result = SDL_CreateCursor(data, mask, size[0], size[1], offset[0], offset[1]);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<native_cursor*, std::string> create_color_cursor     (native_surface* surface, const std::array<std::int32_t, 2>& offset)
{
  auto result = SDL_CreateColorCursor(surface, offset[0], offset[1]);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<native_cursor*, std::string> create_system_cursor    (const system_cursor type)
{
  auto result = SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(type));
  if (!result)
    return std::unexpected(get_error());
  return result;
}

inline void                                       free_cursor             (native_cursor* cursor)
{
  SDL_FreeCursor(cursor);
}
[[nodiscard]]
inline std::expected<native_cursor*, std::string> get_default_cursor      ()
{
  auto result = SDL_GetDefaultCursor();
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<native_cursor*, std::string> get_cursor              ()
{
  auto result = SDL_GetCursor();
  if (!result)
    return std::unexpected(get_error());
  return result;
}

inline void                                       set_cursor              (native_cursor* cursor)
{
  SDL_SetCursor(cursor);
}

inline void                                       redraw_cursor           ()
{
  SDL_SetCursor(nullptr);
}

inline std::expected<void          , std::string> show_cursor             (const bool enabled)
{
  if (SDL_ShowCursor(enabled) < 0)
    return std::unexpected(get_error());
  return {};
}
[[nodiscard]]
inline std::expected<bool          , std::string> is_cursor_shown         ()
{
  const auto result = SDL_ShowCursor(SDL_QUERY);
  if (result < 0)
    return std::unexpected(get_error());
  return result == SDL_ENABLE;
}

// Conveniences.

// Helper function for creating the data and mask byte array arguments of SDL_CreateCursor from a string.
// The size of the `cursor_string` must be a multiple of 8.
//
// Legend:
// '0': White
// '1': Black
// '2': Inverted (or black if not available)
// Any other character e.g. ' ' is transparent.
//
// Example (4x4 checkerboard pattern):
// auto cursor = sdl::create_cursor(std::string(
//   "0000111100001111"
//   "0000111100001111"
//   "0000111100001111"
//   "0000111100001111"
//   "1111000011110000"
//   "1111000011110000"
//   "1111000011110000"
//   "1111000011110000"
//   "0000111100001111"
//   "0000111100001111"
//   "0000111100001111"
//   "0000111100001111"
//   "1111000011110000"
//   "1111000011110000"
//   "1111000011110000"
//   "1111000011110000"
// ), {16, 16}, {0, 0});
// cursor.set_current();
[[nodiscard]]
inline std::expected<native_cursor*, std::string> create_cursor           (const std::string& cursor_string, const std::array<std::int32_t, 2>& size, const std::array<std::int32_t, 2>& offset)
{
  if (cursor_string.size() % 8 != 0)
    return std::unexpected(std::string("The size of `cursor_string` must be a multiple of 8."));

  const std::size_t         packed_size(cursor_string.size() / 8);
  std::vector<std::uint8_t> data       (packed_size);
  std::vector<std::uint8_t> mask       (packed_size);

  for (std::ptrdiff_t i = 0, j = -1; i < static_cast<std::ptrdiff_t>(cursor_string.size()); ++i)
  {
    if (i % 8)
    {
      data[j] <<= 1;
      mask[j] <<= 1;
    }
    else
      ++j;

    if      (cursor_string[i] == '0')
    {
      mask[j] |= 0x01;
    }
    else if (cursor_string[i] == '1')
    {
      data[j] |= 0x01;
      mask[j] |= 0x01;
    }
    else if (cursor_string[i] == '2')
    {
      data[j] |= 0x01;
    }
  }

  return create_cursor(data.data(), mask.data(), size, offset);
}

class cursor
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_default_cursor()` to handle errors.
  cursor           ()
  : native_(get_default_cursor().value_or(nullptr))
  {
    
  }
  // The constructor cannot transmit error state. You should use `sdl::make_cursor(...)` to handle errors.
  cursor           (const std::uint8_t* data, const std::uint8_t* mask, const std::array<int32_t, 2>& size, const std::array<std::int32_t, 2>& offset)
  : native_(create_cursor(data, mask, size, offset).value_or(nullptr))
  {

  }
  // The constructor cannot transmit error state. You should use `sdl::make_cursor(...)` to handle errors.
  cursor           (const std::string& cursor_string                  , const std::array<int32_t, 2>& size, const std::array<std::int32_t, 2>& offset)
  : native_(create_cursor(cursor_string, size, offset).value_or(nullptr))
  {

  }
  // The constructor cannot transmit error state. You should use `sdl::make_color_cursor(...)` to handle errors.
  cursor           (const sdl::surface& surface,                                                            const std::array<std::int32_t, 2>& offset)
  : native_(create_color_cursor(surface.native(), offset).value_or(nullptr))
  {
    
  }
  // The constructor cannot transmit error state. You should use `sdl::make_system_cursor(system_cursor)` to handle errors.
  explicit cursor  (const system_cursor type)
  : native_(create_system_cursor(type).value_or(nullptr))
  {
    
  }
  cursor           (const cursor&  that) = delete;
  cursor           (      cursor&& temp) noexcept
  : native_(temp.native_)
  {
    temp.native_ = nullptr;
  }
 ~cursor           ()
  {
    if (native_)
      free_cursor(native_);
  }
  cursor& operator=(const cursor&  that) = delete;
  cursor& operator=(      cursor&& temp) noexcept
  {
    if (this != &temp)
    {
      if (native_)
        free_cursor(native_);

      native_      = temp.native_;

      temp.native_ = nullptr;
    }
    return *this;
  }

  [[nodiscard]]
  static std::expected<cursor, std::string> get_current()
  {
    auto result = get_cursor();
    if (!result)
      return std::unexpected(result.error());
    return cursor(result.value());
  }

  void                                      set_current() const
  {
    set_cursor(native_);
  }

  [[nodiscard]]
  native_cursor*                            native     () const
  {
    return native_;
  }

private:
  explicit cursor      (native_cursor* native) : native_(native)
  {
    
  }

  native_cursor* native_;
};

[[nodiscard]]
inline std::expected<cursor, std::string>         make_cursor             ()
{
  cursor result;
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<cursor, std::string>         make_cursor             (const std::uint8_t* data, const std::uint8_t* mask, const std::array<int32_t, 2>& size, const std::array<std::int32_t, 2>& offset)
{
  cursor result(data, mask, size, offset);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<cursor, std::string>         make_cursor             (const std::string& cursor_string                  , const std::array<int32_t, 2>& size, const std::array<std::int32_t, 2>& offset)
{
  cursor result(cursor_string, size, offset);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<cursor, std::string>         make_cursor             (const sdl::surface& surface,                                                            const std::array<std::int32_t, 2>& offset)
{
  cursor result(surface, offset);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<cursor, std::string>         make_cursor             (const system_cursor type)
{
  cursor result(type);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}

inline window                                     window::with_mouse_focus()
{
  return {get_mouse_focus(), false};
}
inline void                                       window::warp_mouse      (const std::array<std::int32_t, 2>& position) const
{
  warp_mouse_in_window(native_, position);
}
}