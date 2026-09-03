#include <antbox/sim/ant/steer_home.hpp>
#include <doctest/doctest.h>

#include <ant.testing/schema.hpp>
#include <ant.testing/system.hpp>
#include <antbox/sim/ant/colony_member.hpp>
#include <antbox/sim/colony/colony.hpp>
#include <antbox/sim/motion/position.hpp>
#include <antbox/sim/motion/steering.hpp>
#include <antbox/sim/time/clock.hpp>

namespace antbox::sim { namespace {

struct fixture : ant::testing::system_fixture
{
    fixture()
        : system_fixture(ant::testing::make_schema<
                         clock,
                         position,
                         steering,
                         colony_member,
                         colony>())
    {
    }
};

TEST_CASE_FIXTURE(fixture, "steer_home: adds an inward influence near the colony edge")
{
    const ant::entity home = create_entity<position, colony>(
        position(0.0f, 0.0f),
        colony(100.0f));

    const ant::entity ant = create_entity<position, steering, colony_member>(
        position(90.0f, 0.0f),
        steering(0.0f, 0.0f),
        colony_member(home));

    execute<steer_home>();

    const steering* result = get<steering>(ant);
    REQUIRE_NE(result, nullptr);

    CHECK_LT(result->x, 0.0f);
    CHECK_EQ(result->y, doctest::Approx(0.0f));
}

}} // namespace antbox::sim
