
#include "utils.h"
#include "World/Chunk.h"

#include <string>
#include <spdlog/spdlog.h>

std::string mat4_to_string(const glm::mat4& matrix)
{
    constexpr int sideSize = 4;
    std::string res;

    for (int i = 0; i < sideSize; i++) {
        for (int j = 0; j < sideSize; j++) {
            res += std::to_string(matrix[i][j]);
            res.push_back(' ');
        }
        res.push_back('\n');
    }
    return res;
}

glm::ivec2 fromBlockToChunkCoordinates(glm::ivec3 blockPos) {
    int rx = blockPos.x % CHUNK_WIDTH;
    int rz = blockPos.z % CHUNK_WIDTH;
    int px = blockPos.x < 0 && rx != 0 ? blockPos.x/CHUNK_WIDTH - 1 : blockPos.x/CHUNK_WIDTH;
    int pz = blockPos.z < 0 && rz != 0 ? blockPos.z/CHUNK_WIDTH - 1 : blockPos.z/CHUNK_WIDTH;
    return glm::ivec2(px, pz);
}

glm::ivec3 fromChunkToBlockCoordinates(glm::ivec2 chunkPos, int blockHeight) {
    return glm::ivec3(chunkPos.x *CHUNK_WIDTH, blockHeight, chunkPos.y*CHUNK_LENGTH);
}

glm::ivec3 fromGlobalToInChunkCoordinates(glm::ivec3 blockPos) {
    int xc = blockPos.x % CHUNK_WIDTH;
    if (xc < 0) {
        xc += CHUNK_WIDTH;
    }
    int zc = blockPos.z % CHUNK_LENGTH;
    if (zc < 0) {
        zc += CHUNK_LENGTH;
    }
    return glm::ivec3(xc, blockPos.y, zc);
}

glm::ivec3 fromInChunkToGlobalCoordinates(glm::ivec3 inChunkPos, glm::ivec2 chunkPos) {
    return inChunkPos + glm::ivec3(chunkPos.x*CHUNK_WIDTH, 0, chunkPos.y*CHUNK_LENGTH);
}


glm::ivec3 fromFloatToBlockCoordinates(glm::vec3 point) {
    return glm::ivec3(floor(point.x), floor(point.y), floor(point.z));
}
