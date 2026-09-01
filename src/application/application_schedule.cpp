#include <antbox/application/application_schedule.hpp>

#include <ant/scheduler.hpp>
#include <antbox/application/input/poll_input.hpp>
#include <antbox/rendering/update_camera.hpp>

namespace antbox {

namespace {

struct application_stage
{
    static auto configure(ant::scheduler::stage_handle stage) -> void
    {
        const auto input_system = stage.add<poll_input>();
        stage.add<update_camera>().after(input_system);
    }
};

} // namespace

auto application_schedule::configure(ant::scheduler& scheduler) -> void
{
    application_stage::configure(scheduler.stage<application_schedule, application_stage>());
}

} // namespace antbox
