#include <antbox/rendering/rendering_schedule.hpp>

#include <ant/scheduler.hpp>
#include <antbox/graphics/begin_render.hpp>
#include <antbox/graphics/begin_render_rlimgui.hpp>
#include <antbox/graphics/begin_render_world.hpp>
#include <antbox/graphics/end_render.hpp>
#include <antbox/graphics/end_render_rlimgui.hpp>
#include <antbox/graphics/end_render_world.hpp>
#include <antbox/rendering/draw_ants.hpp>
#include <antbox/rendering/draw_colonies.hpp>

namespace antbox {

namespace {

template<typename T>
auto configure_stage(ant::scheduler& scheduler) -> void
{
    T::configure(scheduler.stage<rendering_schedule, T>());
}

struct pre_render_stage
{
    static auto configure(ant::scheduler::stage_handle stage) -> void
    {
        stage.add<begin_render>();
    }
};

struct pre_render_world_stage
{
    static auto configure(ant::scheduler::stage_handle stage) -> void
    {
        stage.add<begin_render_world>();
    }
};

struct render_world_stage
{
    static auto configure(ant::scheduler::stage_handle stage) -> void
    {
        stage.add<draw_colonies>();
        stage.add<draw_ants>();
    }
};

struct post_render_world_stage
{
    static auto configure(ant::scheduler::stage_handle stage) -> void
    {
        stage.add<end_render_world>();
    }
};

struct pre_render_imgui_stage
{
    static auto configure(ant::scheduler::stage_handle stage) -> void
    {
        stage.add<begin_render_rlimgui>();
    }
};

struct render_imgui_stage
{
    static auto configure(ant::scheduler::stage_handle) -> void
    {
    }
};

struct post_render_imgui_stage
{
    static auto configure(ant::scheduler::stage_handle stage) -> void
    {
        stage.add<end_render_rlimgui>();
    }
};

struct post_render_stage
{
    static auto configure(ant::scheduler::stage_handle stage) -> void
    {
        stage.add<end_render>();
    }
};

} // namespace

auto rendering_schedule::configure(ant::scheduler& scheduler) -> void
{
    configure_stage<pre_render_stage>(scheduler);

    configure_stage<pre_render_world_stage>(scheduler);
    configure_stage<render_world_stage>(scheduler);
    configure_stage<post_render_world_stage>(scheduler);

    configure_stage<pre_render_imgui_stage>(scheduler);
    configure_stage<render_imgui_stage>(scheduler);
    configure_stage<post_render_imgui_stage>(scheduler);

    configure_stage<post_render_stage>(scheduler);
}

} // namespace antbox
