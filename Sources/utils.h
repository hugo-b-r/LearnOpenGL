#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <glm/glm.hpp>

#define UNUSED(x) (void)x

// Quick reminder about coordinates:
// Chunk coordinates : number of chunk along axixes 
// Block coordinates : number of blocks along axixes 

std::string mat4_to_string(const glm::mat4& matrix);


// Returns coordinates in chunk coordinates in world space from block coordinates in world space
glm::ivec2 fromBlockToChunkCoordinates(glm::ivec3 blockPos);

// number of chunk to number of block
// returning first block (x = 0, z = 0 in chunk) with y = blockHeight
glm::ivec3 fromChunkToBlockCoordinates(glm::ivec2 chunkPos, int blockHeight);

// Global -> local (chunk space)
glm::ivec3 fromGlobalToInChunkCoordinates(glm::ivec3 blockPos);

// Local (chunk) -> Global coordinates
glm::ivec3 fromInChunkToGlobalCoordinates(glm::ivec3 inChunkPos, glm::ivec2 chunkPos);

// global but granular -> discrete coordinates (blocks)
glm::ivec3 fromFloatToBlockCoordinates(glm::vec3 point);

// global but granular -> chunk coordinates
// returns the chunk pos is in
glm::ivec2 fromFloatToChunkCoordinates(glm::vec3 pos);

#endif
