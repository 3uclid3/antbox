#include <antbox/bench/playback.hpp>

#include <utility>

namespace antbox::bench {

auto playback::speed_multiplier() const noexcept -> float
{
    switch (_speed)
    {
    case playback_speed::quarter:
        return 0.25f;
    case playback_speed::half:
        return 0.5f;
    case playback_speed::normal:
        return 1.0f;
    case playback_speed::double_speed:
        return 2.0f;
    case playback_speed::quadruple:
        return 4.0f;
    }

    std::unreachable();
}

auto playback::step() noexcept -> void
{
    _paused = true;
    _step_pending = true;
}

auto playback::consume_step() noexcept -> bool
{
    return std::exchange(_step_pending, false);
}

} // namespace antbox::bench
