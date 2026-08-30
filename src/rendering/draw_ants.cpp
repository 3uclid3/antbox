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
    const Vector2 origin{GetScreenWidth() * 0.5F, GetScreenHeight() * 0.5F};
    for (auto ant : ants)
    {
        const auto& pos = ant.get<position>();
        const auto& vel = ant.get<velocity>();
        const auto home = colonies.row(ant.get<colony_member>().entity);
        const color tint = home ? home->get<color>() : color::white;
        const float heading = std::atan2(vel.y, vel.x);
        const Vector2 center{origin.x + pos.x, origin.y + pos.y};
        const Vector2 nose{center.x + std::cos(heading) * 6.0F, center.y + std::sin(heading) * 6.0F};
        DrawLineEx(center, nose, 2.0F, tint);
        DrawCircleV(center, 2.3F, tint);
    }
}

} // namespace antbox
