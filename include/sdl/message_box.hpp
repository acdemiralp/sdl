#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>
#include <string>
#include <type_traits>
#include <vector>

#include <SDL_messagebox.h>

#include <sdl/bitset_enum.hpp>
#include <sdl/error.hpp>
#include <sdl/video.hpp>

namespace sdl
{
enum class message_box_flags : std::uint32_t
{
  error              = SDL_MESSAGEBOX_ERROR,
  warning            = SDL_MESSAGEBOX_WARNING,
  information        = SDL_MESSAGEBOX_INFORMATION,
  left_to_right      = SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
  right_to_left      = SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT
};
enum class message_box_button_flags : std::uint32_t
{
  none               = 0,
  return_key_default = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
  escape_key_default = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT
};

template <>
struct is_bitset_enum<message_box_flags>        : std::true_type {};
template <>
struct is_bitset_enum<message_box_button_flags> : std::true_type {};

using  message_box_color = SDL_MessageBoxColor;

struct message_box_button
{
  [[nodiscard]]
  SDL_MessageBoxButtonData native(const std::int32_t index) const
  {
    return { static_cast<std::uint32_t>(flags), index, text.c_str() };
  }

  std::string              text  {};
  message_box_button_flags flags {};
};
struct message_box_color_scheme
{
  [[nodiscard]]
  const SDL_MessageBoxColorScheme* native() const
  {
    return reinterpret_cast<const SDL_MessageBoxColorScheme*>(this);
  }

  message_box_color background        {200, 200, 200};
  message_box_color text              {  0,   0,   0};
  message_box_color button_border     {175, 175, 175};
  message_box_color button_background {100, 100, 100};
  message_box_color button_selected   {150, 150, 150};
};

// Returns the index of the pressed button.
inline std::expected<std::int32_t, std::string> show_message_box(
  const std::string&                   title         ,
  const std::string&                   message       ,
  const std::span<message_box_button>& buttons       ,
  message_box_flags                    flags         = message_box_flags::information,
  const window*                        window_ptr    = nullptr,
  const message_box_color_scheme&      color_scheme  = message_box_color_scheme())
{
  // Design decisions:
  // - The message box data struct is not wrapped, its contents are the arguments of the function.
  // - Button IDs correspond to the location in the vector.
  std::vector<SDL_MessageBoxButtonData> native_buttons(buttons.size());
  for (std::size_t i = 0; i < native_buttons.size(); ++i)
    native_buttons[i] = buttons[i].native(static_cast<std::int32_t>(i));

  const SDL_MessageBoxData data
  {
    static_cast<std::uint32_t>(flags),
    window_ptr ? window_ptr->native() : nullptr,
    title         .c_str (), 
    message       .c_str (),
    static_cast<std::int32_t>(native_buttons.size()),
    native_buttons.data  (),
    color_scheme  .native()
  };

  std::int32_t result {};
  if (SDL_ShowMessageBox(&data, &result) < 0)
    return std::unexpected(get_error());
  return result;
}

inline std::expected<void, std::string>         show_simple_message_box(
  const std::string&                     title         , 
  const std::string&                     message       , 
  message_box_flags                      flags         = message_box_flags::information, 
  const window*                          window_ptr    = nullptr)
{
  if (SDL_ShowSimpleMessageBox(static_cast<std::uint32_t>(flags), title.c_str(), message.c_str(), window_ptr->native()) < 0)
    return std::unexpected(get_error());
  return {};
}
}