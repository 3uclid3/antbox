#pragma once

#include <antbox/core/vec2.hpp>

namespace antbox::sim {

struct position
{
    vec2 vec{};

    constexpr position() noexcept = default;
    constexpr position(float x, float y) noexcept
        : vec{x, y}
    {
    }
};

} // namespace antbox::sim
