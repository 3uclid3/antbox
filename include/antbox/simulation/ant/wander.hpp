#pragma once

#include <ant/env_fwd.hpp>
#include <ant/query_fwd.hpp>

namespace antbox {

struct clock;
struct steering;

struct wander
{
    auto operator()(ant::env_of<const clock> env, ant::query_of<steering> query) const -> void;
};

} // namespace antbox
