#include "create_schema.hpp"

#include <ant/schema.hpp>
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

auto create_schema() -> ant::schema
{
    ant::schema::builder builder;

    builder.define<application_clock>();
    builder.define<camera>();
    builder.define<sim::clock>();
    builder.define<sim::colony>();
    builder.define<sim::colony_member>();
    builder.define<color>();
    builder.define<graphics_context>();
    builder.define<input>();
    builder.define<bench::playback>();
    builder.define<sim::position>();
    builder.define<sim::steering>();
    builder.define<sim::velocity>();

    return builder.build();
}

} // namespace antbox
