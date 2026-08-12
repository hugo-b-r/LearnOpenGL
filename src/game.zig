const std = @import("std");

const s = @import("scene.zig");
const w = @import("generic_window.zig");

/// A game struct contains all the data and rendering objects
const Game = struct {
    scene: s.Scene,
    window: w.GenericWindow,
    fixed_allocator: std.heap.FixedBufferAllocator,

    pub fn init() Game {
        allo = std.heap.FixedBufferAllocator;
        const scene = s.Scene;

        const try allo.
        
        const window = w.GenericWindow{.opengl = };
        return Game{
            .scene = scene,
            .window = window,
        };
    }

    pub fn deinit() void {
        allo.free(window);
    }

};

test "basic use case for game" {
    _ = Game.Init();
}
