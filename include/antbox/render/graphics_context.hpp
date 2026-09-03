#pragma once

#include <antbox/core/vec2.hpp>
#include <antbox/render/color.hpp>

namespace antbox {

struct graphics_context
{
    color clear_color{color::black};
    vec2 viewport_size{};
};

} // namespace antbox
