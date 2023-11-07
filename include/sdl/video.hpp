#pragma once

#include <expected>
#include <string>

#include <SDL_syswm.h>
#include <SDL_video.h>
#include <SDL_shape.h>

#include <sdl/surface.hpp>

namespace sdl
{
using sys_wm_info       = SDL_SysWMinfo;
using window_shape_mode = SDL_WindowShapeMode;

using native_window     = SDL_Window;

class window
{
public:
  // Conveniences.

  // Conveniences from mouse.hpp.

  [[nodiscard]]
  static window                                 with_mouse_focus      ();

  void                                          warp_mouse            (const std::array<std::int32_t, 2>& position) const;

  // Conveniences from shape.hpp.

  // The constructor cannot transmit error state. You should use `sdl::make_shaped_window(...)` to handle errors.
  window(const std::string& title, const std::array<std::int32_t, 2>& position, const std::array<std::int32_t, 2>& size, const std::uint32_t flags);

  [[nodiscard]]
  bool                                          is_shaped_window      () const;

  std::expected<void             , std::string> set_window_shape      (const sdl::surface& surface, window_shape_mode& mode) const;
  [[nodiscard]]
  std::expected<window_shape_mode, std::string> get_shaped_window_mode() const;

  // Conveniences from syswm.hpp.

  [[nodiscard]]
  std::expected<sys_wm_info      , std::string> get_wm_info           () const;

  [[nodiscard]]
  SDL_Window*                                   native                () const
  {
    return native_;
  }

private:
  window(SDL_Window* native, const bool managed)
  : native_(native), managed_(managed)
  {
    
  }

  SDL_Window* native_  {};
  bool        managed_ {true};
};

[[nodiscard]]
inline std::expected<window, std::string> make_shaped_window(const std::string& title, const std::array<std::int32_t, 2>& position, const std::array<std::int32_t, 2>& size, const std::uint32_t flags)
{
  window result(title, position, size, flags);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
}