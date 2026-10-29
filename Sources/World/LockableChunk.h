#ifndef LOCKABLECHUNK_H
#define LOCKABLECHUNK_H

#include "Chunk.h"

#include <mutex>
#include <optional>
#include <unordered_set>


enum class ReasonToKeep {InDirectRenderZone, UsedForMeshing};
class ReasonToKeepManager {
      public:
        // ReadSafeChunk();
        ReasonToKeepManager();
        ReasonToKeepManager(const ReasonToKeepManager&);
        virtual ~ReasonToKeepManager();


        void addReason(ReasonToKeep r);
        void removeReason(ReasonToKeep r);
        bool anyReasonToKeep();
      protected:
        std::mutex locker_reasons;
        std::unordered_set<ReasonToKeep> reason;

};

#endif
