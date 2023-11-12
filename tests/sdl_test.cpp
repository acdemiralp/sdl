#include <doctest/doctest.h>
#include <sdl/all.hpp>

#include <sdl/atomic.hpp>
#include <sdl/hidapi.hpp>
#include <sdl/log.hpp>
#include <sdl/mouse.hpp>
#include <sdl/rect.hpp>
#include <sdl/rwops.hpp>
#include <sdl/sdl.hpp>
#include <sdl/thread.hpp>
#include <sdl/timer.hpp>

TEST_CASE("SDL Test")
{
  sdl::every_subsystem subsystems;

  sdl::cursor cursor (std::string(
    "0000111100001111"
    "0000111100001111"
    "0000111100001111"
    "0000111100001111"
    "1111000011110000"
    "1111000011110000"
    "1111000011110000"
    "1111000011110000"
    "0000111100001111"
    "0000111100001111"
    "0000111100001111"
    "0000111100001111"
    "1111000011110000"
    "1111000011110000"
    "1111000011110000"
    "1111000011110000"
  ), {16, 16}, {0, 0});
  cursor.set_current();

  sdl::atomic_int      atomic_number (32);
  atomic_number += 32;

  std::int32_t         number        (32);
  sdl::atomic_ptr<int> atomic_pointer(nullptr);
  atomic_pointer.store(&number);
  atomic_pointer.load ();

  auto timer = sdl::make_timer(std::chrono::milliseconds(1000), [&] () { sdl::log_warn(sdl::log_category::application, std::string("Timer is up.")); }, true).value_or(nullptr);

  const std::string string_test = "Test";
  sdl::thread_local_storage tls;
  tls.set(&string_test);
  auto retrieved_string = tls.get_as<std::string>();

  sdl::rectangle<std::int32_t> rect {{0 , 0 , 32, 32}};
  sdl::rectangle<std::int32_t> rect2{{24, 24, 32, 32}};
  sdl::rectangle<float>        rect3{{0 , 0 , 32, 32}};
  sdl::rectangle<float>        rect4{{24, 24, 32, 32}};
  auto intersection = rect.intersect(rect2);
  auto merged       = rect.merged   (rect2);

  //auto device_infos = sdl::hid_device_infos().value();
  //auto device       = sdl::make_hid_device(device_infos[0].path).value();
  //auto device_name  = device.manufacturer_string().value() + L" " + device.product_string().value();

  std::vector values (10, 42);
  std::vector values2(10, 0 );
  auto rw_ops1 = sdl::rw_from_mem      (std::as_writable_bytes(std::span(values)));
  auto rw_ops2 = sdl::rw_from_const_mem(std::as_bytes         (std::span(values)));
  auto rw_ops3 = sdl::rw_from_mem      (std::span<      std::int32_t>(values));
  auto rw_ops4 = sdl::rw_from_const_mem(std::span<const std::int32_t>(values));
  sdl::rw_write_as(rw_ops1.value(), std::span<const std::int32_t>(values ));
  sdl::rw_read_as (rw_ops1.value(), std::span<      std::int32_t>(values2));
  sdl::rw_ops rops(std::span<std::uint32_t>(values));

  const auto window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  bool       quit   = false;
  SDL_Event  event;
  while (quit == false)
    while (SDL_PollEvent(&event))
      if (event.type == SDL_QUIT)
        quit = true;
}