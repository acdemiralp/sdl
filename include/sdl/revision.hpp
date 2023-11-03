#pragma once

#include <cstdint>
#include <string>

#include <SDL_revision.h>

namespace sdl
{
// Bad practice: You should use `sdl::revision` instead.
[[deprecated]]
inline constexpr std::int32_t revision_number = SDL_REVISION_NUMBER;
inline constexpr std::string  revision        = SDL_REVISION;
}