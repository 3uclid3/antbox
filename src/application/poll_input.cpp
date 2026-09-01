#include <antbox/application/input/poll_input.hpp>

#include <imgui.h>
#include <raylib.h>

#include <ant/env.hpp>
#include <antbox/application/input/input.hpp>

namespace antbox {

namespace {

auto update_mouse_button(input::trigger& trigger, MouseButton button) -> void
{
    trigger.down = IsMouseButtonDown(button);
    trigger.pressed = IsMouseButtonPressed(button);
    trigger.released = IsMouseButtonReleased(button);
}

auto update_keyboard_button(input::trigger& trigger, KeyboardKey key) -> void
{
    trigger.down = IsKeyDown(key);
    trigger.pressed = IsKeyPressed(key);
    trigger.released = IsKeyReleased(key);
}

} // namespace

auto poll_input::operator()(ant::env_of<input> env) const -> void
{
    input& in = env.get<input>();
    in.mouse.position = vec2_cast<vec2>(GetMousePosition());

    const ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        in.mouse.delta = {};
        in.mouse.wheel = 0.0f;
        in.mouse.middle = {};
    }
    else
    {
        in.mouse.delta = vec2_cast<vec2>(GetMouseDelta());
        in.mouse.wheel = GetMouseWheelMove();

        update_mouse_button(in.mouse.middle, MOUSE_BUTTON_MIDDLE);
    }

    if (io.WantCaptureKeyboard)
    {
        in.keyboard = {};
    }
    else
    {
        update_keyboard_button(in.keyboard.arrow_left, KEY_LEFT);
        update_keyboard_button(in.keyboard.arrow_right, KEY_RIGHT);
        update_keyboard_button(in.keyboard.arrow_up, KEY_UP);
        update_keyboard_button(in.keyboard.arrow_down, KEY_DOWN);
    }
}

} // namespace antbox
