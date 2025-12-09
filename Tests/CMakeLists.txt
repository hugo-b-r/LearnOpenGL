option(BUILD_TESTS "Build Unit Tests" OFF)

if(BUILD_TESTS)

    add_compile_options(-fsanitize=address,leak -g)

    add_executable(test_learnopengl test_test.cpp )

    target_link_libraries(test_learnopengl GTest::gtest_main asan)
    add_subdirectory(Camera)
    add_subdirectory(Entity)
    #gtest_discover_tests(test_learnopengl)

endif()
