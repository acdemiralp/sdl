#pragma once

#include <cstdarg>
#include <cstdint>
#include <string>

#include <SDL_error.h>

namespace sdl
{
// Bad practice: You should `std::format` the message and use `set_error(const std::string&)` instead.
inline void        set_error  (const char*        message, ...)
{
  std::va_list arguments;
  va_start    (arguments, message);
  SDL_SetError(message, arguments);
  va_end      (arguments);
}
inline void        set_error  (const std::string& message)
{
  SDL_SetError(message.c_str());
}

[[nodiscard]]
inline std::string get_error  ()
{
  return SDL_GetError();
}
// Warning: The buffer must be allocated beforehand.
inline void        get_error  (std::string& buffer)
{
  SDL_GetErrorMsg(buffer.data(), static_cast<std::int32_t>(buffer.size()));
}

inline void        clear_error()
{
  SDL_ClearError();
}
}