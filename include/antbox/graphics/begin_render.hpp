#pragma once

#include <ant/env_fwd.hpp>

namespace antbox {

struct graphics_context;

struct begin_render
{
    auto operator()(ant::env_of<graphics_context> ctx) const -> void;
};

} // namespace antbox
