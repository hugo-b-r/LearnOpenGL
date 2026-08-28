//! A chunk is a piece of the world. The world is subdivided in chunks to be
//! able to get the most flexibility in memory layout and interactions.
const Chunk = @This();

const std = @import("std");
const cglm = @import("c.zig").cglm;
const BlockType = @import("blocks.zig").BlockType;
const World = @import("world.zig");

/// Dimensions of the chunk in blocks : width
pub const chunk_width: u16 = 32;
/// Dimensions of the chunk in blocks : length
pub const chunk_length: u16 = 32;
/// Dimensions of the chunk in blocks : height
pub const chunk_height: u16 = 256;

/// The blocks present in the chunk
blocks: []BlockType,
/// The pos of the world in the world, in chunk coordinates (not block coordinates)
position: cglm.ivec2,

world: *World,

/// Create a chunk with memory allocation so you MUST use the deinit method.
pub fn init(allocator: std.mem.Allocator, position: cglm.ivec2, empty: bool) !Chunk {
    const blocks = try allocator.alloc(chunk_width * chunk_length * chunk_height);

    // reset all the blocks
    if (empty) {
        for (0..chunk_width * chunk_length * chunk_height) |i| {
            blocks[i] = BlockType.Air;
        }
    }
    return .{
        .blocks = blocks,
        .position = position,
    };
}

/// Deinit the chunk (so frees the memory too)
pub fn deinit(self: *Chunk, allocator: std.mem.Allocator) void {
    allocator.free(self.blocks);
}

/// Returns a block located at pos (x, y z) referenced to the origin of the chunk
pub fn getBlockChunkPos(self: *Chunk, x: usize, y: usize, z: usize) *BlockType {
    return &self.blocks[x * chunk_height * chunk_length + z * chunk_height + y];
}
