#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <utility>

#include <SDL2/SDL_cpuinfo.h>

namespace sdl
{
inline std::int32_t get_cpu_count          ()
{
  return SDL_GetCPUCount();
}
inline std::int32_t get_cpu_cache_line_size()
{
  return SDL_GetCPUCacheLineSize();
}

// Note: In mebibytes (1 mebibyte = 1.048576 megabytes).
inline std::int32_t get_system_ram         ()
{
  return SDL_GetSystemRAM();
}

inline bool         has_rdtsc              ()
{
  return SDL_HasRDTSC();
}
inline bool         has_altivec            ()
{
  return SDL_HasAltiVec();
}
inline bool         has_mmx                ()
{
  return SDL_HasMMX();
}
inline bool         has_3dnow              ()
{
  return SDL_Has3DNow();
}
inline bool         has_sse                ()
{
  return SDL_HasSSE();
}
inline bool         has_sse2               ()
{
  return SDL_HasSSE2();
}
inline bool         has_sse3               ()
{
  return SDL_HasSSE3();
}
inline bool         has_sse41              ()
{
  return SDL_HasSSE41();
}
inline bool         has_sse42              ()
{
  return SDL_HasSSE42();
}
inline bool         has_avx                ()
{
  return SDL_HasAVX();
}
inline bool         has_avx2               ()
{
  return SDL_HasAVX2();
}
inline bool         has_avx512f            ()
{
  return SDL_HasAVX512F();
}
inline bool         has_arm_simd           ()
{
  return SDL_HasARMSIMD();
}
inline bool         has_neon               ()
{
  return SDL_HasNEON();
}
inline bool         has_lsx                ()
{
  return SDL_HasLSX();
}
inline bool         has_lasx               ()
{
  return SDL_HasLASX();
}

inline std::size_t  get_simd_alignment     ()
{
  return SDL_SIMDGetAlignment();
}
// Bad practice: You should use simd_make_unique<type>, simd_make_unique_array<type>, simd_make_shared<type> or simd_make_shared_array<type> instead.
inline void*        simd_alloc             (              const std::size_t size)
{
  return SDL_SIMDAlloc(size);
}
inline void*        simd_realloc           (void* memory, const std::size_t size)
{
  return SDL_SIMDRealloc(memory, size);
}
inline void         simd_free              (void* memory)
{
  return SDL_SIMDFree(memory);
}

// Conveniences.

template <typename type, typename... argument_types>
type*                                               simd_new              (argument_types&&... arguments)
{
  auto object = static_cast<type*>(simd_alloc(sizeof(type)));
  new (object) type(std::forward<argument_types>(arguments)...);
  return object;
}
template <typename type>
void                                                simd_delete           (type* object)
{
  object->~type();
  simd_free(static_cast<void*>(object));
}

// Note: `std::span` is used instead of a raw pointer, as `simd_delete_array` requires the size.
template <typename type, typename... argument_types>
std::span<type>                                     simd_new_array        (const std::size_t size, argument_types&&... arguments)
{
  auto objects  = static_cast<type*>(simd_alloc(size * sizeof(type)));

  auto iterator = objects;
  for (std::size_t i = 0; i < size; ++i)
    new (iterator++) type(std::forward<argument_types>(arguments)...);

  return {objects, size};
}
// Note: `std::span` is used instead of a raw pointer, as `simd_delete_array` requires the size.
template <typename type, typename initializer_type = type>
std::span<type>                                     simd_new_array        (std::initializer_list<initializer_type> init)
{
  auto objects  = static_cast<type*>(simd_alloc(init.size() * sizeof(type)));

  auto iterator = objects;
  for (auto& object : init)
    new (iterator++) type(object);

  return {objects, init.size()};
}
// Note: `std::span` is used instead of a raw pointer, as `simd_delete_array` requires the size.
template <typename type>
void                                                simd_delete_array     (const std::span<type>& objects)
{
  auto iterator = objects.data();
  for (std::size_t i = 0; i < objects.size(); ++i)
    (iterator++)->~type();

  simd_free(static_cast<void*>(objects.data()));
}

template <typename type, typename... argument_types>
std::unique_ptr<type, void(*)(type*)>               simd_make_unique      (argument_types&&... arguments)
{
  return std::unique_ptr<type, void(*)(type*)>(simd_new<type>(std::forward<argument_types>(arguments)...), simd_delete<type>);
}
template <typename type, typename... argument_types>
std::shared_ptr<type>                               simd_make_shared      (argument_types&&... arguments)
{
  return std::shared_ptr<type>(simd_new<type>(std::forward<argument_types>(arguments)...), simd_delete<type>);
}

// Note: `std::function` is used instead of a function pointer in this case, as the deleter has to capture the size.
template <typename type, typename... argument_types>
std::unique_ptr<type[], std::function<void(type*)>> simd_make_unique_array(const std::size_t size, argument_types&&... arguments)
{
  return std::unique_ptr<type[], std::function<void(type*)>>(simd_new_array<type>(size, std::forward<argument_types>(arguments)...).data(),
    [size] (type* objects)
    {
      simd_delete_array<type>(std::span<type>(objects, size));
    });
}
// Note: `std::function` is used instead of a function pointer in this case, as the deleter has to capture the size.
template <typename type, typename initializer_type = type>
std::unique_ptr<type[], std::function<void(type*)>> simd_make_unique_array(std::initializer_list<initializer_type> init)
{
  return std::unique_ptr<type[], std::function<void(type*)>>(simd_new_array<type>(init).data(),
    [size=init.size()](type* objects)
    {
      simd_delete_array<type>(std::span<type>(objects, size));
    });
}
template <typename type, typename... argument_types>
std::shared_ptr<type[]>                             simd_make_shared_array(const std::size_t size, argument_types&&... arguments)
{
  return std::shared_ptr<type[]>(simd_new_array<type>(size, std::forward<argument_types>(arguments)...).data(),
    [size] (type* objects)
    {
      simd_delete_array<type>(std::span<type>(objects, size));
    });
}
template <typename type, typename initializer_type = type>
std::shared_ptr<type[]>                             simd_make_shared_array(std::initializer_list<initializer_type> init)
{
  return std::shared_ptr<type[]>(simd_new_array<type>(init).data(),
    [size=init.size()] (type* objects)
    {
      simd_delete_array<type>(std::span<type>(objects, size));
    });
}

/// TODO: SIMD std::allocator.
template <typename type>
struct simd_allocator
{
  
};

// Future direction: simd_renew and simd_renew_array (C++ realloc equivalents).
}