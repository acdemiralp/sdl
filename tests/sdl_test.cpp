#include <doctest/doctest.h>

#include <iostream>
#include <vector>

#include <sdl/all.hpp>
#include <sdl/basics/initialization.hpp>
#include <sdl/basics/log.hpp>
#include <sdl/platform_cpu_info/cpu_info.hpp>

TEST_CASE("SDL Test")
{
  // Logging tests.

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

  // Platform and cpu info tests.

  std::cout << "get_cpu_count          : " << sdl::get_cpu_count          () << "\n";
  std::cout << "get_cpu_cache_line_size: " << sdl::get_cpu_cache_line_size() << "B\n";

  std::cout << "get_system_ram         : " << sdl::get_system_ram         () << "MiB\n";

  std::cout << "has_rdtsc              : " << sdl::has_rdtsc              () << "\n";
  std::cout << "has_altivec            : " << sdl::has_altivec            () << "\n";
  std::cout << "has_mmx                : " << sdl::has_mmx                () << "\n";
  std::cout << "has_3dnow              : " << sdl::has_3dnow              () << "\n";
  std::cout << "has_sse                : " << sdl::has_sse                () << "\n";
  std::cout << "has_sse2               : " << sdl::has_sse2               () << "\n";
  std::cout << "has_sse3               : " << sdl::has_sse3               () << "\n";
  std::cout << "has_sse41              : " << sdl::has_sse41              () << "\n";
  std::cout << "has_sse42              : " << sdl::has_sse42              () << "\n";
  std::cout << "has_avx                : " << sdl::has_avx                () << "\n";
  std::cout << "has_avx2               : " << sdl::has_avx2               () << "\n";
  std::cout << "has_avx512f            : " << sdl::has_avx512f            () << "\n";
  std::cout << "has_arm_simd           : " << sdl::has_arm_simd           () << "\n";
  std::cout << "has_neon               : " << sdl::has_neon               () << "\n";
  std::cout << "has_lsx                : " << sdl::has_lsx                () << "\n";
  std::cout << "has_lasx               : " << sdl::has_lasx               () << "\n";

  std::cout << "get_simd_alignment     : " << sdl::get_simd_alignment     () << "B\n";

  auto simd_ptr        = sdl::simd_new        <std::uint64_t>(1);
  auto simd_unique_ptr = sdl::simd_make_unique<std::uint64_t>(2);
  auto simd_shared_ptr = sdl::simd_make_shared<std::uint64_t>(3);
  REQUIRE(*simd_ptr        == 1);
  REQUIRE(*simd_unique_ptr == 2);
  REQUIRE(*simd_shared_ptr == 3);
  sdl::simd_delete(simd_ptr);
  simd_unique_ptr.reset();
  simd_shared_ptr.reset();

  constexpr std::size_t array_size(8);

  auto simd_array        = sdl::simd_new_array        <std::uint64_t>(array_size, 1);
  auto simd_unique_array = sdl::simd_make_unique_array<std::uint64_t>(array_size, 2);
  auto simd_shared_array = sdl::simd_make_shared_array<std::uint64_t>(array_size, 3);
  for (std::size_t i = 0; i < array_size; ++i)
  {
    REQUIRE(simd_array             [i] == 1);
    REQUIRE(simd_unique_array.get()[i] == 2);
    REQUIRE(simd_shared_array.get()[i] == 3);
  }
  sdl::simd_delete_array(simd_array);
  simd_unique_array.reset();
  simd_shared_array.reset();

  auto simd_array_init_list        = sdl::simd_new_array        <std::uint64_t>({0, 1, 2, 3, 4, 5, 6, 7});
  auto simd_unique_array_init_list = sdl::simd_make_unique_array<std::uint64_t>({0, 1, 2, 3, 4, 5, 6, 7});
  auto simd_shared_array_init_list = sdl::simd_make_shared_array<std::uint64_t>({0, 1, 2, 3, 4, 5, 6, 7});
  for (std::size_t i = 0; i < array_size; ++i)
  {
    REQUIRE(simd_array_init_list             [i] == i);
    REQUIRE(simd_unique_array_init_list.get()[i] == i);
    REQUIRE(simd_shared_array_init_list.get()[i] == i);
  }
  sdl::simd_delete_array(simd_array_init_list);
  simd_unique_array_init_list.reset();
  simd_shared_array_init_list.reset();

  std::vector<std::uint64_t, sdl::simd_allocator<std::uint64_t>> simd_vector(array_size, 1);
  for (auto& value : simd_vector)
    REQUIRE(value == 1);
  simd_vector.clear();

  // Initialization tests.

  sdl::subsystem<sdl::subsystem_type::events>                             events_subsystem;
  sdl::subsystem<sdl::subsystem_type::audio | sdl::subsystem_type::video> audio_video_subsystem;
  sdl::subsystem<sdl::subsystem_type::everything>                         all_subsystems; // Calls global `sdl::quit()` on destruction.

}