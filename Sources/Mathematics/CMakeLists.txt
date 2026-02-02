target_sources(${TARGET_NAME} PRIVATE
    "Noise.cpp"
    "Frustum.cpp"
    "AABB.cpp"
)

if(BUILD_TESTS)
    target_sources(${TARGET_NAME} PRIVATE
        "Noise.cpp"
        "Frustum.cpp"
    )
endif()
