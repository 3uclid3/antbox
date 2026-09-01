#include "bootstrap_simulation.hpp"

#include <random>

#include <ant/changeset.hpp>
#include <ant/database.hpp>
#include <antbox/application/application_clock.hpp>
#include <antbox/application/input/input.hpp>
#include <antbox/graphics/color.hpp>
#include <antbox/graphics/graphics_context.hpp>
#include <antbox/rendering/camera.hpp>
#include <antbox/simulation/ant/colony_member.hpp>
#include <antbox/simulation/clock.hpp>
#include <antbox/simulation/colony/colony.hpp>
#include <antbox/simulation/movement/position.hpp>
#include <antbox/simulation/movement/steering.hpp>
#include <antbox/simulation/movement/velocity.hpp>

namespace antbox {

using bootstrap_changeset_signature = ant::changeset_signature<
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

auto bootstrap_simulation(ant::database& database, float delta) -> void
{
    ant::change_accumulator accumulator{database.schema()};
    ant::changeset cs = database.changeset<bootstrap_changeset_signature>(accumulator);
    cs.set_env<camera>();
    cs.set_env<clock>(clock{.delta = delta});
    cs.set_env<application_clock>();
    cs.set_env<graphics_context>(graphics_context{.clear_color = {24, 27, 33, 255}});
    cs.set_env<input>();

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

    database.flush(std::span{&accumulator, 1U});
}

} // namespace antbox
