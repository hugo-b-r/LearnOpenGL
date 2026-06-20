#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include "Chunk.h"

class WorldGenerator {
      public:
        WorldGenerator();
        static void buildChunk(Chunk &c, World &world);

        static int soilHeightAt(glm::vec2);

        // adds a tree without replacing existing blocks at pos
        static void addTreeAt(int i, int j, int k, World &world);

      protected:
        static int sand_height(glm::ivec2 pos);
};

#endif
