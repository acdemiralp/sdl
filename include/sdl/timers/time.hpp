#pragma once

#include <chrono>
#include <cstdint>

#include <SDL_timer.h>

namespace sdl::time
{
// Bad practice: You should use std::chrono::high_resolution_clock::now() instead.
inline std::chrono::milliseconds get_ticks                ()
{
  return std::chrono::milliseconds(SDL_GetTicks64());
}

inline std::uint64_t             get_performance_counter  ()
{
  return SDL_GetPerformanceCounter();
}
inline std::uint64_t             get_performance_frequency()
{
  return SDL_GetPerformanceFrequency();
}

// Bad practice: You should use std::this_thread::sleep_for(const std::chrono::duration&) instead.
inline void                      delay                    (const std::chrono::milliseconds duration)
{
  SDL_Delay(static_cast<std::uint32_t>(duration.count()));
}
}