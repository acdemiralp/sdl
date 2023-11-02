#pragma once

#include <string>

#include <SDL_revision.h>

namespace sdl
{
inline constexpr std::string revision = SDL_REVISION;
}