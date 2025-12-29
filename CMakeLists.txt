cmake_minimum_required(VERSION 3.14)

project(RaytraCer)

option(BUILD_TESTS "Build Unit Tests" OFF)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For Windows: Prevent overriding the parent project's compiler/linker settings

if(BUILD_TESTS)
    set(TARGET_NAME "test_learnopengl")
else()
    set (TARGET_NAME "learnopengl")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(deps)
find_package(OpenGL REQUIRED)


if (MSVC)
    add_compile_options(/W4)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)

endif()

if(BUILD_TESTS)

    include(FetchContent)
    FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()



    add_executable(test_learnopengl Tests/test_test.cpp)
    target_include_directories(test_learnopengl PUBLIC Sources/)
    target_link_libraries(${TARGET_NAME} PUBLIC GTest::gtest_main asan  glm::glm imgui implot glad glfw OpenGL::GL spdlog::spdlog glm::glm)

    include(GoogleTest)

    add_subdirectory(Sources/)
    add_subdirectory(Tests)
    gtest_discover_tests(${TARGET_NAME})


else()


    add_executable(learnopengl)

    add_subdirectory(Sources/)
    target_link_libraries(learnopengl PUBLIC imgui implot glad glfw OpenGL::GL spdlog::spdlog glm::glm)
    target_include_directories(learnopengl PUBLIC Sources/)

endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



#copy shaders for run time compile
file(COPY ${PROJECT_SOURCE_DIR}/Sources/Shaders
     DESTINATION ${PROJECT_BINARY_DIR}/Shaders
)

# copy assets
file(COPY ${PROJECT_SOURCE_DIR}/Assets/
     DESTINATION ${PROJECT_BINARY_DIR}/Assets
)
