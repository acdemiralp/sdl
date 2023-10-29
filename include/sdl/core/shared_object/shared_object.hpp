#pragma once

#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <SDL2/SDL_loadso.h>

#include <sdl/core/basics/error.hpp>

namespace sdl
{
class shared_object
{
public:
  explicit shared_object  (const std::string&    file)
  : native_(SDL_LoadObject(file.c_str()))
  {
#ifdef SDL_USE_EXCEPTIONS
    if (!native_)
      throw std::runtime_error(get_error());
#endif
  }
  shared_object           (const shared_object&  that) = default;
  shared_object           (      shared_object&& temp) = default;
 ~shared_object           ()
  {
    if (native_)
      SDL_UnloadObject(native_);
  }
  shared_object& operator=(const shared_object&  that) = default;
  shared_object& operator=(      shared_object&& temp) = default;

  [[nodiscard]]
  void*                         load_function          (const std::string& name) const
  {
    auto result = SDL_LoadFunction(native_, name.c_str());
#ifdef SDL_USE_EXCEPTIONS
    if (!result)
      throw std::runtime_error(get_error());
#endif
    return result;
  }

  // Conveniences.

  template <typename function_pointer_type> [[nodiscard]]
  function_pointer_type         load_function_as       (const std::string& name) const
  {
    function_pointer_type result;
    *reinterpret_cast<void**>(&result) = load_function(name);
    return result;
  }
  template <typename signature_type>        [[nodiscard]]
  std::function<signature_type> load_function_as_object(const std::string& name) const
  {
    return load_function_as<std::add_pointer_t<signature_type>>(name);
  }

  // Accessors.

  [[nodiscard]]
  void*                         native                 () const
  {
    return native_;
  }

private:
  void* native_ = nullptr;
};
}