#pragma once

#include <antbox/core/chrono.hpp>

namespace antbox {

class tick_accumulator
{
public:
    explicit tick_accumulator(chrono::clock::duration target_delta) noexcept;

public:
    auto update(chrono::clock::duration elapsed) -> void;
    auto consume_tick() -> bool;
    auto target_delta() const noexcept -> chrono::clock::duration { return _target_delta; }

private:
    chrono::clock::duration _accumulator{};
    chrono::clock::duration _target_delta;
};

} // namespace antbox
