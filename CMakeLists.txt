cmake_minimum_required(VERSION 3.30)
project(antbox VERSION 0.1.0 LANGUAGES CXX)

#
set(CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# dependencies
include(FetchContent)

FetchContent_Declare(
  ant
  GIT_REPOSITORY https://github.com/3uclid3/ant.git
  GIT_TAG a2337a95e051f5665a2e432356bbef401db719bf
)

FetchContent_MakeAvailable(ant)

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
    antbox_warnings
)
target_compile_features(antbox PUBLIC cxx_std_26)
target_compile_options(antbox PUBLIC -freflection) # experimental
target_sources(antbox
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/antbox/dummy.hpp
  PRIVATE
    src/dummy.cpp
)

# antbox.app
add_executable(antbox.app app/main.cpp)
set_target_properties(antbox.app PROPERTIES OUTPUT_NAME antbox)
target_link_libraries(antbox.app PRIVATE antbox_warnings antbox::antbox)

# tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  option(ANTBOX_BUILD_TESTS "Build antbox tests" ON)
else()
  option(ANTBOX_BUILD_TESTS "Build antbox tests" OFF)
endif()

if(ANTBOX_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests/unit)
endif()
