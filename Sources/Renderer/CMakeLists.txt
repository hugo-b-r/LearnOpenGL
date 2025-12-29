target_sources(${TARGET_NAME} PRIVATE
    "NonTexturedCubeRenderer.cpp"
    "QuadRenderer.cpp"
    "TexturedCubeRenderer.cpp"
    "ChunkRenderer.cpp"

)

if(BUILD_TESTS)
    target_sources(${TARGET_NAME} PRIVATE
        "NonTexturedCubeRenderer.cpp"
        "QuadRenderer.cpp"
        "TexturedCubeRenderer.cpp"
        "ChunkRenderer.cpp"

)
endif()
