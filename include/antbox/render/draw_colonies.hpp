#pragma once

#include <ant/env_fwd.hpp>
#include <ant/query_fwd.hpp>

namespace antbox::sim {

struct colony;
struct position;

} // namespace antbox::sim

namespace antbox {

struct color;
struct graphics_context;

struct draw_colonies
{
    auto operator()(ant::env_of<graphics_context> ctx, ant::query_of<const sim::colony, const color, const sim::position> query) const -> void;
};

} // namespace antbox
