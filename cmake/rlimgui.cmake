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
