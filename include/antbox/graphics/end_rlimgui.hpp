#pragma once

#include <ant/env/fwd.hpp>

namespace antbox {

struct graphics_context;

struct end_rlimgui
{
    auto operator()(ant::env_of<graphics_context> ctx) const -> void;
};

} // namespace antbox
