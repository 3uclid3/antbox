#include <antbox/render/update_camera.hpp>

#include <algorithm>
#include <cmath>

#include <ant/env.hpp>
#include <antbox/app/application_clock.hpp>
#include <antbox/app/input.hpp>
#include <antbox/render/camera.hpp>
#include <antbox/render/graphics_context.hpp>

namespace antbox {

auto update_camera::operator()(ant::env_of<camera, const graphics_context, const application_clock, const input> env) const -> void
{
    constexpr float pan_speed = 600.0f;
    constexpr float pan_response = 14.0f;
    constexpr float zoom_step = 1.2f;
    constexpr float zoom_response = 18.0f;
    constexpr float min_zoom = 0.1f;
    constexpr float max_zoom = 8.0f;

    const float delta = std::clamp(env.get<application_clock>().delta, 0.0f, 0.1f);
    const graphics_context& graphics = env.get<graphics_context>();
    const input& in = env.get<input>();
    camera& cam = env.get<camera>();

    vec2 direction{
        static_cast<float>(in.keyboard.arrow_right.down) - static_cast<float>(in.keyboard.arrow_left.down),
        static_cast<float>(in.keyboard.arrow_down.down) - static_cast<float>(in.keyboard.arrow_up.down),
    };

    // Normalize diagonal input so holding two arrows is not faster than holding one.
    direction = normalized_or_zero(direction);

    const vec2 desired_velocity = direction * pan_speed;

    // Exponential smoothing makes the response independent of frame rate. The
    // response value controls how quickly velocity approaches its desired value.
    const float pan_blend = 1.0f - std::exp(-pan_response * delta);
    cam.pan_velocity += (desired_velocity - cam.pan_velocity) * pan_blend;

    // pan_velocity is measured in screen pixels per second. Dividing by zoom
    // converts it to world units while preserving the same apparent screen speed.
    cam.target += cam.pan_velocity * (delta / cam.zoom);

    if (in.mouse.middle.down)
    {
        // Moving the camera target opposite the drag makes the world follow the
        // cursor. Mouse delta is also in screen pixels, so convert it by zoom.
        cam.target -= in.mouse.delta / cam.zoom;
    }

    if (in.mouse.wheel != 0.0f)
    {
        // Multiplication gives each wheel notch the same proportional effect at
        // every zoom level. Remember where this zoom gesture started so easing
        // remains anchored there even if the cursor moves afterward.
        cam.target_zoom = std::clamp(cam.target_zoom * std::pow(zoom_step, in.mouse.wheel), min_zoom, max_zoom);
        cam.zoom_anchor = in.mouse.position;
    }

    // Ease the visible zoom toward the requested zoom using the same
    // frame-rate-independent exponential response as keyboard panning.
    const float zoom_blend = 1.0f - std::exp(-zoom_response * delta);
    const float previous_zoom = cam.zoom;
    cam.zoom += (cam.target_zoom - cam.zoom) * zoom_blend;

    // Camera2D places cam.target at the viewport center. This is the zoom
    // anchor's displacement from that center in screen pixels.
    const vec2 anchor_offset = cam.zoom_anchor - graphics.viewport_size * 0.5f;

    // A screen displacement corresponds to screen / zoom world units. Move the
    // target by the difference between the old and new world displacements so
    // the world point beneath the anchor remains at the same screen position.
    cam.target += anchor_offset * (1.0f / previous_zoom - 1.0f / cam.zoom);
}

} // namespace antbox
