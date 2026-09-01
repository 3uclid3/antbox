#pragma once

#include <ant/env/fwd.hpp>

namespace antbox {

struct clock;
class playback;

struct draw_playback
{
    auto operator()(ant::env_of<const clock, playback> env) const -> void;
};

} // namespace antbox
