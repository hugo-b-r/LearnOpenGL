#ifndef WORLDPARALLELOPERATIONSMANAGER_H
#define WORLDPARALLELOPERATIONSMANAGER_H

#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class WorldQueueOperation;
class World;
class WorldController;

/// Handles all operations happening inside the thread pool of the world.
///
/// It hols the different queued actions, the threads, do the repartition of all
/// that...
class WorldParallelOperationsManager {
      public:
        /// Default constructor
        WorldParallelOperationsManager(WorldController *world_controller);
        /// Default constructor of copy
        WorldParallelOperationsManager(const WorldParallelOperationsManager &m);
        /// Default destructor
        ~WorldParallelOperationsManager();

        /// Add one operation to do
        ///
        /// Adds to the queue with a lock.
        void pushBackOperation(WorldQueueOperation op);

        /// Generic function to pass to the thread execution
        void threadFn(WorldController *world_controller);

      protected:
        /// Tells a thread whether running
        ///
        /// Used to shut down properly when shutting down the program.
        bool running;
        /// The queue of actions to execute
        std::queue<WorldQueueOperation> actions;
        /// The lock mutex for #actions
        std::mutex actions_running_locker;

        /// Number of threads to run
        const int nbThreads = 14;
        /// The threads storage
        std::vector<std::thread> threads;

        /// Reference to the world controller
        ///
        /// Used to talk to the world and the renderer...
        WorldController *world_controller;
};

#endif
