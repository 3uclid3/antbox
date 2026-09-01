#pragma once

#include <antbox/graphics/color.hpp>
#include <antbox/math/vec2.hpp>

namespace antbox {

struct graphics_context
{
    color clear_color{color::black};
    vec2 viewport_size{};
};

} // namespace antbox
