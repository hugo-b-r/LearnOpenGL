const World = @This();
const mem = std.mem;
const Chunk = @import("chunk.zig");

chunks: []Chunk,

pub fn init(allocator: mem.Allocator, render_distance: u8) World {}
