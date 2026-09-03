#pragma once

#include <antbox/core/vec2.hpp>

namespace antbox {

struct input
{
    struct trigger
    {
        bool down{};     // Held: continuous movement
        bool pressed{};  // Became down this frame: toggles/actions
        bool released{}; // Became up this frame
    };

    struct
    {
        vec2 position{};  // Always populated
        vec2 delta{};     // Zero when captured
        float wheel{};    // Zero when captured
        trigger middle{}; // Cleared when captured
    } mouse;

    struct
    {
        trigger arrow_left{};
        trigger arrow_right{};
        trigger arrow_up{};
        trigger arrow_down{};
    } keyboard;
};

} // namespace antbox
