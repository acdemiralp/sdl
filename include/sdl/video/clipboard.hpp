#pragma once

#include <stdexcept>
#include <string>

#include <SDL_clipboard.h>

#include <sdl/basics/error.hpp>

namespace sdl
{
// Warning: This function intentionally does not handle errors. An empty string is not necessarily an error.
inline std::string get_clipboard_text        ()
{
  const auto  text_ptr = SDL_GetClipboardText();
  std::string result   = text_ptr;
  SDL_free(text_ptr);

#ifdef SDL_USE_EXCEPTIONS
  // An empty string is not necessarily an error.
  // if (result.empty())
  //   throw std::runtime_error(get_error());
#endif

  return result;
}
inline void        set_clipboard_text        (const std::string& text)
{
  const auto result = SDL_SetClipboardText(text.data());

#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif
}
inline bool        has_clipboard_text        ()
{
  return SDL_HasClipboardText();
}

// Warning: This function intentionally does not handle errors. An empty string is not necessarily an error.
inline std::string get_primary_selection_text()
{
  const auto  text_ptr = SDL_GetPrimarySelectionText();
  std::string result   = text_ptr;
  SDL_free(text_ptr);

#ifdef SDL_USE_EXCEPTIONS
  // An empty string is not necessarily an error.
  // if (result.empty())
  //   throw std::runtime_error(get_error());
#endif

  return result;
}
inline void        set_primary_selection_text(const std::string& text)
{
  const auto result = SDL_SetPrimarySelectionText(text.data());

#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif
}
inline bool        has_primary_selection_text()
{
  return SDL_HasPrimarySelectionText();
}
}