#ifndef CHUNK_H
#define CHUNK_H

#include "CubeTexture.h"
#include <vector>

const int CHUNK_WIDTH = 16;
const int CHUNK_HEIGHT = 256;
const int CHUNK_LENGTH = 16;

class Chunk {
public:
  Chunk();
  CubeAtlas getBlockAt(int x, int y, int z);
  void setBlockAt(int x, int y, int z, CubeAtlas ca);

protected:
 std::vector<CubeAtlas> blocks;
};

#endif
