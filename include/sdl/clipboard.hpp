#pragma once

#include <expected>
#include <string>

#include <SDL_clipboard.h>

#include <sdl/error.hpp>

namespace sdl
{
inline std::expected<std::string, std::string> get_clipboard_text        ()
{
  const auto  native(SDL_GetClipboardText());
  std::string result(native);
  SDL_free(native);

  if (result.empty())
    return std::unexpected(get_error());
  return result;
}
inline std::expected<void       , std::string> set_clipboard_text        (const std::string& text)
{
  if (SDL_SetClipboardText(text.data()) < 0)
    return std::unexpected(get_error());
  return {};
}
inline bool                                    has_clipboard_text        ()
{
  return SDL_HasClipboardText() == SDL_TRUE;
}

inline std::expected<std::string, std::string> get_primary_selection_text()
{
  const auto  native(SDL_GetPrimarySelectionText());
  std::string result(native);
  SDL_free(native);

  if (result.empty())
    return std::unexpected(get_error());
  return result;
}
inline std::expected<void       , std::string> set_primary_selection_text(const std::string& text)
{
  if (SDL_SetPrimarySelectionText(text.data()) < 0)
    return std::unexpected(get_error());
  return {};
}
inline bool                                    has_primary_selection_text()
{
  return SDL_HasPrimarySelectionText() == SDL_TRUE;
}
}