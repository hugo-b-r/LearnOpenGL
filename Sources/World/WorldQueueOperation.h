#ifndef WORLDQUEUEOPERATION
#define WORLDQUEUEOPERATION

#include <glm/glm.hpp>

#include "World.h"

/* World queue operation
 *
 * this file manages the storing of the  operations added to a queue
 */

enum class WldQOperationType { kCreate, kLoad, kCreateAndLoad, kCreateMesh };

class WorldQueueOperation {
      public:
        // builds a World queue operation that will do type operation at chunk
        // of pos absolute_pos
        WorldQueueOperation(glm::ivec2 absolute_pos, WldQOperationType type,
                            World &world);
        virtual ~WorldQueueOperation();

        void execute();

      protected:
        // the absolute position of the thread that we will manage
        glm::ivec2 abs_pos;
        // the type of operation to apply to it.
        WldQOperationType type;
        // the world, what else ?
        World &world;

        void executeCreate();

        void executeLoad();

        void executeCreateAndLoad();

        void executeCreateMesh();
}

#endif
