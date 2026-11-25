cmake_minimum_required(VERSION 3.22.1)
project(internaltest LANGUAGES CXX)
include(FetchContent)
FetchContent_Declare(imgui
    URL https://github.com/ocornut/imgui/archive/993fa347495860ed44b83574254ef2a317d0c14f.tar.gz
    )
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()
add_library(internaltest SHARED main.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
target_include_directories(internaltest PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_compile_features(internaltest PRIVATE cxx_std_17)
target_compile_definitions(internaltest PRIVATE IMGUI_IMPL_OPENGL_ES3)
target_link_libraries(internaltest PRIVATE GLESv3 log android)
