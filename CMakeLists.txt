cmake_minimum_required(VERSION 3.30)
project(antbox VERSION 0.1.0 LANGUAGES C CXX)

#
set(CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# options
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  option(ANTBOX_BUILD_TESTS "Build antbox tests" ON)
else()
  option(ANTBOX_BUILD_TESTS "Build antbox tests" OFF)
endif()

# dependencies
include(FetchContent)

FetchContent_Declare(
  ant
  GIT_REPOSITORY https://github.com/3uclid3/ant.git
  GIT_TAG 79f964a16a79917c65a1168dd18c035e53980051
)

set(ANT_BUILD_SCHEDULER ON)
set(ANT_BUILD_TESTING ${ANTBOX_BUILD_TESTS})
FetchContent_MakeAvailable(ant)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/rlimgui.cmake")

# warning
add_library(antbox_warnings INTERFACE)
target_compile_options(antbox_warnings INTERFACE
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
  $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wall -Wextra -Wpedantic -Werror>
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

# antbox
add_library(antbox STATIC)
add_library(antbox::antbox ALIAS antbox)
target_link_libraries(antbox
  PUBLIC
    ant::ant
  PRIVATE
    ant::scheduler
    antbox_warnings
    rlimgui::rlimgui
)
target_compile_features(antbox PUBLIC cxx_std_26)
target_compile_options(antbox PUBLIC -freflection) # experimental
target_sources(antbox
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/antbox/app/application_clock.hpp
      include/antbox/app/input.hpp
      include/antbox/app/poll_input.hpp
      include/antbox/app/poll_window.hpp
      include/antbox/app/tick_accumulator.hpp
      include/antbox/bench/draw_playback.hpp
      include/antbox/bench/format_tick_count.hpp
      include/antbox/bench/playback.hpp
      include/antbox/core/chrono.hpp
      include/antbox/core/spatial_grid.hpp
      include/antbox/core/vec2.hpp
      include/antbox/render/begin_render.hpp
      include/antbox/render/begin_render_rlimgui.hpp
      include/antbox/render/begin_render_world.hpp
      include/antbox/render/camera.hpp
      include/antbox/render/color.hpp
      include/antbox/render/draw_ants.hpp
      include/antbox/render/draw_colonies.hpp
      include/antbox/render/end_render.hpp
      include/antbox/render/end_render_rlimgui.hpp
      include/antbox/render/end_render_world.hpp
      include/antbox/render/graphics_context.hpp
      include/antbox/render/update_camera.hpp
      include/antbox/run.hpp
      include/antbox/sim/ant/colony_member.hpp
      include/antbox/sim/ant/steer_home.hpp
      include/antbox/sim/ant/wander.hpp
      include/antbox/sim/colony/colony.hpp
      include/antbox/sim/motion/move.hpp
      include/antbox/sim/motion/position.hpp
      include/antbox/sim/motion/steering.hpp
      include/antbox/sim/motion/velocity.hpp
      include/antbox/sim/time/advance_clock.hpp
      include/antbox/sim/time/clock.hpp
  PRIVATE
    src/app/poll_input.cpp
    src/app/poll_window.cpp
    src/app/tick_accumulator.cpp
    src/bench/draw_playback.cpp
    src/bench/format_tick_count.cpp
    src/bench/playback.cpp
    src/render/begin_render.cpp
    src/render/begin_rlimgui.cpp
    src/render/begin_render_world.cpp
    src/render/color.cpp
    src/render/draw_ants.cpp
    src/render/draw_colonies.cpp
    src/render/end_render.cpp
    src/render/end_rlimgui.cpp
    src/render/end_render_world.cpp
    src/render/update_camera.cpp
    src/sim/ant/steer_home.cpp
    src/sim/ant/wander.cpp
    src/sim/motion/move.cpp
    src/sim/time/advance_clock.cpp
    src/run.cpp
    src/bootstrap_simulation.hpp
    src/bootstrap_simulation.cpp
    src/configure_schedules.hpp
    src/configure_schedules.cpp
    src/create_schema.hpp
    src/create_schema.cpp
)
rlimgui_embed_font(
  TARGET antbox
  FONT fonts/JetBrainsMono-Regular.ttf
  SYMBOL jetbrains_mono_regular
)

# antbox.app
add_executable(antbox.app app/main.cpp)
set_target_properties(antbox.app PROPERTIES OUTPUT_NAME antbox)
target_link_libraries(antbox.app PRIVATE antbox_warnings antbox::antbox)

# tests
if(ANTBOX_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
