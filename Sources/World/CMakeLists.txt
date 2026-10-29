target_sources(${TARGET_NAME} PRIVATE
    "Chunk.cpp"
    "ChunkMeshCreator.cpp"
    "World.cpp"
    "WorldGenerator.cpp"
    "WorldQueueOperation.cpp"
    "WorldParallelOperationsManager.cpp"
    "LockableChunk.cpp"
    "WorldController.cpp"
)

if(BUILD_TESTS)
  target_sources(${TARGET_NAME} PRIVATE
        "Chunk.cpp"
        "ChunkMeshCreator.cpp"
        "World.cpp"
        "WorldGenerator.cpp"
        "WorldQueueOperation.cpp"
        "WorldParallelOperationsManager.cpp"
        "LockableChunk.cpp"
    )
else()

endif()
