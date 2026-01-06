target_sources(${TARGET_NAME} PRIVATE
    "Chunk.cpp"
    "ChunkMeshCreator.cpp"
    "World.cpp"
    "WorldGenerator.cpp"
)

if(BUILD_TESTS)
    target_sources(${TARGET_NAME} PRIVATE
        "Chunk.cpp"
        "ChunkMeshCreator.cpp"
        "World.cpp"
        "WorldRepository.cpp"
        "WorldGenerator.cpp"
    )
else()

endif()
