#ifndef WORLDQUEUEOPERATION_H
#define WORLDQUEUEOPERATION_H

#include <glm/glm.hpp>
#include <World/WorldController.h>
#include <mutex>

/* World queue operation
 *
 * this file manages the storing of the  operations added to a queue
 */

enum class WldQOperationType { LoadOrGenerate, LoadOrGenerateThenMesh, CreateMesh };

class World;

class WorldQueueOperation {
      public:
        // builds a World queue operation that will do type operation at chunk
        // of pos absolute_pos
        WorldQueueOperation(glm::ivec2 absolute_pos, WldQOperationType type);
        virtual ~WorldQueueOperation();

        void execute(WorldController &world_controller);

      protected:
        // the absolute position of the thread that we will manage
        glm::ivec2 abs_pos;
        // the type of operation to apply to it.
        WldQOperationType type;

        void executeLoadOrGenerate(WorldController &world_controller);

        void executeCreateMesh(WorldController &world_controller);
};

#endif
