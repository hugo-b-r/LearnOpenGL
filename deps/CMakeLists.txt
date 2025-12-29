
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(glfw)
find_package(OpenGL REQUIRED)


# ImGui sources
set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
    imgui/
    imgui/backends/
)

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)


# ImGui sources
set(IMPLOT_SOURCES
    implot/implot.cpp
    implot/implot_items.cpp
)



add_library(implot STATIC ${IMPLOT_SOURCES})

target_include_directories(implot PUBLIC
    implot/
)

target_link_libraries(implot PUBLIC imgui glfw OpenGL::GL)
