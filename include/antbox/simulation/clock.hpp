#pragma once

#include <cstdint>

namespace antbox {

struct clock
{
    std::uint64_t tick{0};
    float delta{0.0f};
};

} // namespace antbox
