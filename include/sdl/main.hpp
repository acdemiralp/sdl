#pragma once

#include <cstdint>
#include <expected>
#include <string>

#include <SDL_main.h>

#include <sdl/error.hpp>

namespace sdl
{
using main_function = SDL_main_func;

#undef main
inline std::int32_t                     main                (std::int32_t argc, char* argv[])
{
  return SDL_main(argc, argv);
}
#if defined(SDL_MAIN_NEEDED) || defined(SDL_MAIN_AVAILABLE)
  #define main SDL_main
#endif

inline void                             set_main_ready      ()
{
  SDL_SetMainReady();
}

#if defined(__WIN32__) || defined(__GDK__)
inline std::expected<void, std::string> register_app        (const char* name, const std::uint32_t style, void* hinstance)
{
  if (SDL_RegisterApp(name, style, hinstance) < 0)
    return std::unexpected(get_error());
  return {};
}
inline void                             unregister_app      ()
{
  SDL_UnregisterApp();
}
#endif

#ifdef __WINRT__
inline std::expected<void, std::string> winrt_run_app       (main_function function, void* reserved)
{
  if (SDL_WinRTRunApp(function, reserved) < 0)
    return std::unexpected(get_error());
  return {};
}
#endif

#if defined(__IPHONEOS__)
inline std::expected<void, std::string> uikit_run_app       (std::int32_t argc, char* argv[], main_function function)
{
  if (SDL_UIKitRunApp(argc, argv, function) < 0)
    return std::unexpected(get_error());
  return {};
}
#endif

#ifdef __GDK__
inline std::expected<void, std::string> gdk_run_app         (main_function function, void* reserved)
{
  if (SDL_GDKRunApp(function, reserved) < 0)
    return std::unexpected(get_error());
  return {};
}
inline void                             gdk_suspend_complete()
{
  SDL_GDKSuspendComplete();
}
#endif
}