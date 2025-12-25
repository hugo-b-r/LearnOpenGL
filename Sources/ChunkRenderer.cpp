#include "ChunkRenderer.h"
#include "Camera.h"
#include "Chunk.h"
#include "ChunkMeshCreator.h"
#include "CubeTexture.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "../deps/glm/glm/gtx/string_cast.hpp"

#include <iostream>

const int CUBE_NB_VERTICES = 36;
const glm::mat4 model_mat(1.0f);



ChunkRenderer::ChunkRenderer(Chunk& c, glm::vec3 pos) : m_chunk_data(c), m_texture("./Assets/texture_atlas.jpg"), m_view_mat(1.f), m_chunkMeshCreator(c, m_texture), m_modelLoaded(false) {
    m_chunkMeshCreator.buildMesh();
    m_view_mat = glm::translate(m_view_mat, pos);
}

void ChunkRenderer::render(Shader &shader, Camera &cam) {
    if (m_chunkMeshCreator.isFinished()) {
        if (!m_modelLoaded) {
            spdlog::debug("data loaded");
            m_model.loadData(m_chunkMeshCreator.getMesh(), {3, 2});
            m_modelLoaded = true;
            m_nb_vertices = m_chunkMeshCreator.nbVertices();
        }
        //spdlog::debug(" size : {}", m_chunkMeshCreator.getMesh().vertexPositions);
        shader.use();
        m_model.bindVAO();
        m_texture.bind();
        glm::mat4 projection = cam.getProjectionViewMatrix();
        shader.setMat4("model", model_mat);
        shader.setMat4("view", m_view_mat);
        shader.setMat4("projection", projection);
        // render container
        glDrawArrays(GL_TRIANGLES, 0, m_nb_vertices);
    } else {
        spdlog::debug("stille not finished");
    }

}
