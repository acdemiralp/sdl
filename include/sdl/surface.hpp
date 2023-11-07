#pragma once

#include <SDL_surface.h>

namespace sdl
{
using native_surface = SDL_Surface;

class surface
{
public:
  [[nodiscard]]
  SDL_Surface* native() const noexcept
  {
    return native_;
  }

private:
  SDL_Surface* native_ {};
};
}