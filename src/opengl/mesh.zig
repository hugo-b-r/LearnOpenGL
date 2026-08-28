const g = @import("../c.zig").gl;

const Mesh = @This();

const std = @import("std");

vertex_positions: std.ArrayList(g.GLfloat),
texture_coordinates: std.ArrayList(g.GLfloat),
indices: std.ArrayList(g.GLuint),

// pub fn init() Mesh {}
