#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include "Chunk.h"

/// The world generation is happening right here.
///
/// That mens knowing where to put some blocks, structures, water...
class WorldGenerator {
      public:
        /// The default constructor
        WorldGenerator();

        /// Building of a chunk (globally)
        static void buildChunk(Chunk &c, World &world);

        /// Height for a point in the world
        static int soilHeightAt(glm::vec2);

        /// Adds a tree without replacing existing blocks at pos
        static void addTreeAt(int i, int j, int k, World &world, Chunk& chunk);

      protected:
        /// How much sand to put here, supposing there would be sand
        static int sand_height(glm::ivec2 pos);
};

#endif
