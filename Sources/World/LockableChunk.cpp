#include "LockableChunk.h"
#include <optional>

LockableChunk::LockableChunk(Chunk &c) : chunk_value(c) {}

LockableChunk::LockableChunk(const LockableChunk &other_chunk) {
        chunk_value = other_chunk.chunk_value;
}

LockableChunk::~LockableChunk() {
        // delete chunk_value;
}

Chunk *LockableChunk::lock() {
        locker.lock();
        return &chunk_value;
}

std::optional<Chunk*> LockableChunk::try_lock() {
        if (locker.try_lock()) {
                return &chunk_value;
        } else {
                return std::nullopt;
        }
}

void LockableChunk::unlock() { locker.unlock(); }
