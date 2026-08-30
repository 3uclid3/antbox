#include <antbox/simulation/movement/move.hpp>

#include <cmath>

#include <ant/env.hpp>
#include <ant/query.hpp>
#include <antbox/simulation/clock.hpp>
#include <antbox/simulation/movement/position.hpp>
#include <antbox/simulation/movement/steering.hpp>
#include <antbox/simulation/movement/velocity.hpp>

namespace antbox {

auto move::operator()(ant::env_of<const clock> env, ant::query_of<position, velocity, const steering> query) const -> void
{
    const float delta = env.get<clock>().delta;
    for (auto row : query)
    {
        auto& pos = row.get<position>();
        auto& vel = row.get<velocity>();
        const auto& steer = row.get<steering>();

        const float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
        vel.x += steer.x * delta;
        vel.y += steer.y * delta;

        const float steered_speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
        if (speed > 0.0F && steered_speed > 0.0F)
        {
            vel.x *= speed / steered_speed;
            vel.y *= speed / steered_speed;
        }

        pos.x += vel.x * delta;
        pos.y += vel.y * delta;
    }
}

} // namespace antbox
