#include <tracy/Tracy.hpp>
#include "WorldGenerator.h"
#include "CubeTexture.h"
#include "Mathematics/Noise.h"
#include "World/Chunk.h"
#include "World/World.h"
#include "utils.h"


#include <spdlog/spdlog.h>

#include <algorithm>

const int DIRT_HEIGHT = 3;
const int GRASS_HEIGHT = 1;
const int WATER_ALTITUDE = 64;
const int AVG_SAND_HEIGHT = 4;
const int TREES_LOW_MARGIN = 5;

WorldGenerator::WorldGenerator() {}

void WorldGenerator::buildChunk(Chunk &c, World &world) {
         ZoneScoped; // Need at least one call into Tracy;
        spdlog::debug("building Chunk at pos : {}, {}", c.getChunkPos().x,
                      c.getChunkPos().y);
        const int cote = CHUNK_WIDTH;
        for (int i = 0; i < cote; i++) {
                for (int j = 0; j < cote; j++) {

                         ZoneScoped; // Need at least one call into Tracy;
                        glm::ivec3 blockPosGlobal =
                            fromInChunkToGlobalCoordinates(glm::vec3(i, 0, j),
                                                           c.getChunkPos());

                        glm::ivec2 x_z_pos =
                            glm::vec2(blockPosGlobal.x, blockPosGlobal.z);
                        int height = soilHeightAt(x_z_pos);
                        int sand_h = sand_height(x_z_pos);
                        blockPosGlobal.y = height;

                        if (height <=
                            WATER_ALTITUDE + AVG_SAND_HEIGHT + sand_h) {
                                // end with sand
                                for (int k = 0; k < height - sand_h; k++) {
                                        c.setBlockAt(i, k, j, CubeAtlas::Stone);
                                }
                                for (int k = height - sand_h; k <= height; k++) {
                                        c.setBlockAt(i, k, j, CubeAtlas::Sand);
                                }
                        } else {

                                for (int k = 0;
                                     k < height - DIRT_HEIGHT - GRASS_HEIGHT;
                                     k++) {
                                        c.setBlockAt(i, k, j, CubeAtlas::Stone);
                                }
                                for (int k =
                                         height - DIRT_HEIGHT - GRASS_HEIGHT;
                                     k < height - GRASS_HEIGHT; k++) {
                                        c.setBlockAt(i, k, j, CubeAtlas::Dirt);
                                }
                                for (int k = height - GRASS_HEIGHT; k <= height;
                                     k++) {
                                        c.setBlockAt(i, k, j, CubeAtlas::Grass);
                                }
                        }
                        const int factor = 42;
                        const int offsetX = 120;
                        const int offsetZ = -130;

                        float tree_noise = Noise::turbulence(
                            glm::vec2(blockPosGlobal.x * factor + offsetX,
                                      blockPosGlobal.z * factor + offsetZ),
                            1, 0.1, 0.65, 10);

                        if (tree_noise >= 1.55 && height >= WATER_ALTITUDE+sand_h+TREES_LOW_MARGIN) {
                                addTreeAt(blockPosGlobal.x, height,
                                          blockPosGlobal.z, world);
                        }
                }
        }
        spdlog::debug("Finished building Chunk at pos : {}, {}",
                      c.getChunkPos().x, c.getChunkPos().y);
}

int WorldGenerator::soilHeightAt(glm::vec2 pos) {
        return Noise::turbulence(pos, 45., 0.00005, 0.85, 10) + 128;
}

void WorldGenerator::addTreeAt(int i, int j, int k, World &world) {
        // create a trunk
        for (int l = j; l < std::min(j + 4, 255); l++) {
                // if (world.getBlockAt(glm::ivec3(i, l, k)) == CubeAtlas::Air)
                        // world.setBlockAt(glm::ivec3(i, l, k), CubeAtlas::Wood);
        }
        // for (int a = i - 2; a <= i + 2; a++) {
        //         for (int b = k - 2; b <= k + 2; b++) {
        //                 for (int m = 0; m < 2; m++) {
        //                         if (world.getBlockAt(glm::ivec3(
        //                                 a, j + 2 + m, b)) == CubeAtlas::Air)
        //                                 world.setBlockAt(
        //                                     glm::ivec3(a, j + 2 + m, b),
        //                                     CubeAtlas::Leaves);
        //                 }
        //         }
        // }
        // for (int a = i - 1; a <= i + 1; a++) {
        //         for (int b = k - 1; b <= k + 1; b++) {
        //                 // if (world.getBlockAt(glm::ivec3(a, j + 4, b)) ==
        //                     // CubeAtlas::Air)
        //                         // world.setBlockAt(glm::ivec3(a, j + 4, b),
        //                                          // CubeAtlas::Leaves);
        //         }
        // }
}

int WorldGenerator::sand_height(glm::ivec2 pos) {
        int n_value = floor(Noise::turbulence(pos, 3., 0.0001, 0.85, 10))+10;
        if (n_value > 0)
                return n_value;
        else
                return 0;
}
