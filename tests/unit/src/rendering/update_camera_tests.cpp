#include <antbox/rendering/update_camera.hpp>
#include <doctest/doctest.h>

#include <antbox/application/application_clock.hpp>
#include <antbox/application/input/input.hpp>
#include <antbox/graphics/graphics_context.hpp>
#include <antbox/rendering/camera.hpp>

#include "../system_fixture.hpp"

namespace antbox { namespace {

struct test_schedule;
struct test_stage;

auto make_schema() -> ant::schema
{
    return ant::schema::builder()
        .define<camera>()
        .define<application_clock>()
        .define<graphics_context>()
        .define<input>()
        .build();
}

struct fixture : system_fixture
{
    fixture()
        : system_fixture(make_schema())
    {
    }

    auto execute(camera cam, application_clock app_clock, input in, graphics_context graphics = {}) -> const camera&
    {
        set_env(cam, app_clock, graphics, in);
        scheduler.stage<test_schedule, test_stage>().add<update_camera>();
        scheduler.compile<test_schedule>();
        scheduler.execute<test_schedule>();

        return database.env_of<const camera>().get<camera>();
    }
};

TEST_CASE_FIXTURE(fixture, "update_camera: tracks a middle-mouse drag in screen space")
{
    input in;
    in.mouse.middle.down = true;
    in.mouse.delta = {10.0f, -6.0f};

    camera cam;
    cam.zoom = 2.0f;
    cam.target_zoom = 2.0f;

    const camera& result = execute(cam, application_clock{.delta = 1.0f / 60.0f}, in);

    CHECK_EQ(result.target.x, doctest::Approx(-5.0f));
    CHECK_EQ(result.target.y, doctest::Approx(3.0f));
}

TEST_CASE_FIXTURE(fixture, "update_camera: eases keyboard movement and wheel zoom")
{
    input in;
    in.keyboard.arrow_right.down = true;
    in.mouse.wheel = 1.0f;
    in.mouse.position = {640.0f, 400.0f};

    const camera& result = execute(
        camera{},
        application_clock{.delta = 1.0f / 60.0f},
        in,
        graphics_context{.viewport_size = {1280.0f, 800.0f}});

    CHECK(result.target.x > 0.0f);
    CHECK_EQ(result.target.y, doctest::Approx(0.0f));
    CHECK(result.pan_velocity.x > 0.0f);
    CHECK(result.pan_velocity.x < 600.0f);
    CHECK(result.zoom > 1.0f);
    CHECK(result.zoom < 1.2f);
    CHECK_EQ(result.target_zoom, doctest::Approx(1.2f));
}

TEST_CASE_FIXTURE(fixture, "update_camera: keeps the world beneath the cursor fixed while zooming")
{
    input in;
    in.mouse.position = {750.0f, 400.0f};
    in.mouse.wheel = 1.0f;

    const camera& result = execute(
        camera{},
        application_clock{.delta = 1.0f / 60.0f},
        in,
        graphics_context{.viewport_size = {1000.0f, 800.0f}});

    const float world_before = 250.0f;
    const float world_after = result.target.x + 250.0f / result.zoom;
    CHECK_EQ(world_after, doctest::Approx(world_before));
    CHECK_EQ(result.target.y, doctest::Approx(0.0f));
}

}} // namespace antbox
