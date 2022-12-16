cmake_minimum_required(VERSION 3.0)
project(mdmE)

add_library(deps INTERFACE)
# miniaudio
target_link_libraries(deps INTERFACE miniaudio)
# portable file dialogs
target_link_libraries(deps INTERFACE portable_file_dialogs)
# portable file dialogs
target_link_libraries(deps INTERFACE zip)

add_executable(mdmE
    "./main.cpp"
    "./bms.cpp"
    "./ui.cpp"
    "./kit.cpp"
    "./editor.cpp"
    "./data.cpp"
    "./chaneditor.cpp"
    "./locales.cpp"
    "./audio.cpp"
    "./commands/add_note.cpp"
    "./helpers/bms.cpp"
    "./data/project/project.cpp"
    "./helpers/json.cpp"
    "./data/project/chart.cpp"
    "./data/project/measure.cpp"
)

target_include_directories(mdmE PUBLIC
    "./"
    ${imgui_SOURCE_DIR}
    ${miniaudio_SOURCE_DIR}
    ${zip_SOURCE_DIR}/src
    ${portable_file_dialogs_SOURCE_DIR}
    ${dirent_SOURCE_DIR}/include
    ${nlohmann_json_SOURCE_DIR}/include
    ${SDL_SOURCE_DIR}/include
)

# external librairies
target_link_libraries(mdmE PRIVATE deps SDL2 SDL2main)