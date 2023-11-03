#pragma once

#include <SDL_events.h>
#include <SDL_quit.h>

namespace sdl
{
[[nodiscard]]
inline bool quit_requested()
{
  return SDL_QuitRequested();
}
}