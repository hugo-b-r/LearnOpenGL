target_sources(${TARGET_NAME} PRIVATE
    "Noise.cpp"
)

if(BUILD_TESTS)
    target_sources(${TARGET_NAME} PRIVATE
        "Noise.cpp"
    )
endif()
