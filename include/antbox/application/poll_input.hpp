#pragma once

#include <ant/env/fwd.hpp>

namespace antbox {

struct input;

struct poll_input
{
    auto operator()(ant::env_of<input> env) const -> void;
};

} // namespace antbox
