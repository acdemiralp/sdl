#pragma once

#include <stdexcept>
#include <string>

#include <SDL_filesystem.h>

#include <sdl/basics/error.hpp>

namespace sdl
{
inline std::string get_base_path()
{
  const auto path = SDL_GetBasePath();
#ifdef SDL_USE_EXCEPTIONS
  // A nullptr is not necessarily an error: "Null will be returned on error or when the platform doesn't implement this functionality."
  // if (!path)
  //   throw std::runtime_error(get_error());
#endif

  if (!path)
    return {};

  std::string result(path);
  SDL_free(path);
  return result;
}
inline std::string get_pref_path(const std::string& organization, const std::string& application)
{
  const auto path = SDL_GetPrefPath(organization.c_str(), application.c_str());
#ifdef SDL_USE_EXCEPTIONS
  // A nullptr is not necessarily an error: "Null will be returned on error or when the platform doesn't implement this functionality."
  // if (!path)
  //   throw std::runtime_error(get_error());
#endif

  if (!path)
    return {};

  std::string result(path);
  SDL_free(path);
  return result;
}
}