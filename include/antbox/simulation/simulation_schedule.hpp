#pragma once

namespace ant {
class scheduler;
}

namespace antbox {

struct simulation_schedule
{
    static auto configure(ant::scheduler& scheduler) -> void;
};

} // namespace antbox
