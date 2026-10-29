#include "CubeTexture.h"
#include "Shader.h"
#include "World/LockableChunk.h"
#include "glm/ext/matrix_transform.hpp"
#include "utils.h"
#include <Camera.h>
#include <GLFW/glfw3.h>
#include <Renderer/ChunkRenderer.h>
#include <World/Chunk.h>
#include <World/ChunkMeshCreator.h>
#include <iostream>
#include <spdlog/spdlog.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "../deps/glm/glm/gtx/string_cast.hpp"
#include "Camera.h"

ChunkRenderer::ChunkRenderer()
    : texture("./Assets/texture_atlas.png"),
      m_shader("Sources/Shaders/textured_cube_vertex.glsl",
               "Sources/Shaders/textured_cube_fragment.glsl") {}

// void ChunkRenderer::render(Chunk &c, Camera &cam, int nb_vertices,
//                            bool bindShader, bool bindTexture) {
//         // if (c.isLoaded() && c.hasMesh() && c.hasOnGPUMesh()) {
//                 if (bindShader)
//                         m_shader.use();
//                 // c.bindModel();
//                 if (bindTexture)
//                         texture.bind();
//                 glm::mat4 modelMat = glm::mat4(1.f);
//                 modelMat = glm::translate(modelMat, c.getChunkPosCenter3());
//                 m_shader.setMat4("model", modelMat);
//                 // render container
//                 glDrawArrays(GL_TRIANGLES, 0, nb_vertices);
//         // }
// }

void ChunkRenderer::render(Camera &cam) {
        m_shader.use();
        texture.bind();
        glm::mat4 modelMat = glm::mat4(1.f);
        for (auto it = models_for_render.begin(); it != models_for_render.end();
             ++it) {
                if (cam.getFrustum().IsAabbVisible(it->boundingBox)) {
                        it->model.bindVAO();
                        modelMat = glm::translate(modelMat, it->center_pos);
                        m_shader.setMat4("model", modelMat);
                        // render container
                        glDrawArrays(GL_TRIANGLES, 0, it->nb_vertices);
                }
        }
}

Shader &ChunkRenderer::getShader() { return m_shader; }

void ChunkRenderer::bindTextureAndShader() {
        texture.bind();
        m_shader.use();
}

void ChunkRenderer::addChunkToRender(glm::ivec2 abs_pos, World *w, AABB bounding_box, Mesh * mesh) {
        // load on the gpu
        Model m(*mesh);

        models_lock.lock();

        models_for_render.push_front(LocalizedBoundedModel(m, bounding_box, abs_pos, mesh->getNbVertices(), fromChunkToBlockCoordinatesOfCenterOfChunk(abs_pos)));

        models_lock.unlock();
        // w->IdontNeedAnymore(abs_pos, ReasonToKeep::UsedForMeshing);
}

