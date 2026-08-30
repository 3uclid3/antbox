#pragma once

#include <ant/env/fwd.hpp>
#include <ant/query/fwd.hpp>

namespace antbox {

struct clock;
struct position;
struct steering;
struct velocity;

struct move
{
    auto operator()(ant::env_of<const clock> env, ant::query_of<position, velocity, const steering> query) const -> void;
};

} // namespace antbox
