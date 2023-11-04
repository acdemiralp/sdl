#pragma once

#include <string>

#include <SDL_version.h>

namespace sdl
{
using version_type = SDL_version;

inline constexpr version_type  version         = version_type {SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL};
// Bad practice: You should use `sdl::version` instead.
[[deprecated]]
inline constexpr std::int32_t compiled_version = SDL_COMPILEDVERSION;

[[deprecated, nodiscard]]
constexpr std::int32_t version_number     (const version_type& target)
{
  return SDL_VERSIONNUM(target.major, target.minor, target.patch);
}
[[nodiscard]]
constexpr bool         version_at_least   (const version_type& target)
{
  return SDL_VERSION_ATLEAST(target.major, target.minor, target.patch);
}

[[nodiscard]]
inline    version_type get_version        ()
{
  SDL_version result {};
  SDL_GetVersion(&result);
  return result;
}
[[nodiscard]]
inline    std::string  get_revision       ()
{
  return SDL_GetRevision();
}
[[deprecated, nodiscard]]
inline    std::int32_t get_revision_number()
{
  return SDL_GetRevisionNumber();
}
}