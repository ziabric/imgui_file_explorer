cmake_minimum_required(VERSION 3.7)

project(example)

find_package(SFML COMPONENTS window graphics REQUIRED)

file(GLOB
    EXAMPLE
    "example/*.cpp"
)

file(GLOB
    IMGUI_SOURCE
    "thirdparty/*.h"
    "thirdparty/*.cpp"
)

add_executable(example ${EXAMPLE} ${IMGUI_SOURCE})

target_include_directories(
    example 
    PUBLIC 
    ./thirdparty/
    ./ImGuiFileExplorer/
)

target_link_libraries(example PUBLIC sfml-window sfml-graphics GL)