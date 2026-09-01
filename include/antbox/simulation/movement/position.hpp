#pragma once

#include <antbox/math/vec2.hpp>

namespace antbox {

struct position
{
    vec2 vec{};

    constexpr position() noexcept = default;
    constexpr position(float x, float y) noexcept
        : vec{x, y}
    {
    }
};

} // namespace antbox
