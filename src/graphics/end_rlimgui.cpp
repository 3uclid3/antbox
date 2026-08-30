#include <antbox/graphics/end_rlimgui.hpp>

#include <rlImGui.h>

#include <ant/env.hpp>

namespace antbox {

auto end_rlimgui::operator()(ant::env_of<graphics_context>) const -> void
{
    rlImGuiEnd();
}

} // namespace antbox
