#ifndef WORLDPARALLELOPERATIONSMANAGER_H
#define WORLDPARALLELOPERATIONSMANAGER_H

#include <queue>
#include <thread>
#include <vector>
#include <mutex>


class WorldQueueOperation;
class World;
class WorldController;

class WorldParallelOperationsManager {
      public:
        WorldParallelOperationsManager(WorldController *world_controller);
        WorldParallelOperationsManager(const WorldParallelOperationsManager &m);
        ~WorldParallelOperationsManager();

        void pushBackOperation(WorldQueueOperation op);

        void threadFn(WorldController *world_controller);

      protected:
        bool running;
        std::queue<WorldQueueOperation> actions;
        std::mutex actions_running_locker;

        const int nbThreads = 14;
        std::vector<std::thread> threads;
        WorldController *world_controller;
};

#endif
