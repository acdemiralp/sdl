#pragma once

#include <chrono>
#include <cstdint>

#include <SDL2/SDL_power.h>

#include <sdl/core/power/power_state.hpp>

namespace sdl
{
class power_info
{
public:
  power_info           ()
  {
    update();
  }
  power_info           (const power_info&  that) = default;
  power_info           (      power_info&& temp) = default;
 ~power_info           ()                        = default;
  power_info& operator=(const power_info&  that) = default;
  power_info& operator=(      power_info&& temp) = default;

  void                 update    ()
  {
    std::int32_t native_duration, native_percentage;
    state_      = static_cast<power_state>(SDL_GetPowerInfo(&native_duration, &native_percentage));
    duration_   = std::chrono::seconds(native_duration >= 0 ? native_duration : 0);
    percentage_ = native_percentage >= 0 ? static_cast<float>(native_percentage) / 100.0f : 0.0f;
  }

  [[nodiscard]]
  power_state          state     () const
  {
    return state_;
  }
  [[nodiscard]]
  std::chrono::seconds duration  () const
  {
    return duration_;
  }
  [[nodiscard]]
  float                percentage() const
  {
    return percentage_;
  }

private:
  power_state          state_      {};
  std::chrono::seconds duration_   {};
  float                percentage_ {};
};
}