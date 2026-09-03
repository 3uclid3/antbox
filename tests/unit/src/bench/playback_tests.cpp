#include <antbox/bench/playback.hpp>
#include <doctest/doctest.h>

namespace antbox::bench { namespace {

TEST_CASE("playback::ctor: starts running at normal speed")
{
    const playback controls;

    CHECK_FALSE(controls.paused());
    CHECK_EQ(controls.speed(), playback_speed::normal);
    CHECK_EQ(controls.speed_multiplier(), doctest::Approx(1.0f));
}

TEST_CASE("playback::toggle: toggles between running and paused")
{
    playback controls;

    controls.toggle();
    CHECK(controls.paused());

    controls.toggle();
    CHECK_FALSE(controls.paused());
}

TEST_CASE("playback::step: pauses and produces exactly one request")
{
    playback controls;

    controls.step();

    CHECK(controls.paused());
    CHECK(controls.consume_step());
    CHECK_FALSE(controls.consume_step());
}

TEST_CASE("playback::set_speed: changes simulation speed")
{
    playback controls;

    controls.set_speed(playback_speed::double_speed);

    CHECK_EQ(controls.speed(), playback_speed::double_speed);
}

TEST_CASE("playback::speed_multiplier: converts supported speeds to rates")
{
    playback controls;

    controls.set_speed(playback_speed::quarter);
    CHECK_EQ(controls.speed_multiplier(), doctest::Approx(0.25f));

    controls.set_speed(playback_speed::half);
    CHECK_EQ(controls.speed_multiplier(), doctest::Approx(0.5f));

    controls.set_speed(playback_speed::normal);
    CHECK_EQ(controls.speed_multiplier(), doctest::Approx(1.0f));

    controls.set_speed(playback_speed::double_speed);
    CHECK_EQ(controls.speed_multiplier(), doctest::Approx(2.0f));

    controls.set_speed(playback_speed::quadruple);
    CHECK_EQ(controls.speed_multiplier(), doctest::Approx(4.0f));
}

}} // namespace antbox::bench
