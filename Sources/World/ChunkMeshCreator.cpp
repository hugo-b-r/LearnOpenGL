#include "ChunkMeshCreator.h"
#include "Chunk.h"
#include "CubeTexture.h"
#include "Mesh.h"
#include "World.h"
#include "utils.h"
#include <array>
#include <memory>
#include <spdlog/spdlog.h>

struct NeighbooringFaces {
        void update(int x, int y, int z) {
                left = {x - 1, y, z};
                right = {x + 1, y, z};
                up = {x, y + 1, z};
                down = {x, y - 1, z};
                front = {x, y, z + 1};
                rear = {x, y, z - 1};
        }

        glm::ivec3 left;
        glm::ivec3 right;
        glm::ivec3 up;
        glm::ivec3 down;
        glm::ivec3 front;
        glm::ivec3 rear;
};

std::array<GLfloat, 48> ChunkMeshCreator::upFace(int select, float x, float y,
                                                 float z) const {
        return std::array<GLfloat, 48>({
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.0f,
            (1.0f + select) * tex_vertical_factor,
            0,
            1,
            0,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.166667f,
            (0.0f + select) * tex_vertical_factor,
            0,
            1,
            0,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.166667f,
            (1.0f + select) * tex_vertical_factor,
            0,
            1,
            0,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.166667f,
            (0.0f + select) * tex_vertical_factor,
            0,
            1,
            0,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.0f,
            (1.0f + select) * tex_vertical_factor,
            0,
            1,
            0,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.0f,
            (0.0f + select) * tex_vertical_factor,
            0,
            1,
            0,

        });
}
std::array<GLfloat, 48> ChunkMeshCreator::downFace(int select, float x, float y,
                                                   float z) const {
        return std::array<GLfloat, 48>({

            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            1.0f,
            (0.0f + select) * tex_vertical_factor,
            0,
            -1,
            0,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.833333f,
            (0.0f + select) * tex_vertical_factor,
            0,
            -1,
            0, // Ordre de coins ajusté
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            1.0f,
            (1.0f + select) * tex_vertical_factor,
            0,
            -1,
            0,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            1.0f,
            (1.0f + select) * tex_vertical_factor,
            0,
            -1,
            0,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.833333f,
            (0.0f + select) * tex_vertical_factor,
            0,
            -1,
            0,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.833333f,
            (1.0f + select) * tex_vertical_factor,
            0,
            -1,
            0,
        });
}
std::array<GLfloat, 48> ChunkMeshCreator::leftFace(int select, float x, float y,
                                                   float z) const {
        return std::array<GLfloat, 48>({
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.5f,
            (1.0f + select) * tex_vertical_factor,
            -1,
            0,
            0,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.333333f,
            (1.0f + select) * tex_vertical_factor,
            -1,
            0,
            0,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.5f,
            (0.0f + select) * tex_vertical_factor,
            -1,
            0,
            0,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.333333f,
            (0.0f + select) * tex_vertical_factor,
            -1,
            0,
            0,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.5f,
            (0.0f + select) * tex_vertical_factor,
            -1,
            0,
            0,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.333333f,
            (1.0f + select) * tex_vertical_factor,
            -1,
            0,
            0,

        });
};
std::array<GLfloat, 48> ChunkMeshCreator::rightFace(int select, float x,
                                                    float y, float z) const {
        return std::array<GLfloat, 48>({
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.833333f,
            (1.0f + select) * tex_vertical_factor,
            1,
            0,
            0, // Ordre de coins ajusté pour l'orientation
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.666667f,
            (0.0f + select) * tex_vertical_factor,
            1,
            0,
            0,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.666667f,
            (1.0f + select) * tex_vertical_factor,
            1,
            0,
            0,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.666667f,
            (0.0f + select) * tex_vertical_factor,
            1,
            0,
            0,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.833333f,
            (1.0f + select) * tex_vertical_factor,
            1,
            0,
            0,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.833333f,
            (0.0f + select) * tex_vertical_factor,
            1,
            0,
            0,
        });
};
std::array<GLfloat, 48> ChunkMeshCreator::frontFace(int select, float x,
                                                    float y, float z) const {
        return std::array<GLfloat, 48>({
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.166667f,
            (1.0f + select) * tex_vertical_factor,
            0,
            0,
            -1,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.333333f,
            (1.0f + select) * tex_vertical_factor,
            0,
            0,
            -1,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.333333f,
            (0.0f + select) * tex_vertical_factor,
            0,
            0,
            -1,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.333333f,
            (0.0f + select) * tex_vertical_factor,
            0,
            0,
            -1,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.166667f,
            (0.0f + select) * tex_vertical_factor,
            0,
            0,
            -1,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f + 1.f) * cube_length,
            0.166667f,
            (1.0f + select) * tex_vertical_factor,
            0,
            0,
            -1,
        });
};
std::array<GLfloat, 48> ChunkMeshCreator::rearFace(int select, float x, float y,
                                                   float z) const {
        return std::array<GLfloat, 48>({
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.666667f,
            (1.0f + select) * tex_vertical_factor,
            0,
            0,
            1, // Ordre de coins inversé pour l'orientation
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.5f,
            (0.0f + select) * tex_vertical_factor,
            0,
            0,
            1,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.5f,
            (1.0f + select) * tex_vertical_factor,
            0,
            0,
            1,
            (x - (float)CHUNK_WIDTH / 2.f + 1.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.5f,
            (0.0f + select) * tex_vertical_factor,
            0,
            0,
            1,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.666667f,
            (1.0f + select) * tex_vertical_factor,
            0,
            0,
            1,
            (x - (float)CHUNK_WIDTH / 2.f) * cube_width,
            (y - (float)CHUNK_HEIGHT / 2.f + 1.f) * cube_height,
            (z - (float)CHUNK_LENGTH / 2.f) * cube_length,
            0.666667f,
            (0.0f + select) * tex_vertical_factor,
            0,
            0,
            1,

        });
};

ChunkMeshCreator::ChunkMeshCreator(Chunk &c, float tex_factor_height, World &w,
                                   Mesh *result_location)
    :m_mesh(result_location), tex_vertical_factor(tex_factor_height), data(c),
      status(ChunkMeshBuildStatus::Running), world(w)  {
        *result_location = {{}, {}, {}};
}

bool ChunkMeshCreator::shouldMakeFace(glm::ivec3 side_block,
                                      const NeighboringChunks &neighbors) {
        int x = side_block.x, y = side_block.y, z = side_block.z;
        if (y < 0) {
                // under the sea
                return false;
        } else if (x < CHUNK_WIDTH && x >= 0 && y < CHUNK_HEIGHT &&
                   z < CHUNK_LENGTH && z >= 0) {
                // inside the chunk
                auto block = data.getBlockAt(x, y, z);
                switch (block) {
                case CubeAtlas::Air:
                        return true;
                case CubeAtlas::Leaves:
                        return true;
                default:
                        return false;
                }
                // if (block == CubeAtlas::Air ) {
                //     return true;
                // } else {
                //     return false;
                // }
        } else if (y >= CHUNK_HEIGHT) {
                return true;
        } else {
                auto block =
                    getFromNeighbors(fromInChunkToGlobalCoordinates(
                                         side_block, data.getChunkPos()),
                                     neighbors);
                switch (block) {
                case CubeAtlas::Air:
                        return true;
                case CubeAtlas::Leaves:
                        return true;
                default:
                        return false;
                }
        }
}

CubeAtlas
ChunkMeshCreator::getFromNeighbors(glm::ivec3 block_pos,
                                   const NeighboringChunks &neighbors) {
        glm::ivec2 diff =
            fromBlockToChunkCoordinates(block_pos) - data.getChunkPos();
        if (diff.x == 1) {
                return neighbors.x_p.get()->getBlockAt(
                    fromGlobalToInChunkCoordinates(block_pos));
        }
        if (diff.x == -1) {
                return neighbors.x_m.get()->getBlockAt(
                    fromGlobalToInChunkCoordinates(block_pos));
        }
        if (diff.y == 1) {
                return neighbors.y_p.get()->getBlockAt(
                    fromGlobalToInChunkCoordinates(block_pos));
        }
        if (diff.y == -1) {
                return neighbors.y_m.get()->getBlockAt(
                    fromGlobalToInChunkCoordinates(block_pos));
        }
        return CubeAtlas::Air;
}

/// Contract : hey give me neighbors bruh
bool ChunkMeshCreator::buildMesh() {
        auto neighbors = world.getChunkNeighbors(data.getChunkPos());
        if (neighbors.has_value()) {
                for (int i = 0; i < CHUNK_WIDTH; i++) {
                        for (int j = 0; j < CHUNK_HEIGHT; j++) {
                                for (int k = 0; k < CHUNK_LENGTH; k++) {
                                        addBlockMesh(i, j, k,
                                                     neighbors.value());
                                }
                        }
                }
        } else {
                return false;
        }
        status = ChunkMeshBuildStatus::DoneLoaded;
        return true;
}

void ChunkMeshCreator::addBlockMesh(int i, int j, int k,
                                    NeighboringChunks &neighbors) {
        NeighbooringFaces neighboring_faces;
        neighboring_faces.update(i, j, k);
        CubeAtlas block = data.getBlockAt(i, j, k);
        if (block == CubeAtlas::Air) {
                return;
        }

        if (shouldMakeFace(neighboring_faces.up, neighbors)) {
                std::array<GLfloat, 48> temp =
                    upFace((int)block, (float)i, (float)j, (float)k);

                m_mesh->vertexPositions.insert(m_mesh->vertexPositions.end(),
                                               temp.begin(), temp.end());
        }

        if (shouldMakeFace(neighboring_faces.down, neighbors)) {
                std::array<GLfloat, 48> temp =
                    downFace((int)block, (float)i, (float)j, (float)k);

                m_mesh->vertexPositions.insert(m_mesh->vertexPositions.end(),
                                               temp.begin(), temp.end());
        }
        if (shouldMakeFace(neighboring_faces.left, neighbors)) {
                std::array<GLfloat, 48> temp =
                    leftFace((int)block, (float)i, (float)j, (float)k);

                m_mesh->vertexPositions.insert(m_mesh->vertexPositions.end(),
                                               temp.begin(), temp.end());
        }
        if (shouldMakeFace(neighboring_faces.right, neighbors)) {
                std::array<GLfloat, 48> temp =
                    rightFace((int)block, (float)i, (float)j, (float)k);

                m_mesh->vertexPositions.insert(m_mesh->vertexPositions.end(),
                                               temp.begin(), temp.end());
        }
        if (shouldMakeFace(neighboring_faces.front, neighbors)) {
                std::array<GLfloat, 48> temp =
                    frontFace((int)block, (float)i, (float)j, (float)k);

                m_mesh->vertexPositions.insert(m_mesh->vertexPositions.end(),
                                               temp.begin(), temp.end());
        }
        if (shouldMakeFace(neighboring_faces.rear, neighbors)) {
                std::array<GLfloat, 48> temp =
                    rearFace((int)block, (float)i, (float)j, (float)k);

                m_mesh->vertexPositions.insert(m_mesh->vertexPositions.end(),
                                               temp.begin(), temp.end());
        }
}
