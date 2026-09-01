# doctest
include(FetchContent)
FetchContent_Declare(
      doctest
      GIT_REPOSITORY https://github.com/doctest/doctest.git
      GIT_TAG        v2.5.2
  )
FetchContent_MakeAvailable(doctest)

# antbox_unit
add_executable(antbox_unit
  src/application/tick_accumulator_tests.cpp
  src/math/vec2_tests.cpp
  src/simulation/ant/steer_home_tests.cpp
  src/simulation/movement/move_tests.cpp

  src/system_fixture.hpp

  src/main.cpp
)
target_compile_definitions(antbox_unit PRIVATE DOCTEST_CONFIG_USE_STD_HEADERS)

target_link_libraries(antbox_unit
    PRIVATE
        antbox::antbox
        antbox_warnings
        doctest::doctest
)

target_compile_options(antbox_unit
  PRIVATE
  -fexceptions # exception for doctest REQUIRE
)

# discover tests
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(antbox_unit PROPERTIES LABELS unit)
