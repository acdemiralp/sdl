#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL_messagebox.h>

#include <sdl/core/basics/error.hpp>
#include <sdl/core/video/window.hpp>

namespace sdl
{
enum class message_box_type : std::uint32_t
{
  error       = SDL_MESSAGEBOX_ERROR      ,
  warning     = SDL_MESSAGEBOX_WARNING    ,
  information = SDL_MESSAGEBOX_INFORMATION
};

struct message_box_button
{
  [[nodiscard]]
  SDL_MessageBoxButtonData  native(const std::int32_t index) const
  {
    return
    {
      static_cast<std::uint32_t>(escape_key_default ? SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT : 0) | 
      static_cast<std::uint32_t>(return_key_default ? SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT : 0),
      index,
      text.c_str()
    };
  }

  std::string text               {};
  bool        escape_key_default {false};
  bool        return_key_default {false};
};
struct message_box_color_scheme
{
  [[nodiscard]]
  SDL_MessageBoxColorScheme native() const
  {
    return
    {{
      {background       [0], background       [1], background       [2]},
      {text             [0], text             [1], text             [2]},
      {button_border    [0], button_border    [1], button_border    [2]},
      {button_background[0], button_background[1], button_background[2]},
      {button_selected  [0], button_selected  [1], button_selected  [2]}
    }};
  }

  std::array<std::uint8_t, 3> background        {200, 200, 200};
  std::array<std::uint8_t, 3> text              {  0,   0,   0};
  std::array<std::uint8_t, 3> button_border     {175, 175, 175};
  std::array<std::uint8_t, 3> button_background {100, 100, 100};
  std::array<std::uint8_t, 3> button_selected   {150, 150, 150};
};

// Returns a reference to the pressed button.
// Design Notes:
// - The button IDs correspond to their location within the vector.
// - The flags are split into `type` and `left_to_right`.
inline const message_box_button& show_message_box       (
  const std::string&                     title         ,
  const std::string&                     message       ,
  const std::vector<message_box_button>& buttons       ,
  message_box_type                       type          = message_box_type::information,
  const window*                          window_ptr    = nullptr,
  const message_box_color_scheme&        color_scheme  = message_box_color_scheme(),
  const bool                             left_to_right = true)
{
  std::vector<SDL_MessageBoxButtonData> native_buttons(buttons.size());
  for (std::size_t i = 0; i < native_buttons.size(); ++i)
    native_buttons[i] = buttons[i].native(static_cast<std::int32_t>(i));

  auto native_color_scheme = color_scheme.native();
  
  const SDL_MessageBoxData message_box_data
  {
    static_cast<std::uint32_t>(type) | static_cast<std::uint32_t>(left_to_right ? SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT : SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT),
    window_ptr ? window_ptr->native() : nullptr,
    title  .c_str(), 
    message.c_str(),
    static_cast<std::int32_t>(native_buttons.size()),
    native_buttons.data(),
    &native_color_scheme
  };

  std::int32_t index {};

  const auto result = SDL_ShowMessageBox(&message_box_data, &index);
#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif

  return buttons[index];
}

inline void                      show_simple_message_box(
  const std::string&                     title         , 
  const std::string&                     message       , 
  message_box_type                       type          = message_box_type::information, 
  const window*                          window_ptr    = nullptr)
{
  const auto result = SDL_ShowSimpleMessageBox(static_cast<std::uint32_t>(type), title.c_str(), message.c_str(), window_ptr->native());
#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif
}
}