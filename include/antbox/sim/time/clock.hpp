#pragma once

#include <cstdint>

namespace antbox::sim {

struct clock
{
    std::uint64_t tick{0};
    float delta{0.0f};
};

} // namespace antbox::sim
