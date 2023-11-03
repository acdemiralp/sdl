#pragma once

#include <expected>
#include <string>

#include <SDL_syswm.h>

#include <sdl/error.hpp>
#include <sdl/version.hpp>
#include <sdl/window.hpp>

namespace sdl
{
using sys_wm_type    = SDL_SYSWM_TYPE;

using sys_wm_message = SDL_SysWMmsg;
using sys_wm_info    = SDL_SysWMinfo;

[[nodiscard]]
inline std::expected<sys_wm_info, std::string> get_window_wm_info (const window& window)
{
  sys_wm_info result {version};
  if (SDL_GetWindowWMInfo(window.native(), &result) == SDL_FALSE)
    return std::unexpected(get_error());
  return result;
}

// Conveniences.

inline std::expected<sys_wm_info, std::string> window::get_wm_info() const
{
  return get_window_wm_info(*this);
}
}