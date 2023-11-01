#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL_locale.h>

#include <sdl/basics/error.hpp>

namespace sdl
{
struct locale
{
  std::string language;
  std::string country ;
};

inline std::vector<locale> get_preferred_locales()
{
  std::vector<locale> result;

  const auto locales = SDL_GetPreferredLocales();
#ifdef SDL_USE_EXCEPTIONS
  // A nullptr is not necessarily an error: "This function returns NULL on error, including when the platform does not supply this information at all."
  // if (!locales)
  //   throw std::runtime_error(get_error());
#endif

  if (!locales)
    return result;

  for (auto iterator = locales; iterator->language != nullptr; ++iterator)
    result.push_back({iterator->language, iterator->country ? iterator->country : std::string()});

  SDL_free(locales);

  return result;
}
}