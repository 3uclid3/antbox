#include <antbox/application.hpp>

#include <extras/FA6FreeSolidFontData.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <jetbrains_mono_regular_font_data.hpp>
#include <raylib.h>
#include <rlImGui.h>

#include <ant/database.hpp>
#include <ant/scheduler.hpp>
#include <antbox/application/application_clock.hpp>
#include <antbox/application/chrono.hpp>
#include <antbox/application/tick_accumulator.hpp>
#include <antbox/graphics/graphics_context.hpp>

#include "bootstrap_simulation.hpp"
#include "configure_schedules.hpp"
#include "create_schema.hpp"

namespace antbox {

namespace {

auto setup_imgui() -> void
{
    rlImGuiSetup(true);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    const float dpi = GetWindowScaleDPI().x;
    constexpr float base_font_size = 13.0f;
    const float font_size = base_font_size * dpi;

    ImFontConfig font_config;
    font_config.FontDataOwnedByAtlas = false;
    io.FontDefault = io.Fonts->AddFontFromMemoryCompressedTTF(
        jetbrains_mono_regular_compressed_data,
        jetbrains_mono_regular_compressed_size,
        font_size,
        &font_config);

    ImFontConfig icon_config;
    icon_config.FontDataOwnedByAtlas = false;
    icon_config.MergeMode = true;
    icon_config.PixelSnapH = true;
    static constexpr ImWchar icon_ranges[]{ICON_MIN_FA, ICON_MAX_FA, 0};
    io.Fonts->AddFontFromMemoryCompressedTTF(
        fa_solid_900_compressed_data,
        fa_solid_900_compressed_size,
        font_size,
        &icon_config,
        icon_ranges);
}

} // namespace

class application
{
public:
    application()
    {
        SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_MAXIMIZED);
        InitWindow(1280, 800, "antbox - ant-colony simulation sandbox");
        SetTargetFPS(144);

        setup_imgui();
    }

    ~application()
    {
        rlImGuiShutdown();
        CloseWindow();
    }

    auto run() -> int
    {
        constexpr std::size_t max_ticks_per_frame = 8;

        configure_schedules(_scheduler);

        bootstrap_simulation(_database, std::chrono::duration<float>(_tick_accumulator.target_delta()).count());

        // ensure bootstrap doesn't cause a large first frame
        _last_frame = chrono::clock::now();

        while (!WindowShouldClose())
        {
            const auto now = chrono::clock::now();
            const auto elapsed = now - _last_frame;
            _last_frame = now;

            _tick_accumulator.update(elapsed);

            ant::env_of env = _database.env_of<application_clock>();
            env.get<application_clock>().delta = std::chrono::duration<float>(elapsed).count();

            _scheduler.execute<application_schedule>();

            std::size_t tick_count = 0;
            while (tick_count < max_ticks_per_frame && _tick_accumulator.consume_tick())
            {
                _scheduler.execute<simulation_schedule>();
                ++tick_count;
            }

            if (should_render())
            {
                _scheduler.execute<rendering_schedule>();
            }
        }

        return 0;
    }

private:
    auto should_render() const noexcept -> bool
    {
        return true;
    }

private:
    ant::database _database{create_schema()};
    ant::scheduler _scheduler{_database};

    tick_accumulator _tick_accumulator{std::chrono::duration_cast<chrono::clock::duration>(std::chrono::duration<double>{1.0 / 60.0})};
    chrono::clock::time_point _last_frame{chrono::clock::now()};
};

auto run() -> int
{
    application app;
    return app.run();
}

} // namespace antbox
