#include "World.h"
#include "Camera.h"
#include "CubeTexture.h"
#include "DebugInfos.h"
#include "Renderer/ChunkRenderer.h"
#include "World/Chunk.h"
#include "utils.h"
#include <iostream>

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

const glm::mat4 view_mat(1.0f);

World::World(Camera &cam)
    : chunkWidth((2 * (cam.getRenderDistance() + 1) + 1)),
      chunksNb(chunkWidth * chunkWidth), chunks(chunksNb), camera(cam),
      lastChunkPlayerPos(cam.chunkPosition()) {
        // chunkWidth include a border of chunks needed to build the other
        // chunks so that explains : (2 * (cam.getRenderDistance() + 1) + 1)
        chunks.reserve(chunksNb);
}

World::~World() {
        for (int i = 0; i < chunksNb; i++) {
                delete chunks[i];
        }
}

void World::load() {
        // load all the chunks in a default state
        int worldLimits = (chunkWidth - 1) / 2;
        for (int x = -worldLimits; x <= worldLimits; ++x) {
                for (int y = -worldLimits; y <= worldLimits;
                     ++y) {
                        setRelativeToPlayerAt(
                            x, y,
                            new Chunk(glm::ivec2(x + lastChunkPlayerPos.x,
                                                 y + lastChunkPlayerPos.y)));
                        getRelativeToPlayerAt(x, y)->load(*this);
                }
        }
        // need a second pass because we need other chunk data to compute
        // correct models, but we don't pass on the border, we don't want it to
        // be seen
        int border = (chunkWidth - 1) / 2 - 1;
        for (int x = -border; x <= border; ++x) {
                for (int y = -border; y <= border;
                     ++y) {

                        getRelativeToPlayerAt(x, y)->createMesh(*this);
                        getRelativeToPlayerAt(x, y)->loadMeshOnGPU();
                }
        }
}

void World::render(Camera &cam, ChunkRenderer &renderer, DebugInfos &inf) {
        glm::ivec2 camPos = cam.chunkPosition();
        glm::ivec2 posDiff = camPos - lastChunkPlayerPos;
        constexpr glm::ivec2 varNull = glm::ivec2(0, 0);
        // we knwo that if it didn't move, we don't need to look for every chunk
        // to rebuild or not&
        if (posDiff != varNull) {
                // reset else there will be a fun thing happening
                lastChunkPlayerPos = camPos;
                if (posDiff.x > 0) {

                        for (int x  = 0; x < posDiff.x; ++x) {
                                moveChunkXM();
                        }
                } else if (posDiff.x < 0) {
                        for (int x  = 0; x > posDiff.x; --x) {
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
        }
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
                        if (getRelativeToPlayerAt(tmpPos) != nullptr &&
                            f.IsAabbVisible(getRelativeToPlayerAt(tmpPos)
                                                ->getBoundingBox())) {
                                getRelativeToPlayerAt(tmpPos)->draw(cam,
                                                                    renderer);
                                ++total_drawn;
                        }
                }

                inf.updateDrawnChunks(total_drawn);
        }
}

CubeAtlas World::getBlockAt(glm::ivec3 pos) {
        glm::ivec2 chunkCoord = fromBlockToChunkCoordinates(pos);
        if (existsAt(chunkCoord)) {
                auto blockInChunkPos = fromGlobalToInChunkCoordinates(pos);
                int a = blockInChunkPos.x;
                int b = blockInChunkPos.y;
                int c = blockInChunkPos.z;
                return getAbsoluteAt(chunkCoord) == nullptr
                           ? CubeAtlas::Air
                           : getAbsoluteAt(chunkCoord)->getBlockAt(a, b, c);
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
                if (getAbsoluteAt(chunkCoord) != nullptr)
                        getAbsoluteAt(chunkCoord)->setBlockAt(a, b, c, cube);
        }
}


/// move all the map one Chunk in x + direction
void World::moveChunkXP() {
        // the limit in length to player's position to managed chunks
        int limit = (chunkWidth-1)/2;
        for (int y = -limit; y <= limit; ++y) {
                // managing all the chunks that would get out of the bounds

                // -> we check that these are stored correctly on disk (coming)

                // -> we remove them
                delete getRelativeToPlayerAt(limit, y);
        }
        // all the other that are left, we move them x +
        for (int x = limit-1; x >= -limit; --x) {
                // line per line
                for (int y = -limit; y <= limit; ++y) {
                        setRelativeToPlayerAt(x+1, y, getRelativeToPlayerAt(x, y));
                }
        }
        // we can now manage the last row/column that needs to be built.
        for (int y = -limit; y <= limit; ++y) {
                // allocate a new chunk
                setRelativeToPlayerAt(-limit, y, new Chunk(lastChunkPlayerPos + glm::ivec2(-limit, y)));
                // add it to the queue of loading (future)
                // -> for now : load it directly
                getRelativeToPlayerAt(-limit, y)->load(*this);
        }
        // and finally create the mesh for chunks already loaded but without a mesh
        for (int y = -limit+1; y <= limit-1; ++y) {
                getRelativeToPlayerAt(-limit+1, y)->createMesh(*this);
                getRelativeToPlayerAt(-limit+1, y)->loadMeshOnGPU();

        }
}

/// move all the map one Chunk in x - direction
void World::moveChunkXM() {
        // the limit in length to player's position to managed chunks
        int limit = (chunkWidth-1)/2;
        for (int y = -limit; y <= limit; ++y) {
                // managing all the chunks that would get out of the bounds

                // -> we check that these are stored correctly on disk (coming)

                // -> we remove them
                delete getRelativeToPlayerAt(-limit, y);
        }
        // all the other that are left, we move them x +
        for (int x = -limit+1; x <= limit; ++x) {
                // line per line
                for (int y = -limit; y <= limit; ++y) {
                        setRelativeToPlayerAt(x-1, y, getRelativeToPlayerAt(x, y));
                }
        }
        // we can now manage the last row/column that needs to be built.
        for (int y = -limit; y <= limit; ++y) {
                // allocate a new chunk
                setRelativeToPlayerAt(limit, y, new Chunk(lastChunkPlayerPos + glm::ivec2(limit, y)));
                // add it to the queue of loading (future)
                // -> for now : load it directly
                getRelativeToPlayerAt(limit, y)->load(*this);
        }
        // and finally create the mesh for chunks already loaded but without a mesh
        for (int y = -limit+1; y <= limit-1; ++y) {
                getRelativeToPlayerAt(limit-1, y)->createMesh(*this);
                getRelativeToPlayerAt(limit-1, y)->loadMeshOnGPU();
        }
}

/// move all the map one Chunk in y + direction
void World::moveChunkYP() {
        // the limit in length to player's position to managed chunks
        int limit = (chunkWidth-1)/2;
        for (int x = -limit; x <= limit; ++x) {
                // managing all the chunks that would get out of the bounds

                // -> we check that these are stored correctly on disk (coming)

                // -> we remove them
                delete getRelativeToPlayerAt(x, limit);
        }
        // all the other that are left, we move them x +
        for (int y = limit-1; y >= -limit; --y) {
                // line per line
                for (int x = -limit; x <= limit; ++x) {
                        setRelativeToPlayerAt(x, y+1, getRelativeToPlayerAt(x, y));
                }
        }
        // we can now manage the last row/column that needs to be built.
        for (int x = -limit; x <= limit; ++x) {
                // allocate a new chunk
                setRelativeToPlayerAt(x, -limit, new Chunk(lastChunkPlayerPos + glm::ivec2(x, -limit)));
                // add it to the queue of loading (future)
                // -> for now : load it directly
                getRelativeToPlayerAt(x, -limit)->load(*this);
        }
        // and finally create the mesh for chunks already loaded but without a mesh
        for (int x = -limit+1; x <= limit-1; ++x) {
                getRelativeToPlayerAt(x, -limit+1)->createMesh(*this);
                getRelativeToPlayerAt(x, -limit+1)->loadMeshOnGPU();
        }
}

/// move all the map one Chunk in y - direction
void World::moveChunkYM() {
        // the limit in length to player's position to managed chunks
        int limit = (chunkWidth-1)/2;
        for (int x = -limit; x <= limit; ++x) {
                // managing all the chunks that would get out of the bounds

                // -> we check that these are stored correctly on disk (coming)

                // -> we remove them
                delete getRelativeToPlayerAt(x, -limit);
        }
        // all the other that are left, we move them x +
        for (int y = -limit+1; y <= limit; ++y) {
                // line per line
                for (int x = -limit; x <= limit; ++x) {
                        setRelativeToPlayerAt(x, y-1, getRelativeToPlayerAt(x, y));
                }
        }
        // we can now manage the last row/column that needs to be built.
        for (int x = -limit; x <= limit; ++x) {
                // allocate a new chunk
                setRelativeToPlayerAt(x, limit, new Chunk(lastChunkPlayerPos + glm::ivec2(x, limit)));
                // add it to the queue of loading (future)
                // -> for now : load it directly
                getRelativeToPlayerAt(x, limit)->load(*this);
        }
        // and finally create the mesh for chunks already loaded but without a mesh
        for (int x = -limit+1; x <= limit-1; ++x) {
                getRelativeToPlayerAt(x, limit-1)->createMesh(*this);
                getRelativeToPlayerAt(x, limit-1)->loadMeshOnGPU();
        }
}

// tests wether the chunk exitsts at this position

bool World::isLoadedAt(glm::ivec2 pos) {
        if (!existsAt(pos)) {
                return false;
        }
        return getRelativeToPlayerAt(pos)->isLoaded();
}

void World::createAndLoadMeshAt(glm::ivec2 pos) {
        if (existsAt(pos)) {
                getRelativeToPlayerAt(pos)->createMesh(*this);
                getRelativeToPlayerAt(pos)->loadMeshOnGPU();
        }
}

bool World::isMeshLoadedAt(glm::ivec2 pos) {
        if (existsAt(pos)) {
                return getRelativeToPlayerAt(pos)->hasOnGPUMesh();
        } else {
                return false;
        }
}

Chunk *World::getRelativeToPlayerAt(glm::ivec2 pos) {
        int x = pos.x + (chunkWidth - 1) / 2;
        int y = pos.y + (chunkWidth - 1) / 2;

        assert(x + y * chunkWidth >= 0);
        assert(x + y * chunkWidth < static_cast<int>(chunks.size()));
        return chunks[x + y * chunkWidth];
}

Chunk *World::getRelativeToPlayerAt(int ox, int oy) {
        int x = ox + (chunkWidth - 1) / 2;
        int y = oy + (chunkWidth - 1) / 2;

        assert(x + y * chunkWidth >= 0);
        assert(x + y * chunkWidth < static_cast<int>(chunks.size()));

        return chunks[x + y * chunkWidth];
}

Chunk *World::getAbsoluteAt(glm::ivec2 pos) {
        glm::ivec2 diff = pos - lastChunkPlayerPos;
        return getRelativeToPlayerAt(diff);
}

Chunk *World::getAbsoluteAt(int x, int y) {
        int xrel = x - lastChunkPlayerPos.x;
        int yrel = y - lastChunkPlayerPos.y;
        return getRelativeToPlayerAt(xrel, yrel);
}

void World::setRelativeToPlayerAt(glm::ivec2 pos, Chunk *c) {
        int x = pos.x + (chunkWidth - 1) / 2;
        int y = pos.y + (chunkWidth - 1) / 2;

        assert(x + y * chunkWidth >= 0);
        assert(x + y * chunkWidth < static_cast<int>(chunks.size()));

        chunks[x + y * chunkWidth] = c;
}

void World::setRelativeToPlayerAt(int ox, int oy, Chunk *c) {
        int x = ox + (chunkWidth - 1) / 2;
        int y = oy + (chunkWidth - 1) / 2;

        assert(x + y * chunkWidth >= 0);
        assert(x + y * chunkWidth < static_cast<int>(chunks.size()));

        chunks[x + y * chunkWidth] = c;
}

bool World::existsAt(glm::ivec2 pos) {
        glm::ivec2 diff = pos - lastChunkPlayerPos;
        return (abs(diff.x) <= (chunkWidth - 1) / 2 &&
                abs(diff.y) <= (chunkWidth - 1) / 2);
}

void World::printWorldContent() {
        int limit = (chunkWidth-1)/2;
        for (int i  = -limit; i <= limit; i++) {
                for (int j = -limit; j <= limit; j++) {
                        std::cout << getRelativeToPlayerAt(j, i) << " ";
                }
                std::cout << std::endl;
        }
        std::cout << "----------------------" << std::endl;
}
