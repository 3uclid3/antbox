#include <antbox/core/vec2.hpp>
#include <doctest/doctest.h>

namespace antbox { namespace {

TEST_CASE("vec2: arithmetic operates component-wise")
{
    vec2 value{2.0f, 4.0f};

    value += vec2{1.0f, -2.0f};
    value *= 2.0f;

    CHECK_EQ(value.x, doctest::Approx(6.0f));
    CHECK_EQ(value.y, doctest::Approx(4.0f));

    const vec2 result = value / 2.0f - vec2{1.0f, 1.0f};
    CHECK_EQ(result.x, doctest::Approx(2.0f));
    CHECK_EQ(result.y, doctest::Approx(1.0f));
}

TEST_CASE("length(vec2_like): returns the vector magnitude")
{
    CHECK_EQ(length(vec2{3.0f, 4.0f}), doctest::Approx(5.0f));
}

TEST_CASE("distance(vec2_like, vec2_like): returns the separation between vectors")
{
    CHECK_EQ(distance(vec2{1.0f, 1.0f}, vec2{4.0f, 5.0f}), doctest::Approx(5.0f));
}

TEST_CASE("normalized_or_zero(vec2): normalizes nonzero vectors and handles zero safely")
{
    const vec2 direction = normalized_or_zero(vec2{3.0f, 4.0f});
    CHECK_EQ(direction.x, doctest::Approx(0.6f));
    CHECK_EQ(direction.y, doctest::Approx(0.8f));

    const vec2 zero = normalized_or_zero(vec2{});
    CHECK_EQ(zero.x, doctest::Approx(0.0f));
    CHECK_EQ(zero.y, doctest::Approx(0.0f));
}

}} // namespace antbox
