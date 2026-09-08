const g = @import("../c.zig").gl;

const Mesh = @This();

const std = @import("std");

vertex_positions: std.ArrayList(g.GLfloat),
texture_coordinates: std.ArrayList(g.GLfloat),
indices: std.ArrayList(g.GLuint),

pub fn clear(self: *Mesh) void {
    self.vertex_positions = {};
    self.texture_coordinates = {};
    self.indices = {};
}

// pub fn init() Mesh {}
