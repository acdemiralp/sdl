#pragma once

#include <cstdint>
#include <string>

#include <SDL2/SDL_guid.h>

namespace sdl
{
using guid = SDL_GUID;

inline std::string guid_to_string  (const guid&        id    )
{
  std::string result(32, '\0'); // Setting the std::basic_string terminator to '\0' is valid hence 32 suffices instead of 33. See LWG 2475: https://cplusplus.github.io/LWG/issue2475.
  SDL_GUIDToString(id, result.data(), static_cast<std::int32_t>(result.size()));
  return result;
}
inline guid        guid_from_string(const std::string& string)
{
  return SDL_GUIDFromString(string.c_str());
}
}