#pragma once

#include <cstdint>

#include <SDL_bits.h>

namespace sdl
{
// Returns -1 if value is 0.
[[nodiscard]]
inline std::int32_t most_significant_bit_index_32(const std::uint32_t value)
{
  return SDL_MostSignificantBitIndex32(value);
}
// Bad practice: You should use `std::has_single_bit` instead.
[[nodiscard]]
inline bool         has_exactly_one_bit_set_32   (const std::uint32_t value)
{
  return SDL_HasExactlyOneBitSet32(value) == SDL_TRUE;
}
}