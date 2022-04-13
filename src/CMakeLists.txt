cmake_minimum_required(VERSION 3.0)
project(mdmE)

add_executable(mdmE
    "main.cpp"
    "bms.cpp"
    "ui.cpp"
    "IconsFontAwesome5.h"
 "kit.cpp" "editor.cpp" "data.cpp" "settings.cpp" "logs.cpp")

# add thirdparty libs
target_sources(mdmE PRIVATE
    # imgui
    "../include/imgui/imgui_demo.cpp"
    "../include/imgui/imgui_draw.cpp"
    "../include/imgui/imgui_tables.cpp"
    "../include/imgui/imgui_widgets.cpp"
    "../include/imgui/imgui.cpp"
    "../include/imgui/backends/imgui_impl_sdlrenderer.cpp"
    "../include/imgui/backends/imgui_impl_sdl.cpp"
    "../include/imgui/misc/cpp/imgui_stdlib.cpp"
    # imgui extension
    "../include/ImGuiFileDialog/ImGuiFileDialog.cpp"
    # kuba's zip
    "../include/zip/zip.c"
)

# external librairies
target_link_libraries(mdmE PRIVATE SDL2 SDL2main SDL2_image bass)