#pragma once

#include <functional>
#include <string>
#include <utility>

#include <SDL_hints.h>

namespace sdl
{
// Note: The hint macros are not wrapped. Use them directly.

enum class hint_priority
{
  default_priority  = SDL_HINT_DEFAULT ,
  normal_priority   = SDL_HINT_NORMAL  ,
  override_priority = SDL_HINT_OVERRIDE
};

using native_hint_callback = void (*) (void* user_data, const char* name, const char* old_value, const char* new_value);

[[nodiscard]]
inline std::string get_hint              (const std::string& name)
{
  const auto result = SDL_GetHint(name.c_str());
  return result ? std::string(result) : std::string();
}
[[nodiscard]]
inline bool        get_hint_boolean      (const std::string& name, bool default_value)
{
  return static_cast<bool>(SDL_GetHintBoolean(name.c_str(), static_cast<SDL_bool>(default_value)));
}
inline bool        set_hint              (const std::string& name, const std::string& value)
{
  return static_cast<bool>(SDL_SetHint(name.c_str(), value.c_str()));
}
inline bool        set_hint_with_priority(const std::string& name, const std::string& value, hint_priority priority)
{
  return static_cast<bool>(SDL_SetHintWithPriority(name.c_str(), value.c_str(), static_cast<SDL_HintPriority>(priority)));
}
inline bool        reset_hint            (const std::string& name)
{
  return static_cast<bool>(SDL_ResetHint(name.c_str()));
}
inline void        reset_hints           ()
{
  SDL_ResetHints();
}
inline void        clear_hints           ()
{
  SDL_ClearHints();
}
inline void        add_hint_callback     (const std::string& name, const native_hint_callback callback, void* user_data = nullptr)
{
  SDL_AddHintCallback(name.c_str(), callback, user_data);
}
inline void        del_hint_callback     (const std::string& name, const native_hint_callback callback, void* user_data = nullptr)
{
  SDL_DelHintCallback(name.c_str(), callback, user_data);
}

// Conveniences.
class hint_callback
{
public:
  using callback_type = std::function<void(const std::string&, const std::string&, const std::string&)>;

  hint_callback           (std::string name, callback_type callback)
  : name_    (std::move(name))
  , callback_(std::move(callback))
  , native_  (
    [ ] (void* user_data, const char* name, const char* old_value, const char* new_value)
    {
      static_cast<hint_callback*>(user_data)->callback_(
        name      ? name      : std::string(), 
        old_value ? old_value : std::string(), 
        new_value ? new_value : std::string());
    })
  {
    add_hint_callback(name_, native_, this);
  }
  hint_callback           (const hint_callback&  that) = delete;
  hint_callback           (      hint_callback&& temp) = delete;
 ~hint_callback           ()
  {
    del_hint_callback(name_, native_, this);
  }
  hint_callback& operator=(const hint_callback&  that) = delete;
  hint_callback& operator=(      hint_callback&& temp) = delete;

private:
  std::string          name_    ;
  callback_type        callback_;

  native_hint_callback native_  ;
};
}