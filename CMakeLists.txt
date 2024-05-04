cmake_minimum_required(VERSION 3.22)
project(Transformer)

set(CMAKE_CXX_STANDARD 11)

add_executable(Transformer main.cpp)

# GLFW
add_subdirectory(libs/glfw)
target_link_libraries(Transformer glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(Transformer OpenGL::GL)

# imgui
set(SOURCE_FILES libs/imgui/imgui.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_demo.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_widgets.cpp libs/imgui/imgui_impl_glfw.cpp libs/imgui/imgui_impl_opengl3.cpp) # Add all .cpp files of project root directory as source files
add_library(imgui ${SOURCE_FILES})
target_link_libraries(imgui glfw)

target_link_libraries(Transformer imgui)
