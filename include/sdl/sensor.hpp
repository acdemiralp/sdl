#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

#include <SDL_sensor.h>

#include <sdl/error.hpp>

namespace sdl
{
using native_sensor = SDL_Sensor;

enum class sensor_type
{
  //invalid           = SDL_SENSOR_INVALID,
  unknown             = SDL_SENSOR_UNKNOWN,
  accelerometer       = SDL_SENSOR_ACCEL  ,
  gyroscope           = SDL_SENSOR_GYRO   ,
  accelerometer_left  = SDL_SENSOR_ACCEL_L,
  gyroscope_left      = SDL_SENSOR_GYRO_L ,
  accelerometer_right = SDL_SENSOR_ACCEL_R,
  gyroscope_right     = SDL_SENSOR_GYRO_R
};

struct     sensor_data
{
  explicit sensor_data  (const std::size_t size)
  : data(size), timestamps(size)
  {
    
  }
  sensor_data           (const sensor_data&  that) = default;
  sensor_data           (      sensor_data&& temp) = default;
 ~sensor_data           ()                         = default;
  sensor_data& operator=(const sensor_data&  that) = default;
  sensor_data& operator=(      sensor_data&& temp) = default;

  std::vector<float>         data;
  std::vector<std::uint64_t> timestamps;
};

inline constexpr float standard_gravity = SDL_STANDARD_GRAVITY;

inline void                                           lock_sensors                       ()
{
  SDL_LockSensors  ();
}
inline void                                           unlock_sensors                     ()
{
  SDL_UnlockSensors();
}

[[nodiscard]]
inline std::int32_t                                   num_sensors                        ()
{
  return SDL_NumSensors();
}
[[nodiscard]]
inline std::expected<std::string       , std::string> sensor_get_device_name             (const std::int32_t device_index)
{
  const auto result = SDL_SensorGetDeviceName(device_index);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<sensor_type       , std::string> sensor_get_device_type             (const std::int32_t device_index)
{
  const auto result = SDL_SensorGetDeviceType(device_index);
  if (result == SDL_SENSOR_INVALID)
    return std::unexpected(get_error());
  return static_cast<sensor_type>(result);
}
[[nodiscard]]
inline std::expected<std::int32_t      , std::string> sensor_get_device_non_portable_type(const std::int32_t device_index)
{
  const auto result = SDL_SensorGetDeviceNonPortableType(device_index);
  if (result < 0)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<std::int32_t      , std::string> sensor_get_device_instance_id      (const std::int32_t device_index)
{
  const auto result = SDL_SensorGetDeviceInstanceID(device_index);
  if (result < 0)
    return std::unexpected(get_error());
  return result;
}

[[nodiscard]]
inline std::expected<native_sensor*    , std::string> sensor_open                        (const std::int32_t device_index)
{
  const auto result = SDL_SensorOpen(device_index);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<native_sensor*    , std::string> sensor_from_instance_id            (const std::int32_t instance_id)
{
  const auto result = SDL_SensorFromInstanceID(instance_id);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<std::string       , std::string> sensor_get_name                    (native_sensor* sensor)
{
  const auto result = SDL_SensorGetName(sensor);
  if (!result)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<sensor_type       , std::string> sensor_get_type                    (native_sensor* sensor)
{
  const auto result = SDL_SensorGetType(sensor);
  if (result == SDL_SENSOR_INVALID)
    return std::unexpected(get_error());
  return static_cast<sensor_type>(result);
}
[[nodiscard]]
inline std::expected<std::int32_t      , std::string> sensor_get_non_portable_type       (native_sensor* sensor)
{
  const auto result = SDL_SensorGetNonPortableType(sensor);
  if (result < 0)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<std::int32_t      , std::string> sensor_get_instance_id             (native_sensor* sensor)
{
  const auto result = SDL_SensorGetInstanceID(sensor);
  if (result < 0)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<std::vector<float>, std::string> sensor_get_data                    (native_sensor* sensor, const std::int32_t count = 1)
{
  std::vector<float> result(static_cast<std::size_t>(count));
  if (SDL_SensorGetData(sensor, result.data(), count) < 0)
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::expected<sensor_data       , std::string> sensor_get_data_with_timestamp     (native_sensor* sensor, const std::int32_t count = 1)
{
  sensor_data result(static_cast<std::size_t>(count));
  if (SDL_SensorGetDataWithTimestamp(sensor, result.timestamps.data(), result.data.data(), count) < 0)
    return std::unexpected(get_error());
  return result;
}

inline void                                           sensor_close                       (native_sensor* sensor)
{
  SDL_SensorClose(sensor);
}

inline void                                           sensor_update                      ()
{
  SDL_SensorUpdate();
}

// Conveniences.

class sensor;

class sensor_info
{
public:
  explicit sensor_info  (const std::int32_t  index)
  : index_(index)
  {
    
  }
  sensor_info           (const sensor_info&  that ) = default;
  sensor_info           (      sensor_info&& temp ) = default;
 ~sensor_info           ()                          = default;
  sensor_info& operator=(const sensor_info&  that ) = default;
  sensor_info& operator=(      sensor_info&& temp ) = default;

  [[nodiscard]]
  std::expected<std::string , std::string> name             () const
  {
    return sensor_get_device_name             (index_);
  }
  [[nodiscard]]
  std::expected<sensor_type , std::string> type             () const
  {
    return sensor_get_device_type             (index_);
  }
  [[nodiscard]]
  std::expected<std::int32_t, std::string> non_portable_type() const
  {
    return sensor_get_device_non_portable_type(index_);
  }
  [[nodiscard]]
  std::expected<std::int32_t, std::string> instance_id      () const
  {
    return sensor_get_device_instance_id      (index_);
  }

  [[nodiscard]]
  std::expected<sensor      , std::string> open             () const;

  [[nodiscard]]
  std::int32_t                             index            () const
  {
    return index_;
  }

private:
  std::int32_t index_;
};

class sensor
{
public:
  // The constructor cannot transmit error state. You should use `sdl::make_sensor(std::int32_t, bool)` to handle errors.
  explicit sensor  (const std::int32_t index, const bool is_instance_id = false)
  : native_(is_instance_id ? sensor_from_instance_id(index).value_or(nullptr) : sensor_open(index).value_or(nullptr))
  {
    
  }
  sensor           (const sensor&      that ) = delete;
  sensor           (      sensor&&     temp ) noexcept
  : native_(temp.native_)
  {
    temp.native_ = nullptr;
  }
 ~sensor           ()
  {
    if (native_)
      sensor_close(native_);
  }
  sensor& operator=(const sensor&      that ) = delete;
  sensor& operator=(      sensor&&     temp ) noexcept
  {
    if (this != &temp)
    {
      if (native_)
        sensor_close(native_);

      native_      = temp.native_;

      temp.native_ = nullptr;
    }

    return *this;
  }

  [[nodiscard]]
  std::expected<std::string       , std::string> name               () const
  {
    return sensor_get_name               (native_);
  }
  [[nodiscard]]
  std::expected<sensor_type       , std::string> type               () const
  {
    return sensor_get_type               (native_);
  }
  [[nodiscard]]
  std::expected<std::int32_t      , std::string> non_portable_type  () const
  {
    return sensor_get_non_portable_type  (native_);
  }
  [[nodiscard]]
  std::expected<std::int32_t      , std::string> instance_id        () const
  {
    return sensor_get_instance_id        (native_);
  }

  [[nodiscard]]
  std::expected<std::vector<float>, std::string> data               (const std::int32_t count = 1) const
  {
    return sensor_get_data               (native_, count);
  }
  [[nodiscard]]
  std::expected<sensor_data       , std::string> data_with_timestamp(const std::int32_t count = 1) const
  {
    return sensor_get_data_with_timestamp(native_, count);
  }

  [[nodiscard]]
  native_sensor*                                 native             () const
  {
    return native_;
  }

private:
  native_sensor* native_ {};
};

[[nodiscard]]
inline std::expected<sensor, std::string>             make_sensor                        (const std::int32_t index, const bool is_instance_id = false)
{
  sensor result(index, is_instance_id);
  if (!result.native())
    return std::unexpected(get_error());
  return result;
}
[[nodiscard]]
inline std::vector<sensor_info>                       get_sensor_infos                   ()
{
  const auto count = num_sensors();

  std::vector<sensor_info> result;
  result.reserve(static_cast<std::size_t>(count));
  for (auto i = 0; i < count; ++i)
    result.emplace_back(i);
  return result;
}
// Warning: This function opens all sensors which is a potentially expensive operation. Prefer `sdl::get_sensor_infos()` and `sdl::sensor_info::open()` if possible.
[[nodiscard]]
inline std::vector<sensor>                            get_sensors                        ()
{
  const auto count = num_sensors();

  std::vector<sensor> result;
  result.reserve(static_cast<std::size_t>(count));
  for (auto i = 0; i < count; ++i)
    result.emplace_back(i);
  return result;
}

inline std::expected<sensor, std::string> sensor_info::open() const
{
  return make_sensor(index_);
}
}