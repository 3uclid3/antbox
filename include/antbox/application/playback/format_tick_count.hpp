#pragma once

#include <cstdint>
#include <string>

namespace antbox {

auto format_tick_count(std::uint64_t tick_count) -> std::string;
auto format_tick_count_exact(std::uint64_t tick_count) -> std::string;

} // namespace antbox
