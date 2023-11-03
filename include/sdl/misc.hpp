#pragma once

#include <expected>
#include <string>

#include <SDL_misc.h>

#include <sdl/error.hpp>

namespace sdl
{
inline std::expected<void, std::string> open_url(const std::string& url)
{
  if (SDL_OpenURL(url.c_str()) < 0)
    return std::unexpected(get_error());
  return {};
}
}