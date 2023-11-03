#pragma once

#include <chrono>
#include <cstdint>

#include <SDL_power.h>

namespace sdl
{
enum class power_state
{
  unknown    = SDL_POWERSTATE_UNKNOWN   ,
  on_battery = SDL_POWERSTATE_ON_BATTERY,
  no_battery = SDL_POWERSTATE_NO_BATTERY,
  charging   = SDL_POWERSTATE_CHARGING  ,
  charged    = SDL_POWERSTATE_CHARGED
};

struct power_info
{
  power_state          state      {};
  std::chrono::seconds duration   {};
  float                percentage {};
};

// Note: Duration and percentage are set to 0 if they cannot be determined (in contrast to -1 in SDL).
[[nodiscard]]
inline power_info get_power_info()
{
  power_info   result;
  std::int32_t duration, percentage;
  result.state      = static_cast<power_state>(SDL_GetPowerInfo(&duration, &percentage));
  result.duration   = std::chrono::seconds(duration > 0 ? duration : 0);
  result.percentage = percentage > 0 ? static_cast<float>(percentage) / 100.0f : 0.0f;
  return result;
}
}