const std = @import("std");

const ogl = @import("opengl/opengl.zig");
const conf = @import("configuration.zig");
const cglm = @import("c.zig").cglm;
const blocks = @import("blocks.zig");

const generic_window_allocator: std.mem.Allocator = std.heap.c_allocator;

/// A generic window is a vtable
/// It is an abstraction layer over the different rendering system
pub const GenericWindow = struct {
    ptr: *anyopaque,
    vtable: *const GenericWindow.VTable,

    pub const VTable = struct {
        initWindow: *const fn (ptr: *anyopaque) anyerror!void,
        shouldClose: *const fn (ptr: *anyopaque) bool,
        addBlockFace: *const fn (ptr: *anyopaque, block_pos: cglm.ivec3, block_face: blocks.BlockFaces) anyerror!void,
        removeBlockFace: *const fn (ptr: *anyopaque, block_pos: cglm.ivec3, block_face: blocks.BlockFaces) anyerror!void,
        deinitWindow: *const fn (ptr: *anyopaque) void,
    };
};
