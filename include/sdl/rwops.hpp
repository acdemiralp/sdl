#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <expected>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include <SDL_rwops.h>

#include <sdl/error.hpp>

namespace sdl
{
enum class rw_ops_type
{
  unknown   = SDL_RWOPS_UNKNOWN  ,
  win_file  = SDL_RWOPS_WINFILE  ,
  std_file  = SDL_RWOPS_STDFILE  ,
  jni_file  = SDL_RWOPS_JNIFILE  ,
  memory    = SDL_RWOPS_MEMORY   ,
  memory_ro = SDL_RWOPS_MEMORY_RO
};
enum class seek_mode
{
  set = 0,
  cur = 1,
  end = 2
};

using native_rw_ops = SDL_RWops;

// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<native_rw_ops*           , std::string> rw_from_file     (const std::string& filepath, const std::string& mode)
{
  auto result = SDL_RWFromFile(filepath.c_str(), mode.c_str());
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<native_rw_ops*           , std::string> rw_from_fp       (std::FILE* file, bool auto_close = false)
{
  auto result = SDL_RWFromFP(file, static_cast<SDL_bool>(auto_close));
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<native_rw_ops*           , std::string> rw_from_mem      (const std::span<      std::byte>& memory)
{
  auto result = SDL_RWFromMem     (memory.data(), static_cast<std::int32_t>(memory.size()));
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<native_rw_ops*           , std::string> rw_from_const_mem(const std::span<const std::byte>& memory)
{
  auto result = SDL_RWFromConstMem(memory.data(), static_cast<std::int32_t>(memory.size()));
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<native_rw_ops*           , std::string> alloc_rw         ()
{
  auto result = SDL_AllocRW();
  if (!result)
    return std::unexpected(get_error());
  return result;
}

// Bad practice: You should use `std::fstream` instead.
inline void                                                  free_rw          (native_rw_ops* ops)
{
  SDL_FreeRW(ops);
}

// Bad practice: You should use `std::fstream` instead.
inline std::expected<void                     , std::string> rw_close         (native_rw_ops* ops)
{
  if (SDL_RWclose(ops) < 0)
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::int64_t             , std::string> rw_size          (native_rw_ops* ops)
{
  auto result = SDL_RWsize(ops);
  if (result < 0)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::int64_t             , std::string> rw_tell          (native_rw_ops* ops)
{
  auto result = SDL_RWtell(ops);
  if (result < 0)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::int64_t             , std::string> rw_seek          (native_rw_ops* ops, const std::int64_t offset, seek_mode origin = seek_mode::set)
{
  auto result = SDL_RWseek(ops, offset, static_cast<std::int32_t>(origin));
  if (result < 0)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::size_t              , std::string> rw_read          (native_rw_ops* ops,       void* buffer, const std::size_t size, const std::size_t count)
{
  // Design notes: This interface is left identical to `std::fread` instead of using a byte span as input. See `rw_read_as`.
  auto result = SDL_RWread(ops, buffer, size, count);
  if (!result)
    return std::unexpected(get_error());
  return result;
}

// Bad practice: You should use `std::fstream` instead.
inline std::expected<std::size_t              , std::string> rw_write         (native_rw_ops* ops, const void* buffer, const std::size_t size, const std::size_t count)
{
  // Design notes: This interface is left identical to `std::fread` instead of using a byte span as input. See `rw_write_as`.
  auto result = SDL_RWwrite(ops, buffer, size, count);
  if (result < count)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::vector<std::uint8_t>, std::string> load_file_rw     (native_rw_ops* ops, const bool free_source = false)
{
  std::size_t size {};

  const auto data = static_cast<std::uint8_t*>(SDL_LoadFile_RW(ops, &size, free_source));
  if (!data)
    return std::unexpected(get_error());

  std::vector result(data, data + size);
  SDL_free(data);
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::vector<std::uint8_t>, std::string> load_file        (const std::string& filepath)
{
  std::size_t size {};

  const auto data = static_cast<std::uint8_t*>(SDL_LoadFile(filepath.c_str(), &size));
  if (!data)
    return std::unexpected(get_error());

  std::vector result(data, data + size);
  SDL_free(data);
  return result;
}

// Note: This interface is erroneous in SDL2 as its failure state is a zero, which may conflict the value read from a file. It will be fixed in SDL3.

// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::uint8_t             , std::string> read_u8          (native_rw_ops* ops)
{
  auto result = SDL_ReadU8(ops);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::uint16_t            , std::string> read_le16        (native_rw_ops* ops)
{
  auto result = SDL_ReadLE16(ops);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::uint16_t            , std::string> read_be16        (native_rw_ops* ops)
{
  auto result = SDL_ReadBE16(ops);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::uint32_t            , std::string> read_le32        (native_rw_ops* ops)
{
  auto result = SDL_ReadLE32(ops);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::uint32_t            , std::string> read_be32        (native_rw_ops* ops)
{
  auto result = SDL_ReadBE32(ops);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::uint64_t            , std::string> read_le64        (native_rw_ops* ops)
{
  auto result = SDL_ReadLE64(ops);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<std::uint64_t            , std::string> read_be64        (native_rw_ops* ops)
{
  auto result = SDL_ReadBE64(ops);
  if (!result)
    return std::unexpected(get_error());
  return result;
}

// Bad practice: You should use `std::fstream` instead.
inline std::expected<void                     , std::string> write_u8         (native_rw_ops* ops, const std::uint8_t  value)
{
  if (!SDL_WriteU8(ops, value))
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::fstream` instead.
inline std::expected<void                     , std::string> write_le16       (native_rw_ops* ops, const std::uint16_t value)
{
  if (!SDL_WriteLE16(ops, value))
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::fstream` instead.
inline std::expected<void                     , std::string> write_be16       (native_rw_ops* ops, const std::uint16_t value)
{
  if (!SDL_WriteBE16(ops, value))
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::fstream` instead.
inline std::expected<void                     , std::string> write_le32       (native_rw_ops* ops, const std::uint32_t value)
{
  if (!SDL_WriteLE32(ops, value))
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::fstream` instead.
inline std::expected<void                     , std::string> write_be32       (native_rw_ops* ops, const std::uint32_t value)
{
  if (!SDL_WriteBE32(ops, value))
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::fstream` instead.
inline std::expected<void                     , std::string> write_le64       (native_rw_ops* ops, const std::uint64_t value)
{
  if (!SDL_WriteLE64(ops, value))
    return std::unexpected(get_error());
  return {};
}
// Bad practice: You should use `std::fstream` instead.
inline std::expected<void                     , std::string> write_be64       (native_rw_ops* ops, const std::uint64_t value)
{
  if (!SDL_WriteBE64(ops, value))
    return std::unexpected(get_error());
  return {};
}

// Conveniences.

// Bad practice: You should use `std::fstream` instead.
template <typename type>      [[nodiscard]]
std::expected<native_rw_ops*                  , std::string> rw_from_mem      (const std::span<      type>& memory)
{
  return sdl::rw_from_mem      (std::as_writable_bytes(memory));
}
// Bad practice: You should use `std::fstream` instead.
template <typename type>      [[nodiscard]]
std::expected<native_rw_ops*                  , std::string> rw_from_const_mem(const std::span<const type>& memory)
{
  return sdl::rw_from_const_mem(std::as_bytes         (memory));
}
// Bad practice: You should use `std::fstream` instead.
template <typename type>      [[nodiscard]]
std::expected<std::size_t                     , std::string> rw_read_as       (native_rw_ops* ops, const std::span<      type>& buffer)
{
  return rw_read (ops, buffer.data(), sizeof(type), buffer.size());
}
// Bad practice: You should use `std::fstream` instead.
template <typename type>      [[nodiscard]]
std::expected<std::size_t                     , std::string> rw_write_as      (native_rw_ops* ops, const std::span<const type>& buffer)
{
  return rw_write(ops, buffer.data(), sizeof(type), buffer.size());
}
// Bad practice: You should use `std::fstream` instead.
template <std::integral type> [[nodiscard]]
std::expected<type                            , std::string> read_le_integer  (native_rw_ops* ops)
{
  if      constexpr (sizeof(type) == 1)
    return static_cast<type>(read_u8  (ops));
  else if constexpr (sizeof(type) == 2)
    return static_cast<type>(read_le16(ops));
  else if constexpr (sizeof(type) == 4)
    return static_cast<type>(read_le32(ops));
  else if constexpr (sizeof(type) == 8)
    return static_cast<type>(read_le64(ops));
  else
  {
    static_assert(sizeof(type) == 0, "Invalid integer type.");
    return 0;
  }
}
// Bad practice: You should use `std::fstream` instead.
template <std::integral type> [[nodiscard]]
std::expected<type                            , std::string> read_be_integer  (native_rw_ops* ops)
{
  if      constexpr (sizeof(type) == 1)
    return static_cast<type>(read_u8  (ops));
  else if constexpr (sizeof(type) == 2)
    return static_cast<type>(read_be16(ops));
  else if constexpr (sizeof(type) == 4)
    return static_cast<type>(read_be32(ops));
  else if constexpr (sizeof(type) == 8)
    return static_cast<type>(read_be64(ops));
  else
  {
    static_assert(sizeof(type) == 0, "Invalid integer type.");
    return 0;
  }
}
// Bad practice: You should use `std::fstream` instead.
template <std::integral type> [[nodiscard]]
std::expected<void                            , std::string> write_le_integer (native_rw_ops* ops, const type& value)
{
  if      constexpr (sizeof(type) == 1)
    return write_u8  (ops, static_cast<std::uint8_t> (value));
  else if constexpr (sizeof(type) == 2)
    return write_le16(ops, static_cast<std::uint16_t>(value));
  else if constexpr (sizeof(type) == 4)
    return write_le32(ops, static_cast<std::uint32_t>(value));
  else if constexpr (sizeof(type) == 8)
    return write_le64(ops, static_cast<std::uint64_t>(value));
  else
  {
    static_assert(sizeof(type) == 0, "Invalid integer type.");
    return {};
  }
}
// Bad practice: You should use `std::fstream` instead.
template <std::integral type> [[nodiscard]]
std::expected<void                            , std::string> write_be_integer (native_rw_ops* ops, const type& value)
{
  if      constexpr (sizeof(type) == 1)
    return write_u8  (ops, static_cast<std::uint8_t> (value));
  else if constexpr (sizeof(type) == 2)
    return write_be16(ops, static_cast<std::uint16_t>(value));
  else if constexpr (sizeof(type) == 4)
    return write_be32(ops, static_cast<std::uint32_t>(value));
  else if constexpr (sizeof(type) == 8)
    return write_be64(ops, static_cast<std::uint64_t>(value));
  else
  {
    static_assert(sizeof(type) == 0, "Invalid integer type.");
    return {};
  }
}

// Bad practice: You should use `std::fstream` instead.
class rw_ops
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_rw_ops(...)` to handle errors.
  rw_ops           ()
  : native_     (alloc_rw().value_or(nullptr))
  , should_free_(true)
  {
    
  }
  // The constructor cannot transmit error state. You should use `sdl::make_rw_ops(...)` to handle errors.
  rw_ops           (const std::string& filepath, const std::string& mode)
  : native_(rw_from_file(filepath, mode).value_or(nullptr))
  {
    
  }
  // The constructor cannot transmit error state. You should use `sdl::make_rw_ops(...)` to handle errors.
  rw_ops           (std::FILE* file, const bool auto_close = false)
  : native_(rw_from_fp(file, auto_close).value_or(nullptr))
  {
    
  }
  // The constructor cannot transmit error state. You should use `sdl::make_rw_ops(...)` to handle errors.
  rw_ops           (const std::span<      std::byte>& memory)
  : native_(rw_from_mem(memory).value_or(nullptr))
  {
    
  }
  // The constructor cannot transmit error state. You should use `sdl::make_rw_ops(...)` to handle errors.
  rw_ops           (const std::span<const std::byte>& memory)
  : native_(rw_from_const_mem(memory).value_or(nullptr))
  {
    
  }
  // The constructor cannot transmit error state. You should use `sdl::make_rw_ops(...)` to handle errors.
  template <typename type>
  rw_ops           (const std::span<      type>&      memory)
  : native_(rw_from_mem(memory).value_or(nullptr))
  {
    
  }
  // The constructor cannot transmit error state. You should use `sdl::make_rw_ops(...)` to handle errors.
  template <typename type>
  rw_ops           (const std::span<const type>&      memory)
  : native_(rw_from_const_mem(memory).value_or(nullptr))
  {
    
  }
  rw_ops           (const rw_ops&  that) = delete;
  rw_ops           (      rw_ops&& temp) noexcept
  : native_     (temp.native_)
  , should_free_(temp.should_free_)
  {
    temp.native_ = nullptr;
  }
 ~rw_ops           ()
  {
    if (native_)
      should_free_ ? free_rw (native_) : rw_close(native_).value();
  }
  rw_ops& operator=(const rw_ops&  that) = delete;
  rw_ops& operator=(      rw_ops&& temp) noexcept
  {
    if (this != &temp)
    {
      if (native_)
        should_free_ ? free_rw(native_) : rw_close(native_).value();

      std::swap(native_     , temp.native_);
      std::swap(should_free_, temp.should_free_);
    }
    return *this;
  }

  [[nodiscard]]
  std::expected<std::int64_t             , std::string> size            () const
  {
    return rw_size(native_);
  }
  [[nodiscard]]
  std::expected<std::int64_t             , std::string> tell            () const
  {
    return rw_tell(native_);
  }
  [[nodiscard]]
  std::expected<std::int64_t             , std::string> seek            (const std::int64_t offset, const seek_mode origin = seek_mode::set) const
  {
    return rw_seek(native_, offset, origin);
  }
  [[nodiscard]]
  std::expected<std::size_t              , std::string> read            (      void* buffer, const std::size_t size, const std::size_t count) const
  {
    return rw_read (native_, buffer, size, count);
  }
  
  std::expected<std::size_t              , std::string> write           (const void* buffer, const std::size_t size, const std::size_t count)
  {
    return rw_write(native_, buffer, size, count);
  }
  [[nodiscard]]
  std::expected<std::vector<std::uint8_t>, std::string> load            () const
  {
    return load_file_rw(native_, false); // The source will be freed in the destructor.
  }

  template <typename type>      [[nodiscard]]
  std::expected<std::size_t              , std::string> read_as         (const std::span<      type>& buffer) const
  {
    return rw_read_as<type>(buffer.data(), buffer.size());
  }
  template <typename type>
  std::expected<std::size_t              , std::string> write_as        (const std::span<const type>& buffer)
  {
    return rw_write_as<type>(native_, buffer.data(), buffer.size());
  }

  template <std::integral type> [[nodiscard]]
  std::expected<type                     , std::string> read_le_integer () const
  {
    return sdl::read_le_integer<type>(native_);
  }
  template <std::integral type> [[nodiscard]]
  std::expected<type                     , std::string> read_be_integer () const
  {
    return sdl::read_be_integer<type>(native_);
  }
  template <std::integral type> [[nodiscard]]
  std::expected<void                     , std::string> write_le_integer(const type& value)
  {
    return sdl::write_le_integer<type>(native_, value);
  }
  template <std::integral type> [[nodiscard]]
  std::expected<void                     , std::string> write_be_integer(const type& value)
  {
    return sdl::write_be_integer<type>(native_, value);
  }

  [[nodiscard]]
  SDL_RWops*                                            native          () const
  {
    return native_;
  }

protected:
  SDL_RWops* native_      {};
  bool       should_free_ {false};
};

// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<rw_ops                   , std::string> make_rw_ops      ()
{
  auto result = rw_ops();
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<rw_ops                   , std::string> make_rw_ops      (const std::string& filepath, const std::string& mode)
{
  auto result = rw_ops(filepath, mode);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<rw_ops                   , std::string> make_rw_ops      (std::FILE* file, const bool auto_close = false)
{
  auto result = rw_ops(file, auto_close);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<rw_ops                   , std::string> make_rw_ops      (const std::span<      std::byte>& memory)
{
  auto result = rw_ops(memory);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
[[nodiscard]]
inline std::expected<rw_ops                   , std::string> make_rw_ops      (const std::span<const std::byte>& memory)
{
  auto result = rw_ops(memory);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
template <typename type> [[nodiscard]]
std::expected       <rw_ops                   , std::string> make_rw_ops      (const std::span<      type>&      memory)
{
  auto result = rw_ops(memory);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
// Bad practice: You should use `std::fstream` instead.
template <typename type> [[nodiscard]]
std::expected       <rw_ops                   , std::string> make_rw_ops      (const std::span<const type>&      memory)
{
  auto result = rw_ops(memory);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
}