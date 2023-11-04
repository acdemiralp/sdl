#pragma once

#include <concepts>
#include <cstdint>

#include <SDL_endian.h>

namespace sdl
{
// Bad practice: You should use `std::endian` instead. The design of this enum is based on `std::endian`.
enum class endian
{
  little = SDL_LIL_ENDIAN,
  big    = SDL_BIG_ENDIAN,
  native = SDL_BYTEORDER
};

inline constexpr endian byte_order       = static_cast<endian>(SDL_BYTEORDER);
inline constexpr endian float_word_order = static_cast<endian>(SDL_FLOATWORDORDER);

// Bad practice: You should use `std::byteswap` instead.
inline std::uint16_t swap_16      (const std::uint16_t value) noexcept
{
  return SDL_Swap16(value);
}
// Bad practice: You should use `std::byteswap` instead.
inline std::uint32_t swap_32      (const std::uint32_t value) noexcept
{
  return SDL_Swap32(value);
}
// Bad practice: You should use `std::byteswap` instead.
inline std::uint64_t swap_64      (const std::uint64_t value) noexcept
{
  return SDL_Swap64(value);
}
inline float         swap_float   (const float         value) noexcept
{
  return SDL_SwapFloat(value);
}

inline std::uint16_t swap_le_16   (const std::uint16_t value) noexcept
{
  return SDL_SwapLE16(value);
}
inline std::uint32_t swap_le_32   (const std::uint32_t value) noexcept
{
  return SDL_SwapLE32(value);
}
inline std::uint64_t swap_le_64   (const std::uint64_t value) noexcept
{
  return SDL_SwapLE64(value);
}
inline float         swap_float_le(const float         value) noexcept
{
  return SDL_SwapFloatLE(value);
}

inline std::uint16_t swap_be_16   (const std::uint16_t value) noexcept
{
  return SDL_SwapBE16(value);
}
inline std::uint32_t swap_be_32   (const std::uint32_t value) noexcept
{
  return SDL_SwapBE32(value);
}
inline std::uint64_t swap_be_64   (const std::uint64_t value) noexcept
{
  return SDL_SwapBE64(value);
}
inline float         swap_float_be(const float         value) noexcept
{
  return SDL_SwapFloatBE(value);
}

// Conveniences.

// Bad practice: You should use `std::byteswap` instead. The design of this function is based on `std::byteswap`.
template <std::integral type> [[nodiscard]]
constexpr type byteswap   (type value) noexcept
{
  if      constexpr (sizeof(type) == 1)
    return value;
  else if constexpr (sizeof(type) == 2)
    return static_cast<type>(SDL_Swap16(static_cast<std::uint16_t>(value)));
  else if constexpr (sizeof(type) == 4)
    return static_cast<type>(SDL_Swap32(static_cast<std::uint32_t>(value)));
  else if constexpr (sizeof(type) == 8)
    return static_cast<type>(SDL_Swap64(static_cast<std::uint64_t>(value)));
  else
    static_assert(sizeof(type) == 1 || sizeof(type) == 2 || sizeof(type) == 4 || sizeof(type) == 8, "Unsupported type size.");
  return {};
}
template <std::integral type> [[nodiscard]]
constexpr type byteswap_le(type value) noexcept
{
  if      constexpr (sizeof(type) == 1)
    return value;
  else if constexpr (sizeof(type) == 2)
    return static_cast<type>(SDL_SwapLE16(static_cast<std::uint16_t>(value)));
  else if constexpr (sizeof(type) == 4)
    return static_cast<type>(SDL_SwapLE32(static_cast<std::uint32_t>(value)));
  else if constexpr (sizeof(type) == 8)
    return static_cast<type>(SDL_SwapLE64(static_cast<std::uint64_t>(value)));
  else
    static_assert(sizeof(type) == 1 || sizeof(type) == 2 || sizeof(type) == 4 || sizeof(type) == 8, "Unsupported type size.");
  return {};
}
template <std::integral type> [[nodiscard]]
constexpr type byteswap_be(type value) noexcept
{
  if      constexpr (sizeof(type) == 1)
    return value;
  else if constexpr (sizeof(type) == 2)
    return static_cast<type>(SDL_SwapBE16(static_cast<std::uint16_t>(value)));
  else if constexpr (sizeof(type) == 4)
    return static_cast<type>(SDL_SwapBE32(static_cast<std::uint32_t>(value)));
  else if constexpr (sizeof(type) == 8)
    return static_cast<type>(SDL_SwapBE64(static_cast<std::uint64_t>(value)));
  else
    static_assert(sizeof(type) == 1 || sizeof(type) == 2 || sizeof(type) == 4 || sizeof(type) == 8, "Unsupported type size.");
  return {};
}
}
