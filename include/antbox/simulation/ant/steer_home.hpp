#pragma once

#include <ant/query_fwd.hpp>

namespace antbox {

struct colony;
struct colony_member;
struct position;
struct steering;

struct steer_home
{
    using ants_query_signature = ant::query_signature<const position, const colony_member, steering>;
    using colonies_query_signature = ant::query_signature<const position, const colony>;

    auto operator()(ant::query<ants_query_signature> ants, ant::query<colonies_query_signature> colonies) const -> void;
};

} // namespace antbox
