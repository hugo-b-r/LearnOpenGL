cmake_minimum_required(VERSION 3.14)

project(RaytraCer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


option(BUILD_TESTS "Build Unit Tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(deps/glad)
add_subdirectory(deps/spdlog)
add_subdirectory(deps/glm)
add_subdirectory(deps/imgui)
add_subdirectory(deps/implot)

add_compile_options(-Wall -Wextra -Wpedantic)


add_executable(learnopengl)

add_subdirectory(Sources/)
target_link_libraries(learnopengl PRIVATE imgui implot glad glfw OpenGL::GL spdlog::spdlog glm::glm)

#copy shaders for run time compile
file(COPY ${PROJECT_SOURCE_DIR}/Sources/Shaders
     DESTINATION ${PROJECT_BINARY_DIR}/Shaders
)

# copy assets
file(COPY ${PROJECT_SOURCE_DIR}/Assets/
     DESTINATION ${PROJECT_BINARY_DIR}/Assets
)
