#include <antbox/bench/format_tick_count.hpp>

#include <algorithm>
#include <array>
#include <charconv>

namespace antbox::bench {

namespace {

struct unit
{
    std::uint64_t threshold;
    std::uint64_t divisor;
    char suffix;
};

constexpr std::array units{
    unit{999'500'000U, 1'000'000'000U, 'B'},
    unit{999'500U, 1'000'000U, 'M'},
    unit{10'000U, 1'000U, 'k'},
};

auto append_number(std::string& result, std::uint64_t value) -> void
{
    std::array<char, 20> buffer{};
    const auto [end, error] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);
    if (error == std::errc{})
    {
        result.append(buffer.data(), end);
    }
}

} // namespace

auto format_tick_count(std::uint64_t tick_count) -> std::string
{
    const auto selected = std::ranges::find_if(units, [tick_count](const unit& option) {
        return tick_count >= option.threshold;
    });
    if (selected == units.end())
    {
        return format_tick_count_exact(tick_count);
    }

    const std::uint64_t whole = tick_count / selected->divisor;
    const std::uint64_t remainder = tick_count % selected->divisor;

    std::string result;
    if (whole >= 100U)
    {
        append_number(result, whole + static_cast<std::uint64_t>(remainder >= selected->divisor / 2U));
    }
    else
    {
        std::uint64_t tenths = (remainder * 10U + selected->divisor / 2U) / selected->divisor;
        std::uint64_t rounded_whole = whole;
        if (tenths == 10U)
        {
            ++rounded_whole;
            tenths = 0U;
        }

        append_number(result, rounded_whole);
        if (tenths != 0U)
        {
            result.push_back('.');
            result.push_back(static_cast<char>('0' + tenths));
        }
    }

    result.push_back(selected->suffix);
    return result;
}

auto format_tick_count_exact(std::uint64_t tick_count) -> std::string
{
    std::string digits;
    append_number(digits, tick_count);

    std::string result;
    result.reserve(digits.size() + (digits.size() - 1U) / 3U);

    for (std::size_t index = 0; index < digits.size(); ++index)
    {
        if (index != 0U && (digits.size() - index) % 3U == 0U)
        {
            result.push_back(',');
        }

        result.push_back(digits[index]);
    }
    return result;
}

} // namespace antbox::bench
