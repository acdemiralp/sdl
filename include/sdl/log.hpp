#pragma once

#include <cstdarg>
#include <cstdint>
#include <functional>
#include <string>

#include <SDL_log.h>

namespace sdl
{
[[deprecated]]
inline constexpr std::int32_t max_log_message_size = SDL_MAX_LOG_MESSAGE;

enum class log_category
{
  application        = SDL_LOG_CATEGORY_APPLICATION,
  error              = SDL_LOG_CATEGORY_ERROR      ,
  assert             = SDL_LOG_CATEGORY_ASSERT     ,
  system             = SDL_LOG_CATEGORY_SYSTEM     ,
  audio              = SDL_LOG_CATEGORY_AUDIO      ,
  video              = SDL_LOG_CATEGORY_VIDEO      ,
  render             = SDL_LOG_CATEGORY_RENDER     ,
  input              = SDL_LOG_CATEGORY_INPUT      ,
  test               = SDL_LOG_CATEGORY_TEST       ,
  reserved_1         = SDL_LOG_CATEGORY_RESERVED1  ,
  reserved_2         = SDL_LOG_CATEGORY_RESERVED2  ,
  reserved_3         = SDL_LOG_CATEGORY_RESERVED3  ,
  reserved_4         = SDL_LOG_CATEGORY_RESERVED4  ,
  reserved_5         = SDL_LOG_CATEGORY_RESERVED5  ,
  reserved_6         = SDL_LOG_CATEGORY_RESERVED6  ,
  reserved_7         = SDL_LOG_CATEGORY_RESERVED7  ,
  reserved_8         = SDL_LOG_CATEGORY_RESERVED8  ,
  reserved_9         = SDL_LOG_CATEGORY_RESERVED9  ,
  reserved_10        = SDL_LOG_CATEGORY_RESERVED10 ,
  custom             = SDL_LOG_CATEGORY_CUSTOM
};
enum class log_priority
{
  verbose            = SDL_LOG_PRIORITY_VERBOSE    ,
  debug              = SDL_LOG_PRIORITY_DEBUG      ,
  info               = SDL_LOG_PRIORITY_INFO       ,
  warn               = SDL_LOG_PRIORITY_WARN       ,
  error              = SDL_LOG_PRIORITY_ERROR      ,
  critical           = SDL_LOG_PRIORITY_CRITICAL   //,
  //num_log_priorities = SDL_NUM_LOG_PRIORITIES
};

inline void                log_set_all_priority   (const log_priority priority)
{
  SDL_LogSetAllPriority (static_cast<SDL_LogPriority>(priority));
}
inline void                log_set_priority       (const log_category category, const log_priority priority)
{
  SDL_LogSetPriority    (static_cast<std::int32_t>(category), static_cast<SDL_LogPriority>(priority));
}
[[nodiscard]]
inline log_priority        log_get_priority       (const log_category category)
{
  return static_cast<log_priority>(SDL_LogGetPriority(static_cast<std::int32_t>(category)));
}
inline void                log_reset_priorities   ()
{
  SDL_LogResetPriorities();
}

// Bad practice: You should `std::format` the message and use `sdl::log(const std::string&)` instead.
inline void                log                    (const char*        message, ...)
{
  std::va_list arguments;
  va_start(arguments, message);
  SDL_Log (message, arguments);
  va_end  (arguments);
}
inline void                log                    (const std::string& message)
{
  SDL_Log(message.c_str());
}

// Bad practice: You should `std::format` the message and use `sdl::log_verbose(const log_category, const std::string&)` instead.
inline void                log_verbose            (const log_category category, const char*        message, ...)
{
  std::va_list arguments;
  va_start(arguments, message);
  SDL_LogVerbose(static_cast<std::int32_t>(category), message, arguments);
  va_end  (arguments);
}
inline void                log_verbose            (const log_category category, const std::string& message)
{
  SDL_LogVerbose(static_cast<std::int32_t>(category), message.c_str());
}

// Bad practice: You should `std::format` the message and use `sdl::log_debug(const log_category, const std::string&)` instead.
inline void                log_debug              (const log_category category, const char*        message, ...)
{
  std::va_list arguments;
  va_start(arguments, message);
  SDL_LogDebug(static_cast<std::int32_t>(category), message, arguments);
  va_end  (arguments);
}
inline void                log_debug              (const log_category category, const std::string& message)
{
  SDL_LogDebug(static_cast<std::int32_t>(category), message.c_str());
}

// Bad practice: You should `std::format` the message and use `sdl::log_info(const log_category, const std::string&)` instead.
inline void                log_info               (const log_category category, const char*        message, ...)
{
  std::va_list arguments;
  va_start(arguments, message);
  SDL_LogInfo(static_cast<std::int32_t>(category), message, arguments);
  va_end  (arguments);
}
inline void                log_info               (const log_category category, const std::string& message)
{
  SDL_LogInfo(static_cast<std::int32_t>(category), message.c_str());
}

// Bad practice: You should `std::format` the message and use `sdl::log_warn(const log_category, const std::string&)` instead.
inline void                log_warn               (const log_category category, const char*        message, ...)
{
  std::va_list arguments;
  va_start(arguments, message);
  SDL_LogWarn(static_cast<std::int32_t>(category), message, arguments);
  va_end  (arguments);
}
inline void                log_warn               (const log_category category, const std::string& message)
{
  SDL_LogWarn(static_cast<std::int32_t>(category), message.c_str());
}

// Bad practice: You should `std::format` the message and use `sdl::log_error(const log_category, const std::string&)` instead.
inline void                log_error              (const log_category category, const char*        message, ...)
{
  std::va_list arguments;
  va_start(arguments, message);
  SDL_LogError(static_cast<std::int32_t>(category), message, arguments);
  va_end  (arguments);
}
inline void                log_error              (const log_category category, const std::string& message)
{
  SDL_LogError(static_cast<std::int32_t>(category), message.c_str());
}

// Bad practice: You should `std::format` the message and use `sdl::log_critical(const log_category, const std::string&)` instead.
inline void                log_critical           (const log_category category, const char*        message, ...)
{
  std::va_list arguments;
  va_start(arguments, message);
  SDL_LogCritical(static_cast<std::int32_t>(category), message, arguments);
  va_end  (arguments);
}
inline void                log_critical           (const log_category category, const std::string& message)
{
  SDL_LogCritical(static_cast<std::int32_t>(category), message.c_str());
}

// Bad practice: You should `std::format` the message and use `sdl::log_message(const log_category, const log_priority, const std::string&)` instead.
inline void                log_message            (const log_category category, const log_priority priority, const char*        message, ...)
{
  std::va_list arguments;
  va_start(arguments, message);
  SDL_LogMessage(static_cast<std::int32_t>(category), static_cast<SDL_LogPriority>(priority), message, arguments);
  va_end  (arguments);
}
inline void                log_message            (const log_category category, const log_priority priority, const std::string& message)
{
  SDL_LogMessage(static_cast<std::int32_t>(category), static_cast<SDL_LogPriority>(priority), message.c_str());
}

// Bad practice: You should `std::format` the message and use `sdl::log_message(const log_category, const log_priority, const std::string&)` instead.
inline void                log_message_v          (const log_category category, const log_priority priority, const char*        message, const std::va_list va_list)
{
  SDL_LogMessageV(static_cast<std::int32_t>(category), static_cast<SDL_LogPriority>(priority), message, va_list);
}

using log_output_function = std::function<void(log_category, log_priority, const std::string&)>;

[[nodiscard]]
inline log_output_function log_get_output_function()
{
  static SDL_LogOutputFunction static_function {};
  static void*                 user_data       {};
  SDL_LogGetOutputFunction(&static_function, &user_data);

  return [ ] (log_category category, log_priority priority, const std::string& message)
  {
    static_function(user_data, static_cast<std::int32_t>(category), static_cast<SDL_LogPriority>(priority), message.c_str());
  };
}
inline void                log_set_output_function(const log_output_function& function)
{
  static log_output_function static_function;
  static_function = function; // Split declaration and assignment is necessary for assign to work on calls after the first.

  SDL_LogSetOutputFunction([ ] (void*, std::int32_t category, SDL_LogPriority priority, const char* message)
  {
    static_function(static_cast<log_category>(category), static_cast<log_priority>(priority), message);
  }, nullptr);
}
}