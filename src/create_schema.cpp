#include "create_schema.hpp"

#include <ant/schema.hpp>
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

auto create_schema() -> ant::schema
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

} // namespace antbox
