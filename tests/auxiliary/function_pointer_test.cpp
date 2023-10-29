#include <doctest/doctest.h>

#include <cstdint>
#include <functional>
#include <type_traits>

// This auxiliary test demonstrates:
// - Casting a function         to a void     pointer.
// - Casting a void     pointer to a function pointer.
// - Casting a function pointer to a function object .
// in a portable manner.

std::int32_t                  example_function    (const std::int32_t lhs, const std::int32_t rhs)
{
  return lhs + rhs;
}

void*                         get_void_pointer    ()
{
  auto result = &example_function;
  return reinterpret_cast<void*&>(result); // Extra & is necessary for portability.
}
template <typename function_pointer_type>
function_pointer_type         get_function_pointer()
{
  function_pointer_type result;
  *reinterpret_cast<void**>(&result) = get_void_pointer(); // Extra indirection is necessary for portability.
  return result;
}
template <typename signature_type>
std::function<signature_type> get_function_object ()
{
  return get_function_pointer<std::add_pointer_t<signature_type>>();
}

TEST_CASE("Function pointer test")
{
  using function_pointer_type              = std::int32_t (*) (std::int32_t, std::int32_t);
  using signature_type                     = std::int32_t     (std::int32_t, std::int32_t);
  using function_pointer_to_signature_type = std::remove_pointer_t<function_pointer_type>;
  using signature_to_function_pointer_type = std::add_pointer_t   <signature_type>;

  REQUIRE(std::is_same_v<function_pointer_type, signature_to_function_pointer_type>);
  REQUIRE(std::is_same_v<signature_type       , function_pointer_to_signature_type>);

  const auto function_pointer = get_function_pointer<function_pointer_type>();
  const auto function_object  = get_function_object <signature_type>       ();

  REQUIRE(example_function(1, 2) == 3);
  REQUIRE(function_pointer(1, 2) == 3);
  REQUIRE(function_object (1, 2) == 3);
}