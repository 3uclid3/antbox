# antbox

[![Build](https://github.com/3uclid3/antbox/actions/workflows/build.yml/badge.svg)](https://github.com/3uclid3/antbox/actions/workflows/build.yml)
[![codecov](https://codecov.io/gh/3uclid3/antbox/graph/badge.svg?token=2SGO73MLXC)](https://codecov.io/gh/3uclid3/antbox)

An ant-colony simulation sandbox built with [ant](https://github.com/3uclid3/ant).

`antbox` is both an executable sandbox and a library containing the simulation itself. Keeping the application logic in `libantbox` makes the simulation directly testable without driving the executable.

> **The project is experimental and actively evolving.**

## Requirements

- GCC 16+ (or a C++26-conformant compiler with reflection support)
- CMake 3.30+
- Ninja

## Getting Started

Configure, build, and run a debug build:

```sh
cmake --preset debug
cmake --build build
./build/antbox
```

Run the tests with:

```sh
ctest --test-dir build --output-on-failure
```

## Developing with ant

The default presets fetch the pinned version of `ant`. When the `ant` and `antbox` repositories are sibling directories, use a local preset to build against the local checkout instead:

```text
projects/
├── ant/
└── antbox/
```

```sh
cmake --preset debug-local
cmake --build build
```

Local variants are also available for the `release` and `coverage` presets. All presets share the `build` directory so editors can consistently find `build/compile_commands.json`; delete that directory when switching between fetched and local `ant` if a clean dependency configuration is needed.

## Design

**The library is the application.**
Simulation behavior lives in the `antbox` library. The `antbox.app` target is a thin executable entry point, while unit tests link the same library directly.

**The sandbox develops alongside the framework.**
Remote builds stay reproducible by pinning `ant` to a commit, while local presets make it straightforward to evolve both repositories together.

## License

[MIT](LICENSE)
