#include <antbox/render/update_camera.hpp>
#include <doctest/doctest.h>

#include <ant.testing/schema.hpp>
#include <ant.testing/system.hpp>
#include <antbox/app/application_clock.hpp>
#include <antbox/app/input.hpp>
#include <antbox/render/camera.hpp>
#include <antbox/render/graphics_context.hpp>

namespace antbox { namespace {

struct fixture : ant::testing::system_fixture
{
    fixture()
        : system_fixture(ant::testing::make_schema<
                         camera,
                         application_clock,
                         graphics_context,
                         input>())
    {
        set_env(camera{}, application_clock{.delta = 1.0f / 60.0f}, input{}, graphics_context{.viewport_size = {1280.0f, 800.0f}});
    }
};

TEST_CASE_FIXTURE(fixture, "update_camera: tracks a middle-mouse drag in screen space")
{
    camera& cam = get_env<camera>();
    cam.zoom = 2.0f;
    cam.target_zoom = 2.0f;

    input& in = get_env<input>();
    in.mouse.middle.down = true;
    in.mouse.delta = {10.0f, -6.0f};

    execute<update_camera>();

    CHECK_EQ(cam.target.x, doctest::Approx(-5.0f));
    CHECK_EQ(cam.target.y, doctest::Approx(3.0f));
}

TEST_CASE_FIXTURE(fixture, "update_camera: eases keyboard movement and wheel zoom")
{
    input& in = get_env<input>();
    in.keyboard.arrow_right.down = true;
    in.mouse.wheel = 1.0f;
    in.mouse.position = {640.0f, 400.0f};

    execute<update_camera>();

    camera& cam = get_env<camera>();

    CHECK_GT(cam.target.x, 0.0f);
    CHECK_EQ(cam.target.y, doctest::Approx(0.0f));
    CHECK_GT(cam.pan_velocity.x, 0.0f);
    CHECK_LT(cam.pan_velocity.x, 600.0f);
    CHECK_GT(cam.zoom, 1.0f);
    CHECK_LT(cam.zoom, 1.2f);
    CHECK_EQ(cam.target_zoom, doctest::Approx(1.2f));
}

TEST_CASE_FIXTURE(fixture, "update_camera: keeps the world beneath the cursor fixed while zooming")
{
    get_env<graphics_context>().viewport_size = {1000.0f, 800.0f};

    input& in = get_env<input>();
    in.mouse.position = {750.0f, 400.0f};
    in.mouse.wheel = 1.0f;

    execute<update_camera>();

    camera& cam = get_env<camera>();

    const float world_before = 250.0f;
    const float world_after = cam.target.x + 250.0f / cam.zoom;

    CHECK_EQ(world_after, doctest::Approx(world_before));
    CHECK_EQ(cam.target.y, doctest::Approx(0.0f));
}

}} // namespace antbox
