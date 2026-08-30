#pragma once

#include <cstdint>

struct Color; // raylib

namespace antbox {

struct color
{
    static const color black;
    static const color white;
    static const color transparent;

    static const color red;
    static const color green;
    static const color blue;

    operator Color() const noexcept;

    std::uint8_t r{};
    std::uint8_t g{};
    std::uint8_t b{};
    std::uint8_t a{255};
};

constexpr color color::black{0, 0, 0, 255};
constexpr color color::white{255, 255, 255, 255};
constexpr color color::transparent{0, 0, 0, 0};

constexpr color color::red{224, 108, 117, 255};
constexpr color color::green{152, 195, 121, 255};
constexpr color color::blue{97, 175, 239, 255};

} // namespace antbox
