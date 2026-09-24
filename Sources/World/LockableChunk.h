#ifndef LOCKABLECHUNK_H
#define LOCKABLECHUNK_H

#include "Chunk.h"

#include <mutex>
#include <optional>
#include <unordered_set>


enum class ReasonToKeep {InDirectRenderZone, UsedForMeshing};
class LockableChunk {
      public:
        LockableChunk();
        LockableChunk(Chunk &c);
        LockableChunk(const LockableChunk &);
        ~LockableChunk();

        Chunk * lock();
        std::optional<Chunk *> try_lock();
        void unlock();

        void addReason(ReasonToKeep r);
        void removeReason(ReasonToKeep r);
        bool anyReasonToKeep();
      protected:
        std::unordered_set<ReasonToKeep> reason;
        std::mutex locker;
        Chunk chunk_value;
};

#endif
