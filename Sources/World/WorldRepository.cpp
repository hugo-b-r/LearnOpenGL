#include "WorldRepository.h"
#include "World/Chunk.h"
#include "glm/fwd.hpp"
#include <unordered_set>

#include <spdlog/spdlog.h>

WorldRepository::WorldRepository() : m_chunks() {

}

void WorldRepository::loadAt(glm::ivec2 pos) {
    m_chunks.insert({pos, Chunk(pos)});
    m_chunks[pos].load();
    m_chunks[pos].createMesh();
    m_chunks[pos].loadMeshOnGPU();
}

void WorldRepository::unloadAt(glm::ivec2 pos) {
    m_chunks.erase(pos);
}

bool WorldRepository::isLoadedAt(glm::ivec2 pos) {
    if (!existsAt(pos)) {
        return false;
    }
    return m_chunks[pos].hasOnGPUMesh();
}

Chunk WorldRepository::getAt(glm::ivec2 pos) {
    if (existsAt(pos)) {
        return m_chunks.at(pos);
    } else {
        return Chunk(pos);
    }
}

std::unordered_set<glm::ivec2> WorldRepository::getLoadedChunkPos() {
    std::unordered_set<glm::ivec2> res;
    for (auto& c: m_chunks) {
        res.insert(std::get<0>(c));
    }
    return res;
}

std::unordered_map<glm::ivec2, Chunk> WorldRepository::getChunks() {
    return m_chunks;
}

bool WorldRepository::existsAt(glm::ivec2 pos) {
    return m_chunks.find(pos) != m_chunks.end();
}
