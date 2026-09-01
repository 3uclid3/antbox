#include <antbox/graphics/end_render_rlimgui.hpp>

#include <rlImGui.h>

#include <ant/env.hpp>

namespace antbox {

auto end_render_rlimgui::operator()(ant::env_of<graphics_context>) const -> void
{
    rlImGuiEnd();
}

} // namespace antbox
