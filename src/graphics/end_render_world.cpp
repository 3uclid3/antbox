#include <antbox/graphics/end_render_world.hpp>

#include <raylib.h>

#include <ant/env.hpp>

namespace antbox {

auto end_render_world::operator()(ant::env_of<graphics_context>) const -> void
{
    EndMode2D();
}

} // namespace antbox
