#include <antbox/render/begin_render_world.hpp>

#include <raylib.h>

#include <ant/env.hpp>
#include <antbox/render/camera.hpp>
#include <antbox/render/graphics_context.hpp>

namespace antbox {

auto begin_render_world::operator()(ant::env_of<const camera, const graphics_context> env) const -> void
{
    const camera& cam = env.get<camera>();
    const graphics_context& graphics = env.get<graphics_context>();

    Camera2D rl_cam{
        .offset = {
            graphics.viewport_size.x * 0.5f,
            graphics.viewport_size.y * 0.5f,
        },
        .target = vec2_cast<Vector2>(cam.target),
        .rotation = 0.0f,
        .zoom = cam.zoom,
    };

    BeginMode2D(rl_cam);
}

} // namespace antbox
