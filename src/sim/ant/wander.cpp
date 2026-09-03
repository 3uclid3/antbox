#include <antbox/sim/ant/wander.hpp>

#include <cstdint>

#include <ant/env.hpp>
#include <ant/query.hpp>
#include <antbox/sim/motion/steering.hpp>
#include <antbox/sim/time/clock.hpp>

namespace antbox::sim {
namespace {

auto noise(std::uint64_t value) noexcept -> float
{
    value ^= value >> 30U;
    value *= 0xBF58476D1CE4E5B9ULL;
    value ^= value >> 27U;
    value *= 0x94D049BB133111EBULL;
    value ^= value >> 31U;
    return static_cast<float>(value & 0xFFFFU) / 32767.5F - 1.0F;
}

} // namespace

auto wander::operator()(ant::env_of<const clock> env, ant::query_of<steering> query) const -> void
{
    const std::uint64_t tick = env.get<clock>().tick;
    for (auto row : query)
    {
        const auto entity = static_cast<std::uint64_t>(row.entity());
        auto& steer = row.get<steering>();
        steer.x = noise(entity ^ (tick * 2U)) * 45.0F;
        steer.y = noise(entity ^ (tick * 2U + 1U)) * 45.0F;
    }
}

} // namespace antbox::sim
