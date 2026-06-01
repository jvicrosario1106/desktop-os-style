cmake_minimum_required(VERSION 3.16)

project(OSEmulator)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)

add_executable(OSEmulator

    src/main.cpp

    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp

    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(OSEmulator PRIVATE
    include
    external/imgui
    external/imgui/backends
)

target_link_libraries(OSEmulator PRIVATE glfw)

if(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)

    target_link_libraries(OSEmulator PRIVATE
        ${OpenGL_LIBRARY}
        "-framework Cocoa"
        "-framework IOKit"
    )
endif()