#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include "Chunk.h"

class WorldGenerator {
public:
  WorldGenerator();
  static void buildChunk(Chunk &c);

  static int soilHeightAt(glm::vec2);

protected:
};

#endif
