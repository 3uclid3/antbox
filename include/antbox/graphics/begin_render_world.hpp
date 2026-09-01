#pragma once

#include <ant/env/fwd.hpp>

namespace antbox {

struct camera;
struct graphics_context;

struct begin_render_world
{
    auto operator()(ant::env_of<const camera, const graphics_context> env) const -> void;
};

} // namespace antbox
