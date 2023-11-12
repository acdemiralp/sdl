#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

#include <SDL_vulkan.h>

#include <sdl/error.hpp>
#include <sdl/video.hpp>

namespace sdl
{
#ifndef NO_SDL_VULKAN_TYPEDEFS
#define VK_MAX_EXTENSION_NAME_SIZE 256U
using PFN_vkVoidFunction    = void              (*) ();
using vkGetInstanceProcAddr = PFN_vkVoidFunction(*) (VkInstance, const char*);
#endif

using vulkan_instance       = SDL_vulkanInstance;
using vulkan_surface        = SDL_vulkanSurface ;

inline std::expected<void                    , std::string> vulkan_load_library             (const std::string& path)
{
  if (SDL_Vulkan_LoadLibrary(path.empty() ? nullptr : path.c_str()) < 0)
    return std::unexpected(get_error());
  return {};
}
inline void                                                 vulkan_unload_library           ()
{
  SDL_Vulkan_UnloadLibrary();
}
[[nodiscard]]
inline std::expected<vkGetInstanceProcAddr   , std::string> vulkan_get_vkGetInstanceProcAddr()
{
  const auto void_ptr = SDL_Vulkan_GetVkGetInstanceProcAddr();
  if (!void_ptr)
    return std::unexpected(get_error());

  vkGetInstanceProcAddr result;
  reinterpret_cast<void*&>(result) = void_ptr; // Intentional undefined behavior. Valid in most implementations.
  return result;
}
[[nodiscard]]
inline std::expected<std::vector<std::string>, std::string> vulkan_get_instance_extensions  (const window& window)
{
  std::uint32_t count {};
  
  if (SDL_Vulkan_GetInstanceExtensions(window.native(), &count, nullptr) == SDL_FALSE)
    return std::unexpected(get_error());

  std::vector              results    (count, std::string(VK_MAX_EXTENSION_NAME_SIZE, '\0'));
  std::vector<const char*> result_ptrs(count);
  for (std::size_t i = 0; i < results.size(); ++i)
    result_ptrs[i] = results[i].data();

  if (SDL_Vulkan_GetInstanceExtensions(window.native(), &count, result_ptrs.data()) == SDL_FALSE)
    return std::unexpected(get_error());

  for (auto& result : results)
    result.erase(result.find('\0'));
  return results;
}
[[nodiscard]]
inline std::expected<VkSurfaceKHR            , std::string> vulkan_create_surface           (const window& window, VkInstance instance)
{
  VkSurfaceKHR surface {};
  if (SDL_Vulkan_CreateSurface(window.native(), instance, &surface) == SDL_FALSE)
    return std::unexpected(get_error());
  return surface;
}
[[nodiscard]]
inline std::array<std::int32_t, 2>                          vulkan_get_drawable_size        (const window& window)
{
  std::array<std::int32_t, 2> result {};
  SDL_Vulkan_GetDrawableSize(window.native(), result.data(), &result[1]);
  return result;
}
}