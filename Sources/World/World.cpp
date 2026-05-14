#include "World.h"
#include "Camera.h"
#include "CubeTexture.h"
#include "DebugInfos.h"
#include "Renderer/ChunkRenderer.h"
#include "World/Chunk.h"
#include "World/LockableChunk.h"
#include "World/WorldParallelOperationsManager.h"
#include "WorldQueueOperation.h"
#include "utils.h"
#include <iostream>

#include <glm/glm.hpp>
#include <optional>
#include <spdlog/spdlog.h>

const glm::mat4 view_mat(1.0f);

World::World(Camera &cam)
    : chunkWidth((2 * (cam.getRenderDistance() + 1) + 1)),
      chunksNb(chunkWidth * chunkWidth), chunks(chunksNb), camera(cam),
      lastChunkPlayerPos(cam.chunkPosition()) {
        // chunkWidth include a border of chunks needed to build the other
        // chunks so that explains : (2 * (cam.getRenderDistance() + 1) + 1)
        chunks.reserve(chunksNb);
        thread_pool_manager = new WorldParallelOperationsManager(*this);
}

World::World(const World &w)
    : chunkWidth(w.chunkWidth), chunksNb(w.chunksNb), chunks(w.chunks),
      camera(w.camera), lastChunkPlayerPos(w.lastChunkPlayerPos) {
        thread_pool_manager =
            new WorldParallelOperationsManager(*(w.thread_pool_manager));
}

World::~World() {
        for (int i = 0; i < chunksNb; i++) {

                delete chunks[i].lock();
                chunks[i].unlock();
        }
        delete thread_pool_manager;
}

void World::load() {
        spdlog::debug("Starting to load the world...");
        // load all the chunks in a default state
        chunks_mutex.lock();
        int worldLimits = (chunkWidth - 1) / 2;

        for (int y = -worldLimits; y <= worldLimits; ++y) {
                glm::ivec2 chunk_pos = glm::ivec2(0 + lastChunkPlayerPos.x,
                                                  y + lastChunkPlayerPos.y);
                Chunk *chunk_ptr = new Chunk(chunk_pos);
                setRelativeToPlayerAt(0, y, chunk_ptr);
                thread_pool_manager->pushBackOperation(
                    WorldQueueOperation(chunk_pos, WldQOperationType::kLoad));
                std::cout << "pos " << chunk_pos.x <<" " << chunk_pos.y << std::endl;
        }

        for (int x = 1; x <= worldLimits; ++x) {
                for (int y = -worldLimits; y <= worldLimits; ++y) {
                        glm::ivec2 chunk_pos = glm::ivec2(
                            x + lastChunkPlayerPos.x, y + lastChunkPlayerPos.y);
                        Chunk *chunk_ptr = new Chunk(chunk_pos);
                        setRelativeToPlayerAt(x, y, chunk_ptr);

                        thread_pool_manager->pushBackOperation(
                            WorldQueueOperation(chunk_pos,
                                                WldQOperationType::kLoad));

                        chunk_pos = glm::ivec2(-x + lastChunkPlayerPos.x,
                                               y + lastChunkPlayerPos.y);
                        chunk_ptr = new Chunk(chunk_pos);
                        setRelativeToPlayerAt(-x, y, chunk_ptr);

                        thread_pool_manager->pushBackOperation(
                            WorldQueueOperation(chunk_pos,
                                                WldQOperationType::kLoad));
                }
        }
        spdlog::debug("Finished loading the world");
        spdlog::debug("Starting to create meshes and load them on the GPU...");
        // need a second pass because we need other chunk data to compute
        // correct models, but we don't pass on the border, we don't want it to
        // be seen
        int border = (chunkWidth - 1) / 2 - 1;

        glm::ivec2 chunk_pos(0, 0);
        thread_pool_manager->pushBackOperation(
            WorldQueueOperation(
                chunk_pos, WldQOperationType::kCreateMesh));
        for (int a = 1; a <= border; ++a) {
                int x = -a;
                int y = -a;
                while (x <= a) {

                        glm::ivec2 chunk_pos(x, y);
                        thread_pool_manager->pushBackOperation(
                            WorldQueueOperation(
                                chunk_pos, WldQOperationType::kCreateMesh));
                        x++;
                }
                x--;
                while (y <= a) {

                        glm::ivec2 chunk_pos(x, y);
                        thread_pool_manager->pushBackOperation(
                            WorldQueueOperation(
                                chunk_pos, WldQOperationType::kCreateMesh));
                        y++;
                }
                y--;
                while (x >= -a) {
                        
                        glm::ivec2 chunk_pos(x, y);
                        thread_pool_manager->pushBackOperation(
                            WorldQueueOperation(
                                chunk_pos, WldQOperationType::kCreateMesh));
                        x--;
                }
                x++;
                while (y >= -a) {
                        
                        glm::ivec2 chunk_pos(x, y);
                        thread_pool_manager->pushBackOperation(
                            WorldQueueOperation(
                                chunk_pos, WldQOperationType::kCreateMesh));
                        y--;
                }
                y++;
        }
        chunks_mutex.unlock();
        spdlog::debug("Finished loading the world.");
}

void World::render(Camera &cam, ChunkRenderer &renderer, DebugInfos &inf) {
        chunks_mutex.lock();
        glm::ivec2 camPos = cam.chunkPosition();
        glm::ivec2 posDiff = camPos - lastChunkPlayerPos;
        constexpr glm::ivec2 varNull = glm::ivec2(0, 0);
        // we knwo that if it didn't move, we don't need to look for every chunk
        // to rebuild or not&
        if (posDiff != varNull) {
                // reset else there will be a fun thing happening
                lastChunkPlayerPos = camPos;
                if (posDiff.x > 0) {

                        for (int x = 0; x < posDiff.x; ++x) {
                                moveChunkXM();
                        }
                } else if (posDiff.x < 0) {
                        for (int x = 0; x > posDiff.x; --x) {
                                moveChunkXP();
                        }
                }
                if (posDiff.y > 0) {
                        for (int y = 0; y < posDiff.y; ++y) {
                                moveChunkYM();
                        }
                } else if (posDiff.y < 0) {
                        for (int y = 0; y > posDiff.y; --y) {
                                moveChunkYP();
                        }
                }
                if (abs(posDiff.x) + abs(posDiff.y) > 1) {
                        std::cout << "sup a 1" << std::endl;
                }
        }

        // if there is one chunk that needs to be uploaded on the gpu, then do
        // it
        loadAChunkMeshOnGPU();

        // do the rendering
        renderer.getShader().setVec3("lightPos", glm::vec3(0, 300, 0));
        renderer.getShader().setVec3("viewPos", cam.position);
        renderer.bindTextureAndShader();
        Frustum &f = cam.getFrustum();

        //  Draw each chunk
        int total_drawn = 0;
        renderer.getShader().setMat4("view", view_mat);
        renderer.getShader().setMat4("projection",
                                     cam.getProjectionViewMatrix());
        // we don't pass on the border as we don't want to render it
        int border = (chunkWidth - 1) / 2 - 1;
        for (int i = -border; i <= border; ++i) {
                for (int j = -border; j <= border; ++j) {
                        // remove rendererd chunks
                        auto tmpPos = glm::ivec2(i, j);
                        // if (posToUnLoad.contains(tmpPos))
                        //   posToUnLoad.erase(tmpPos);
                        LockableChunk l_chunk = getRelativeToPlayerAt(tmpPos);
                        std::optional<Chunk *> chunk_ptr_opt =
                            l_chunk.try_lock();
                        if (chunk_ptr_opt.has_value()) {
                                Chunk *chunk_ptr = chunk_ptr_opt.value();
                                if (chunk_ptr != nullptr &&
                                    f.IsAabbVisible(
                                        chunk_ptr->getBoundingBox())) {
                                        chunk_ptr->draw(cam, renderer);
                                        ++total_drawn;
                                } else if (chunk_ptr == nullptr) {
                                        spdlog::debug(
                                            "skipped because nullptr");
                                }
                                l_chunk.unlock();
                        } else {
                                spdlog::debug("skipped because not lockable");
                        }
                }
        }
        chunks_mutex.unlock();
        inf.updateDrawnChunks(total_drawn);
}

CubeAtlas World::getBlockAt(glm::ivec3 pos) {
        glm::ivec2 chunkCoord = fromBlockToChunkCoordinates(pos);
        if (existsAt(chunkCoord)) {
                auto blockInChunkPos = fromGlobalToInChunkCoordinates(pos);
                int a = blockInChunkPos.x;
                int b = blockInChunkPos.y;
                int c = blockInChunkPos.z;
                LockableChunk l_chunk = getAbsoluteAt(chunkCoord);
                Chunk *chunk_ptr = l_chunk.lock();
                CubeAtlas res = chunk_ptr == nullptr
                                    ? CubeAtlas::Air
                                    : chunk_ptr->getBlockAt(a, b, c);
                l_chunk.unlock();
                return res;
        } else {
                return CubeAtlas::Air;
        }
}

void World::setBlockAt(glm::ivec3 pos, CubeAtlas cube) {

        auto chunkCoord = fromBlockToChunkCoordinates(pos);

        if (existsAt(chunkCoord)) {
                auto blockInChunkPos = fromGlobalToInChunkCoordinates(pos);
                int a = blockInChunkPos.x, b = blockInChunkPos.y,
                    c = blockInChunkPos.z;
                LockableChunk l_chunk = getAbsoluteAt(chunkCoord);
                Chunk *chunk_ptr = l_chunk.lock();
                if (chunk_ptr != nullptr) {
                        chunk_ptr->setBlockAt(a, b, c, cube);
                }
                l_chunk.unlock();
        }
}

/// move all the map one Chunk in x + direction
void World::moveChunkXP() {
        // the limit in length to player's position to managed chunks
        int limit = (chunkWidth - 1) / 2;
        for (int y = -limit; y <= limit; ++y) {
                // managing all the chunks that would get out of the bounds

                // -> we check that these are stored correctly on disk (coming)

                // -> we remove them
                delete getRelativeToPlayerAt(limit, y).lock();
                getRelativeToPlayerAt(limit, y).unlock();
        }
        // all the other that are left, we move them x +
        for (int x = limit - 1; x >= -limit; --x) {
                // line per line
                for (int y = -limit; y <= limit; ++y) {
                        setRelativeToPlayerAt(
                            x + 1, y, getRelativeToPlayerAt(x, y).lock());
                        getRelativeToPlayerAt(x, y).unlock();
                }
        }
        // we can now manage the last row/column that needs to be built.
        for (int y = -limit; y <= limit; ++y) {
                // allocate a new chunk
                setRelativeToPlayerAt(
                    -limit, y,
                    new Chunk(lastChunkPlayerPos + glm::ivec2(-limit, y)));
                // add it to the queue of loading
                thread_pool_manager->pushBackOperation(WorldQueueOperation(
                    glm::ivec2(-limit, y) + lastChunkPlayerPos,
                    WldQOperationType::kLoad));
        }
        // and finally create the mesh for chunks already loaded but without a
        // mesh
        for (int y = -limit + 1; y <= limit - 1; ++y) {
                thread_pool_manager->pushBackOperation(WorldQueueOperation(
                    glm::ivec2(-limit + 1, y) + lastChunkPlayerPos,
                    WldQOperationType::kCreateMesh));
        }
}

/// move all the map one Chunk in x - direction
void World::moveChunkXM() {
        // the limit in length to player's position to managed chunks
        int limit = (chunkWidth - 1) / 2;
        for (int y = -limit; y <= limit; ++y) {
                // managing all the chunks that would get out of the bounds

                // -> we check that these are stored correctly on disk (coming)

                // -> we remove them
                delete getRelativeToPlayerAt(-limit, y).lock();
                getRelativeToPlayerAt(-limit, y).unlock();
        }
        // all the other that are left, we move them x +
        for (int x = -limit + 1; x <= limit; ++x) {
                // line per line
                for (int y = -limit; y <= limit; ++y) {
                        LockableChunk l_chunk = getRelativeToPlayerAt(x, y);
                        Chunk *chunk_ptr = l_chunk.lock();
                        setRelativeToPlayerAt(x - 1, y, chunk_ptr);
                        l_chunk.unlock();
                }
        }
        // we can now manage the last row/column that needs to be built.
        for (int y = -limit; y <= limit; ++y) {
                // allocate a new chunk
                setRelativeToPlayerAt(
                    limit, y,
                    new Chunk(lastChunkPlayerPos + glm::ivec2(limit, y)));
                // add it to the queue of loading (future)
                // -> for now : load it directly
                thread_pool_manager->pushBackOperation(WorldQueueOperation(
                    glm::ivec2(limit, y) + lastChunkPlayerPos,
                    WldQOperationType::kLoad));
        }
        // and finally create the mesh for chunks already loaded but without a
        // mesh
        for (int y = -limit + 1; y <= limit - 1; ++y) {
                thread_pool_manager->pushBackOperation(WorldQueueOperation(
                    glm::ivec2(limit - 1, y) + lastChunkPlayerPos,
                    WldQOperationType::kCreateMesh));
        }
}

/// move all the map one Chunk in y + direction
void World::moveChunkYP() {
        // the limit in length to player's position to managed chunks
        int limit = (chunkWidth - 1) / 2;
        for (int x = -limit; x <= limit; ++x) {
                // managing all the chunks that would get out of the bounds

                // -> we check that these are stored correctly on disk (coming)

                // -> we remove them
                delete getRelativeToPlayerAt(x, limit).lock();
                getRelativeToPlayerAt(x, limit).unlock();
        }
        // all the other that are left, we move them x +
        for (int y = limit - 1; y >= -limit; --y) {
                // line per line
                for (int x = -limit; x <= limit; ++x) {
                        setRelativeToPlayerAt(
                            x, y + 1, getRelativeToPlayerAt(x, y).lock());
                        getRelativeToPlayerAt(x, y).unlock();
                }
        }
        // we can now manage the last row/column that needs to be built.
        for (int x = -limit; x <= limit; ++x) {
                // allocate a new chunk
                setRelativeToPlayerAt(
                    x, -limit,
                    new Chunk(lastChunkPlayerPos + glm::ivec2(x, -limit)));
                // add it to the queue of loading
                thread_pool_manager->pushBackOperation(WorldQueueOperation(
                    glm::ivec2(x, -limit) + lastChunkPlayerPos,
                    WldQOperationType::kLoad));
        }
        // and finally create the mesh for chunks already loaded but without a
        // mesh
        for (int x = -limit + 1; x <= limit - 1; ++x) {
                thread_pool_manager->pushBackOperation(WorldQueueOperation(
                    glm::ivec2(x, -limit + 1) + lastChunkPlayerPos,
                    WldQOperationType::kCreateMesh));
        }
}

/// move all the map one Chunk in y - direction
void World::moveChunkYM() {
        // the limit in length to player's position to managed chunks
        int limit = (chunkWidth - 1) / 2;
        for (int x = -limit; x <= limit; ++x) {
                // managing all the chunks that would get out of the bounds

                // -> we check that these are stored correctly on disk (coming)

                // -> we remove them
                delete getRelativeToPlayerAt(x, -limit).lock();
                getRelativeToPlayerAt(x, -limit).unlock();
        }
        // all the other that are left, we move them x +
        for (int y = -limit + 1; y <= limit; ++y) {
                // line per line
                for (int x = -limit; x <= limit; ++x) {
                        setRelativeToPlayerAt(
                            x, y - 1, getRelativeToPlayerAt(x, y).lock());
                        getRelativeToPlayerAt(x, y).unlock();
                }
        }
        // we can now manage the last row/column that needs to be built.
        for (int x = -limit; x <= limit; ++x) {
                // allocate a new chunk
                setRelativeToPlayerAt(
                    x, limit,
                    new Chunk(lastChunkPlayerPos + glm::ivec2(x, limit)));
                // add it to the queue of loading
                thread_pool_manager->pushBackOperation(WorldQueueOperation(
                    glm::ivec2(x, limit) + lastChunkPlayerPos,
                    WldQOperationType::kLoad));
        }
        // and finally create the mesh for chunks already loaded but without a
        // mesh
        for (int x = -limit + 1; x <= limit - 1; ++x) {
                thread_pool_manager->pushBackOperation(WorldQueueOperation(
                    glm::ivec2(x, limit - 1) + lastChunkPlayerPos,
                    WldQOperationType::kCreateMesh));
        }
}

// tests wether the chunk exitsts at this position

bool World::isLoadedAt(glm::ivec2 pos) {
        if (!existsAt(pos)) {
                return false;
        }
        Chunk *chunk_ptr = getRelativeToPlayerAt(pos).lock();
        bool res = false;
        if (chunk_ptr != nullptr) {
                res = chunk_ptr->isLoaded();
        }
        getRelativeToPlayerAt(pos).unlock();
        return res;
}

void World::createAndLoadMeshAt(glm::ivec2 pos) {
        if (existsAt(pos)) {
                // chunks_mutex.lock();
                LockableChunk l_chunk = getRelativeToPlayerAt(pos);
                Chunk *chunk_ptr = l_chunk.lock();
                // chunks_mutex.unlock();

                chunk_ptr->createMesh(*this);
                chunk_ptr->loadMeshOnGPU();

                l_chunk.unlock();
        }
}

bool World::isMeshLoadedAt(glm::ivec2 pos) {
        if (existsAt(pos)) {
                LockableChunk l_chunk = getRelativeToPlayerAt(pos);
                Chunk *chunk_ptr = l_chunk.lock();
                bool res = chunk_ptr->hasOnGPUMesh();
                l_chunk.unlock();
                return res;
        } else {
                return false;
        }
}

LockableChunk World::getRelativeToPlayerAt(glm::ivec2 pos) {
        int x = pos.x + (chunkWidth - 1) / 2;
        int y = pos.y + (chunkWidth - 1) / 2;

        assert(x + y * chunkWidth >= 0);
        assert(x + y * chunkWidth < static_cast<int>(chunks.size()));
        // chunks_mutex.lock();
        LockableChunk res = chunks[x + y * chunkWidth];
        // chunks_mutex.unlock();
        return res;
}

LockableChunk World::getRelativeToPlayerAt(int ox, int oy) {
        int x = ox + (chunkWidth - 1) / 2;
        int y = oy + (chunkWidth - 1) / 2;

        assert(x + y * chunkWidth >= 0);
        assert(x + y * chunkWidth < static_cast<int>(chunks.size()));
        // chunks_mutex.lock();
        LockableChunk res = chunks[x + y * chunkWidth];
        // chunks_mutex.unlock();
        return res;
}

LockableChunk World::getAbsoluteAt(glm::ivec2 pos) {
        glm::ivec2 diff = pos - lastChunkPlayerPos;
        return getRelativeToPlayerAt(diff);
}

LockableChunk World::getAbsoluteAt(int x, int y) {
        int xrel = x - lastChunkPlayerPos.x;
        int yrel = y - lastChunkPlayerPos.y;
        return getRelativeToPlayerAt(xrel, yrel);
}

void World::setRelativeToPlayerAt(glm::ivec2 pos, Chunk *c) {
        int x = pos.x + (chunkWidth - 1) / 2;
        int y = pos.y + (chunkWidth - 1) / 2;

        assert(x + y * chunkWidth >= 0);
        assert(x + y * chunkWidth < static_cast<int>(chunks.size()));
        // chunks_mutex.lock();
        chunks[x + y * chunkWidth].lock() = c;
        chunks[x + y * chunkWidth].unlock();
        // chunks_mutex.unlock();
}

void World::setRelativeToPlayerAt(int ox, int oy, Chunk *c) {
        int x = ox + (chunkWidth - 1) / 2;
        int y = oy + (chunkWidth - 1) / 2;

        assert(x + y * chunkWidth >= 0);
        assert(x + y * chunkWidth < static_cast<int>(chunks.size()));
        // chunks_mutex.lock();

        chunks[x + y * chunkWidth].lock() = c;
        chunks[x + y * chunkWidth].unlock();
        // chunks_mutex.unlock();
}

void World::setAbsoluteAt(glm::ivec2 pos, Chunk *c) {
        glm::ivec2 diff = pos - lastChunkPlayerPos;
        return setRelativeToPlayerAt(diff, c);
}

void World::setAbsoluteAt(int ox, int oy, Chunk *c) {
        int x = ox - lastChunkPlayerPos.x;
        int y = oy - lastChunkPlayerPos.y;

        assert(x + y * chunkWidth >= 0);
        assert(x + y * chunkWidth < static_cast<int>(chunks.size()));
        // chunks_mutex.lock();
        chunks[x + y * chunkWidth].lock() = c;
        chunks[x + y * chunkWidth].unlock();
        // chunks_mutex.unlock();
}

void World::addChunkMeshToLoad(glm::ivec2 abs_chunk_pos) {
        chunks_meshes_to_load_locker.lock();
        chunks_meshes_to_load.push(abs_chunk_pos);
        chunks_meshes_to_load_locker.unlock();
}

bool World::existsAt(glm::ivec2 pos) {
        glm::ivec2 diff = pos - lastChunkPlayerPos;
        return (abs(diff.x) <= (chunkWidth - 1) / 2 &&
                abs(diff.y) <= (chunkWidth - 1) / 2);
}

void World::printWorldContent() {
        int limit = (chunkWidth - 1) / 2;
        for (int i = -limit; i <= limit; i++) {
                for (int j = -limit; j <= limit; j++) {
                        Chunk *chunk_ptr = getRelativeToPlayerAt(j, i).lock();
                        std::cout.width(7);
                        if (chunk_ptr != nullptr)
                                std::cout << chunk_ptr->getChunkPos().x << ":"
                                          << chunk_ptr->getChunkPos().y
                                          << chunk_ptr->isLoaded()
                                          << chunk_ptr->hasMesh()
                                          << chunk_ptr->hasOnGPUMesh() << " ";
                        else
                                std::cout << "---" << " ";
                        getRelativeToPlayerAt(j, i).unlock();
                }
                std::cout << std::endl;
        }
        std::cout << "----------------------" << std::endl;
}

std::mutex &World::get_mutex() { return chunks_mutex; }

void World::loadAChunkMeshOnGPU() {
        chunks_meshes_to_load_locker.lock();
        if (chunks_meshes_to_load.size() > 0) {
                glm::ivec2 chunk_position = chunks_meshes_to_load.front();
                chunks_meshes_to_load.pop();
                chunks_meshes_to_load_locker.unlock();

                LockableChunk l_chunk = getAbsoluteAt(chunk_position);
                Chunk *chunk_ptr = l_chunk.lock();

                chunk_ptr->loadMeshOnGPU();

                l_chunk.unlock();
        } else {
                chunks_meshes_to_load_locker.unlock();
        }
}
