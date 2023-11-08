#include <doctest/doctest.h>
#include <sdl/all.hpp>

#include <sdl/atomic.hpp>
#include <sdl/log.hpp>
#include <sdl/mouse.hpp>
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
  auto retrieved_string = tls.get<std::string>();

  const auto window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  bool       quit   = false;
  SDL_Event  event;
  while (quit == false)
    while (SDL_PollEvent(&event))
      if (event.type == SDL_QUIT)
        quit = true;
}