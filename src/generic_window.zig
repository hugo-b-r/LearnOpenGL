const std = @import("std");

const ogl = @import("opengl.zig");
const conf = @import("configuration.zig");

/// A generic window is a vtable
/// It is an abstraction layer over the different rendering system
pub const GenericWindow = struct {
    ptr: *anyopaque,
    init: *const fn (ptr: *anyopaque) anyerror!void,
    addBlockFace: *const fn (ptr: *anyopaque) anyerror!void,
    removeBlockFace: *const fn (ptr: *anyopaque) anyerror!void,

    /// Init a window according to the choice passed as argument. All the
    /// programming function ptr logic goes here.
    pub fn create(window_kind: conf.BackendKind) GenericWindow {
        var alloc = std.heap.FixedBufferAllocator;
        switch (window_kind) {
            // Init an OpenGL window
            conf.BackendKind.OpenGL => {},
        }
    }
};
