cmake_minimum_required(VERSION 3.22)
project(Transformer)

set(CMAKE_CXX_STANDARD 11)

add_executable(Transformer main.cpp)

include(CPM.cmake)

# GLFW
CPMAddPackage(
        NAME GLFW
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.4
        OPTIONS
        "GLFW_BUILD_TESTS OFF"
        "GLFW_BUILD_EXAMPLES OFF"
        "GLFW_BULID_DOCS OFF"
)

if(GLFW_FOUND)
    target_link_libraries (Transformer glfw)
endif()

find_package(OpenGL REQUIRED)
target_link_libraries(Transformer OpenGL::GL)

# imgui
set(SOURCE_FILES libs/imgui/imgui.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_demo.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_widgets.cpp libs/imgui/imgui_impl_glfw.cpp libs/imgui/imgui_impl_opengl3.cpp)
add_library(imgui ${SOURCE_FILES})
target_link_libraries(imgui glfw)

target_link_libraries(Transformer imgui)

# Eigen
CPMAddPackage(
        NAME Eigen
        VERSION 3.4.0
        URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
        # Eigen's CMakelists are not intended for library use
        DOWNLOAD_ONLY YES
)

if(Eigen_ADDED)
    add_library(Eigen INTERFACE IMPORTED)
    target_include_directories(Eigen INTERFACE ${Eigen_SOURCE_DIR})
    target_link_libraries (Transformer Eigen)
endif()
