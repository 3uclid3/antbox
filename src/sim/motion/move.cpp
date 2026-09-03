#include <antbox/sim/motion/move.hpp>

#include <ant/env.hpp>
#include <ant/query.hpp>
#include <antbox/core/vec2.hpp>
#include <antbox/sim/motion/position.hpp>
#include <antbox/sim/motion/steering.hpp>
#include <antbox/sim/motion/velocity.hpp>
#include <antbox/sim/time/clock.hpp>

namespace antbox::sim {

auto move::operator()(ant::env_of<const clock> env, ant::query_of<position, velocity, const steering> query) const -> void
{
    const float delta = env.get<clock>().delta;
    for (auto row : query)
    {
        auto& pos = row.get<position>();
        auto& vel = row.get<velocity>();
        const auto& steer = row.get<steering>();

        const float speed = length(vel);
        vel.x += steer.x * delta;
        vel.y += steer.y * delta;

        const float steered_speed = length(vel);
        if (speed > 0.0F && steered_speed > 0.0F)
        {
            vel.x *= speed / steered_speed;
            vel.y *= speed / steered_speed;
        }

        pos.vec.x += vel.x * delta;
        pos.vec.y += vel.y * delta;
    }
}

} // namespace antbox::sim
