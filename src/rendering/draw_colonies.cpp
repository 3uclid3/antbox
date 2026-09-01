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
    for (auto row : query)
    {
        const auto& home = row.get<colony>();
        const auto& tint = row.get<color>();
        const auto& pos = row.get<position>();
        const Vector2 center = vec2_cast<Vector2>(pos.vec);
        DrawCircleLinesV(center, home.range, Fade(tint, 0.22F));
        DrawCircleV(center, 9.0F, tint);
        DrawCircleLinesV(center, 13.0F, Fade(tint, 0.55F));
    }
}

} // namespace antbox
