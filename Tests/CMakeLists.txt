option(BUILD_TESTS "Build Unit Tests" OFF)

if(BUILD_TESTS)

    add_compile_options(-fsanitize=address,leak -g)


    target_sources(${TARGET_NAME} PRIVATE test_utils.cpp test_frustum.cpp)

    add_subdirectory(Camera)
    add_subdirectory(Entity)


endif()
