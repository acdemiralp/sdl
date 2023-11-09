#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include <SDL_hidapi.h>

#include <sdl/error.hpp>

namespace sdl
{
using native_hid_device      = SDL_hid_device;
using native_hid_device_info = SDL_hid_device_info;

inline std::expected<void                   , std::string> hid_init                    ()
{
  if (SDL_hid_init() < 0)
    return std::unexpected(get_error());
  return {};
}

inline std::expected<void                   , std::string> hid_exit                    ()
{
  if (SDL_hid_exit() < 0)
    return std::unexpected(get_error());
  return {};
}

inline void                                                hid_ble_scan                (bool active)
{
  SDL_hid_ble_scan(static_cast<SDL_bool>(active));
}
[[nodiscard]]
inline std::uint32_t                                       hid_device_change_count     ()
{
  return SDL_hid_device_change_count();
}


[[nodiscard]]
inline std::expected<native_hid_device_info*, std::string> hid_enumerate               (const std::uint16_t vendor_id = 0, const std::uint16_t product_id = 0)
{
  auto result = SDL_hid_enumerate(vendor_id, product_id);
  if (!result)
    return std::unexpected(get_error());
  return result;
}

inline void                                                hid_free_enumeration        (native_hid_device_info* device_info)
{
  SDL_hid_free_enumeration(device_info);
}


[[nodiscard]]
inline std::expected<native_hid_device*     , std::string> hid_open                    (const std::uint16_t vendor_id    , const std::uint16_t product_id    , const std::optional<std::wstring>& serial_number = std::nullopt)
{
  auto result = SDL_hid_open(vendor_id, product_id, serial_number ? serial_number->c_str() : nullptr);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<native_hid_device*     , std::string> hid_open_path               (const std::string& path, const bool exclusive = false)
{
  auto result = SDL_hid_open_path(path.c_str(), exclusive);
  if (!result)
    return std::unexpected(get_error());
  return result;
}

inline void                                                hid_close                   (native_hid_device* device)
{
  SDL_hid_close(device);
}

inline std::expected<void                   , std::string> hid_set_nonblocking         (native_hid_device* device, const bool nonblocking)
{
  if (SDL_hid_set_nonblocking(device, nonblocking) < 0)
    return std::unexpected(get_error());
  return {};
}
[[nodiscard]]
inline std::expected<std::wstring           , std::string> hid_get_manufacturer_string (native_hid_device* device,                           const std::size_t buffer_size = 256)
{
  std::wstring buffer(buffer_size, L'\0');
  if (SDL_hid_get_manufacturer_string(device, buffer.data(), buffer.size()) < 0)
    return std::unexpected(get_error());
  buffer.erase(buffer.find(L'\0'));
  return buffer;
}
[[nodiscard]]
inline std::expected<std::wstring           , std::string> hid_get_product_string      (native_hid_device* device,                           const std::size_t buffer_size = 256)
{
  std::wstring buffer(buffer_size, L'\0');
  if (SDL_hid_get_product_string(device, buffer.data(), buffer.size()) < 0)
    return std::unexpected(get_error());
  buffer.erase(buffer.find(L'\0'));
  return buffer;
}
[[nodiscard]]
inline std::expected<std::wstring           , std::string> hid_get_serial_number_string(native_hid_device* device,                           const std::size_t buffer_size = 256)
{
  std::wstring buffer(buffer_size, L'\0');
  if (SDL_hid_get_serial_number_string(device, buffer.data(), buffer.size()) < 0)
    return std::unexpected(get_error());
  buffer.erase(buffer.find(L'\0'));
  return buffer;
}
[[nodiscard]]
inline std::expected<std::wstring           , std::string> hid_get_indexed_string      (native_hid_device* device, const std::int32_t index, const std::size_t buffer_size = 256)
{
  std::wstring buffer(buffer_size, L'\0');
  if (SDL_hid_get_indexed_string(device, index, buffer.data(), buffer.size()) < 0)
    return std::unexpected(get_error());
  buffer.erase(buffer.find(L'\0'));
  return buffer;
}
// The size of the span determines the number of bytes to be read.
inline std::expected<void                   , std::string> hid_read                    (native_hid_device* device,       std::span<std::uint8_t>& data)
{
  if (SDL_hid_read(device, data.data(), data.size()) < 0)
    return std::unexpected(get_error());
  return {};
}
// The size of the span determines the number of bytes to be read.
inline std::expected<void                   , std::string> hid_read_timeout            (native_hid_device* device,       std::span<std::uint8_t>& data, const std::chrono::milliseconds timeout)
{
  if (SDL_hid_read_timeout(device, data.data(), data.size(), static_cast<std::int32_t>(timeout.count())) < 0)
    return std::unexpected(get_error());
  return {};
}

inline std::expected<void                   , std::string> hid_write                   (native_hid_device* device, const std::span<std::uint8_t>& data)
{
  if (SDL_hid_write(device, data.data(), data.size()) < 0)
    return std::unexpected(get_error());
  return {};
}
// The size of the span determines the number of bytes to be read.
inline std::expected<void                   , std::string> hid_get_feature_report      (native_hid_device* device,       std::span<std::uint8_t>& data)
{
  if (SDL_hid_get_feature_report(device, data.data(), data.size()) < 0)
    return std::unexpected(get_error());
  return {};
}

inline std::expected<void                   , std::string> hid_send_feature_report     (native_hid_device* device, const std::span<std::uint8_t>& data)
{
  if (SDL_hid_send_feature_report(device, data.data(), data.size()) < 0)
    return std::unexpected(get_error());
  return {};
}

// Conveniences.

struct hid_device_info
{
  std::string   path;
  std::uint16_t vendor_id;
  std::uint16_t product_id;
  std::wstring  serial_number;
  std::uint16_t release_number;
  std::wstring  manufacturer_string;
  std::wstring  product_string;
  std::uint16_t usage_page;
  std::uint16_t usage;
  std::int32_t  interface_number;
  std::int32_t  interface_class;
  std::int32_t  interface_subclass;
  std::int32_t  interface_protocol;
};

class  hid_device
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_hid_device(std::uint16_t, std::uint16_t, const std::optional<std::wstring>&)` to handle errors.
  hid_device           (const std::uint16_t vendor_id, const std::uint16_t product_id, const std::optional<std::wstring>& serial_number = std::nullopt)
  : native_(hid_open(vendor_id, product_id, serial_number).value_or(nullptr))
  {
    
  }
  // The constructor cannot transmit error state. You should use `sdl::make_hid_device(const std::string&, bool)` to handle errors.
  hid_device           (const std::string& path, const bool exclusive = false)
  : native_(hid_open_path(path, exclusive).value_or(nullptr))
  {
    
  }
  hid_device           (const hid_device&  that) = delete;
  hid_device           (      hid_device&& temp) noexcept
  {
    std::swap(native_, temp.native_);
  }
 ~hid_device           ()
  {
    if (native_)
      hid_close(native_);
  }
  hid_device& operator=(const hid_device&  that) = delete;
  hid_device& operator=(      hid_device&& temp) noexcept
  {
    if (this != &temp)
    {
      if (native_)
        hid_close(native_);

      std::swap(native_, temp.native_);
    }
    return *this;
  }

  std::expected<void        , std::string> set_nonblocking         (const bool nonblocking) const
  {
    return hid_set_nonblocking(native_, nonblocking);
  }
  [[nodiscard]]
  std::expected<std::wstring, std::string> manufacturer_string     (                          const std::size_t buffer_size = 256) const
  {
    return hid_get_manufacturer_string(native_, buffer_size);
  }
  [[nodiscard]]
  std::expected<std::wstring, std::string> product_string          (                          const std::size_t buffer_size = 256) const
  {
    return hid_get_product_string(native_, buffer_size);
  }
  [[nodiscard]]
  std::expected<std::wstring, std::string> serial_number_string    (                          const std::size_t buffer_size = 256) const
  {
    return hid_get_serial_number_string(native_, buffer_size);
  }
  [[nodiscard]]
  std::expected<std::wstring, std::string> indexed_string          (const std::int32_t index, const std::size_t buffer_size = 256) const
  {
    return hid_get_indexed_string(native_, index, buffer_size);
  }
  // The size of the span determines the number of bytes to be read.
  std::expected<void        , std::string> read                    (      std::span<std::uint8_t>& data) const
  {
    return hid_read(native_, data);
  }
  // The size of the span determines the number of bytes to be read.
  std::expected<void        , std::string> read_timeout            (      std::span<std::uint8_t>& data, const std::chrono::milliseconds timeout) const
  {
    return hid_read_timeout(native_, data, timeout);
  }

  std::expected<void        , std::string> write                   (const std::span<std::uint8_t>& data) const
  {
    return hid_write(native_, data);
  }
  // The size of the span determines the number of bytes to be read.
  std::expected<void        , std::string> feature_report          (      std::span<std::uint8_t>& data) const
  {
    return hid_get_feature_report(native_, data);
  }

  std::expected<void        , std::string> send_feature_report     (const std::span<std::uint8_t>& data) const
  {
    return hid_send_feature_report(native_, data);
  }

  template <typename type>
  std::expected<void        , std::string> read_as                 (      std::span<type>& data) const
  {
    return hid_read               (native_, std::span<std::uint8_t>(reinterpret_cast<std::uint8_t*>(data.data()), data.size() * sizeof(type)));
  }
  template <typename type>
  std::expected<void        , std::string> read_as_timeout         (      std::span<type>& data, const std::chrono::milliseconds timeout) const
  {
    return hid_read_timeout       (native_, std::span<std::uint8_t>(reinterpret_cast<std::uint8_t*>(data.data()), data.size() * sizeof(type)), timeout);
  }
  template <typename type>
  std::expected<void        , std::string> write_as                (const std::span<type>& data) const
  {
    return hid_write              (native_, std::span<std::uint8_t>(reinterpret_cast<std::uint8_t*>(data.data()), data.size() * sizeof(type)));
  }
  template <typename type>
  std::expected<void        , std::string> feature_report_as       (      std::span<type>& data) const
  {
    return hid_get_feature_report (native_, std::span<std::uint8_t>(reinterpret_cast<std::uint8_t*>(data.data()), data.size() * sizeof(type)));
  }
  template <typename type>
  std::expected<void        , std::string> send_feature_report_as  (const std::span<type>& data) const
  {
    return hid_send_feature_report(native_, std::span<std::uint8_t>(reinterpret_cast<std::uint8_t*>(data.data()), data.size() * sizeof(type)));
  }

  [[nodiscard]]
  const native_hid_device*                 native                  () const noexcept
  {
    return native_;
  }

private:
  native_hid_device* native_ {};
};

[[nodiscard]]
inline std::expected<std::vector<hid_device_info>, std::string> hid_device_infos(const std::uint16_t vendor_id = 0, const std::uint16_t product_id = 0)
{
  const auto native = hid_enumerate(vendor_id, product_id);
  if (!native)
    return std::unexpected(native.error());

  std::vector<hid_device_info> result;
  for (auto iterator = native.value(); iterator != nullptr; iterator = iterator->next)
    result.emplace_back(
      iterator->path               ,
      iterator->vendor_id          ,
      iterator->product_id         ,
      iterator->serial_number      ,
      iterator->release_number     ,
      iterator->manufacturer_string,
      iterator->product_string     ,
      iterator->usage_page         ,
      iterator->usage              ,
      iterator->interface_number   ,
      iterator->interface_class    ,
      iterator->interface_subclass ,
      iterator->interface_protocol );
  hid_free_enumeration(native.value());
  return result;
}
[[nodiscard]]
inline std::expected<hid_device                  , std::string> make_hid_device (const std::uint16_t vendor_id    , const std::uint16_t product_id    , const std::optional<std::wstring>& serial_number = std::nullopt)
{
  hid_device result(vendor_id, product_id, serial_number);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<hid_device                  , std::string> make_hid_device (const std::string& path, const bool exclusive = false)
{
  hid_device result(path, exclusive);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
}