#pragma once

#include <cstdint>
#include <expected>
#include <string>
#include <type_traits>

#include <SDL.h>

#include <sdl/bitset_enum.hpp>
#include <sdl/error.hpp>

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
  no_parachute    = SDL_INIT_NOPARACHUTE   , // Deprecated.
  everything      = SDL_INIT_EVERYTHING
};

template <>
struct is_bitset_enum<subsystem_type> : std::true_type {};

[[nodiscard]]
inline bool                             is_initialized      (const subsystem_type subsystem)
{
  return SDL_WasInit(static_cast<std::uint32_t>(subsystem)) == static_cast<std::uint32_t>(subsystem);
}
// Bad practice: You should use `sdl::initialize_subsystem(const subsystem_type)` instead.
[[deprecated]]
inline std::expected<void, std::string> initialize          (const subsystem_type subsystem)
{
  if (SDL_Init(static_cast<std::uint32_t>(subsystem)) < 0)
    return std::unexpected(get_error());
  return {};
}
inline std::expected<void, std::string> initialize_subsystem(const subsystem_type subsystem)
{
  if (SDL_InitSubSystem(static_cast<std::uint32_t>(subsystem)) < 0)
    return std::unexpected(get_error());
  return {};
}
// Warning: You should still call `sdl::quit()` even if you quit each subsystem individually.
inline void                             quit_subsystem      (const subsystem_type subsystem)
{
  SDL_QuitSubSystem(static_cast<std::uint32_t>(subsystem));
}
inline void                             quit                ()
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
using every_subsystem           = subsystem<subsystem_type::everything>;
}