#include <spdlog/spdlog.h>

#include "Mesh.h"
#include "World/ChunkMeshCreator.h"
#include "World/LockableChunk.h"

#include "Chunk.h"
#include "World/World.h"
#include "WorldQueueOperation.h"
#include "Renderer/ChunkRenderer.h"
#include "World/WorldGenerator.h"

#include <iostream>
#include <thread>

using namespace std::chrono_literals;

WorldQueueOperation::WorldQueueOperation(glm::ivec2 absolute_pos,
                                         WldQOperationType type)
    : abs_pos(absolute_pos), type(type) {}

// empty destructor as no emory management
WorldQueueOperation::~WorldQueueOperation() {}

// exeuctes the operations of the type
void WorldQueueOperation::execute(WorldController &world_controller) {
        switch (type) {
        case WldQOperationType::LoadOrGenerate:
                return executeLoadOrGenerate(world_controller);
                break;
        case WldQOperationType::LoadOrGenerateThenMesh:
                return executeLoadOrGenerate(world_controller);
                // executeCreateMesh(world_controller);
                break;
        case WldQOperationType::CreateMesh:
                return executeCreateMesh(world_controller);
                break;
        }
        spdlog::info("end of execution !, type : {}", (int)type);
}

// create a chunk data
void WorldQueueOperation::executeLoadOrGenerate(WorldController &world_controller) {

        std::shared_ptr<Chunk> c = std::make_shared<Chunk>(Chunk(abs_pos));

        WorldGenerator::buildChunk(*c.get(), *world_controller.world);
        world_controller.addMeshAndGPULoadChunkJob(c);
}


/// create the mesh for an existing chunk
void WorldQueueOperation::executeCreateMesh(WorldController &world_controller) {
        // std::this_thread::sleep_for(1000ms);
        auto l_chunk_res = world_controller.world->getAbsoluteChunkAt(abs_pos);
        if (l_chunk_res.has_value()) { // should normally but hey let's check
                Mesh * m = new Mesh();
                ChunkMeshCreator builder (*l_chunk_res.value().get(), world_controller.renderer->texture.FactorHeight(), *world_controller.world, m);

                if (builder.buildMesh()) {
                        l_chunk_res.value().get()->createBoundingBox();
                        AABB bounding_box = l_chunk_res.value().get()->getBoundingBox();
                        world_controller.addMeshToLoadJob(abs_pos, m, bounding_box);
                }
        }
}
