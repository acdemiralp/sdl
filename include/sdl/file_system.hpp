#pragma once

#include <expected>
#include <string>

#include <SDL_filesystem.h>

#include <sdl/error.hpp>

namespace sdl
{
[[nodiscard]]
inline std::expected<std::string, std::string> get_base_path()
{
  const auto native = SDL_GetBasePath();
  if (!native)
    return std::unexpected(get_error());

  std::string result(native);
  SDL_free(native);
  return result;
}
[[nodiscard]]
inline std::expected<std::string, std::string> get_pref_path(const std::string& organization, const std::string& application)
{
  const auto native = SDL_GetPrefPath(organization.c_str(), application.c_str());
  if (!native)
    return std::unexpected(get_error());

  std::string result(native);
  SDL_free(native);
  return result;
}
}