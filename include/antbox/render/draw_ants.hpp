#pragma once

#include <ant/env_fwd.hpp>
#include <ant/query_fwd.hpp>

namespace antbox::sim {

struct colony;
struct colony_member;
struct position;
struct velocity;

} // namespace antbox::sim

namespace antbox {

struct color;
struct graphics_context;

struct draw_ants
{
    using ants_query_signature = ant::query_signature<const sim::position, const sim::velocity, const sim::colony_member>;
    using colonies_query_signature = ant::query_signature<const sim::colony, const color>;

    auto operator()(ant::env_of<graphics_context> ctx, ant::query<ants_query_signature> ants, ant::query<colonies_query_signature> colonies) const -> void;
};

} // namespace antbox
