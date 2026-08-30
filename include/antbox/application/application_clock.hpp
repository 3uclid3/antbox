#pragma once

#include <antbox/application/chrono.hpp>

namespace antbox {

class application_clock
{
public:
    auto update() -> void;

    [[nodiscard]] auto delta() const noexcept -> chrono::clock::duration;

private:
    chrono::clock::time_point _last_tick{chrono::clock::now()};
    chrono::clock::duration _delta{};
};

inline auto application_clock::delta() const noexcept -> chrono::clock::duration
{
    return _delta;
}

} // namespace antbox
