#include <antbox/simulation/ant/steer_home.hpp>
#include <doctest/doctest.h>

#include <antbox/simulation/ant/colony_member.hpp>
#include <antbox/simulation/clock.hpp>
#include <antbox/simulation/colony/colony.hpp>
#include <antbox/simulation/movement/position.hpp>
#include <antbox/simulation/movement/steering.hpp>

#include "../../system_fixture.hpp"

namespace antbox { namespace {

struct test_schedule;
struct test_stage;

auto make_schema() -> ant::schema
{
    return ant::schema::builder()
        .define<clock>()
        .define<position>()
        .define<steering>()
        .define<colony_member>()
        .define<colony>()
        .build();
}

struct fixture : system_fixture
{
    fixture()
        : system_fixture(make_schema())
    {
    }
};

TEST_CASE_FIXTURE(fixture, "steer_home: adds an inward influence near the colony edge")
{
    const ant::entity home = create_entity<position, colony>(
        position(0.0F, 0.0F),
        colony(100.0F));

    const ant::entity ant = create_entity<position, steering, colony_member>(
        position(90.0F, 0.0F),
        steering(0.0F, 0.0F),
        colony_member(home));

    scheduler.stage<test_schedule, test_stage>().add<steer_home>();
    scheduler.compile<test_schedule>();
    scheduler.execute<test_schedule>();

    ant::compiled_query cq = database.compile_query_of<const steering>();
    ant::query q = cq.query();

    std::optional result = q.row(ant);
    REQUIRE(result.has_value());

    CHECK(result->get<steering>().x < 0.0F);
    CHECK_EQ(result->get<steering>().y, doctest::Approx(0.0F));
}

}} // namespace antbox
