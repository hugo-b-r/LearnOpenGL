#ifndef WORLDQUEUEOPERATION_H
#define WORLDQUEUEOPERATION_H

#include <World/WorldController.h>
#include <glm/glm.hpp>
#include <mutex>

/// The possible operation executable on the thread pool associated with the
/// world
enum class WldQOperationType {
        LoadOrGenerate,
        LoadOrGenerateThenMesh,
        CreateMesh,
        ReloadThenMesh,
        RecreateMesh
};

class World;

/// World queue operation
///
/// This file manages the storing of the  operations added to a queue
class WorldQueueOperation {
      public:
        /// Builds a World queue operation
        ///
        /// It will do \a type operation at chunk
        // of pos \& absolute_pos
        WorldQueueOperation(glm::ivec2 absolute_pos, WldQOperationType type);

        /// Default destructor
        virtual ~WorldQueueOperation();

        /// Execution of the planned operation #type
        ///
        /// Redirects to #executeLoadOrGenerate or #executeCreateMesh.
        void execute(WorldController &world_controller);

      protected:
        /// The absolute position of the thread that we will manage
        glm::ivec2 abs_pos;
        /// The type of operation to apply to it.
        WldQOperationType type;

        /// execution for Load Or generate, using \a world_controller
        void executeLoadOrGenerate(WorldController &world_controller);

        /// execution for Mesh creation, using \a world_controller
        void executeCreateMesh(WorldController &world_controller);

        /// Same as #executeCreateMesh, but remove the older one, using \a world_controller
        void executeRecreateMesh(WorldController &world_controller);

        /// execution when reloading chunk, using \a world_controller
        void executeReloadThenMesh(WorldController &world_controller);
};

#endif
