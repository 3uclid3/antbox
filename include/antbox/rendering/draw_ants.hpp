#pragma once

#include <ant/env/fwd.hpp>
#include <ant/query/fwd.hpp>

namespace antbox {

struct colony;
struct colony_member;
struct color;
struct graphics_context;
struct position;
struct velocity;

struct draw_ants
{
    using ants_query_signature = ant::query_signature<const position, const velocity, const colony_member>;
    using colonies_query_signature = ant::query_signature<const colony, const color>;

    auto operator()(ant::env_of<graphics_context> ctx, ant::query<ants_query_signature> ants, ant::query<colonies_query_signature> colonies) const -> void;
};

} // namespace antbox
