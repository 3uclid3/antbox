#include <antbox/graphics/begin_render_rlimgui.hpp>

#include <rlImGui.h>

#include <ant/env.hpp>

namespace antbox {

auto begin_render_rlimgui::operator()(ant::env_of<graphics_context>) const -> void
{
    rlImGuiBegin();
}

} // namespace antbox
