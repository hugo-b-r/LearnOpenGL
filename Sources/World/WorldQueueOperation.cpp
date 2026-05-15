#include <spdlog/spdlog.h>

#include "Chunk.h"
#include "World/LockableChunk.h"
#include "WorldQueueOperation.h"

#include <iostream>
#include <thread>

using namespace std::chrono_literals;

WorldQueueOperation::WorldQueueOperation(glm::ivec2 absolute_pos,
                                         WldQOperationType type)
    : abs_pos(absolute_pos), type(type) {}

// empty destructor as no emory management
WorldQueueOperation::~WorldQueueOperation() {}

// exeuctes the operations of the type
void WorldQueueOperation::execute(World &world) {
        switch (type) {
        case WldQOperationType::kCreate:
                return executeCreate(world);
                break;
        case WldQOperationType::kLoad:
                return executeLoad(world);
                break;
        case WldQOperationType::kCreateAndLoad:
                return executeCreateAndLoad(world);
                break;
        case WldQOperationType::kCreateMesh:
                return executeCreateMesh(world);
                break;
        }
        spdlog::info("end of execution !");
}

void WorldQueueOperation::executeCreate(World &world) {
        world.get_mutex().lock();
        if (!world.existsAt(abs_pos)) {
                world.get_mutex().unlock();
                return;
        }
        LockableChunk l_chunk = world.getAbsoluteAt(abs_pos);
        Chunk *actual_chunk_ptr = l_chunk.lock();
        if (actual_chunk_ptr == nullptr) {
                Chunk *chunk_ptr = new Chunk(abs_pos);
                if (!world.existsAt(abs_pos)) {
                        l_chunk.unlock();
                        world.get_mutex().unlock();
                        return;
                }
                world.setAbsoluteAt(abs_pos, chunk_ptr);
        }

        l_chunk.unlock();
        world.get_mutex().unlock();
}

void WorldQueueOperation::executeLoad(World &world) {
        world.get_mutex().lock();
        if (!world.existsAt(abs_pos)) {
                world.get_mutex().unlock();
                return;
        }
        LockableChunk l_chunk = world.getAbsoluteAt(abs_pos);
        Chunk *actual_chunk_ptr = l_chunk.lock();
        world.get_mutex().unlock();
        if (actual_chunk_ptr != nullptr) {
                actual_chunk_ptr->load(world);
        }
        l_chunk.unlock();
}

void WorldQueueOperation::executeCreateAndLoad(World &world) {
        executeCreate(world);
        executeLoad(world);
}

void WorldQueueOperation::executeCreateMesh(World &world) {
        // std::this_thread::sleep_for(1000ms);
        world.get_mutex().lock();
        LockableChunk l_chunk = world.getAbsoluteAt(abs_pos);
        Chunk *actual_chunk_ptr = l_chunk.lock();

        world.get_mutex().unlock();

        if (actual_chunk_ptr != nullptr) {
                // creating the mesh
                actual_chunk_ptr->createMesh(world);
                // ask the main thread to be loaded on the gpu
                world.addChunkMeshToLoad(abs_pos);
        }
        l_chunk.unlock();
}
