#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SDL_keyboard.h>

#include <sdl/key_code.hpp>
#include <sdl/rect.hpp>
#include <sdl/scan_code.hpp>
#include <sdl/video.hpp>

namespace sdl
{
struct key_sym
{
  scan_code scan_code;
  key_code  key_code ;
  uint16_t  mod      ;
  uint32_t  unused   ;
};

[[nodiscard]]
inline native_window*            get_keyboard_focus         ()
{
  return SDL_GetKeyboardFocus();  
}
[[nodiscard]]
inline std::vector<std::uint8_t> get_keyboard_state         ()
{
  std::int32_t size;
  const auto   data = SDL_GetKeyboardState(&size);
  return {data, data + size};
}
inline void                      reset_keyboard             ()
{
  SDL_ResetKeyboard();
}

[[nodiscard]]
inline key_mod                   get_mod_state              ()
{
  return static_cast<key_mod>(SDL_GetModState());
}
inline void                      set_mod_state              (key_mod                             mod)
{
  SDL_SetModState(static_cast<SDL_Keymod>(mod));
}

[[nodiscard]]
inline scan_code                 get_scan_code_from_key     (key_code                            code)
{
  return static_cast<scan_code>(SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(code)));
}
[[nodiscard]]
inline std::string               get_key_name               (key_code                            code)
{
  return SDL_GetKeyName(static_cast<SDL_Keycode>(code));
}
[[nodiscard]]
inline scan_code                 get_scan_code_from_name    (const std::string&                  name)
{
  return static_cast<scan_code>(SDL_GetScancodeFromName(name.c_str()));
}
[[nodiscard]]
inline key_code                  get_key_from_scan_code     (scan_code                           code)
{
  return static_cast<key_code>(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(code)));
}
[[nodiscard]]
inline std::string               get_scan_code_name         (scan_code                           code)
{
  return SDL_GetScancodeName(static_cast<SDL_Scancode>(code));
}
[[nodiscard]]
inline key_code                  get_key_from_name          (const std::string&                  name)
{
  return static_cast<key_code>(SDL_GetKeyFromName(name.c_str()));
}

inline void                      set_text_input_rect        (const sdl::rectangle<std::int32_t>& rectangle)
{
  SDL_SetTextInputRect(static_cast<const SDL_Rect*>(&rectangle));
}
inline void                      start_text_input           ()
{
  SDL_StartTextInput  ();
}
inline void                      stop_text_input            ()
{
  SDL_StopTextInput   ();
}
inline void                      clear_composition          ()
{
  SDL_ClearComposition();
}
[[nodiscard]]
inline bool                      is_text_input_active       ()
{
  return static_cast<bool>(SDL_IsTextInputActive());
}
[[nodiscard]]
inline bool                      is_text_input_shown        ()
{
  return static_cast<bool>(SDL_IsTextInputShown());
}

[[nodiscard]]
inline bool                      has_screen_keyboard_support()
{
  return static_cast<bool>(SDL_HasScreenKeyboardSupport());
}
[[nodiscard]]
inline bool                      is_screen_keyboard_shown   (native_window*        window)
{
  return static_cast<bool>(SDL_IsScreenKeyboardShown(window));
}

// Conveniences.

inline window window::with_keyboard_focus     ()
{
  return {get_keyboard_focus(), false};
}
inline bool   window::is_screen_keyboard_shown() const
{
  return sdl::is_screen_keyboard_shown(native_);
}
}