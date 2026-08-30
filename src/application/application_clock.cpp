#include <antbox/application/application_clock.hpp>

namespace antbox {

auto application_clock::update() -> void
{
    const chrono::clock::time_point now = chrono::clock::now();

    _delta = now - _last_tick;
    _last_tick = now;
}

} // namespace antbox
