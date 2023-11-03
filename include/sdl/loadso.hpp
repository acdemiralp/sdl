#pragma once

#include <expected>
#include <functional>
#include <string>
#include <type_traits>

#include <SDL_loadso.h>

#include <sdl/error.hpp>

namespace sdl
{
[[nodiscard]]
inline std::expected<void*, std::string>                  load_object            (const std::string& file)
{
  const auto result = SDL_LoadObject(file.c_str());
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<void*, std::string>                  load_function          (void* object, const std::string& name)
{
  const auto result = SDL_LoadFunction(object, name.c_str());
  if (!result)
    return std::unexpected(get_error());
  return result;
}
inline void                                               unload_object          (void* object)
{
  SDL_UnloadObject(object);
}

// Conveniences.

template <typename function_pointer_type> [[nodiscard]]
std::expected<function_pointer_type        , std::string> load_function_as       (void* object, const std::string& name)
{
  auto void_ptr = load_function(object, name);
  if (!void_ptr)
    return std::unexpected(void_ptr.error());

  function_pointer_type result;
  *reinterpret_cast<void**>(&result) = void_ptr.value();
  return result;
}
template <typename signature_type>        [[nodiscard]]
std::expected<std::function<signature_type>, std::string> load_function_as_object(void* object, const std::string& name)
{
  auto func_ptr = load_function_as<std::add_pointer_t<signature_type>>(object, name);
  if (!func_ptr)
    return std::unexpected(func_ptr.error());
  return func_ptr.value();
}

class shared_object
{
public:
  // Bad practice: The constructor cannot transmit error state. You should use `sdl::make_shared_object(const std::string&)` instead.
  explicit shared_object  (const std::string& file)
  : native_(SDL_LoadObject(file.c_str())), managed_(true)
  {

  }
  explicit shared_object  (void* native, const bool managed)
  : native_(native), managed_(managed)
  {
    
  }
  shared_object           (const shared_object&  that) = delete;
  shared_object           (      shared_object&& temp) noexcept
  : native_(temp.native_), managed_(temp.managed_)
  {
    temp.native_  = nullptr;
    temp.managed_ = false;
  }
 ~shared_object           ()
  {
    if (native_ && managed_)
      unload_object(native_);
  }
  shared_object& operator=(const shared_object&  that) = delete;
  shared_object& operator=(      shared_object&& temp) noexcept
  {
    if (this != &temp)
    {
      if (native_ && managed_)
        unload_object(native_);

      native_       = temp.native_;
      managed_      = temp.managed_;
      temp.native_  = nullptr;
      temp.managed_ = false;
    }
    return *this;
  }

  [[nodiscard]]
  std::expected<void*                        , std::string> load_function          (const std::string& name) const
  {
    return sdl::load_function(native_, name);
  }
  template <typename function_pointer_type> [[nodiscard]]
  std::expected<function_pointer_type        , std::string> load_function_as       (const std::string& name) const
  {
    return sdl::load_function_as<function_pointer_type>(native_, name);
  }
  template <typename signature_type>        [[nodiscard]]
  std::expected<std::function<signature_type>, std::string> load_function_as_object(const std::string& name) const
  {
    return sdl::load_function_as_object<signature_type>(native_, name);
  }

  [[nodiscard]]
  void*                                                     native                 () const
  {
    return native_;
  }
  [[nodiscard]]
  bool                                                      managed                () const
  {
    return managed_;
  }

private:
  void* native_  {};
  bool  managed_ {};
};

[[nodiscard]]
inline std::expected<shared_object, std::string>          make_shared_object     (const std::string& file)
{
  void* native = SDL_LoadObject(file.c_str());
  if  (!native)
    return std::unexpected(get_error());
  return shared_object(native, true);
}
}