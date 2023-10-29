#include <doctest/doctest.h>

#include <iostream>

#include <sdl/core/all.hpp>
#include <sdl/core/basics/log.hpp>

TEST_CASE("SDL Test")
{
  const auto output_function = sdl::get_log_output_function();

  output_function(sdl::log_category::application, sdl::log_priority::info, "Testing output function getter.");

  sdl::set_log_output_function([&] (const sdl::log_category, const sdl::log_priority, const std::string& message)
  {
    std::cout << "Log function 1: " << message << "\n";
  });

  sdl::log("Testing log output function setter 1.");
  
  sdl::set_log_output_function([&] (const sdl::log_category, const sdl::log_priority, const std::string& message)
  {
    std::cout << "Log function 2: " << message << "\n";
  });

  sdl::log("Testing log output function setter 2.");
}