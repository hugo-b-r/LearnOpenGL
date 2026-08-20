const g = @import("../c.zig").gl;

const Mesh = @This();

vertex_positions: g.GLfloat,
texture_coordinates: g.GLfloat,
indices: g.GLuint,

// pub fn init() Mesh {}
