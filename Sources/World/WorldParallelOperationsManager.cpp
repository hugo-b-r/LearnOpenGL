#include "WorldParallelOperationsManager.h"

WorldParallelOperationsManager::WorldParallelOperationsManager(WorldController &world_controller)
    : running(true), world_controller(world_controller) {
        for (int i = 0; i < nbThreads; ++i) {
                threads.push_back(std::thread(
                    &WorldParallelOperationsManager::threadFn, this, &world_controller));
        }
}

WorldParallelOperationsManager::WorldParallelOperationsManager(
    const WorldParallelOperationsManager &m)
    : running(true), world_controller(m.world_controller) {
        for (int i = 0; i < nbThreads; ++i) {
                threads.push_back(std::thread(
                    &WorldParallelOperationsManager::threadFn, this, &world_controller));
        }
}

WorldParallelOperationsManager::~WorldParallelOperationsManager() {
        actions_running_locker.lock();
        running = false;
        actions_running_locker.unlock();
        for (auto &t : threads) {
                t.join();
        }
}

void WorldParallelOperationsManager::pushBackOperation(WorldQueueOperation op) {
        actions_running_locker.lock();
        actions.push(op);
        actions_running_locker.unlock();
}

void WorldParallelOperationsManager::threadFn(WorldController *world_controller) {
        bool thread_running = true;
        while (thread_running) {
                actions_running_locker.lock();
                while (actions.size() == 0 && running) {
                        actions_running_locker.unlock();
                        actions_running_locker.lock(); // active wait !!!
                }
                if (running) {
                        // active wait
                        WorldQueueOperation op = actions.front();
                        // remove the acessed element
                        actions.pop();
                        actions_running_locker.unlock();

                        op.execute(*world_controller);
                } else {
                        actions_running_locker.unlock();
                        thread_running = false;
                }
        }
}
