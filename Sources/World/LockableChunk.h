#ifndef LOCKABLECHUNK_H
#define LOCKABLECHUNK_H

#include "Chunk.h"

#include <mutex>
#include <optional>

class Chunk;

class LockableChunk {
      public:
        LockableChunk();
        LockableChunk(const LockableChunk &);
        ~LockableChunk();

        Chunk *&lock();
        std::optional<Chunk *> try_lock();
        void unlock();

      protected:
        std::mutex locker;
        Chunk *chunk_value;
};

#endif
