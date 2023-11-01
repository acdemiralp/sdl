#pragma once

#include <SDL_events.h>
#include <SDL_quit.h>

namespace sdl
{
inline bool quit_requested()
{
  return SDL_QuitRequested();
}
}