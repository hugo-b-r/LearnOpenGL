cmake_minimum_required(VERSION 3.14)

project(RaytraCer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)




add_subdirectory(deps/glad)
add_executable(learnopengl "Sources/main.cpp" "Sources/Shader.cpp" "Sources/Shaders/shader.fs" "Sources/Shaders/shader.vs")

target_link_libraries(learnopengl PRIVATE glad glfw OpenGL::GL)

file(COPY ${PROJECT_SOURCE_DIR}/Sources/Shaders  
     DESTINATION ${PROJECT_BINARY_DIR}/Shaders
)  