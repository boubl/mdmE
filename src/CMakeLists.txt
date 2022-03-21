cmake_minimum_required(VERSION 3.0)
project(mdmE)

add_executable(mdmE
    "main.cpp"
    "bms.cpp"
    "ui.cpp"
    "IconsFontAwesome5.h"
)

#add imgui
target_sources(mdmE PRIVATE
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
)

# external librairies
target_link_libraries(mdmE PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(mdmE PRIVATE SDL2::SDL2_image)
target_link_libraries(mdmE PRIVATE kubazip)