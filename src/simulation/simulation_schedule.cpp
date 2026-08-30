#include <antbox/simulation/simulation_schedule.hpp>

#include <ant/scheduler.hpp>
#include <antbox/simulation/ant/steer_home.hpp>
#include <antbox/simulation/ant/wander.hpp>
#include <antbox/simulation/clock/advance_clock.hpp>
#include <antbox/simulation/movement/move.hpp>

namespace antbox {

namespace {

template<typename T>
auto configure_stage(ant::scheduler& scheduler) -> void
{
    T::configure(scheduler.stage<simulation_schedule, T>());
}

struct clock_stage
{
    static auto configure(ant::scheduler::stage_handle stage) -> void
    {
        stage.add<advance_clock>();
    }
};

struct update_stage
{
    static auto configure(ant::scheduler::stage_handle stage) -> void
    {
        const auto wander_system = stage.add<wander>();
        const auto home_system = stage.add<steer_home>();
        auto move_system = stage.add<move>();
        move_system.after(wander_system).after(home_system);
    }
};

} // namespace

auto simulation_schedule::configure(ant::scheduler& scheduler) -> void
{
    configure_stage<clock_stage>(scheduler);
    configure_stage<update_stage>(scheduler);
}

} // namespace antbox
