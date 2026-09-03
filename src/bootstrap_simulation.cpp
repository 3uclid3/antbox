#include "bootstrap_simulation.hpp"

#include <random>

#include <ant/changeset.hpp>
#include <ant/database.hpp>
#include <antbox/app/application_clock.hpp>
#include <antbox/app/input.hpp>
#include <antbox/bench/playback.hpp>
#include <antbox/render/camera.hpp>
#include <antbox/render/color.hpp>
#include <antbox/render/graphics_context.hpp>
#include <antbox/sim/ant/colony_member.hpp>
#include <antbox/sim/colony/colony.hpp>
#include <antbox/sim/motion/position.hpp>
#include <antbox/sim/motion/steering.hpp>
#include <antbox/sim/motion/velocity.hpp>
#include <antbox/sim/time/clock.hpp>

namespace antbox {

using bootstrap_changeset_signature = ant::changeset_signature<
    ant::create,
    ant::attach<
        sim::position,
        sim::velocity,
        sim::steering,
        sim::colony_member,
        sim::colony,
        color>,
    ant::set_env<
        application_clock,
        camera,
        sim::clock,
        graphics_context,
        input,
        bench::playback>>;

constexpr std::array centers{
    sim::position{-230.0f, -100.0f},
    sim::position{230.0f, -100.0f},
    sim::position{0.0f, 170.0f},
};

constexpr std::array colors{
    color::red,
    color::green,
    color::blue,
};

auto bootstrap_simulation(ant::database& database, float delta) -> void
{
    ant::change_accumulator accumulator{database.schema()};
    ant::changeset cs = database.changeset<bootstrap_changeset_signature>(accumulator);
    cs.set_env<camera>();
    cs.set_env<sim::clock>(sim::clock{.delta = delta});
    cs.set_env<application_clock>();
    cs.set_env<graphics_context>(graphics_context{.clear_color = {24, 27, 33, 255}});
    cs.set_env<input>();
    cs.set_env<bench::playback>();

    std::array<ant::entity, centers.size()> colonies;
    std::mt19937 random{0xA17B0U};
    std::uniform_real_distribution<float> angle_distribution{0.0f, 2.0f * std::numbers::pi_v<float>};
    std::uniform_real_distribution<float> radius_distribution{0.0f, 40.0f};

    for (std::size_t index = 0; index < colonies.size(); ++index)
    {
        colonies[index] = cs.create();
        cs.attach<sim::position>(colonies[index], centers[index]);
        cs.attach<sim::colony>(colonies[index], 180.0f);
        cs.attach<color>(colonies[index], colors[index]);
    }

    for (std::size_t colony_index = 0; colony_index < colonies.size(); ++colony_index)
    {
        const ant::entity home = colonies[colony_index];
        const sim::position& center = centers[colony_index];
        for (std::size_t index = 0; index < 80; ++index)
        {
            const float angle = angle_distribution(random);
            const float radius = radius_distribution(random);
            const ant::entity entity = cs.create();
            cs.attach<sim::position>(entity, center.vec.x + std::cos(angle) * radius, center.vec.y + std::sin(angle) * radius);
            cs.attach<sim::velocity>(entity, std::cos(angle) * 52.0f, std::sin(angle) * 52.0f);
            cs.attach<sim::steering>(entity);
            cs.attach<sim::colony_member>(entity, home);
        }
    }

    database.flush(std::span{&accumulator, 1U});
}

} // namespace antbox
