#include <antbox/app/poll_window.hpp>

#include <raylib.h>

#include <ant/env.hpp>
#include <antbox/render/graphics_context.hpp>

namespace antbox {

auto poll_window::operator()(ant::env_of<graphics_context> env) const -> void
{
    env.get<graphics_context>().viewport_size = {
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight()),
    };
}

} // namespace antbox
