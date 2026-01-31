#include "CubeTexture.h"
#include "Shader.h"
#include "glm/ext/matrix_transform.hpp"
#include <Camera.h>
#include <GLFW/glfw3.h>
#include <Renderer/ChunkRenderer.h>
#include <World/Chunk.h>
#include <World/ChunkMeshCreator.h>
#include <spdlog/spdlog.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "../deps/glm/glm/gtx/string_cast.hpp"


ChunkRenderer::ChunkRenderer()
    : m_texture("./Assets/texture_atlas.jpg"),
      m_shader("Sources/Shaders/textured_cube_vertex.glsl",
               "Sources/Shaders/textured_cube_fragment.glsl") {}

void ChunkRenderer::render(Chunk &c, Camera &cam, int nb_vertices, bool bindShader, bool bindTexture) {
  if (c.isLoaded() && c.hasMesh() && c.hasOnGPUMesh()) {

    // spdlog::debug(" size : {}",
    // m_chunkMeshCreator.getMesh().vertexPositions);
    if (bindShader)
        m_shader.use();
    c.bindModel();
    if (bindTexture)
        m_texture.bind();
    glm::mat4 modelMat = glm::mat4(1.f);
    modelMat = glm::translate(modelMat, c.getChunkPosCenter3());
    m_shader.setMat4("model", modelMat);
   // render container
    glDrawArrays(GL_TRIANGLES, 0, nb_vertices);
  }
}

Shader& ChunkRenderer::getShader() {
  return m_shader;
}


void ChunkRenderer::bindTextureAndShader() {
    m_texture.bind();
    m_shader.use();
}
