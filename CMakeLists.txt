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
      include/antbox/application/application_clock.hpp
      include/antbox/application/chrono.hpp
      include/antbox/application/input.hpp
      include/antbox/application/playback/draw_playback.hpp
      include/antbox/application/playback/format_tick_count.hpp
      include/antbox/application/playback/playback.hpp
      include/antbox/application/poll_input.hpp
      include/antbox/application/poll_window.hpp
      include/antbox/application/tick_accumulator.hpp
      include/antbox/graphics/begin_render.hpp
      include/antbox/graphics/begin_render_rlimgui.hpp
      include/antbox/graphics/begin_render_world.hpp
      include/antbox/graphics/color.hpp
      include/antbox/graphics/end_render.hpp
      include/antbox/graphics/end_render_rlimgui.hpp
      include/antbox/graphics/end_render_world.hpp
      include/antbox/graphics/graphics_context.hpp
      include/antbox/math/vec2.hpp
      include/antbox/rendering/camera.hpp
      include/antbox/rendering/draw_ants.hpp
      include/antbox/rendering/draw_colonies.hpp
      include/antbox/rendering/update_camera.hpp
      include/antbox/simulation/ant/colony_member.hpp
      include/antbox/simulation/ant/steer_home.hpp
      include/antbox/simulation/ant/wander.hpp
      include/antbox/simulation/clock.hpp
      include/antbox/simulation/clock/advance_clock.hpp
      include/antbox/simulation/colony/colony.hpp
      include/antbox/simulation/movement/move.hpp
      include/antbox/simulation/movement/position.hpp
      include/antbox/simulation/movement/steering.hpp
      include/antbox/simulation/movement/velocity.hpp
      include/antbox/application.hpp
  PRIVATE
    src/application/playback/draw_playback.cpp
    src/application/playback/format_tick_count.cpp
    src/application/playback/playback.cpp
    src/application/poll_input.cpp
    src/application/poll_window.cpp
    src/application/tick_accumulator.cpp
    src/graphics/begin_render.cpp
    src/graphics/begin_rlimgui.cpp
    src/graphics/begin_render_world.cpp
    src/graphics/color.cpp
    src/graphics/end_render.cpp
    src/graphics/end_rlimgui.cpp
    src/graphics/end_render_world.cpp
    src/rendering/draw_ants.cpp
    src/rendering/draw_colonies.cpp
    src/rendering/update_camera.cpp
    src/simulation/ant/steer_home.cpp
    src/simulation/ant/wander.cpp
    src/simulation/clock/advance_clock.cpp
    src/simulation/movement/move.cpp
    src/application.cpp
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
