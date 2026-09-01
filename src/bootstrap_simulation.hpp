#pragma once

namespace ant { class database; }

namespace antbox {

auto bootstrap_simulation(ant::database& database, float delta) -> void;

} // namespace antbox
