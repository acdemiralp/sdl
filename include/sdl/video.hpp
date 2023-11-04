#pragma once

#include <expected>
#include <string>

#include <SDL_syswm.h>
#include <SDL_video.h>

namespace sdl
{
using sys_wm_info = SDL_SysWMinfo;

class window
{
public:
  // Conveniences.

  [[nodiscard]]
  std::expected<sys_wm_info, std::string> get_wm_info() const;

  [[nodiscard]]
  SDL_Window*                             native     () const
  {
    return native_;
  }

protected:
  SDL_Window* native_ {};
};
}