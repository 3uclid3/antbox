#include <antbox/application/playback/draw_playback.hpp>

#include <algorithm>
#include <array>
#include <string>

#include <extras/IconsFontAwesome6.h>
#include <imgui.h>

#include <ant/env.hpp>
#include <antbox/application/playback/format_tick_count.hpp>
#include <antbox/application/playback/playback.hpp>
#include <antbox/simulation/clock.hpp>

namespace antbox {

namespace {

struct speed_option
{
    playback_speed value;
    const char* label;
};

constexpr std::array speed_options{
    speed_option{playback_speed::quarter, "0.25x"},
    speed_option{playback_speed::half, "0.5x"},
    speed_option{playback_speed::normal, "1x"},
    speed_option{playback_speed::double_speed, "2x"},
    speed_option{playback_speed::quadruple, "4x"},
};

constexpr auto widest_tick_label = "99.9M";
} // namespace

auto draw_playback::operator()(ant::env_of<const clock, playback> env) const -> void
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(
        {viewport->WorkPos.x + viewport->WorkSize.x * 0.5f, viewport->WorkPos.y + viewport->WorkSize.y - 12.0f},
        ImGuiCond_Always,
        {0.5f, 1.0f});

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration
                                       | ImGuiWindowFlags_AlwaysAutoResize
                                       | ImGuiWindowFlags_NoSavedSettings
                                       | ImGuiWindowFlags_NoMove;

    if (!ImGui::Begin("Playback", nullptr, flags))
    {
        ImGui::End();
        return;
    }

    // use biggest button as base
    const ImVec2 control_button_size{
        ImGui::CalcTextSize(ICON_FA_PLAY).x + ImGui::GetStyle().FramePadding.x * 2.0f,
        0.f};

    playback& controls = env.get<playback>();
    if (ImGui::Button(controls.paused() ? ICON_FA_PLAY "##play" : ICON_FA_PAUSE "##pause", control_button_size))
    {
        controls.toggle();
    }

    ImGui::SameLine();
    ImGui::BeginDisabled(!controls.paused());
    if (ImGui::Button(ICON_FA_FORWARD_STEP "##step", control_button_size))
    {
        controls.step();
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    ImGui::SetNextItemWidth(70.0f);
    const auto selected = std::ranges::find(speed_options, controls.speed(), &speed_option::value);
    const char* selected_label = selected == speed_options.end() ? "custom" : selected->label;
    if (ImGui::BeginCombo("##speed", selected_label))
    {
        for (const speed_option& option : speed_options)
        {
            if (ImGui::Selectable(option.label, controls.speed() == option.value))
            {
                controls.set_speed(option.value);
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::TextDisabled("tick");

    ImGui::SameLine();
    const std::uint64_t tick_count = env.get<clock>().tick;
    const std::string tick_label = format_tick_count(tick_count);
    const float tick_label_width = ImGui::CalcTextSize(tick_label.c_str()).x;
    const float reserved_tick_width = std::max(ImGui::CalcTextSize(widest_tick_label).x, tick_label_width);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + reserved_tick_width - tick_label_width);
    ImGui::TextUnformatted(tick_label.c_str());

    if (ImGui::IsItemHovered())
    {
        const std::string exact_tick_count = format_tick_count_exact(tick_count);
        ImGui::SetTooltip("%s ticks", exact_tick_count.c_str());
    }

    ImGui::End();
}

} // namespace antbox
