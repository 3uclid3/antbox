#include <antbox/simulation/clock/advance_clock.hpp>

#include <ant/env.hpp>
#include <antbox/simulation/clock.hpp>

namespace antbox {

auto advance_clock::operator()(ant::env_of<clock> env) const -> void
{
    clock& c = env.get<clock>();
    ++c.tick;
}

} // namespace antbox
