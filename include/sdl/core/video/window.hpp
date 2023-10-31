#pragma once

#include <SDL_video.h>

namespace sdl
{
class window
{
public:

  [[nodiscard]]
  SDL_Window* native() const
  {
    return native_;
  }

protected:
  SDL_Window* native_ {};
};
}