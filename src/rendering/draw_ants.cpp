#include <antbox/rendering/draw_ants.hpp>

#include <cmath>

#include <raylib.h>

#include <ant/env.hpp>
#include <ant/query.hpp>
#include <antbox/graphics/color.hpp>
#include <antbox/simulation/ant/colony_member.hpp>
#include <antbox/simulation/colony/colony.hpp>
#include <antbox/simulation/movement/position.hpp>
#include <antbox/simulation/movement/velocity.hpp>

namespace antbox {

auto draw_ants::operator()(ant::env_of<graphics_context>, ant::query<ants_query_signature> ants, ant::query<colonies_query_signature> colonies) const -> void
{
    for (auto ant : ants)
    {
        const auto& pos = ant.get<position>();
        const auto& vel = ant.get<velocity>();
        const auto home = colonies.row(ant.get<colony_member>().entity);
        const color tint = home ? home->get<color>() : color::white;
        const float heading = std::atan2(vel.y, vel.x);
        const Vector2 center = vec2_cast<Vector2>(pos.vec);
        const Vector2 nose{center.x + std::cos(heading) * 6.0f, center.y + std::sin(heading) * 6.0f};
        DrawLineEx(center, nose, 2.0f, tint);
        DrawCircleV(center, 2.3f, tint);
    }
}

} // namespace antbox
