#include <antbox/graphics/end_render.hpp>

#include <raylib.h>

#include <ant/env.hpp>

namespace antbox {

auto end_render::operator()(ant::env_of<graphics_context>) const -> void
{
    EndDrawing();
}

} // namespace antbox
