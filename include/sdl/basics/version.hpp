#pragma once

#include <string>

#include <SDL_version.h>

namespace sdl
{
using version_type = SDL_version;

inline constexpr version_type version = version_type {SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL};

constexpr bool         is_version_at_least(const version_type& target)
{
  return SDL_VERSION_ATLEAST(target.major, target.minor, target.patch);
}

inline    version_type get_linked_version ()
{
  SDL_version result {};
  SDL_GetVersion(&result);
  return      result;
}
inline    std::string  get_linked_revision()
{
  return SDL_GetRevision();
}
}