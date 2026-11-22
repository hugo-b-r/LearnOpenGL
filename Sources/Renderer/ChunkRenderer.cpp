#include "CubeTexture.h"
#include "Shader.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_int2.hpp"
#include "utils.h"
#include <Camera.h>
#include <GLFW/glfw3.h>
#include <Renderer/ChunkRenderer.h>
#include <World/Chunk.h>
#include <World/ChunkMeshCreator.h>
#include <spdlog/spdlog.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "../deps/glm/glm/gtx/string_cast.hpp"
#include "Camera.h"

ChunkRenderer::ChunkRenderer()
    : texture("./Assets/texture_atlas.png"),
      m_shader("Sources/Shaders/textured_cube_vertex.glsl",
               "Sources/Shaders/textured_cube_fragment.glsl") {}

void ChunkRenderer::doOneQueuedChunkRemovalIfNeeded() {
        chunks_models_to_remove_lock.lock();
        if (chunks_models_to_remove.size() > 0) {
                glm::ivec2 toRemove = chunks_models_to_remove.front();
                removeChunkFromRender(toRemove);
                chunks_models_to_remove.pop();
        }
        chunks_models_to_remove_lock.unlock();
}

void ChunkRenderer::render(Camera &cam) {

        // do the rendering
        getShader().setVec3("lightPos", glm::vec3(0, 300, 0));
        getShader().setVec3("viewPos", cam.position);
        bindTextureAndShader();

        //  Draw each chunk
        getShader().setMat4("view", view_mat);
        getShader().setMat4("projection", cam.getProjectionViewMatrix());

        m_shader.use();
        texture.bind();
        glm::mat4 modelMat = glm::mat4(1.f);
        for (auto it = models_for_render.begin(); it != models_for_render.end();
             ++it) {

                if (cam.getFrustum().IsAabbVisible(it->boundingBox)) {
                        // spdlog::debug("there is one rendered");
                        it->model.bindVAO();
                        modelMat = glm::mat4(1.f);
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

void ChunkRenderer::addChunkToRender(glm::ivec2 abs_pos, AABB bounding_box,
                                     Mesh *mesh) {
        // load on the gpu
        Model m(mesh);

        m.loadData({3, 2, 3});

        // models_lock.lock();

        // spdlog::debug("bounded box : {}{}{}{}", bounding_box.getVmax().x,
        //               bounding_box.getVmin().x, bounding_box.getVmax().y,
        //               bounding_box.getVmin().y);

        models_for_render.push_front(LocalizedBoundedModel(
            m, bounding_box, abs_pos, mesh->getNbVertices(),
            fromChunkToBlockCoordinatesOfCenterOfChunk(abs_pos)));

        // for (const auto& el: models_for_render) {
        //         spdlog::debug("{}:{}", el.center_pos.x, el.center_pos.y);
        // }
        // models_lock.unlock();

        loaded_pos_lock.lock();
        loaded_pos.insert(abs_pos);
        loaded_pos_lock.unlock();

        // delete mesh;
        // w->IdontNeedAnymore(abs_pos, ReasonToKeep::UsedForMeshing);
}

void ChunkRenderer::replaceChunkToRender(glm::ivec2 abs_pos, AABB bounding_box,
                                     Mesh *mesh) {

        // load on the gpu
        Model m(mesh);

        m.loadData({3, 2, 3});

        // models_lock.lock();

        // spdlog::debug("bounded box : {}{}{}{}", bounding_box.getVmax().x,
        //               bounding_box.getVmin().x, bounding_box.getVmax().y,
        //               bounding_box.getVmin().y);

        removeChunkFromRender(abs_pos);
        models_for_render.push_front(LocalizedBoundedModel(
            m, bounding_box, abs_pos, mesh->getNbVertices(),
            fromChunkToBlockCoordinatesOfCenterOfChunk(abs_pos)));

        // for (const auto& el: models_for_render) {
        //         spdlog::debug("{}:{}", el.center_pos.x, el.center_pos.y);
        // }
        // models_lock.unlock();

        loaded_pos_lock.lock();
        loaded_pos.insert(abs_pos);
        loaded_pos_lock.unlock();

        // delete mesh;
        // w->IdontNeedAnymore(abs_pos, ReasonToKeep::UsedForMeshing);
}

void ChunkRenderer::sendCommandToRemoveChunk(glm::ivec2 chunk_pos) {
        chunks_models_to_remove_lock.lock();
        chunks_models_to_remove.emplace(chunk_pos);
        chunks_models_to_remove_lock.unlock();
}

bool ChunkRenderer::isChunkInRenderer(glm::ivec2 abs_pos) {
        loaded_pos_lock.lock();
        bool res = loaded_pos.contains(abs_pos);
        loaded_pos_lock.unlock();
        return res;
}

void ChunkRenderer::removeChunkFromRender(glm::ivec2 chunk_pos) {
        loaded_pos_lock.lock();
        loaded_pos.insert(chunk_pos);
        loaded_pos_lock.unlock();
        // std::cout << "trying to erase" << std::endl;
        models_for_render.remove_if([&](const LocalizedBoundedModel &it) {
                return it.pos == chunk_pos;
        });
}
