#include <antbox/simulation/ant/steer_home.hpp>

#include <algorithm>

#include <ant/query.hpp>
#include <antbox/math/vec2.hpp>
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

        const vec2 displacement = home_position.vec - ant_position.vec;
        const float distance = length(displacement);
        const float edge = home_colony.range * 0.8F;
        if (distance <= edge || distance == 0.0F)
            continue;

        const float strength = std::clamp((distance - edge) / (home_colony.range - edge), 0.0F, 1.0F);
        steer.x += displacement.x / distance * strength * 140.0F;
        steer.y += displacement.y / distance * strength * 140.0F;
    }
}

} // namespace antbox
