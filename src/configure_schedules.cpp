#include "configure_schedules.hpp"

#include <ant/scheduler.hpp>
#include <antbox/app/poll_input.hpp>
#include <antbox/app/poll_window.hpp>
#include <antbox/bench/draw_playback.hpp>
#include <antbox/render/begin_render.hpp>
#include <antbox/render/begin_render_rlimgui.hpp>
#include <antbox/render/begin_render_world.hpp>
#include <antbox/render/draw_ants.hpp>
#include <antbox/render/draw_colonies.hpp>
#include <antbox/render/end_render.hpp>
#include <antbox/render/end_render_rlimgui.hpp>
#include <antbox/render/end_render_world.hpp>
#include <antbox/render/update_camera.hpp>
#include <antbox/sim/ant/steer_home.hpp>
#include <antbox/sim/ant/wander.hpp>
#include <antbox/sim/motion/move.hpp>
#include <antbox/sim/time/advance_clock.hpp>

namespace antbox {

namespace {

auto configure_application_schedule(ant::scheduler& scheduler) -> void
{
    auto update = scheduler.stage<application_schedule::update>();

    auto window_system = update.add<poll_window>();
    auto input_system = update.add<poll_input>();

    update.add<update_camera>()
        .after(window_system)
        .after(input_system);

    scheduler.compile<application_schedule>();
}

auto configure_simulation_schedule(ant::scheduler& scheduler) -> void
{
    auto clock = scheduler.stage<simulation_schedule::clock>();
    clock.add<sim::advance_clock>();

    auto update = scheduler.stage<simulation_schedule::update>();
    auto wander_system = update.add<sim::wander>();
    auto home_system = update.add<sim::steer_home>();
    auto move_system = update.add<sim::move>();

    move_system.after(wander_system)
        .after(home_system);

    scheduler.compile<simulation_schedule>();
}

auto configure_rendering_schedule(ant::scheduler& scheduler) -> void
{
    // pre_render stage
    auto pre_render = scheduler.stage<rendering_schedule::pre_render>();
    pre_render.add<begin_render>();

    // pre_render_world stage
    auto pre_render_world = scheduler.stage<rendering_schedule::pre_render_world>();
    pre_render_world.add<begin_render_world>();

    // render_world stage
    auto render_world = scheduler.stage<rendering_schedule::render_world>();
    render_world.add<draw_colonies>();
    render_world.add<draw_ants>();

    // post_render_world stage
    auto post_render_world = scheduler.stage<rendering_schedule::post_render_world>();
    post_render_world.add<end_render_world>();

    // pre_render_imgui stage
    auto pre_render_imgui = scheduler.stage<rendering_schedule::pre_render_imgui>();
    pre_render_imgui.add<begin_render_rlimgui>();

    // render_imgui stage
    auto render_imgui = scheduler.stage<rendering_schedule::render_imgui>();
    render_imgui.add<bench::draw_playback>();

    // post_render_imgui stage
    auto post_render_imgui = scheduler.stage<rendering_schedule::post_render_imgui>();
    post_render_imgui.add<end_render_rlimgui>();

    // post_render stage
    auto post_render = scheduler.stage<rendering_schedule::post_render>();
    post_render.add<end_render>();

    scheduler.compile<rendering_schedule>();
}

} // namespace

auto configure_schedules(ant::scheduler& scheduler) -> void
{
    configure_application_schedule(scheduler);
    configure_simulation_schedule(scheduler);
    configure_rendering_schedule(scheduler);
}

} // namespace antbox
