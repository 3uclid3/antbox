#include <antbox/bench/format_tick_count.hpp>
#include <doctest/doctest.h>

namespace antbox::bench { namespace {

TEST_CASE("format_tick_count: leaves small counts unabridged")
{
    CHECK_EQ(format_tick_count(0U), "0");
    CHECK_EQ(format_tick_count(9'999U), "9,999");
}

TEST_CASE("format_tick_count: abbreviates large counts")
{
    CHECK_EQ(format_tick_count(10'000U), "10k");
    CHECK_EQ(format_tick_count(12'500U), "12.5k");
    CHECK_EQ(format_tick_count(1'000'000U), "1M");
    CHECK_EQ(format_tick_count(1'250'000U), "1.3M");
    CHECK_EQ(format_tick_count(1'000'000'000U), "1B");
}

TEST_CASE("format_tick_count: promotes values that round into the next unit")
{
    CHECK_EQ(format_tick_count(999'499U), "999k");
    CHECK_EQ(format_tick_count(999'500U), "1M");
    CHECK_EQ(format_tick_count(999'500'000U), "1B");
}

TEST_CASE("format_tick_count_exact: groups thousands")
{
    CHECK_EQ(format_tick_count_exact(1'234'567'890U), "1,234,567,890");
}

}} // namespace antbox::bench
