#include "CubeTexture.h"
#include "Shader.h"
#include "glm/ext/matrix_transform.hpp"
#include <Camera.h>
#include <GLFW/glfw3.h>
#include <Renderer/ChunkRenderer.h>
#include <World/Chunk.h>
#include <World/ChunkMeshCreator.h>
#include <spdlog/spdlog.h>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "../deps/glm/glm/gtx/string_cast.hpp"

#include <iostream>

const int CUBE_NB_VERTICES = 36;
const glm::mat4 view_mat(1.0f);

ChunkRenderer::ChunkRenderer()
    : m_texture("./Assets/texture_atlas.jpg"),
      m_shader("Sources/Shaders/textured_cube_vertex.glsl",
               "Sources/Shaders/textured_cube_fragment.glsl") {}

void ChunkRenderer::render(Chunk &c, Camera &cam, int nb_vertices) {
  if (!c.isLoaded()) {
    c.load();
  }
  if (!c.hasMesh()) {
    c.createMesh();
  }
  if (!c.hasOnGPUMesh()) {
    c.loadMeshOnGPU();
  }

  // spdlog::debug(" size : {}", m_chunkMeshCreator.getMesh().vertexPositions);
  m_shader.use();
  c.bindModel();
  m_texture.bind();
  glm::mat4 projection = cam.getProjectionViewMatrix();
  glm::mat4 modelMat = glm::mat4(1.f);
  modelMat = glm::translate(modelMat, c.getChunkPosCenter3());
  m_shader.setMat4("model", modelMat);
  m_shader.setMat4("view", view_mat);
  m_shader.setMat4("projection", projection);
  // render container
  glDrawArrays(GL_TRIANGLES, 0, nb_vertices);
}
