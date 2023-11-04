#pragma once

#include <SDL_rwops.h>

namespace sdl
{
class rw_ops
{
public:
  [[nodiscard]]
  SDL_RWops* native() const
  {
    return native_;
  }

protected:
  SDL_RWops* native_ {};
};
}