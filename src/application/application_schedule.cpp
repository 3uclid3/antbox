#include <antbox/application/application_schedule.hpp>

#include <ant/scheduler.hpp>

namespace antbox {

namespace {

struct application_stage
{
};

} // namespace

auto application_schedule::configure(ant::scheduler& scheduler) -> void
{
    scheduler.stage<application_schedule, application_stage>();
}

} // namespace antbox
