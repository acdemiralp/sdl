#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <SDL_assert.h>

#include <sdl/error.hpp>

namespace sdl
{
struct assert_data
{
  bool          always_ignore;
  std::uint32_t trigger_count;
  std::string   condition    ;
  std::string   filename     ;
  std::int32_t  line_number  ;
  std::string   function     ;
};

enum class assert_state
{
  retry         = SDL_ASSERTION_RETRY ,
  break_        = SDL_ASSERTION_BREAK ,
  abort         = SDL_ASSERTION_ABORT ,
  ignore        = SDL_ASSERTION_IGNORE,
  always_ignore = SDL_ASSERTION_ALWAYS_IGNORE
};

using assertion_handler = std::function<assert_state(const std::vector<assert_data>&)>;

inline constexpr auto assert_level = SDL_ASSERT_LEVEL;

inline void                     trigger_breakpoint           () noexcept
{
  SDL_TriggerBreakpoint();
}
// Bad practice: You should use the `SDL_assert`, `SDL_assert_release`, `SDL_assert_paranoid` SDL macros, or the standard `assert` macro instead.
[[nodiscard]]
inline assert_state             report_assertion             (const assert_data& data, const std::string& function, const std::string& file, std::int32_t line)
{
  SDL_AssertData native 
  {
    data.always_ignore        , 
    data.trigger_count        , 
    data.condition    .c_str(), 
    data.filename     .c_str(), 
    data.line_number          , 
    data.function     .c_str()
  };
  return static_cast<assert_state>(SDL_ReportAssertion(&native, function.c_str(), file.c_str(), line));
}

inline void                     set_assertion_handler        (const assertion_handler& handler) noexcept
{
  static assertion_handler static_function;
  static_function = handler; // Split declaration and assignment is necessary for assign to work on calls after the first.

  SDL_SetAssertionHandler([ ] (const SDL_AssertData* data, void* user_data)
  {
    std::vector<assert_data> report;
    for (auto iterator = data; iterator != nullptr; iterator = iterator->next)
      report.emplace_back(assert_data
      {
        static_cast<bool>(iterator->always_ignore),
        iterator->trigger_count                   ,
        iterator->condition                       ,
        iterator->filename                        ,
        iterator->linenum                         ,
        iterator->function
      });
    return static_cast<SDL_AssertState>(static_function(report));
  }, nullptr);
}
[[nodiscard]]
inline assertion_handler        get_default_assertion_handler() noexcept
{
  static SDL_AssertionHandler static_function;
  static_function = SDL_GetDefaultAssertionHandler();

  return [ ] (const std::vector<assert_data>& report) 
  {
    std::vector<SDL_AssertData> native(report.size());
    for (std::size_t i = 0; i < report.size(); ++i)
    {
      native[i] = SDL_AssertData
      {
        report[i].always_ignore        ,
        report[i].trigger_count        ,
        report[i].condition    .c_str(),
        report[i].filename     .c_str(),
        report[i].line_number          ,
        report[i].function     .c_str(),
        i + 1 < report.size() ? &native[i + 1] : nullptr
      };
    }
    return static_cast<assert_state>(static_function(native.data(), nullptr));
  };
}
[[nodiscard]]
inline assertion_handler        get_assertion_handler        () noexcept
{
  static SDL_AssertionHandler static_function {};
  static void*                user_data       {};
  static_function = SDL_GetAssertionHandler(&user_data);

  return [ ] (const std::vector<assert_data>& report)
  {
    std::vector<SDL_AssertData> native(report.size());
    for (std::size_t i = 0; i < report.size(); ++i)
    {
      native[i] = SDL_AssertData
      {
        report[i].always_ignore        ,
        report[i].trigger_count        ,
        report[i].condition    .c_str(),
        report[i].filename     .c_str(),
        report[i].line_number          ,
        report[i].function     .c_str(),
        i + 1 < report.size() ? &native[i + 1] : nullptr
      };
    }
    return static_cast<assert_state>(static_function(native.data(), nullptr));
  };
}
[[nodiscard]]
inline std::vector<assert_data> get_assertion_report         () noexcept
{
  std::vector<assert_data> result;
  for (auto iterator = SDL_GetAssertionReport(); iterator != nullptr; iterator = iterator->next)
    result.emplace_back(assert_data
    {
      static_cast<bool>(iterator->always_ignore),
      iterator->trigger_count                   ,
      iterator->condition                       ,
      iterator->filename                        ,
      iterator->linenum                         ,
      iterator->function
    });
  return result;
}

inline void                     reset_assertion_report       () noexcept
{
  SDL_ResetAssertionReport();
}

// The following macros are omitted. Use them directly if needed:
// SDL_DEFAULT_ASSERT_LEVEL
// SDL_FUNCTION
// SDL_FILE
// SDL_LINE
// SDL_NULL_WHILE_LOOP_CONDITION
// SDL_disabled_assert
// SDL_enabled_assert
// SDL_assert
// SDL_assert_release
// SDL_assert_paranoid
// SDL_assert_always
}