#pragma once

#include <optional>

#include <SDL_syswm.h>

#include <sdl/basics/version.hpp>
#include <sdl/video/window.hpp>

namespace sdl
{
using window_manager_type    = SDL_SYSWM_TYPE;

using window_manager_message = SDL_SysWMmsg  ;
using window_manager_info    = SDL_SysWMinfo ;

inline std::optional<window_manager_info> get_window_manager_info(const window& window)
{
  window_manager_info info {version};
  return static_cast<bool>(SDL_GetWindowWMInfo(window.native(), &info)) ? info : std::optional<window_manager_info>(std::nullopt);
}
}