#include "WorldParallelOperationsManager.h"

WorldParallelOperationsManager::WorldParallelOperationsManager(World &world)
    : running(true), world(world) {
        for (int i = 0; i < nbThreads; ++i) {
                threads.push_back(std::thread(
                    &WorldParallelOperationsManager::threadFn, this, &world));
        }
}

WorldParallelOperationsManager::WorldParallelOperationsManager(
    const WorldParallelOperationsManager &m)
    : running(true), world(m.world) {
        for (int i = 0; i < nbThreads; ++i) {
                threads.push_back(std::thread(
                    &WorldParallelOperationsManager::threadFn, this, &world));
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

void WorldParallelOperationsManager::threadFn(World *world) {
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

                        op.execute(*world);
                } else {
                        actions_running_locker.unlock();
                        thread_running = false;
                }
        }
}
