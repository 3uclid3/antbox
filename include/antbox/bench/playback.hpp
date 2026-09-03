#pragma once

namespace antbox::bench {

enum class playback_speed
{
    quarter,
    half,
    normal,
    double_speed,
    quadruple,
};

class playback
{
public:
    auto paused() const noexcept -> bool { return _paused; }
    auto speed() const noexcept -> playback_speed { return _speed; }
    auto speed_multiplier() const noexcept -> float;

    auto toggle() noexcept -> void { _paused = !_paused; }
    auto set_speed(playback_speed speed) noexcept -> void { _speed = speed; }
    auto step() noexcept -> void;
    auto consume_step() noexcept -> bool;

private:
    bool _paused{};
    bool _step_pending{};
    playback_speed _speed{playback_speed::normal};
};

} // namespace antbox::bench
