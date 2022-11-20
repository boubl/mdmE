cmake_minimum_required(VERSION 3.0)
project(mdmE)

add_executable(mdmE
    "../icon.rc"
    "main.cpp"
    "bms.cpp"
    "ui.cpp"
    "kit.cpp"
    "editor.cpp"
    "data.cpp"
    
      "chaneditor.cpp" "locales.cpp" "audio.cpp" "drpc.cpp"  "commands/add_note.cpp" "helpers/bms.cpp" "data/project/project.cpp" "helpers/json.cpp" "data/project/chart.cpp" "data/project/measure.cpp")

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
    "../include/tinyfiledialogs/tinyfiledialogs.c"
    # kuba's zip
    "../include/zip/zip.c"
    # discord
    "../include/discord/achievement_manager.cpp"
    "../include/discord/achievement_manager.h"
    "../include/discord/activity_manager.cpp"
    "../include/discord/activity_manager.h"
    "../include/discord/application_manager.cpp" "../include/discord/application_manager.h"
    "../include/discord/core.cpp" "../include/discord/core.h"
    "../include/discord/discord.h"
    "../include/discord/event.h"
    "../include/discord/ffi.h"
    "../include/discord/image_manager.cpp" "../include/discord/image_manager.h"
    "../include/discord/lobby_manager.cpp" "../include/discord/lobby_manager.h"
    "../include/discord/network_manager.cpp" "../include/discord/network_manager.h"
    "../include/discord/overlay_manager.cpp" "../include/discord/overlay_manager.h"
    "../include/discord/relationship_manager.cpp" "../include/discord/relationship_manager.h"
    "../include/discord/storage_manager.cpp" "../include/discord/storage_manager.h"
    "../include/discord/store_manager.cpp" "../include/discord/store_manager.h"
    "../include/discord/types.cpp" "../include/discord/types.h"
    "../include/discord/user_manager.cpp" "../include/discord/user_manager.h"
    "../include/discord/voice_manager.cpp" "../include/discord/voice_manager.h"
)
# external librairies
target_link_libraries(mdmE PRIVATE SDL2 SDL2main SDL2_ttf SDL2_image bass discord_game_sdk)