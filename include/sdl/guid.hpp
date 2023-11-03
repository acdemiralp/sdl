#pragma once

#include <cstdint>
#include <string>

#include <SDL_guid.h>

namespace sdl
{
using guid = SDL_GUID;

[[nodiscard]]
inline std::string guid_to_string  (const sdl::guid&   guid)
{
  // Setting the std::basic_string terminator to '\0' is valid hence 32 suffices instead of 33.
  // See LWG 2475: https://cplusplus.github.io/LWG/issue2475.
  std::string result(32, '\0'); 
  SDL_GUIDToString(guid, result.data(), static_cast<std::int32_t>(result.size()));
  // result.erase(result.find('\0')); Unnecessary as it is an exact fit. 
  return result;
}
[[nodiscard]]
inline guid        guid_from_string(const std::string& string)
{
  return SDL_GUIDFromString(string.c_str());
}
}