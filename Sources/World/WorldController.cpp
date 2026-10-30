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

void WorldController::moveWorld(glm::ivec2 new_player_pos) {
        int manhattan_radius_limit = game->configuration.renderDistance+1;

        world->chunk_map_locker.lock();

        for (int i = lastPlayerChunkPos.x - manhattan_radius_limit - 1;
             i <= lastPlayerChunkPos.x + manhattan_radius_limit + 1; ++i) {

                for (int j = lastPlayerChunkPos.y - manhattan_radius_limit - 1;
                     j <= lastPlayerChunkPos.y + manhattan_radius_limit + 1;
                     ++j) {

                        if (!(abs(j - new_player_pos.y) <=
                                  manhattan_radius_limit  &&
                              abs(i - new_player_pos.x) <=
                                  manhattan_radius_limit )) {
                                // std::cout << "heho la !" << std::endl;
                                auto pos = glm::ivec2(i, j);
                                removeChunkFromGPU(pos);
                                removeChunkFromWorld(pos);
                        }
                }
        }
        // for (auto const &[key, value] : world->chunks) {
        //         int i = key.x;
        //         int j = key.y;
        //         // int j = it->first.y;
        //         if (!(abs(j - new_player_pos.y) <= manhattan_radius_limit &&
        //               abs(i - new_player_pos.x) <= manhattan_radius_limit)) {
        //                 // std::cout << "heho la !" << std::endl;
        //                 world->unloadChunk(key);
        //                 renderer->sendCommandToRemoveChunk(key);
        //         }
        // }
        world->chunk_map_locker.unlock();

        for (int i = new_player_pos.x - manhattan_radius_limit;
             i <= new_player_pos.x + manhattan_radius_limit; ++i) {

                for (int j = new_player_pos.y - manhattan_radius_limit;
                     j <= new_player_pos.y + manhattan_radius_limit; ++j) {

                        // if (abs(j - new_player_pos.y) <= render_d ||
                        //     abs(i - new_player_pos.x) <= render_d) {

                        auto pos = glm::ivec2(i, j);
                        if (!world->isChunkLoaded(pos))
                                game->thread_pool_manager->pushBackOperation(
                                    WorldQueueOperation(
                                        pos, WldQOperationType::
                                                 LoadOrGenerateThenMesh));

                        spdlog::debug("send task to create mesh at {} : {}", i,
                                      j);

                        if (world->isChunkLoaded(pos) && !renderer->isChunkInRenderer(pos)) {
                                spdlog::debug("add to create mesh");
                                game->thread_pool_manager->pushBackOperation(
                                    WorldQueueOperation(
                                        pos, WldQOperationType::CreateMesh));
                        }
                        // game->thread_pool_manager
                        //     ->pushBackOperation(
                        //         WorldQueueOperation(
                        //             glm::ivec2(i, j),
                        //             WldQOperationType::Load));
                        // }
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
// ) {
//         spdlog::debug("Starting to load the world...");
//         // load all the chunks in a default state
//         chunks_mutex.lock();
//         int worldLimits = (chunkWidth - 1) / 2;

//         for (int y = -worldLimits; y <= worldLimits; ++y) {
//                 glm::ivec2 chunk_pos = glm::ivec2(0 + lastChunkPlayerPos.x,
//                                                   y + lastChunkPlayerPos.y);
//                 thread_pool_manager->pushBackOperation(
//                     WorldQueueOperation(chunk_pos,
//                     WldQOperationType::kLoad));
//         }

//         for (int x = 1; x <= worldLimits; ++x) {
//                 for (int y = -worldLimits; y <= worldLimits; ++y) {
//                         glm::ivec2 chunk_pos = glm::ivec2(
//                             x + lastChunkPlayerPos.x, y +
//                             lastChunkPlayerPos.y);

//                         thread_pool_manager->pushBackOperation(
//                             WorldQueueOperation(chunk_pos,
//                                                 WldQOperationType::kLoad));

//                         chunk_pos = glm::ivec2(-x + lastChunkPlayerPos.x,
//                                                y + lastChunkPlayerPos.y);

//                         thread_pool_manager->pushBackOperation(
//                             WorldQueueOperation(chunk_pos,
//                                                 WldQOperationType::kLoad));
//                 }
//         }
//         spdlog::debug("Finished loading the world");
//         spdlog::debug("Starting to create meshes and load them on the
//         GPU...");
//         // need a second pass because we need other chunk data to compute
//         // correct models, but we don't pass on the border, we don't want it
//         to
//         // be seen
//         int border = (chunkWidth - 1) / 2 - 1;

//         glm::ivec2 chunk_pos(0, 0);
//         thread_pool_manager->pushBackOperation(
//             WorldQueueOperation(chunk_pos, WldQOperationType::kCreateMesh));
//         for (int a = 1; a <= border; ++a) {
//                 int x = -a;
//                 int y = -a;
//                 while (x <= a) {

//                         glm::ivec2 chunk_pos(x, y);
//                         thread_pool_manager->pushBackOperation(
//                             WorldQueueOperation(
//                                 chunk_pos, WldQOperationType::kCreateMesh));
//                         x++;
//                 }
//                 x--;
//                 while (y <= a) {

//                         glm::ivec2 chunk_pos(x, y);
//                         thread_pool_manager->pushBackOperation(
//                             WorldQueueOperation(
//                                 chunk_pos, WldQOperationType::kCreateMesh));
//                         y++;
//                 }
//                 y--;
//                 while (x >= -a) {

//                         glm::ivec2 chunk_pos(x, y);
//                         thread_pool_manager->pushBackOperation(
//                             WorldQueueOperation(
//                                 chunk_pos, WldQOperationType::kCreateMesh));
//                         x--;
//                 }
//                 x++;
//                 while (y >= -a) {

//                         glm::ivec2 chunk_pos(x, y);
//                         thread_pool_manager->pushBackOperation(
//                             WorldQueueOperation(
//                                 chunk_pos, WldQOperationType::kCreateMesh));
//                         y--;
//                 }
//                 y++;
//         }
//         chunks_mutex.unlock();
//         spdlog::debug("Finished loading the world.");
// //// move all the map one Chunk in x + direction
// void World::moveChunkXP() {
//         // the limit in length to player's position to managed chunks
//         int limit = (chunkWidth - 1) / 2;
//         for (int y = -limit; y <= limit; ++y) {
//                 // managing all the chunks that would get out of the bounds

//                 // -> we check that these are stored correctly on disk
//                 (coming)

//                 // -> we remove them
//                 //
//                 chunks_meshes_to_load_locker.lock();
//                 chunks_meshes_to_load.push(glm::ivec2(limit, y));
//                 chunks_meshes_to_load_locker.unlock();
//                 delete getRelativeToPlayerAt(limit, y).lock();
//                 getRelativeToPlayerAt(limit, y).unlock();
//         }
//         // all the other that are left, we move them x +
//         for (int x = limit - 1; x >= -limit; --x) {
//                 // line per line
//                 for (int y = -limit; y <= limit; ++y) {
//                         setRelativeToPlayerAt(
//                             x + 1, y, getRelativeToPlayerAt(x, y).lock());
//                         getRelativeToPlayerAt(x, y).unlock();
//                 }
//         }
//         // we can now manage the last row/column that needs to be built.
//         for (int y = -limit; y <= limit; ++y) {
//                 // allocate a new chunk
//                 setRelativeToPlayerAt(
//                     -limit, y,
//                     new Chunk(lastChunkPlayerPos + glm::ivec2(-limit, y)));
//                 // add it to the queue of loading
//                 thread_pool_manager->pushBackOperation(WorldQueueOperation(
//                     glm::ivec2(-limit, y) + lastChunkPlayerPos,
//                     WldQOperationType::kLoad));
//         }
//         // and finally create the mesh for chunks already loaded but without
//         a
//         // mesh
//         for (int y = -limit + 1; y <= limit - 1; ++y) {
//                 thread_pool_manager->pushBackOperation(WorldQueueOperation(
//                     glm::ivec2(-limit + 1, y) + lastChunkPlayerPos,
//                     WldQOperationType::kCreateMesh));
//         }
// }

// /// move all the map one Chunk in x - direction
// void World::moveChunkXM() {
//         // the limit in length to player's position to managed chunks
//         int limit = (chunkWidth - 1) / 2;
//         for (int y = -limit; y <= limit; ++y) {
//                 // managing all the chunks that would get out of the bounds

//                 // -> we check that these are stored correctly on disk
//                 (coming)

//                 // -> we remove them
//                 chunks_meshes_to_load_locker.lock();
//                 chunks_meshes_to_load.push(glm::ivec2(-limit, y));
//                 chunks_meshes_to_load_locker.unlock();
//                 delete getRelativeToPlayerAt(-limit, y).lock();
//                 getRelativeToPlayerAt(-limit, y).unlock();
//         }
//         // all the other that are left, we move them x +
//         for (int x = -limit + 1; x <= limit; ++x) {
//                 // line per line
//                 for (int y = -limit; y <= limit; ++y) {
//                         LockableChunk l_chunk = getRelativeToPlayerAt(x, y);
//                         Chunk *chunk_ptr = l_chunk.lock();
//                         setRelativeToPlayerAt(x - 1, y, chunk_ptr);
//                         l_chunk.unlock();
//                 }
//         }
//         // we can now manage the last row/column that needs to be built.
//         for (int y = -limit; y <= limit; ++y) {
//                 // allocate a new chunk
//                 setRelativeToPlayerAt(
//                     limit, y,
//                     new Chunk(lastChunkPlayerPos + glm::ivec2(limit, y)));
//                 // add it to the queue of loading (future)
//                 // -> for now : load it directly
//                 thread_pool_manager->pushBackOperation(WorldQueueOperation(
//                     glm::ivec2(limit, y) + lastChunkPlayerPos,
//                     WldQOperationType::kLoad));
//         }
//         // and finally create the mesh for chunks already loaded but without
//         a
//         // mesh
//         for (int y = -limit + 1; y <= limit - 1; ++y) {
//                 thread_pool_manager->pushBackOperation(WorldQueueOperation(
//                     glm::ivec2(limit - 1, y) + lastChunkPlayerPos,
//                     WldQOperationType::kCreateMesh));
//         }
// }

// /// move all the map one Chunk in y + direction
// void World::moveChunkYP() {
//         // the limit in length to player's position to managed chunks
//         int limit = (chunkWidth - 1) / 2;
//         for (int x = -limit; x <= limit; ++x) {
//                 // managing all the chunks that would get out of the bounds

//                 // -> we check that these are stored correctly on disk
//                 (coming)

//                 // -> we remove them
//                 chunks_meshes_to_load_locker.lock();
//                 chunks_meshes_to_load.push(glm::ivec2(x, limit));
//                 chunks_meshes_to_load_locker.unlock();
//                 delete getRelativeToPlayerAt(x, limit).lock();
//                 getRelativeToPlayerAt(x, limit).unlock();
//         }
//         // all the other that are left, we move them x +
//         for (int y = limit - 1; y >= -limit; --y) {
//                 // line per line
//                 for (int x = -limit; x <= limit; ++x) {
//                         setRelativeToPlayerAt(
//                             x, y + 1, getRelativeToPlayerAt(x, y).lock());
//                         getRelativeToPlayerAt(x, y).unlock();
//                 }
//         }
//         // we can now manage the last row/column that needs to be built.
//         for (int x = -limit; x <= limit; ++x) {
//                 // allocate a new chunk
//                 setRelativeToPlayerAt(
//                     x, -limit,
//                     new Chunk(lastChunkPlayerPos + glm::ivec2(x, -limit)));
//                 // add it to the queue of loading
//                 thread_pool_manager->pushBackOperation(WorldQueueOperation(
//                     glm::ivec2(x, -limit) + lastChunkPlayerPos,
//                     WldQOperationType::kLoad));
//         }
//         // and finally create the mesh for chunks already loaded but without
//         a
//         // mesh
//         for (int x = -limit + 1; x <= limit - 1; ++x) {
//                 thread_pool_manager->pushBackOperation(WorldQueueOperation(
//                     glm::ivec2(x, -limit + 1) + lastChunkPlayerPos,
//                     WldQOperationType::kCreateMesh));
//         }
// }

// /// move all the map one Chunk in y - direction
// void World::moveChunkYM() {
//         // the limit in length to player's position to managed chunks
//         int limit = (chunkWidth - 1) / 2;
//         for (int x = -limit; x <= limit; ++x) {
//                 // managing all the chunks that would get out of the bounds

//                 // -> we check that these are stored correctly on disk
//                 (coming)

//                 // -> we remove them
//                 chunks_meshes_to_load_locker.lock();
//                 chunks_meshes_to_load.push(glm::ivec2(x, -limit));
//                 chunks_meshes_to_load_locker.unlock();
//                 delete getRelativeToPlayerAt(x, -limit).lock();
//                 getRelativeToPlayerAt(x, -limit).unlock();
//         }
//         // all the other that are left, we move them x +
//         for (int y = -limit + 1; y <= limit; ++y) {
//                 // line per line
//                 for (int x = -limit; x <= limit; ++x) {
//                         setRelativeToPlayerAt(
//                             x, y - 1, getRelativeToPlayerAt(x, y).lock());
//                         getRelativeToPlayerAt(x, y).unlock();
//                 }
//         }
//         // we can now manage the last row/column that needs to be built.
//         for (int x = -limit; x <= limit; ++x) {
//                 // allocate a new chunk
//                 setRelativeToPlayerAt(
//                     x, limit,
//                     new Chunk(lastChunkPlayerPos + glm::ivec2(x, limit)));
//                 // add it to the queue of loading
//                 thread_pool_manager->pushBackOperation(WorldQueueOperation(
//                     glm::ivec2(x, limit) + lastChunkPlayerPos,
//                     WldQOperationType::kLoad));
//         }
//         // and finally create the mesh for chunks already loaded but without
//         a
//         // mesh
//         for (int x = -limit + 1; x <= limit - 1; ++x) {
//                 thread_pool_manager->pushBackOperation(WorldQueueOperation(
//                     glm::ivec2(x, limit - 1) + lastChunkPlayerPos,
//                     WldQOperationType::kCreateMesh));
//         }
// /void World::addChunkMeshToLoad(glm::ivec2 abs_chunk_pos) {

//         get_mutex().lock();
//         LockableChunk l_chunk = getAbsoluteAt(abs_chunk_pos);
//         Chunk *actual_chunk_ptr = l_chunk.lock();
//         get_mutex().unlock();
//         if (actual_chunk_ptr != nullptr) {
//                 // creating the mesh
//                 chunks_models_to_load_and_add_lock.lock();
//                 chunks_models_to_load_and_add.push(ChunkToLoadOnGPUBundle(actual_chunk_ptr));
//                 chunks_models_to_load_and_add_lock.unlock();
//                 // ask the main thread to be loaded on the gpu
//                 // world.addChunkMeshToLoad(abs_pos);
//         }
//         l_chunk.unlock();
// }

// void World::loadAChunkMeshOnGPU() {
//         chunks_models_to_load_and_add_lock.lock();
//         if (chunks_models_to_load_and_add.size() > 0) {
//                 ChunkToLoadOnGPUBundle m =
//                 chunks_models_to_load_and_add.front();
//                 chunks_models_to_load_and_add.pop();
//                 chunks_models_to_load_and_add_lock.unlock();

//                 models_for_render.push_front(LocalizedBoundedModel(Model(),
//                 m.c->getBoundingBox(), m.c->getChunkPos(),
//                 m.c->m_mesh.vertexPositions.size(),
//                 m.c->getChunkPosCenter3()));
//                 models_for_render.front().ptr.loadData(m.c->m_mesh, {3, 2,
//                 3});

//         } else {
//                 chunks_meshes_to_load_locker.unlock();
//         }
//         chunks_models_to_load_and_add_lock.unlock();

//         chunks_models_to_remove_lock.lock();
//         if (chunks_models_to_remove.size()>0) {
//                 glm::ivec2 pos = chunks_models_to_remove.front();
//                 chunks_models_to_remove.pop();

//                 // remove all that have same position
//                 models_for_render.remove_if([&](LocalizedBoundedModel m) ->
//                 bool {return m.pos == pos;});
//         }
//         chunks_models_to_remove_lock.unlock();
// }/ }/ }
//
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
