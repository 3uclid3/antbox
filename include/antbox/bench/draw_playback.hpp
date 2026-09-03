#pragma once

#include <ant/env_fwd.hpp>

namespace antbox::sim {

struct clock;

} // namespace antbox::sim

namespace antbox::bench {

class playback;

struct draw_playback
{
    auto operator()(ant::env_of<const sim::clock, playback> env) const -> void;
};

} // namespace antbox::bench
