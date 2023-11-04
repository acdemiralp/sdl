#pragma once

#include <array>
#include <cstdint>

#include <SDL_metal.h>

#include <sdl/video.hpp>

namespace sdl
{
inline void*                       metal_create_view      (const sdl::window& window    )
{
  return SDL_Metal_CreateView(window.native());
}
inline void                        metal_destroy_view     (void*              metal_view)
{
  SDL_Metal_DestroyView(metal_view);
}
inline void*                       metal_get_layer        (void*              metal_view)
{
  return SDL_Metal_GetLayer(metal_view);
}
inline std::array<std::int32_t, 2> metal_get_drawable_size(const sdl::window& window    )
{
  std::array<std::int32_t, 2> result {};
  SDL_Metal_GetDrawableSize(window.native(), result.data(), &result[1]);
  return result;
}

// Conveniences.

class metal_view
{
public:
  explicit metal_view  (const sdl::window& window) noexcept
  : native_(metal_create_view(window))
  {
    
  }
  metal_view           (const metal_view&  that) = delete;
  metal_view           (      metal_view&& temp) noexcept
  : native_(temp.native_)
  {
    temp.native_ = nullptr;
  }
 ~metal_view           ()
  {
    if (native_)
      metal_destroy_view(native_);
  }
  metal_view& operator=(const metal_view&  that) = delete;
  metal_view& operator=(      metal_view&& temp) noexcept
  {
    if (this != &temp)
    {
      if (native_)
        metal_destroy_view(native_);

      native_      = temp.native_;

      temp.native_ = nullptr;
    }
    return *this;
  }

  [[nodiscard]]
  void* layer () const noexcept
  {
    return metal_get_layer(native_);
  }

  [[nodiscard]]
  void* native() const noexcept
  {
    return native_;
  }

private:
  void* native_;
};
}