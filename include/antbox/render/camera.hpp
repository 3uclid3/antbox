#pragma once

#include <antbox/core/vec2.hpp>

namespace antbox {

struct camera
{
    vec2 target{0.0f, 0.0f};
    vec2 pan_velocity{};
    vec2 zoom_anchor{};
    float zoom{1.0f};
    float target_zoom{1.0f};
};

} // namespace antbox
