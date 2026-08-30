#include <antbox/rendering/draw_colonies.hpp>

#include <raylib.h>

#include <ant/env.hpp>
#include <ant/query.hpp>
#include <antbox/graphics/color.hpp>
#include <antbox/simulation/colony/colony.hpp>
#include <antbox/simulation/movement/position.hpp>

namespace antbox {

auto draw_colonies::operator()(ant::env_of<graphics_context>, ant::query_of<const colony, const color, const position> query) const -> void
{
    const Vector2 origin{GetScreenWidth() * 0.5F, GetScreenHeight() * 0.5F};
    for (auto row : query)
    {
        const auto& home = row.get<colony>();
        const auto& tint = row.get<color>();
        const auto& pos = row.get<position>();
        const Vector2 center{origin.x + pos.x, origin.y + pos.y};
        DrawCircleLinesV(center, home.range, Fade(tint, 0.22F));
        DrawCircleV(center, 9.0F, tint);
        DrawCircleLinesV(center, 13.0F, Fade(tint, 0.55F));
    }
}

} // namespace antbox
