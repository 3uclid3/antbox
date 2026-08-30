#include <antbox/simulation/ant/steer_home.hpp>

#include <algorithm>
#include <cmath>

#include <ant/query.hpp>
#include <antbox/simulation/ant/colony_member.hpp>
#include <antbox/simulation/colony/colony.hpp>
#include <antbox/simulation/movement/position.hpp>
#include <antbox/simulation/movement/steering.hpp>

namespace antbox {

auto steer_home::operator()(ant::query<ants_query_signature> ants, ant::query<colonies_query_signature> colonies) const -> void
{
    for (auto ant : ants)
    {
        const auto home = colonies.row(ant.get<colony_member>().entity);
        if (!home)
            continue;

        const auto& ant_position = ant.get<position>();
        const auto& home_position = home->get<position>();
        const auto& home_colony = home->get<colony>();
        auto& steer = ant.get<steering>();

        const float dx = home_position.x - ant_position.x;
        const float dy = home_position.y - ant_position.y;
        const float distance = std::sqrt(dx * dx + dy * dy);
        const float edge = home_colony.range * 0.8F;
        if (distance <= edge || distance == 0.0F)
            continue;

        const float strength = std::clamp((distance - edge) / (home_colony.range - edge), 0.0F, 1.0F);
        steer.x += dx / distance * strength * 140.0F;
        steer.y += dy / distance * strength * 140.0F;
    }
}

} // namespace antbox
