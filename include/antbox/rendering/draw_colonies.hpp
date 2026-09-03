#pragma once

#include <ant/env_fwd.hpp>
#include <ant/query_fwd.hpp>

namespace antbox {

struct colony;
struct color;
struct graphics_context;
struct position;

struct draw_colonies
{
    auto operator()(ant::env_of<graphics_context> ctx, ant::query_of<const colony, const color, const position> query) const -> void;
};

} // namespace antbox
