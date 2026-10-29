#include "LockableChunk.h"
#include <optional>

// ReadSafeChunk::ReadSafeChunk() : chunk_value() {}
ReasonToKeepManager::ReasonToKeepManager() {}

ReasonToKeepManager::ReasonToKeepManager(const ReasonToKeepManager &other_chunk) {
}

ReasonToKeepManager::~ReasonToKeepManager() {
        // delete chunk_value;
}



void ReasonToKeepManager::addReason(ReasonToKeep r) {
        locker_reasons.lock();

        reason.insert(r);
        locker_reasons.unlock();
}

void ReasonToKeepManager::removeReason(ReasonToKeep r) {
        locker_reasons.lock();

        reason.erase(r);

        locker_reasons.unlock();
}

bool ReasonToKeepManager::anyReasonToKeep() {
        locker_reasons.lock();

        bool res = !reason.empty();

        locker_reasons.unlock();

        return res;
}


