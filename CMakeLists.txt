cmake_minimum_required(VERSION 3.16)

project(OSEmulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)

add_executable(OSEmulator

    src/main.cpp

    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp

    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
 "desktop/desktop.h" "desktop/desktop.cpp" "desktop/background.h" "desktop/background.cpp" "desktop/taskbar.h" "desktop/taskbar.cpp" "desktop/datetimecontroller.h" "desktop/datetimecontroller.cpp" "desktop/powermenu.h" "desktop/powermenu.cpp" "desktop/imguielement.h" "desktop/imguielement.cpp" "desktop/desktopwindows.h" "desktop/desktopwindows.cpp" "desktop/process.h" "desktop/process.cpp" "desktop/taskmanager.h" "desktop/taskmanager.cpp")

target_include_directories(OSEmulator PRIVATE
    include
    external/imgui
    external/imgui/backends
)

target_link_libraries(OSEmulator PRIVATE glfw)

if(WIN32)
    target_link_libraries(OSEmulator PRIVATE opengl32)
elseif(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(OSEmulator PRIVATE
        ${OpenGL_LIBRARY}
        "-framework Cocoa"
        "-framework IOKit"
    )
else()
    target_link_libraries(OSEmulator PRIVATE GL)
endif()

# Forces to copy the resources folder to the build output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/desktop
    # ... keep any other existing include directories here, like imgui ...
)