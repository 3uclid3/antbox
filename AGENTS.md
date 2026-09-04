# AGENTS.md

## Project overview

`antbox` is an experimental ant-colony simulation sandbox built with the C++26 `ant` Entity Component System library. Most application behavior belongs in the `antbox` static library; `antbox.app` is the thin executable entry point and produces the `antbox` binary.

The repository deliberately uses the following source layout:

- Public headers are under `include/antbox`.
- Library implementation files are under `src`.
- The executable entry point is under `app`.
- Runtime unit tests are under `tests/unit/src`.

## Product direction

- Read `docs/design/vision.md` before making product or simulation design decisions.
- Use `docs/design/roadmap.md` for the intended capability order. It is directional, not a strict implementation plan.

## Build and test

The project requires CMake 3.30+, Ninja, and a C++26 compiler with reflection support (currently GCC 16+ with `-freflection`).

```sh
cmake --preset debug
cmake --build build
ctest --test-dir build --output-on-failure
```

Use `release` for release builds and `coverage` for coverage builds. Run unit tests with:

```sh
ctest --test-dir build -L unit --output-on-failure
ctest --test-dir build -R <name> --output-on-failure
```

All presets use the same `build` directory to keep `build/compile_commands.json` stable for editors. Delete `build` when a clean reconfiguration is required.

## Developing with ant

Normal presets fetch the commit of `ant` pinned in the top-level `CMakeLists.txt`. When `ant` is checked out at `../ant`, use `debug-local`, `release-local`, or `coverage-local` to build both repositories together.

Do not change the pinned remote revision merely to select a local checkout. Use the local presets, which set CMake's standard `FETCHCONTENT_SOURCE_DIR_ANT` override.

## Targets and CMake conventions

- Put simulation and application behavior in the `antbox` library.
- Link consumers and tests through the `antbox::antbox` alias.
- Keep `antbox.app` limited to executable startup and process-level concerns.
- Add every new public header and implementation file to `target_sources` in the top-level `CMakeLists.txt`.
- Keep dependencies linked at the narrowest appropriate visibility. `ant` is public because it may appear in the `antbox` API.
- Treat warnings as errors and avoid introducing compiler warnings.

## Code conventions

- Follow the repository `.clang-format`; format changed C++ files with `clang-format -i`.
- Keep function declarations and definitions separate, even in headers. Explicitly defaulted functions may remain in the enclosing class or struct, and valid correctness-driven exceptions are allowed.
- Keep out-of-class definitions in declaration order: define nested class and struct methods first, each in their declaration order, then define the enclosing type's methods in declaration order.
- Keep public declarations under `include/antbox` and non-public implementation details under `src`.
- Prefer testable library APIs over behavior implemented directly in `main`.
- Keep changes focused and avoid adding abstraction or directory layers without a concrete need.

## Tests

- Add runtime unit tests under `tests/unit/src` using doctest.
- Update `tests/unit/CMakeLists.txt` when adding a test source file.
- Tests should exercise an observable library behavior rather than only checking constants.
- The custom test entry point converts `ant` assertions into doctest failures; preserve that behavior when changing test setup.
- Build and run the full test suite before finishing a code change.

Update `README.md` when requirements, build commands, public usage, or the relationship with `ant` changes.
