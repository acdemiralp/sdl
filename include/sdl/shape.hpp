#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <string>

#include <SDL_shape.h>

#include <sdl/error.hpp>
#include <sdl/surface.hpp>
#include <sdl/video.hpp>

namespace sdl
{
inline constexpr std::int32_t non_shapeable_window   = -1;
inline constexpr std::int32_t invalid_shape_argument = -2;
inline constexpr std::int32_t window_lacks_shape     = -3;

enum class window_shape_type
{
  _default               = ShapeModeDefault,
  binarize_alpha         = ShapeModeBinarizeAlpha,
  reverse_binarize_alpha = ShapeModeReverseBinarizeAlpha,
  color_key              = ShapeModeColorKey
};

using window_shape_params = SDL_WindowShapeParams;
using window_shape_mode   = SDL_WindowShapeMode;

[[nodiscard]]
constexpr bool                                       shape_mode_alpha      (const window_shape_type type)
{
  return SDL_SHAPEMODEALPHA(static_cast<WindowShapeMode>(type));
}

[[nodiscard]]
inline std::expected<native_window*   , std::string> create_shaped_window  (const std::string& title, const std::array<std::int32_t, 2>& position, const std::array<std::int32_t, 2>& size, const std::uint32_t flags)
{
  auto result = SDL_CreateShapedWindow(title.c_str(), position[0], position[1], size[0], size[1], flags);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline bool                                          is_shaped_window      (const native_window* window)
{
  return static_cast<bool>(SDL_IsShapedWindow(window));
}

inline std::expected<void             , std::string> set_window_shape      (      native_window* window, native_surface* surface, window_shape_mode& mode)
{
  if (SDL_SetWindowShape(window, surface, &mode) < 0)
    return std::unexpected(get_error());
  return {};
}
[[nodiscard]]
inline std::expected<window_shape_mode, std::string> get_shaped_window_mode(      native_window* window)
{
  window_shape_mode result;
  if (SDL_GetShapedWindowMode(window, &result) < 0)
    return std::unexpected(get_error());
  return result;
}

// Conveniences.

inline window::window(const std::string& title, const std::array<std::int32_t, 2>& position, const std::array<std::int32_t, 2>& size, const std::uint32_t flags)
: native_(create_shaped_window(title, position, size, flags).value_or(nullptr))
{
  
}

[[nodiscard]]
inline bool                                          window::is_shaped_window      () const
{
  return sdl::is_shaped_window(native_);
}

inline std::expected<void             , std::string> window::set_window_shape      (const sdl::surface& surface, window_shape_mode& mode) const
{
  return sdl::set_window_shape(native_, surface.native(), mode);
}
[[nodiscard]]
inline std::expected<window_shape_mode, std::string> window::get_shaped_window_mode() const
{
  return sdl::get_shaped_window_mode(native_);
}
}