#include "Chunk.h"
#include "CubeTexture.h"

#include <spdlog/spdlog.h>

Chunk::Chunk() : blocks (CHUNK_HEIGHT*CHUNK_LENGTH*CHUNK_WIDTH, CubeAtlas::Air){

}

CubeAtlas Chunk::getBlockAt(int x, int y, int z) {
    return blocks[x*CHUNK_HEIGHT*CHUNK_LENGTH+z*CHUNK_HEIGHT+y];
}

void Chunk::setBlockAt(int x, int y, int z, CubeAtlas ca) {
    blocks[x*CHUNK_HEIGHT*CHUNK_LENGTH+z*CHUNK_HEIGHT+y] = ca;
}
