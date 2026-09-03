#include <antbox/render/begin_render.hpp>

#include <raylib.h>

#include <ant/env.hpp>
#include <antbox/render/graphics_context.hpp>

namespace antbox {

auto begin_render::operator()(ant::env_of<graphics_context> env) const -> void
{
    BeginDrawing();
    ClearBackground(env.get<graphics_context>().clear_color);
}

} // namespace antbox
