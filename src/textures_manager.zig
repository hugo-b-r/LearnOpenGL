//! Texture manager Look in textureManager for the answers

const std = @import("std");

const Texture = @import("texture.zig");

const stb = @import("c.zig").stb;

const TextureManager = struct {
    block_atlas: *Texture,

    pub fn init() TextureManager {
        var t: TextureManager = undefined;
        t.block_atlas = Texture.create("../Assets/texture_atlas.png");

        return t;
    }

    pub fn destroy(self: TextureManager) void {
        self.block_atlas.deinit();
    }
};

// THE texture manager
pub var texture_manager: TextureManager = undefined;

pub inline fn initTextureManager() void {
    texture_manager = TextureManager.init();
}

pub inline fn deinitTextureManager() void {
    texture_manager.destroy();
}
