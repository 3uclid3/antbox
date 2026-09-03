#include <antbox/sim/time/advance_clock.hpp>

#include <ant/env.hpp>
#include <antbox/sim/time/clock.hpp>

namespace antbox::sim {

auto advance_clock::operator()(ant::env_of<clock> env) const -> void
{
    clock& c = env.get<clock>();
    ++c.tick;
}

} // namespace antbox::sim
