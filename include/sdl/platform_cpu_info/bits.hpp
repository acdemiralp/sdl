#pragma once

#include <cstdint>

#include <SDL2/SDL_bits.h>

namespace sdl
{
// Note: Returns -1 if the value is 0.
inline std::int32_t most_significant_bit_index_32(const std::uint32_t value)
{
  return SDL_MostSignificantBitIndex32(value);
}
// Bad Practice: You should use `std::has_single_bit<type>(type)` instead.
inline bool         has_exactly_one_bit_set_32   (const std::uint32_t value)
{
  return SDL_HasExactlyOneBitSet32    (value);
}
}