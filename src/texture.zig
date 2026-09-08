//! A texture to be loaded with STB Image

const Texture = @This();

const std = @import("std");

const stbi = @import("c.zig").stb;
const g = @import("c.zig").gl;

ident: c_int,
height: u16,
width: u16,

const used_texture_height = 16;
const used_texture_width = 96;

pub fn create(path: []const u8) Texture {
    var correctly_loaded = false;
    // vars to be returned by the load
    var real_texture_width = undefined;
    var real_texture_height = undefined;
    var nb_channels = undefined;

    const data = stbi.stbi_load(path, &real_texture_height, &real_texture_width, &nb_channels, 0);

    var id = undefined;
    if (data != null) {
        g.glGenTextures(1, &id);
        g.glBindTexture(g.GL_TEXTURE_2D, id);

        g.glTexImage2D(g.GL_TEXTURE_2D, 0, g.GL_RGBA, real_texture_width, real_texture_height, 0, g.GL_RGBA, g.GL_UNSIGNED_BYTE, data);
        g.glGenerateMipmap(g.GL_TEXTURE_2D);
        correctly_loaded = true;

        stbi.stbi_image_free(data);
    } else {
        std.log.err("Could not load the data for image at path :", .{path});
    }

    return .{
        .ident = id,
        .height = real_texture_height,
        .width = real_texture_width,
    };
}

pub fn bind(self: *Texture) void {
    g.glTexParameteri(g.GL_TEXTURE_2D, g.GL_TEXTURE_MIN_FILTER, g.GL_NEAREST_MIPMAP_NEAREST);
    g.glTexParameteri(g.GL_TEXTURE_2D, g.GL_TEXTURE_MAG_FILTER, g.GL_NEAREST);

    g.glBindTexture(g.GL_TEXTURE_2D, self.ident);
}

pub fn deinit(self: *Texture) void {
    g.glDeleteTextures(1, &self.ident);
}

pub fn getVerticalRatio(self: *Texture) f32 {
    return used_texture_height / self.height;
}
