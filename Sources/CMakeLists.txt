target_sources(${TARGET_NAME} PRIVATE
    "Scene.cpp"
    "Camera.cpp"
    "CubeTexture.cpp"
    "DebugInfos.cpp"
    "Entity.cpp"
    "Model.cpp"
    "Shader.cpp"
    "utils.cpp"
)

add_subdirectory(World)
add_subdirectory(Renderer)
add_subdirectory(Mathematics)

if(NOT BUILD_TESTS)
    target_sources(${TARGET_NAME} PRIVATE
    "main.cpp"
)
endif()
