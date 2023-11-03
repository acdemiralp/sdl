#pragma once

#include <expected>
#include <string>
#include <vector>

#include <SDL_locale.h>

#include <sdl/error.hpp>

namespace sdl
{
struct locale
{
  std::string language;
  std::string country ;
};

[[nodiscard]]
inline std::expected<std::vector<locale>, std::string> get_preferred_locales()
{
  const auto native = SDL_GetPreferredLocales();
  if (!native)
    return std::unexpected(get_error());

  std::vector<locale> result;
  for (auto iterator = native; iterator->language != nullptr; ++iterator)
    result.push_back({iterator->language, iterator->country ? iterator->country : std::string()});
  SDL_free(native);
  return result;
}
}