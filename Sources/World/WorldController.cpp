#include "World/WorldController.h"
#include "Renderer/ChunkRenderer.h"
#include "Scene.h"
#include "World/LockableChunk.h"
#include "World/WorldQueueOperation.h"

#include <iostream>
#include <spdlog/spdlog.h>

WorldController::WorldController(World *w, ChunkRenderer *r, Game *g,
                                 int render_distance)
    : world(w), renderer(r), chunkWidth(2 * (render_distance + 1) + 1), game(g),
      lastPlayerChunkPos(glm::ivec2(0, 0)) {
        // init world by loading a lot of chunks and tell I need them
}

WorldController::~WorldController() {}

void WorldController::perFrameControl(glm::ivec2 new_player_pos) {
        // check if player pos moved
        // if so move the world accordingly
        
        for (int i = 0; i < number_chunks_to_delete_at_a_time; ++i) {
                renderer->doOneQueuedChunkRemovalIfNeeded();
        }

        if (new_player_pos != lastPlayerChunkPos) {
                spdlog::debug("moving world hehe !");
                moveWorld(new_player_pos);
        }

        meshes_to_load_lock.lock();
        if (meshes_to_load.size() > 0) {
                loadOneMeshOnGpu();
        }
        meshes_to_load_lock.unlock();
}

void WorldController::removeChunkFromWorld(glm::ivec2 abs_pos) {
        chunk_properties_lock.lock();
        chunk_properties[abs_pos].in_world = false;
        if (chunk_properties[abs_pos].in_renderer == false) {
                chunk_properties.erase(abs_pos);
        }
        chunk_properties_lock.unlock();

        world->unloadChunk(abs_pos);
}

void WorldController::removeChunkFromGPU(glm::ivec2 abs_pos) {
        chunk_properties_lock.lock();
        chunk_properties[abs_pos].in_renderer = false;
        if (chunk_properties[abs_pos].in_world == false) {
                chunk_properties.erase(abs_pos);
        }
        chunk_properties_lock.unlock();
        renderer->sendCommandToRemoveChunk(abs_pos);
}

void WorldController::removeChunkHandleWorldAndGPU(glm::ivec2 chunk_pos) {}

void WorldController::moveWorld(glm::ivec2 new_player_pos) {
        int manhattan_radius_limit = game->configuration.renderDistance;

        world->chunk_map_locker.lock();

        for (int i = lastPlayerChunkPos.x - manhattan_radius_limit - 1;
             i <= lastPlayerChunkPos.x + manhattan_radius_limit + 1; ++i) {

                for (int j = lastPlayerChunkPos.y - manhattan_radius_limit - 1;
                     j <= lastPlayerChunkPos.y + manhattan_radius_limit + 1;
                     ++j) {

                        int diff_x = abs(i - new_player_pos.x);
                        int diff_y = abs(j - new_player_pos.y);
                        if (!(diff_y <= manhattan_radius_limit &&
                              diff_x <= manhattan_radius_limit)) {
                                // std::cout << "heho la !" << std::endl;
                                auto pos = glm::ivec2(i, j);
                                removeChunkFromGPU(pos);
                                if (!(diff_y <= manhattan_radius_limit &&
                                      diff_x <= manhattan_radius_limit)) {
                                        removeChunkFromWorld(pos);
                                }
                        }
                }
        }
        world->chunk_map_locker.unlock();

        for (int i = new_player_pos.x - manhattan_radius_limit;
             i <= new_player_pos.x + manhattan_radius_limit; ++i) {

                for (int j = new_player_pos.y - manhattan_radius_limit;
                     j <= new_player_pos.y + manhattan_radius_limit; ++j) {

                        auto pos = glm::ivec2(i, j);
                        if (!world->isChunkLoaded(pos))
                                game->thread_pool_manager->pushBackOperation(
                                    WorldQueueOperation(
                                        pos, WldQOperationType::
                                                 LoadOrGenerateThenMesh));

                        spdlog::debug("send task to create mesh at {} : {}", i,
                                      j);

                        if (world->isChunkLoaded(pos) &&
                            !renderer->isChunkInRenderer(pos)) {
                                spdlog::debug("add to create mesh");
                                game->thread_pool_manager->pushBackOperation(
                                    WorldQueueOperation(
                                        pos, WldQOperationType::CreateMesh));
                        }
                }
        }
        lastPlayerChunkPos = new_player_pos;
}

void WorldController::loadOneMeshOnGpu() {
        MeshToLoadBundle &meshB = meshes_to_load.front();
        meshes_to_load.pop();
        chunk_properties_lock.lock();
        if (chunk_properties.contains(meshB.abs_pos) &&
            !chunk_properties.at(meshB.abs_pos).in_renderer) {
                renderer->addChunkToRender(meshB.abs_pos, world,
                                           meshB.bounding_box, meshB.ptr);
                chunk_properties.at(meshB.abs_pos).in_renderer = true;
        }
        chunk_properties_lock.unlock();
}

void WorldController::addMeshAndGPULoadChunkJob(std::shared_ptr<Chunk> chunk) {
        glm::ivec2 chunk_pos = chunk.get()->getChunkPos();

        chunk_properties_lock.lock();
        chunk_properties.insert({chunk.get()->getChunkPos(), {true, false}});
        chunk_properties_lock.unlock();
        world->loadChunk(chunk);
        chunk_properties_lock.lock();
        chunk_properties[chunk_pos].in_world = true;
        game->thread_pool_manager->pushBackOperation(WorldQueueOperation(
            chunk.get()->getChunkPos(), WldQOperationType::CreateMesh));

        for (const auto &key : {
                 chunk_pos + glm::ivec2(1, 0),
                 chunk_pos + glm::ivec2(1, 0),
                 chunk_pos + glm::ivec2(-1, 0),
                 chunk_pos + glm::ivec2(0, 1),
                 chunk_pos + glm::ivec2(0, -1),
             }) {
                if (chunk_properties.contains(key) &&
                    !chunk_properties.at(key).in_renderer &&
                    chunk_properties.at(key).in_world) {
                        spdlog::debug("haha !!!");
                        game->thread_pool_manager->pushBackOperation(
                            WorldQueueOperation(key,
                                                WldQOperationType::CreateMesh));
                }
        }

        chunk_properties_lock.unlock();
}

void WorldController::addMeshToLoadJob(glm::ivec2 abs_pos, Mesh *mesh,
                                       AABB bounding_box) {
        meshes_to_load_lock.lock();
        meshes_to_load.emplace(MeshToLoadBundle(mesh, abs_pos, bounding_box));
        meshes_to_load_lock.unlock();
}



void WorldController::initialLoad(
    WorldParallelOperationsManager *thread_pool_manager) {
        spdlog::debug("Starting to load the world...");
        // load all the chunks in a default state
        int worldLimits = (chunkWidth - 1) / 2;

        for (int y = -worldLimits; y <= worldLimits; ++y) {
                glm::ivec2 chunk_pos = glm::ivec2(0 + lastPlayerChunkPos.x,
                                                  y + lastPlayerChunkPos.y);
                spdlog::debug("pshed {} {}", 0 + lastPlayerChunkPos.x,
                              y + lastPlayerChunkPos.y);
                thread_pool_manager->pushBackOperation(WorldQueueOperation(
                    chunk_pos, WldQOperationType::LoadOrGenerate));
        }

        for (int x = 1; x <= worldLimits; ++x) {
                for (int y = -worldLimits; y <= worldLimits; ++y) {
                        glm::ivec2 chunk_pos = glm::ivec2(
                            x + lastPlayerChunkPos.x, y + lastPlayerChunkPos.y);

                        thread_pool_manager->pushBackOperation(
                            WorldQueueOperation(
                                chunk_pos, WldQOperationType::LoadOrGenerate));
                        spdlog::debug("pshed {} {}", x + lastPlayerChunkPos.x,
                                      y + lastPlayerChunkPos.y);

                        chunk_pos = glm::ivec2(-x + lastPlayerChunkPos.x,
                                               y + lastPlayerChunkPos.y);

                        thread_pool_manager->pushBackOperation(
                            WorldQueueOperation(
                                chunk_pos, WldQOperationType::LoadOrGenerate));
                        spdlog::debug("pshed {} {}", -x + lastPlayerChunkPos.x,
                                      y + lastPlayerChunkPos.y);
                }
        }
        spdlog::debug("Finished loading the world");
}
