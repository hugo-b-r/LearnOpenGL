#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <glm/glm.hpp>

#define UNUSED(x) (void)x

std::string mat4_to_string(const glm::mat4& matrix);

glm::ivec2 fromBlockToChunkCoordinates(glm::ivec3 blockPos);
glm::ivec3 fromChunkToBlockCoordinates(glm::ivec2 chunkPos, int blockHeight);
glm::ivec3 fromGlobalToInChunkCoordinates(glm::ivec3 blockPos);
glm::ivec3 fromInChunkToGlobalCoordinates(glm::ivec3 inChunkPos, glm::ivec2 chunkPos);

#endif
