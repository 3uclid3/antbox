#pragma once

#include <ant/env/fwd.hpp>

namespace antbox {

struct graphics_context;

struct poll_window
{
    auto operator()(ant::env_of<graphics_context> env) const -> void;
};

} // namespace antbox
