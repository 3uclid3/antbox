#pragma once

namespace ant {
class scheduler;
}

namespace antbox {

struct rendering_schedule
{
    static auto configure(ant::scheduler& scheduler) -> void;
};

} // namespace antbox
