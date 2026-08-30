#pragma once

#include <ant/env/fwd.hpp>

namespace antbox {

struct clock;

struct advance_clock
{
    auto operator()(ant::env_of<clock> env) const -> void;
};

} // namespace antbox
