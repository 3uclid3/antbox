#pragma once

#include <ant/env_fwd.hpp>

namespace antbox::sim {

struct clock;

struct advance_clock
{
    auto operator()(ant::env_of<clock> env) const -> void;
};

} // namespace antbox::sim
