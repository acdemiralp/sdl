#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <memory>
#include <span>
#include <utility>

#include <SDL_cpuinfo.h>

namespace sdl
{
inline constexpr std::int32_t cache_line_size = SDL_CACHELINE_SIZE;

[[nodiscard]]
inline std::int32_t get_cpu_count          ()
{
  return SDL_GetCPUCount();
}
[[nodiscard]]
inline std::int32_t get_cpu_cache_line_size()
{
  return SDL_GetCPUCacheLineSize();
}

// Note: In mebibytes (1 mebibyte = 1.048576 megabytes).
[[nodiscard]]
inline std::int32_t get_system_ram         ()
{
  return SDL_GetSystemRAM();
}

[[nodiscard]]
inline bool         has_rdtsc              ()
{
  return SDL_HasRDTSC  () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_altivec            ()
{
  return SDL_HasAltiVec() == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_mmx                ()
{
  return SDL_HasMMX    () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_3dnow              ()
{
  return SDL_Has3DNow  () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_sse                ()
{
  return SDL_HasSSE    () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_sse2               ()
{
  return SDL_HasSSE2   () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_sse3               ()
{
  return SDL_HasSSE3   () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_sse41              ()
{
  return SDL_HasSSE41  () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_sse42              ()
{
  return SDL_HasSSE42  () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_avx                ()
{
  return SDL_HasAVX    () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_avx2               ()
{
  return SDL_HasAVX2   () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_avx512f            ()
{
  return SDL_HasAVX512F() == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_arm_simd           ()
{
  return SDL_HasARMSIMD() == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_neon               ()
{
  return SDL_HasNEON   () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_lsx                ()
{
  return SDL_HasLSX    () == SDL_TRUE;
}
[[nodiscard]]
inline bool         has_lasx               ()
{
  return SDL_HasLASX   () == SDL_TRUE;
}

[[nodiscard]]
inline std::size_t  simd_get_alignment     ()
{
  return SDL_SIMDGetAlignment();
}
// Bad practice: You should use `sdl::simd_make_unique<type>`, `sdl::simd_make_unique_array<type>`, `sdl::simd_make_shared<type>` or `sdl::simd_make_shared_array<type>` instead.
[[nodiscard]]
inline void*        simd_alloc             (              const std::size_t size)
{
  return SDL_SIMDAlloc(size);
}
// Bad practice: You should use `sdl::simd_make_unique<type>`, `sdl::simd_make_unique_array<type>`, `sdl::simd_make_shared<type>` or `sdl::simd_make_shared_array<type>` instead.
[[nodiscard]]
inline void*        simd_realloc           (void* memory, const std::size_t size)
{
  return SDL_SIMDRealloc(memory, size);
}
// Bad practice: You should use `sdl::simd_make_unique<type>`, `sdl::simd_make_unique_array<type>`, `sdl::simd_make_shared<type>` or `sdl::simd_make_shared_array<type>` instead.
inline void         simd_free              (void* memory)
{
  return SDL_SIMDFree(memory);
}

// Conveniences.

template <typename type, typename... argument_types>       [[nodiscard]]
type*                                               simd_new              (argument_types&&... arguments)
{
  auto result = static_cast<type*>(simd_alloc(sizeof(type)));
  new (result) type(std::forward<argument_types>(arguments)...);
  return result;
}
template <typename type>
void                                                simd_delete           (type* object)
{
  object->~type();
  simd_free(static_cast<void*>(object));
}

template <typename type, typename... argument_types>       [[nodiscard]]
std::span<type>                                     simd_new_array        (const std::size_t size, argument_types&&... arguments)
{
  auto objects  = static_cast<type*>(simd_alloc(size * sizeof(type)));

  auto iterator = objects;
  for (std::size_t i = 0; i < size; ++i)
    new (iterator++) type(std::forward<argument_types>(arguments)...);

  return std::span<type>(objects, size);
}
template <typename type, typename initializer_type = type> [[nodiscard]]
std::span<type>                                     simd_new_array        (std::initializer_list<initializer_type> init)
{
  auto objects  = static_cast<type*>(simd_alloc(init.size() * sizeof(type)));

  auto iterator = objects;
  for (auto& object : init)
    new (iterator++) type(object);

  return std::span<type>(objects, init.size());
}
template <typename type>
void                                                simd_delete_array     (const std::span<type>& objects)
{
  for (auto& object : objects)
    object.~type();
  simd_free(static_cast<void*>(objects.data()));
}

template <typename type, typename... argument_types>       [[nodiscard]]
std::unique_ptr<type, void(*)(type*)>               simd_make_unique      (argument_types&&... arguments)
{
  return std::unique_ptr<type, void(*)(type*)>(simd_new<type>(std::forward<argument_types>(arguments)...), simd_delete<type>);
}
template <typename type, typename... argument_types>       [[nodiscard]]
std::unique_ptr<type[], std::function<void(type*)>> simd_make_unique_array(const std::size_t size, argument_types&&... arguments)
{
  return std::unique_ptr<type[], std::function<void(type*)>>(simd_new_array<type>(size, std::forward<argument_types>(arguments)...).data(),
    [size] (type* objects)
    {
      simd_delete_array(std::span<type>(objects, size));
    });
}
template <typename type, typename initializer_type = type> [[nodiscard]]
std::unique_ptr<type[], std::function<void(type*)>> simd_make_unique_array(std::initializer_list<initializer_type> init)
{
  return std::unique_ptr<type[], std::function<void(type*)>>(simd_new_array<type>(init).data(),
    [size=init.size()](type* objects)
    {
      simd_delete_array(std::span<type>(objects, size));
    });
}

template <typename type, typename... argument_types>       [[nodiscard]]
std::shared_ptr<type>                               simd_make_shared      (argument_types&&... arguments)
{
  return std::shared_ptr<type>(simd_new<type>(std::forward<argument_types>(arguments)...), simd_delete<type>);
}
template <typename type, typename... argument_types>       [[nodiscard]]
std::shared_ptr<type[]>                             simd_make_shared_array(const std::size_t size, argument_types&&... arguments)
{
  return std::shared_ptr<type[]>(simd_new_array<type>(size, std::forward<argument_types>(arguments)...).data(),
    [size] (type* objects)
    {
      simd_delete_array(std::span<type>(objects, size));
    });
}
template <typename type, typename initializer_type = type> [[nodiscard]]
std::shared_ptr<type[]>                             simd_make_shared_array(std::initializer_list<initializer_type> init)
{
  return std::shared_ptr<type[]>(simd_new_array<type>(init).data(),
    [size=init.size()] (type* objects)
    {
      simd_delete_array(std::span<type>(objects, size));
    });
}

template <typename type>
struct simd_allocator
{
  using value_type                             = type;
  using size_type                              = std::size_t;
  using difference_type                        = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;
  using is_always_equal                        = std::true_type;

  constexpr  simd_allocator           ()                                       noexcept = default;
  constexpr  simd_allocator           (const simd_allocator&             that) noexcept = default;
  constexpr  simd_allocator           (      simd_allocator&&            temp) noexcept = default;
  template <typename that_type>
  constexpr  simd_allocator           (const simd_allocator<that_type>&  that) noexcept
  {
    // Intentionally blank.
  }
  template <typename that_type>
  constexpr  simd_allocator           (      simd_allocator<that_type>&& temp) noexcept
  {
    // Intentionally blank.
  }
  constexpr ~simd_allocator           () = default;
  constexpr  simd_allocator& operator=(const simd_allocator&             that) noexcept = default;
  constexpr  simd_allocator& operator=(      simd_allocator&&            temp) noexcept = default;

  [[nodiscard]]
  constexpr type* allocate  (               std::size_t size)
  {
    return simd_new_array<type>(size).data();
  }
  constexpr void  deallocate(type* pointer, std::size_t size)
  {
    simd_delete_array(std::span<type>(pointer, size));
  }
};
template <typename lhs_type, typename rhs_type>
constexpr bool operator==(const simd_allocator<lhs_type>& lhs, const simd_allocator<rhs_type>& rhs) noexcept
{
  return std::is_same_v<lhs_type, rhs_type>; // Statelessness implies that all allocators of the same type are identical.
}

// Future direction: simd_renew and simd_renew_array (C++ realloc equivalents).
}