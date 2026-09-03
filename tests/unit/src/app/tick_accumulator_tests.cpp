#include <antbox/app/tick_accumulator.hpp>
#include <doctest/doctest.h>

#include <chrono>

namespace antbox { namespace {

TEST_CASE("tick_accumulator: produces one tick for each complete interval")
{
    tick_accumulator cadence{std::chrono::milliseconds{100}};

    cadence.update(std::chrono::milliseconds{250});

    CHECK(cadence.consume_tick());
    CHECK(cadence.consume_tick());
    CHECK_FALSE(cadence.consume_tick());
}

TEST_CASE("tick_accumulator: preserves incomplete intervals")
{
    tick_accumulator cadence{std::chrono::milliseconds{100}};

    cadence.update(std::chrono::milliseconds{60});
    CHECK_FALSE(cadence.consume_tick());

    cadence.update(std::chrono::milliseconds{40});
    CHECK(cadence.consume_tick());
    CHECK_FALSE(cadence.consume_tick());
}

}} // namespace antbox
