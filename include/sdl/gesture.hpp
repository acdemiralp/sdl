#pragma once

#include <cstdint>
#include <expected>
#include <string>

#include <SDL_gesture.h>

#include <sdl/error.hpp>
#include <sdl/rwops.hpp>

namespace sdl
{
[[deprecated]]
inline std::expected<void        , std::string> record_gesture           (const std::int64_t touch_id = -1)
{
  if (!SDL_RecordGesture(touch_id))
    return std::unexpected(get_error());
  return {};
}
[[deprecated]]
inline std::expected<std::int32_t, std::string> load_dollar_templates    (const std::int64_t touch_id  , const rw_ops& source     )
{
  const auto result = SDL_LoadDollarTemplates(touch_id, source.native());
  if (result <= 0)
    return std::unexpected(get_error());  
  return result;
}
[[deprecated]]
inline std::expected<void        , std::string> save_dollar_template     (const std::int64_t gesture_id, const rw_ops& destination)
{
  if (!SDL_SaveDollarTemplate(gesture_id, destination.native()))
    return std::unexpected(get_error());
  return {};
}
[[deprecated]]
inline std::expected<std::int32_t, std::string> save_all_dollar_templates(                               const rw_ops& destination)
{
  const auto result = SDL_SaveAllDollarTemplates(destination.native());
  if (!result)
    return std::unexpected(get_error());
  return result;
}
}