#pragma once

#include <ant/change/change_accumulator.hpp>
#include <ant/changeset.hpp>
#include <ant/changeset/changeset_signature.hpp>
#include <ant/database.hpp>
#include <ant/scheduler.hpp>
#include <ant/schema.hpp>

#include <type_traits>
#include <utility>

namespace antbox {

struct system_fixture
{
    template<typename... T>
    auto set_env() -> void
    {
        ant::change_accumulator accumulator{database.schema()};
        ant::changeset cs = database.changeset_of<ant::set_env<T...>>(accumulator);

        (cs.template set_env<T>(), ...);

        database.flush(std::span{&accumulator, 1});
    }

    template<typename... T>
    auto set_env(T&&... args) -> void
    {
        ant::change_accumulator accumulator{database.schema()};
        ant::changeset cs = database.changeset_of<ant::set_env<std::decay_t<T>...>>(accumulator);

        (cs.template set_env<std::decay_t<T>>(std::forward<T>(args)), ...);

        database.flush(std::span{&accumulator, 1});
    }

    template<typename... T>
    auto create_entity() -> ant::entity
    {
        ant::change_accumulator accumulator{database.schema()};
        ant::changeset cs = database.changeset_of<ant::create, ant::attach<T...>>(accumulator);

        ant::entity e = cs.create();
        (cs.template attach<T>(e), ...);

        database.flush(std::span{&accumulator, 1});

        return e;
    }

    template<typename... T>
    auto create_entity(T&&... args) -> ant::entity
    {
        ant::change_accumulator accumulator{database.schema()};
        ant::changeset cs = database.changeset_of<ant::create, ant::attach<std::decay_t<T>...>>(accumulator);

        ant::entity e = cs.create();
        (cs.template attach<std::decay_t<T>>(e, std::forward<T>(args)), ...);

        database.flush(std::span{&accumulator, 1});

        return e;
    }

    system_fixture(ant::schema&& schema)
        : database(std::move(schema))
    {
    }

    ant::database database;
    ant::scheduler scheduler{database};
};

} // namespace antbox
