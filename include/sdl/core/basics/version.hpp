#pragma once

#include <cstdint>
#include <string>

#include <SDL2/SDL_version.h>

namespace sdl
{
using version = SDL_version;

inline version      get_version        ()
{
  SDL_version result {};
  SDL_GetVersion(&result);
  return      result;
}
inline std::string  get_revision       ()
{
  return SDL_GetRevision();
}
}