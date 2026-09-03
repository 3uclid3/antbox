#pragma once

#include <ant/env_fwd.hpp>

namespace antbox {

struct camera;
struct application_clock;
struct graphics_context;
struct input;

struct update_camera
{
    auto operator()(ant::env_of<camera, const graphics_context, const application_clock, const input> env) const -> void;
};

} // namespace antbox
