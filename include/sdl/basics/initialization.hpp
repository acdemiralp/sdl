#pragma once

#include <cstdint>
#include <stdexcept>
#include <type_traits>

#include <SDL2/SDL.h>

#include <sdl/basics/error.hpp>
#include <sdl/utility/bitset_enum.hpp>

namespace sdl
{
enum class subsystem_type : std::uint32_t
{
  timer           = SDL_INIT_TIMER         ,
  audio           = SDL_INIT_AUDIO         ,
  video           = SDL_INIT_VIDEO         ,
  joystick        = SDL_INIT_JOYSTICK      ,
  haptic          = SDL_INIT_HAPTIC        ,
  game_controller = SDL_INIT_GAMECONTROLLER,
  events          = SDL_INIT_EVENTS        ,
  sensor          = SDL_INIT_SENSOR        ,
  everything      = SDL_INIT_EVERYTHING
};

template <>
struct is_bitset_enum<subsystem_type> : std::true_type {};

inline bool is_initialized      (const subsystem_type subsystem)
{
  return SDL_WasInit(static_cast<std::uint32_t>(subsystem)) == static_cast<std::uint32_t>(subsystem);
}
inline void initialize_subsystem(const subsystem_type subsystem)
{
  const auto result = SDL_InitSubSystem(static_cast<std::uint32_t>(subsystem));
#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif
}
// Warning: You should still call `sdl::quit()` even if you quit each subsystem individually.
inline void quit_subsystem      (const subsystem_type subsystem)
{
  SDL_QuitSubSystem(static_cast<std::uint32_t>(subsystem));
}
inline void quit                ()
{
  SDL_Quit();
}

// Conveniences.

struct quitter
{
  explicit quitter  ()                     = default;
  quitter           (const quitter&  that) = delete ;
  quitter           (      quitter&& temp) = delete ;
 ~quitter           ()
  {
    quit();
  }
  quitter& operator=(const quitter&  that) = delete ;
  quitter& operator=(      quitter&& temp) = delete ;
};

// Note: This is a last resort to call `sdl::quit()`. You should probably call it before this.
inline static quitter quitter_instance {};

template <subsystem_type type, bool global_quit = type == subsystem_type::everything>
struct subsystem
{
  explicit subsystem  ()
  {
    initialize_subsystem(type);
  }
  subsystem           (const subsystem&  that) = delete;
  subsystem           (      subsystem&& temp) = delete;
 ~subsystem           ()
  {
    if constexpr (global_quit)
      quit();
    else
      quit_subsystem(type);
  }
  subsystem& operator=(const subsystem&  that) = delete;
  subsystem& operator=(      subsystem&& temp) = delete;
};

using timer_subsystem           = subsystem<subsystem_type::timer>;
using audio_subsystem           = subsystem<subsystem_type::audio>;
using video_subsystem           = subsystem<subsystem_type::video>;
using joystick_subsystem        = subsystem<subsystem_type::joystick>;
using haptic_subsystem          = subsystem<subsystem_type::haptic>;
using game_controller_subsystem = subsystem<subsystem_type::game_controller>;
using events_subsystem          = subsystem<subsystem_type::events>;
using sensor_subsystem          = subsystem<subsystem_type::sensor>;
using all_subsystems            = subsystem<subsystem_type::everything>;
}