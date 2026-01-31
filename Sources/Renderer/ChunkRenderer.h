#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include "Camera.h"
#include "World/Chunk.h"
#include <World/ChunkMeshCreator.h>
#include <CubeTexture.h>
#include <Model.h>
#include <Shader.h>

class Chunk;

class ChunkRenderer {
public:
    ChunkRenderer();
    void render(Chunk& c, Camera& cam, int nb_vertices, bool bindShader = false, bool bindTexture = false);

    Shader& getShader();
    void bindTextureAndShader();

private:
    CubeTexture m_texture;
    Shader m_shader;
};

#endif
