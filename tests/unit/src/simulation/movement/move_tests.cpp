#include <antbox/simulation/movement/move.hpp>
#include <doctest/doctest.h>

#include <antbox/simulation/clock.hpp>
#include <antbox/simulation/movement/position.hpp>
#include <antbox/simulation/movement/steering.hpp>
#include <antbox/simulation/movement/velocity.hpp>

#include "../../system_fixture.hpp"

namespace antbox { namespace {

struct test_schedule;
struct test_stage;

auto make_schema() -> ant::schema
{
    return ant::schema::builder()
        .define<clock>()
        .define<position>()
        .define<velocity>()
        .define<steering>()
        .build();
}

struct fixture : system_fixture
{
    fixture()
        : system_fixture(make_schema())
    {
    }
};

TEST_CASE_FIXTURE(fixture, "move: applies steering and advances position")
{
    set_env<clock>(clock{.delta = 0.5F});

    const ant::entity entity = create_entity<position, velocity, steering>(position(0.0F, 0.0F), velocity(10.0F, 0.0F), steering(0.0F, 10.0F));

    scheduler.stage<test_schedule, test_stage>().add<move>();
    scheduler.compile<test_schedule>();
    scheduler.execute<test_schedule>();

    ant::compiled_query cq = database.compile_query<ant::query_signature<const position>>();
    ant::query q = cq.query();

    std::optional result = q.row(entity);
    REQUIRE(result.has_value());

    CHECK(result->get<position>().vec.x > 0.0F);
    CHECK(result->get<position>().vec.y > 0.0F);
}

}} // namespace antbox
