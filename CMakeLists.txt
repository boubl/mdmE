cmake_minimum_required(VERSION 3.14...3.22)

project(
  mdmE
  VERSION 0.1.0
  LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME nlohmann_json
  VERSION 3.11.2
  # the git repo is incredibly large, so we download the archived include directory
  URL https://github.com/nlohmann/json/releases/download/v3.11.2/include.zip
  URL_HASH SHA256=e5c7a9f49a16814be27e4ed0ee900ecd0092bfb7dbfca65b5a421b774dccaaed
)

if (nlohmann_json_ADDED)
  add_library(nlohmann_json INTERFACE IMPORTED)
  target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR}/include)
endif()
CPMAddPackage("gh:realboubli/miniaudio@0.11.11")
CPMAddPackage("gh:kuba--/zip@0.2.6")
CPMAddPackage(
  NAME portable_file_dialogs
  GIT_REPOSITORY https://github.com/samhocevar/portable-file-dialogs.git
  GIT_TAG 0.1.0
  VERSION 0.1.0
)
CPMAddPackage(
  NAME dirent
  GIT_REPOSITORY https://github.com/tronkko/dirent.git
  GIT_TAG 1.23.2
  VERSION 1.23.2
)
CPMAddPackage(
  NAME SDL
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.26.1
  VERSION 2.26.1
)
CPMAddPackage(
  NAME imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.89.1
  VERSION v1.89.1
  DOWNLOAD_ONLY YES
)
if (imgui_ADDED)
  # imgui has no CMake support, so we create our own target

  add_library(imgui STATIC
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp"
    "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"
  )

  target_include_directories(imgui
    PUBLIC
      $<BUILD_INTERFACE:${imgui_SOURCE_DIR}>
  )
endif()


include_directories(
    "./src/"
    ${imgui_SOURCE_DIR}
    ${miniaudio_SOURCE_DIR}
    ${zip_SOURCE_DIR}/src
    ${portable_file_dialogs_SOURCE_DIR}
    ${dirent_SOURCE_DIR}/include
    ${nlohmann_json_SOURCE_DIR}/include
    ${SDL_SOURCE_DIR}/include
)

link_directories(
    "lib/"
)

add_subdirectory("src")

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}/src)
file(GLOB DLLS "lib/*.dll")
file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR}/src)
