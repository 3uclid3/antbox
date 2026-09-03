#include <antbox/application/tick_accumulator.hpp>

#include <ant/detail/core/assert.hpp>

namespace antbox {

tick_accumulator::tick_accumulator(chrono::clock::duration target_delta) noexcept
    : _target_delta(target_delta)
{
    ANT_ASSERT(_target_delta > chrono::clock::duration::zero());
}

auto tick_accumulator::update(chrono::clock::duration elapsed) -> void
{
    _accumulator += elapsed;
}

auto tick_accumulator::consume_tick() -> bool
{
    if (_accumulator < _target_delta)
        return false;

    _accumulator -= _target_delta;
    return true;
}

} // namespace antbox
