include_guard(GLOBAL)
include(FetchContent)

# FetchContent
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        6.0
)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.92.7
)
FetchContent_Declare(
  rlimgui
  GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
  GIT_TAG        Raylib_6_0
)
FetchContent_MakeAvailable(raylib imgui rlimgui)

# imgui
add_library(imgui STATIC)
add_library(imgui::imgui ALIAS imgui)
target_sources(imgui
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      ${imgui_SOURCE_DIR}
    FILES
      ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
      ${imgui_SOURCE_DIR}/imgui.h
  PRIVATE
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

add_executable(imgui_binary_to_compressed_c EXCLUDE_FROM_ALL
  ${imgui_SOURCE_DIR}/misc/fonts/binary_to_compressed_c.cpp
)

# rlimgui
add_library(rlimgui STATIC)
add_library(rlimgui::rlimgui ALIAS rlimgui)
target_link_libraries(rlimgui
  PUBLIC
    imgui::imgui
    raylib
)
target_sources(rlimgui
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      ${rlimgui_SOURCE_DIR}
    FILES
      ${rlimgui_SOURCE_DIR}/extras/FA6FreeSolidFontData.h
      ${rlimgui_SOURCE_DIR}/extras/IconsFontAwesome6.h
      ${rlimgui_SOURCE_DIR}/rlImGui.h
      ${rlimgui_SOURCE_DIR}/rlImGuiColors.h
  PRIVATE
    ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)

function(rlimgui_embed_font)
  set(one_value_arguments TARGET FONT SYMBOL)
  cmake_parse_arguments(ARG "" "${one_value_arguments}" "" ${ARGN})

  foreach(required_argument IN ITEMS TARGET FONT SYMBOL)
    if(NOT ARG_${required_argument})
      message(FATAL_ERROR "rlimgui_embed_font requires ${required_argument}")
    endif()
  endforeach()

  if(NOT TARGET ${ARG_TARGET})
    message(FATAL_ERROR "rlimgui_embed_font target does not exist: ${ARG_TARGET}")
  endif()

  get_filename_component(font "${ARG_FONT}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
  set(font_data "${generated_dir}/${ARG_SYMBOL}_font_data.hpp")
  set(generator "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/generate_embedded_font.cmake")

  add_custom_command(
    OUTPUT "${font_data}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${generated_dir}"
    COMMAND ${CMAKE_COMMAND}
      "-DCONVERTER=$<TARGET_FILE:imgui_binary_to_compressed_c>"
      "-DINPUT=${font}"
      "-DOUTPUT=${font_data}"
      "-DSYMBOL=${ARG_SYMBOL}"
      -P "${generator}"
    DEPENDS
      imgui_binary_to_compressed_c
      "${font}"
      "${generator}"
    COMMENT "Embedding ${ARG_FONT}"
    VERBATIM
  )

  target_include_directories(${ARG_TARGET} PRIVATE "${generated_dir}")
  target_sources(${ARG_TARGET} PRIVATE "${font_data}")
endfunction()
