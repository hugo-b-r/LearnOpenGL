#include "World.h"
#include "CubeTexture.h"
#include "Renderer/ChunkRenderer.h"
#include "World/Chunk.h"
#include "World/ChunkMeshCreator.h"
#include "World/WorldGenerator.h"
#include "World/WorldParallelOperationsManager.h"
#include "WorldQueueOperation.h"
#include "utils.h"

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <vector>

World::World(WorldController& world_controller) : world_controller(world_controller){
        // chunkWidth include a border of chunks needed to build the other
        // chunks so that explains : (2 * (cam.getRenderDistance() + 1) + 1)
        // chunks.reserve(chunksNb);
}

World::World(const World &w) : chunks(w.chunks), world_controller(w.world_controller) {}

World::~World() {}

std::optional<ReadSafeChunk> World::getAbsoluteChunkAt(glm::ivec2 abs_pos) {
        chunk_map_locker.lock();
        if (chunks.contains(abs_pos)) {
                auto res = chunks.at(abs_pos);
                chunk_map_locker.unlock();
                return res;
        } else {
                chunk_map_locker.unlock();
                return std::nullopt;
        }
}

std::optional<CubeAtlas> World::getBlockAt(glm::ivec3 pos) {
        glm::ivec2 chunkCoord = fromBlockToChunkCoordinates(pos);
        auto l_chunk_res = getAbsoluteChunkAt(chunkCoord);
        if (l_chunk_res.has_value()) {
                auto blockInChunkPos = fromGlobalToInChunkCoordinates(pos);
                int a = blockInChunkPos.x;
                int b = blockInChunkPos.y;
                int c = blockInChunkPos.z;
                CubeAtlas res = l_chunk_res.value().get()->getBlockAt(a, b, c);
                return res;
        } else {
                return std::nullopt;
        }
}

/// Returns neighbors only if they exist all
std::optional<NeighboringChunks> World::getChunkNeighbors(glm::ivec2 abs_pos) {
        chunk_map_locker.lock();
        glm::ivec2 x_p_pos(1, 0);
        glm::ivec2 x_m_pos(-1, 0);
        glm::ivec2 y_p_pos(0, 1);
        glm::ivec2 y_m_pos(0, -1);
        std::shared_ptr<Chunk> x_p;
        std::shared_ptr<Chunk> x_m;
        std::shared_ptr<Chunk> y_p;
        std::shared_ptr<Chunk> y_m;

        if (chunks.contains(abs_pos + x_p_pos)) {
                x_p = chunks.at(abs_pos + x_p_pos);
        } else {
                chunk_map_locker.unlock();
                return std::nullopt;
        }
        if (chunks.contains(abs_pos + x_m_pos)) {
                x_m = chunks.at(abs_pos + x_m_pos);
        } else {
                chunk_map_locker.unlock();
                return std::nullopt;
        }
        if (chunks.contains(abs_pos + y_p_pos)) {
                y_p = chunks.at(abs_pos + y_p_pos);
        } else {
                chunk_map_locker.unlock();
                return std::nullopt;
        }
        if (chunks.contains(abs_pos + y_m_pos)) {
                y_m = chunks.at(abs_pos + y_m_pos);
        } else {
                chunk_map_locker.unlock();
                return std::nullopt;
        }
        chunk_map_locker.unlock();

        return NeighboringChunks({x_p, x_m, y_p, y_m});
}

//
// void World::loadChunk(glm::ivec2 chunk_pos) {
//         // for now, the only source for a chunk is the chunk builder
//         auto c = std::make_shared<Chunk>(Chunk(chunk_pos));

//         WorldGenerator::buildChunk(*(chunks[chunk_pos].get()), *this);

//         loadChunk(c);
// }

void World::loadChunk(ReadSafeChunk chunk) {
        // for now, the only source for a chunk is the chunk builder
        chunk_map_locker.lock();
        chunks[chunk.get()->getChunkPos()] = chunk;
        chunk_map_locker.unlock();
        ;
}

void World::unloadChunk(glm::ivec2 chunk_pos) {
        // chunk_map_locker.lock();
        chunks.erase(chunk_pos);
        // chunk_map_locker.unlock();
}

bool World::isChunkLoaded(glm::ivec2 chunk_pos) {
        return chunks.contains(chunk_pos);
}


void World::applyUnloadedBlockstoNewChunk(Chunk& chunk) {
        blocks_to_load_locker.lock();
        auto chunk_pos = chunk.getChunkPos();
        if (blocks_to_load.contains(chunk_pos)) {
                auto blocks_to_add = blocks_to_load.at(chunk_pos);
                for (auto it_block = blocks_to_add.begin(); it_block != blocks_to_add.end(); ++it_block) {
                        auto block_pos = fromGlobalToInChunkCoordinates(it_block.base()->first);
                        auto block_value = it_block.base()->second;
                        spdlog::info("adding block to pos {} {} {} ", block_pos.x, block_pos.y, block_pos.z);
                        chunk.setBlockAt(block_pos, block_value);
                }
        }
        
        
        blocks_to_load_locker.unlock();
}


void World::appendNewUnloadedBlocks(std::vector<std::pair<glm::ivec3, CubeAtlas>> blocks) {
        // remember which chunks were modified
        std::unordered_set<glm::ivec2, KeyFuncs, KeyFuncs> chunks_to_reload;
        blocks_to_load_locker.lock();
        for (auto it_block = blocks.begin(); it_block != blocks.end(); ++it_block) {
                auto chunk_pos = fromBlockToChunkCoordinates(it_block.base()->first);
                auto block_in_chunk_pos = fromGlobalToInChunkCoordinates(it_block.base()->first);
                auto block_type_to_add = it_block.base()->second;
                auto block_pos_type = *it_block.base();

                // access chunks data
                chunk_map_locker.lock();
                if (!(chunks.contains(chunk_pos) && chunks[chunk_pos].get()->getBlockAt(block_in_chunk_pos) == block_type_to_add)) {
                        if (blocks_to_load.contains(chunk_pos)) {
                                blocks_to_load.at(chunk_pos).push_back(block_pos_type);
                        } else {
                                blocks_to_load[chunk_pos] = {block_pos_type};
                        }
                        // this chunk has been modified
                        chunks_to_reload.insert(chunk_pos);
                }
                chunk_map_locker.unlock();
        }
        blocks_to_load_locker.unlock();

        for (auto it = chunks_to_reload.begin(); it != chunks_to_reload.end(); ++it) {
                world_controller.addChunkToReload(*it);
        }

        
}


std::optional<std::shared_ptr<Chunk>> World::buildNextChunkFor(glm::ivec2 pos) {
        chunk_map_locker.lock();
        if (!chunks.contains(pos)) {
                chunk_map_locker.unlock();
                return std::nullopt;
        }

        
        std::shared_ptr<Chunk> new_chunk = std::make_shared<Chunk>(*chunks[pos]);
                
        chunk_map_locker.unlock();

        blocks_to_load_locker.lock();
        if (blocks_to_load.contains(pos)) {
                auto blocks_to_add_for_this_chunk = blocks_to_load[pos];

                for (auto it = blocks_to_add_for_this_chunk.begin(); it != blocks_to_add_for_this_chunk.end(); ++it) {
                        new_chunk->setBlockAt(it.base()->first, it.base()->second);
                }
                blocks_to_load.erase(pos);
        }
        blocks_to_load_locker.unlock();
        return new_chunk;
}

