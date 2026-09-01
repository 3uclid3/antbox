#include <antbox/simulation/movement/move.hpp>
#include <doctest/doctest.h>

#include <ant.testing/schema.hpp>
#include <ant.testing/system.hpp>
#include <antbox/simulation/clock.hpp>
#include <antbox/simulation/movement/position.hpp>
#include <antbox/simulation/movement/steering.hpp>
#include <antbox/simulation/movement/velocity.hpp>

namespace antbox { namespace {

struct fixture : ant::testing::system_fixture
{
    fixture()
        : system_fixture(ant::testing::make_schema<clock, position, velocity, steering>())
    {
    }
};

TEST_CASE_FIXTURE(fixture, "move: applies steering and advances position")
{
    set_env<clock>(clock{.delta = 0.5F});

    const ant::entity entity = create_entity<position, velocity, steering>(position(0.0f, 0.0f), velocity(10.0f, 0.0f), steering(0.0f, 10.0f));

    execute<move>();

    const position* pos = get<position>(entity);
    REQUIRE_NE(pos, nullptr);

    CHECK_GT(pos->vec.x, 0.0f);
    CHECK_GT(pos->vec.y, 0.0f);
}

}} // namespace antbox
