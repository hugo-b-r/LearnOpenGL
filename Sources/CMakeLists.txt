target_sources(learnopengl PRIVATE
    "Camera.cpp"
    "CubeTexture.cpp"
    "DebugInfos.cpp"
    "Entity.cpp"
    "main.cpp"
    "Model.cpp"
    "Shader.cpp"
    "utils.cpp"
)

add_subdirectory(World)
add_subdirectory(Renderer)
