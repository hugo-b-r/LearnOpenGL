#include "WorldGenerator.h"
#include "CubeTexture.h"
#include "Mathematics/Noise.h"
#include "World/Chunk.h"

#include <spdlog/spdlog.h>

WorldGenerator::WorldGenerator() {}

void WorldGenerator::buildChunk(Chunk &c) {
  const int cote = CHUNK_WIDTH;
  for (int i = 0; i < cote; i++) {
      for (int j = 0; j < cote ; j++) {

      //int height = (int)(5.f * Noise::perlin((float) (i+c.getChunkPos().x)/CST, (float)(j+c.getChunkPos().y)/CST, 1.)) + 128+(int)(10.f * Noise::perlin(2.f*(float)(i+c.getChunkPos().x)/CST, 2.f*(float)(j+c.getChunkPos().y)/CST, 1.));
      int height = Noise::turbulence(glm::vec2(i + c.getChunkPos().x*CHUNK_WIDTH, j + c.getChunkPos().y*CHUNK_LENGTH), 60., 0.00005, 0.85, 10)+128;

      for (int k = 0; k < height; k++) {
        c.setBlockAt(i, k, j, CubeAtlas::Grass);
      }
    }
  }
}
