#include <antbox/application.hpp>

#include <array>
#include <cmath>
#include <extras/FA6FreeSolidFontData.h> // from rlimgui
#include <imgui.h>
#include <imgui_internal.h>
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
#include <antbox/application/tick_accumulator.hpp>
#include <antbox/graphics/color.hpp>
#include <antbox/graphics/graphics_context.hpp>
#include <antbox/rendering/rendering_schedule.hpp>
#include <antbox/simulation/ant/colony_member.hpp>
#include <antbox/simulation/clock.hpp>
#include <antbox/simulation/colony/colony.hpp>
#include <antbox/simulation/movement/position.hpp>
#include <antbox/simulation/movement/steering.hpp>
#include <antbox/simulation/movement/velocity.hpp>
#include <antbox/simulation/simulation_schedule.hpp>

namespace antbox {

class application
{
public:
    application()
    {
        SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_MAXIMIZED);
        InitWindow(1280, 800, "antbox - ant-colony simulation sandbox");
        SetTargetFPS(144);

        rlImGuiSetup(true);
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
        _clock.update();

        while (!WindowShouldClose())
        {
            _clock.update();
            _tick_accumulator.update(_clock.delta());

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

        builder.define<clock>();
        builder.define<color>();
        builder.define<graphics_context>();
        builder.define<position>();
        builder.define<velocity>();
        builder.define<steering>();
        builder.define<colony_member>();
        builder.define<colony>();

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
        using signature = ant::changeset_signature<ant::create, ant::attach<position, velocity, steering, colony_member, colony, color>, ant::set_env<clock, graphics_context>>;

        ant::change_accumulator accumulator{_database.schema()};
        ant::changeset<signature> cs = _database.changeset<signature>(accumulator);
        cs.set_env<clock>(clock{.delta = std::chrono::duration<float>(_tick_accumulator.target_delta()).count()});
        cs.set_env<graphics_context>(graphics_context{.clear_color = {24, 27, 33, 255}});

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
                cs.attach<position>(entity, center.x + std::cos(angle) * radius, center.y + std::sin(angle) * radius);
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

    application_clock _clock;
    tick_accumulator _tick_accumulator{std::chrono::duration_cast<chrono::clock::duration>(std::chrono::duration<double>{1.0 / 60.0})};
};

auto run() -> int
{
    application app;
    const int result = app.run();

    return result;
}

} // namespace antbox
