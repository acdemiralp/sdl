#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL_vulkan.h>

#include <sdl/basics/error.hpp>
#include <sdl/video/window.hpp>

namespace sdl::vulkan
{
#ifndef NO_SDL_VULKAN_TYPEDEFS
#define VK_MAX_EXTENSION_NAME_SIZE 256U
using PFN_vkVoidFunction    = void              (*) ();
using vkGetInstanceProcAddr = PFN_vkVoidFunction(*) (VkInstance, const char*);
#endif

using vulkan_instance       = SDL_vulkanInstance;
using vulkan_surface        = SDL_vulkanSurface ;

inline void                        load_library             (const std::string& path)
{
  const auto result = SDL_Vulkan_LoadLibrary(path.empty() ? nullptr : path.c_str());
#ifdef SDL_USE_EXCEPTIONS
  if (result < 0)
    throw std::runtime_error(get_error());
#endif
}
inline void                        unload_library           ()
{
  SDL_Vulkan_UnloadLibrary();
}
inline vkGetInstanceProcAddr       get_vkGetInstanceProcAddr()
{
  const auto void_ptr = SDL_Vulkan_GetVkGetInstanceProcAddr();
#ifdef SDL_USE_EXCEPTIONS
  if (!void_ptr)
    throw std::runtime_error(get_error());
#endif

  vkGetInstanceProcAddr result;
  *reinterpret_cast<void**>(&result) = void_ptr;
  return result;
}
inline std::vector<std::string>    get_instance_extensions  (const window& window)
{
  std::uint32_t count {};
  
  auto result = SDL_Vulkan_GetInstanceExtensions(window.native(), &count, nullptr);
#ifdef SDL_USE_EXCEPTIONS
  if (!result)
    throw std::runtime_error(get_error());
#endif

  std::vector              extensions    (count, std::string(VK_MAX_EXTENSION_NAME_SIZE, '\0'));
  std::vector<const char*> extension_ptrs(count);
  for (std::size_t i = 0; i < extensions.size(); ++i)
    extension_ptrs[i] = extensions[i].data();

  result      = SDL_Vulkan_GetInstanceExtensions(window.native(), &count, extension_ptrs.data());
#ifdef SDL_USE_EXCEPTIONS
  if (!result)
    throw std::runtime_error(get_error());
#endif

  for (auto& extension : extensions)
    extension.erase(extension.find('\0'));

  return extensions;
}
inline VkSurfaceKHR                create_surface           (const window& window, VkInstance instance)
{
  VkSurfaceKHR surface {};

  const auto result = SDL_Vulkan_CreateSurface(window.native(), instance, &surface);
#ifdef SDL_USE_EXCEPTIONS
  if (!result)
    throw std::runtime_error(get_error());
#endif

  return surface;
}
inline std::array<std::int32_t, 2> get_drawable_size        (const window& window)
{
  std::array<std::int32_t, 2> size {};
  SDL_Vulkan_GetDrawableSize(window.native(), size.data(), &size[1]);
  return size;
}
}