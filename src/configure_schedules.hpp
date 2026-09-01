#pragma once

#include <ant/scheduler/stage.hpp>

namespace ant { class scheduler; }

namespace antbox {

struct application_schedule
{
    using stage = ant::stage_of<application_schedule>;

    struct update : stage
    {};
};

struct simulation_schedule
{
    using stage = ant::stage_of<simulation_schedule>;

    struct clock : stage
    {};

    struct update : stage
    {};
};

struct rendering_schedule
{
    using stage = ant::stage_of<rendering_schedule>;

    struct pre_render : stage
    {};

    struct pre_render_world : stage
    {};

    struct render_world : stage
    {};

    struct post_render_world : stage
    {};

    struct pre_render_imgui : stage
    {};

    struct render_imgui : stage
    {};

    struct post_render_imgui : stage
    {};

    struct post_render : stage
    {};
};

auto configure_schedules(ant::scheduler& scheduler) -> void;

} // namespace antbox
