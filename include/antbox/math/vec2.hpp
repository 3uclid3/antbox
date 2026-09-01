#pragma once

#include <cmath>
#include <concepts>

namespace antbox {

template<typename T>
concept vec2_like = requires(const T& value, float x, float y) {
    { value.x } -> std::convertible_to<float>;
    { value.y } -> std::convertible_to<float>;
    T{x, y};
};

struct vec2
{
    float x{};
    float y{};
};

[[nodiscard]] constexpr auto operator+(vec2 lhs, vec2 rhs) noexcept -> vec2
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

[[nodiscard]] constexpr auto operator-(vec2 lhs, vec2 rhs) noexcept -> vec2
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

[[nodiscard]] constexpr auto operator*(vec2 value, float scalar) noexcept -> vec2
{
    return {value.x * scalar, value.y * scalar};
}

[[nodiscard]] constexpr auto operator*(float scalar, vec2 value) noexcept -> vec2
{
    return value * scalar;
}

[[nodiscard]] constexpr auto operator/(vec2 value, float scalar) noexcept -> vec2
{
    return {value.x / scalar, value.y / scalar};
}

constexpr auto operator+=(vec2& lhs, vec2 rhs) noexcept -> vec2&
{
    lhs = lhs + rhs;
    return lhs;
}

constexpr auto operator-=(vec2& lhs, vec2 rhs) noexcept -> vec2&
{
    lhs = lhs - rhs;
    return lhs;
}

constexpr auto operator*=(vec2& value, float scalar) noexcept -> vec2&
{
    value = value * scalar;
    return value;
}

constexpr auto operator/=(vec2& value, float scalar) noexcept -> vec2&
{
    value = value / scalar;
    return value;
}

template<vec2_like To, vec2_like From>
[[nodiscard]] constexpr auto vec2_cast(const From& value) noexcept(noexcept(To{static_cast<float>(value.x), static_cast<float>(value.y)})) -> To
{
    return To{
        static_cast<float>(value.x),
        static_cast<float>(value.y),
    };
}

template<vec2_like T>
[[nodiscard]] constexpr auto length_squared(const T& value) noexcept -> float
{
    const float x = static_cast<float>(value.x);
    const float y = static_cast<float>(value.y);
    return x * x + y * y;
}

template<vec2_like T>
[[nodiscard]] constexpr auto length(const T& value) noexcept -> float
{
    return std::sqrt(length_squared(value));
}

template<vec2_like From, vec2_like To>
[[nodiscard]] constexpr auto distance(const From& from, const To& to) noexcept -> float
{
    const vec2 displacement{
        static_cast<float>(to.x) - static_cast<float>(from.x),
        static_cast<float>(to.y) - static_cast<float>(from.y),
    };
    return length(displacement);
}

[[nodiscard]] constexpr auto normalized_or_zero(vec2 value) noexcept -> vec2
{
    const float magnitude = length(value);
    return magnitude > 0.0f ? value / magnitude : vec2{};
}

} // namespace antbox
