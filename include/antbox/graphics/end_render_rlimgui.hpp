#pragma once

#include <ant/env_fwd.hpp>

namespace antbox {

struct graphics_context;

struct end_render_rlimgui
{
    auto operator()(ant::env_of<graphics_context> ctx) const -> void;
};

} // namespace antbox
