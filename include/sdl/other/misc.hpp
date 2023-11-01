#pragma once

#include <stdexcept>
#include <string>

#include <SDL_misc.h>

#include <sdl/basics/error.hpp>

namespace sdl
{
inline void open_url(const std::string& url)
{
  const auto result = SDL_OpenURL(url.c_str());
#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif
}
}