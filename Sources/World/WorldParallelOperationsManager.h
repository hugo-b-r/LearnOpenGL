#ifndef WORLDPARALLELOPERATIONSMANAGER_H
#define WORLDPARALLELOPERATIONSMANAGER_H

#include <queue>
#include <thread>
#include <vector>

#include "World.h"
#include "WorldQueueOperation.h"

class WorldQueueOperation;
class World;

class WorldParallelOperationsManager {
      public:
        WorldParallelOperationsManager(World &world);
        WorldParallelOperationsManager(const WorldParallelOperationsManager &m);
        ~WorldParallelOperationsManager();

        void pushBackOperation(WorldQueueOperation op);

        void threadFn(World *world);

      protected:
        bool running;
        std::queue<WorldQueueOperation> actions;
        std::mutex actions_running_locker;

        const int nbThreads = 7;
        std::vector<std::thread> threads;
        World &world;
};

#endif
