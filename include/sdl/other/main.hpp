#pragma once

#include <cstdint>
#include <stdexcept>

#include <SDL_main.h>

#include <sdl/basics/error.hpp>

namespace sdl
{
using main_function = SDL_main_func;

#undef main
inline std::int32_t main                (std::int32_t argc, char* argv[])
{
  return SDL_main(argc, argv);
}
#if defined(SDL_MAIN_NEEDED) || defined(SDL_MAIN_AVAILABLE)
  #define main SDL_main
#endif

inline void         set_main_ready      ()
{
  SDL_SetMainReady();
}

#if defined(__WIN32__) || defined(__GDK__)
inline void         register_app        (const char* name, const std::uint32_t style, void* hinstance)
{
  const auto result = SDL_RegisterApp(name, style, hinstance);
#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif
}
inline void         unregister_app      ()
{
  SDL_UnregisterApp();
}
#endif

#ifdef __WINRT__
inline void         winrt_run_app       (main_function function, void* reserved)
{
  const auto result = SDL_WinRTRunApp(function, reserved);
#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif
}
#endif

#if defined(__IPHONEOS__)
inline void         uikit_run_app       (std::int32_t argc, char* argv[], main_function function)
{
  const auto result = SDL_UIKitRunApp(argc, argv, function);
#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif
}
#endif

#ifdef __GDK__
inline void         gdk_run_app         (main_function function, void* reserved)
{
  const auto result = SDL_GDKRunApp(function, reserved);
#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif
}
inline void         gdk_suspend_complete()
{
  SDL_GDKSuspendComplete();
}
#endif
}