#ifndef WORLDREPOSITORY_H
#define WORLDREPOSITORY_H

#include <unordered_map>
#include <unordered_set>
#include <glm/glm.hpp>
// in order to have the ability to hash the type for unordered map
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>


#include "Chunk.h"

class WorldRepository {
    public:
    WorldRepository();

    void loadAt(glm::ivec2 pos);
    void unloadAt(glm::ivec2 pos);
    bool isLoadedAt(glm::ivec2 pos);

    Chunk getAt(glm::ivec2 pos);

    std::unordered_set<glm::ivec2> getLoadedChunkPos();

    std::unordered_map<glm::ivec2, Chunk> getChunks();
    protected:
    bool existsAt(glm::ivec2 pos);
    std::unordered_map<glm::ivec2, Chunk> m_chunks;

};

#endif
