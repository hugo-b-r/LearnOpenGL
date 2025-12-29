
#include "utils.h"
#include "World/Chunk.h"

#include <string>

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
    return glm::ivec2(blockPos.x/CHUNK_WIDTH, blockPos.z/CHUNK_LENGTH);
}

glm::ivec3 fromChunkToBlockCoordinates(glm::ivec2 chunkPos, int blockHeight) {
    return glm::ivec3(chunkPos.x *CHUNK_WIDTH, blockHeight, chunkPos.y*CHUNK_LENGTH);
}

glm::ivec3 fromGlobalToInChunkCoordinates(glm::ivec3 blockPos) {
    return glm::ivec3(blockPos.x % CHUNK_WIDTH, blockPos.y, blockPos.z % CHUNK_LENGTH);
}
