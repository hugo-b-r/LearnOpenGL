#include "WorldQueueOperation.h"
#include <filesystem>

WorldQueueOperation::WorldQueueOperation(glm::ivec2 absolute_pos,
                                         WldQOperationType type, World &world)
    : abs_pos(absolute_pos), type(type), world(world) {}

// empty destructor as no emory management
WorldQueueOperation::~WorldQueueOperation() {}


// exeuctes the operations of the type
void WorldQueueOperation::execute() {
        switch (type) {
        case WldQOperationType::kCreate:
                return executeCreate();
                break;
        case WldQOperationType::kLoad:
                return executeLoad();
                break;
        case WldQOperationType::kCreateAndLoad:
                return executeCreateAndLoad();
                break;
        case WldQOperationType::kCreateMesh:
                return executeCreateMesh();
                break;
        }
}

void WorldQueueOperation::executeCreate() {
        Chunk* chunk_ptr = new Chunk(abs_pos);
        if (world.getAbsoluteAt(abs_pos) == nullptr) {
                world.setAbsoluteAt(abs_pos, chunk_ptr);
        }
}

void WorldQueueOperation::executeLoad() {
       if (world.getAbsoluteAt(abs_pos) != nullptr) {
                Chunk* chunk_ptr = world.getAbsoluteAt(abs_pos);
                chunk_ptr->load(world);
        }
}

void WorldQueueOperation::executeCreateAndLoad() {
        executeCreate();
        executeLoad();
}

void WorldQueueOperation::executeCreateMesh() {
       if (world.getAbsoluteAt(abs_pos) != nullptr) {
                Chunk* chunk_ptr = world.getAbsoluteAt(abs_pos);
                chunk_ptr->createMesh(world);
                chunk_ptr->loadMeshOnGPU();
        }
}
