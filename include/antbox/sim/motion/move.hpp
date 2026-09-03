#pragma once

#include <ant/env_fwd.hpp>
#include <ant/query_fwd.hpp>

namespace antbox::sim {

struct clock;
struct position;
struct steering;
struct velocity;

struct move
{
    auto operator()(ant::env_of<const clock> env, ant::query_of<position, velocity, const steering> query) const -> void;
};

} // namespace antbox::sim
