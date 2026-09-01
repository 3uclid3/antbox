#include <antbox/application.hpp>

#include <array>
#include <cmath>
#include <extras/FA6FreeSolidFontData.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <jetbrains_mono_regular_font_data.hpp>
#include <numbers>
#include <random>
#include <raylib.h>
#include <rlImGui.h>

#include <ant/change/change_accumulator.hpp>
#include <ant/changeset.hpp>
#include <ant/database.hpp>
#include <ant/scheduler.hpp>
#include <ant/schema.hpp>
#include <antbox/application/application_clock.hpp>
#include <antbox/application/application_schedule.hpp>
#include <antbox/application/chrono.hpp>
#include <antbox/application/input/input.hpp>
#include <antbox/application/tick_accumulator.hpp>
#include <antbox/graphics/color.hpp>
#include <antbox/graphics/graphics_context.hpp>
#include <antbox/rendering/camera.hpp>
#include <antbox/rendering/rendering_schedule.hpp>
#include <antbox/simulation/ant/colony_member.hpp>
#include <antbox/simulation/clock.hpp>
#include <antbox/simulation/colony/colony.hpp>
#include <antbox/simulation/movement/position.hpp>
#include <antbox/simulation/movement/steering.hpp>
#include <antbox/simulation/movement/velocity.hpp>
#include <antbox/simulation/simulation_schedule.hpp>

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

        configure_schedules();

        bootstrap();

        // ensure bootstrap doesn't cause a large first frame
        _last_frame = chrono::clock::now();

        while (!WindowShouldClose())
        {
            const auto now = chrono::clock::now();
            const auto elapsed = now - _last_frame;
            _last_frame = now;

            _tick_accumulator.update(elapsed);
            _database.env_of<application_clock>().get<application_clock>().delta = std::chrono::duration<float>(elapsed).count();
            _database.env_of<graphics_context>().get<graphics_context>().viewport_size = {
                static_cast<float>(GetScreenWidth()),
                static_cast<float>(GetScreenHeight()),
            };

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
    static auto create_schema() -> ant::schema
    {
        ant::schema::builder builder;

        builder.define<application_clock>();
        builder.define<camera>();
        builder.define<clock>();
        builder.define<colony>();
        builder.define<colony_member>();
        builder.define<color>();
        builder.define<graphics_context>();
        builder.define<input>();
        builder.define<position>();
        builder.define<steering>();
        builder.define<velocity>();

        return builder.build();
    }

    auto configure_schedules() -> void
    {
        application_schedule::configure(_scheduler);
        rendering_schedule::configure(_scheduler);
        simulation_schedule::configure(_scheduler);

        _scheduler.compile<application_schedule>();
        _scheduler.compile<rendering_schedule>();
        _scheduler.compile<simulation_schedule>();
    }

    auto bootstrap() -> void
    {
        using signature = ant::changeset_signature<
            ant::create,
            ant::attach<
                position,
                velocity,
                steering,
                colony_member,
                colony,
                color>,
            ant::set_env<
                application_clock,
                camera,
                clock,
                graphics_context,
                input>>;

        ant::change_accumulator accumulator{_database.schema()};
        ant::changeset<signature> cs = _database.changeset<signature>(accumulator);
        cs.set_env<camera>();
        cs.set_env<clock>(clock{.delta = std::chrono::duration<float>(_tick_accumulator.target_delta()).count()});
        cs.set_env<application_clock>();
        cs.set_env<graphics_context>(graphics_context{.clear_color = {24, 27, 33, 255}});
        cs.set_env<input>();

        constexpr std::array centers{
            position{-230.0f, -100.0f},
            position{230.0f, -100.0f},
            position{0.0f, 170.0f},
        };
        constexpr std::array colors{
            color::red,
            color::green,
            color::blue,
        };

        std::array<ant::entity, centers.size()> colonies;
        std::mt19937 random{0xA17B0U};
        std::uniform_real_distribution<float> angle_distribution{0.0f, 2.0f * std::numbers::pi_v<float>};
        std::uniform_real_distribution<float> radius_distribution{0.0f, 40.0f};

        for (std::size_t index = 0; index < colonies.size(); ++index)
        {
            colonies[index] = cs.create();
            cs.attach<position>(colonies[index], centers[index]);
            cs.attach<colony>(colonies[index], 180.0f);
            cs.attach<color>(colonies[index], colors[index]);
        }

        for (std::size_t colony_index = 0; colony_index < colonies.size(); ++colony_index)
        {
            const ant::entity home = colonies[colony_index];
            const position& center = centers[colony_index];
            for (std::size_t index = 0; index < 80; ++index)
            {
                const float angle = angle_distribution(random);
                const float radius = radius_distribution(random);
                const ant::entity entity = cs.create();
                cs.attach<position>(entity, center.vec.x + std::cos(angle) * radius, center.vec.y + std::sin(angle) * radius);
                cs.attach<velocity>(entity, std::cos(angle) * 52.0f, std::sin(angle) * 52.0f);
                cs.attach<steering>(entity);
                cs.attach<colony_member>(entity, home);
            }
        }

        _database.flush(std::span{&accumulator, 1U});
    }

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
    const int result = app.run();

    return result;
}

} // namespace antbox
