#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include <SDL2/SDL_cpuinfo.h>

namespace sdl
{
inline std::int32_t   get_cpu_count          ()
{
  return SDL_GetCPUCount();
}
inline std::int32_t   get_cpu_cache_line_size()
{
  return SDL_GetCPUCacheLineSize();
}

// Note: In mebibytes (1 mebibyte = 1.048576 megabytes).
inline std::int32_t   get_system_ram         ()
{
  return SDL_GetSystemRAM();
}

inline bool           has_rdtsc              ()
{
  return SDL_HasRDTSC();
}
inline bool           has_altivec            ()
{
  return SDL_HasAltiVec();
}
inline bool           has_mmx                ()
{
  return SDL_HasMMX();
}
inline bool           has_3dnow              ()
{
  return SDL_Has3DNow();
}
inline bool           has_sse                ()
{
  return SDL_HasSSE();
}
inline bool           has_sse2               ()
{
  return SDL_HasSSE2();
}
inline bool           has_sse3               ()
{
  return SDL_HasSSE3();
}
inline bool           has_sse41              ()
{
  return SDL_HasSSE41();
}
inline bool           has_sse42              ()
{
  return SDL_HasSSE42();
}
inline bool           has_avx                ()
{
  return SDL_HasAVX();
}
inline bool           has_avx2               ()
{
  return SDL_HasAVX2();
}
inline bool           has_avx512f            ()
{
  return SDL_HasAVX512F();
}
inline bool           has_arm_simd           ()
{
  return SDL_HasARMSIMD();
}
inline bool           has_neon               ()
{
  return SDL_HasNEON();
}
inline bool           has_lsx                ()
{
  return SDL_HasLSX();
}
inline bool           has_lasx               ()
{
  return SDL_HasLASX();
}

inline std::size_t    get_simd_alignment     ()
{
  return SDL_SIMDGetAlignment();
}
inline void*          simd_alloc             (              const std::size_t size)
{
  return SDL_SIMDAlloc(size);
}
inline void*          simd_realloc           (void* memory, const std::size_t size)
{
  return SDL_SIMDRealloc(memory, size);
}
inline void           simd_free              (void* memory)
{
  return SDL_SIMDFree(memory);
}

// Convenience.

template <typename type, typename... argument_types>
type*                                 simd_new        (argument_types&&... arguments)
{
  auto object = static_cast<type*>(simd_alloc(sizeof(type)));
  new (object) type(std::forward<argument_types...>(arguments));
  return object;
}
template <typename type>
void                                  simd_delete     (type*               object   )
{
  object->~type();
  simd_free(static_cast<void*>(object));
}

template <typename type, typename... argument_types>
std::unique_ptr<type, void(*)(type*)> simd_make_unique (argument_types&&... arguments)
{
  return std::unique_ptr<type, void(*)(type*)>(simd_new<type>(std::forward<argument_types...>(arguments)), simd_delete<type>);
}
template <typename type, typename... argument_types>
std::shared_ptr<type>                 simd_make_shared (argument_types&&... arguments)
{
  return std::shared_ptr<type>(simd_new<type>(std::forward<argument_types...>(arguments)), simd_delete<type>);
}

/// TODO: Multi (array/vector/... container) new/delete, copy from existing, initializer list.

template <typename type, typename... argument_types>
type*                                 simd_array_new   (const std::size_t count, argument_types&&... arguments)
{
  auto array = static_cast<type*>(simd_alloc(count * sizeof(type)));

  for (std::size_t i = 0; i < count; ++i)
    new (array) type(std::forward<argument_types...>(arguments)); // TODO

  return array;
}
template <typename type>
void                                  simd_array_delete(type*             array)
{
  const auto count = sizeof(array) / sizeof(type);

  for (std::size_t i = 0; i < count; ++i)
    array->~type(); // TODO

  simd_free(static_cast<void*>(array));
}

}