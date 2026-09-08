//! The chunk mesh is the mesh representing the chunk's terrain.
const ChunkMesh = @This();

const std = @import("std");

const Chunk = @import("chunk.zig");
const Mesh = @import("opengl/mesh.zig");
const ChunkMeshBuilder = @import("chunk_mesh_builder.zig");
const TexturesManager = @import("textures_manager.zig");
const g = @import("c.zig").gl;

chunk: *Chunk,
mesh: Mesh,
vao: g.GLuint,
vbo: g.GLuint,
ebo: g.GLuint,

pub fn init(allocator: std.mem.Allocator, c: *Chunk) ChunkMesh {
    _ = allocator;
    var m: ChunkMesh = .{
        .chunk = c,
    };
    m.mesh = Mesh.init();
    return m;
}

pub fn deinit(self: *ChunkMesh, allocator: std.mem.Allocator) void {}

pub fn create_the_mesh(self: *ChunkMesh) void {
    var builder: ChunkMeshBuilder = ChunkMeshBuilder.init(
        self.chunk,
        TexturesManager.texture_manager.block_atlas.getVerticalRatio(),
    );

    var nb_vert = builder.buildMesh();
}

pub fn render(self: *ChunkMesh) void {}
