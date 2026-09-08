//! Building a mesh requires a lot of work and methods so here are they.
const ChunkMeshBuilder = @This();

const std = @import("std");

const cglm = @import("c.zig").cglm;
const g = @import("c.zig").gl;
const BlockType = @import("blocks.zig").BlockType;
const Chunk = @import("chunk.zig");
const World = @import("world.zig");
const Mesh = @import("opengl/mesh.zig");

const ivec3 = cglm.ivec3;
const NeighboringFaces = struct {
    pub fn update(self: *NeighboringFaces, x: c_int, y: c_int, z: c_int) void {
        self.left = .{ x - 1, y, z };
        self.right = .{ x + 1, y, z };
        self.up = .{ x, y + 1, z };
        self.down = .{ x, y - 1, z };
        self.front = .{ x, y, z + 1 };
        self.rear = .{ x, y, z - 1 };
    }

    left: ivec3,
    right: ivec3,
    up: ivec3,
    down: ivec3,
    front: ivec3,
    rear: ivec3,
};

const c_width = @as(g.GLfloat, @floatFromInt(Chunk.chunk_width));
const c_length = @as(g.GLfloat, @floatFromInt(Chunk.chunk_length));
const c_height = @as(g.GLfloat, @floatFromInt(Chunk.chunk_height));
const cube_width = 1.0;
const cube_height = 1.0;
const cube_length = 1.0;

chunk: *Chunk,
world: *World,
texture_vert_factor: f32,
mesh: *Mesh,

// Your way to init a chunk mesh builder.
pub fn init(chunk: *Chunk, texture_vertical_factor: f32) ChunkMeshBuilder {
    return .{
        .chunk = chunk,
        .world = chunk.world,
        .texture_vert_factor = texture_vertical_factor,
        .mesh = chunk.mesh.mesh,
    };
}

/// returns whether the side_block is transparent or not. If not, we can avoid creating
/// the face as it won't be visible anyway.
fn shouldMakeFace(
    self: *ChunkMeshBuilder,
    /// a block to check against if it is transparent. Coordinates refered to chunk origin.
    side_block: ivec3,
) bool {
    const x = side_block.x;
    const y = side_block.y;
    const z = side_block.z;

    var res: bool = false;

    if (y < 0) {
        // under everything
        res = false;
    } else if (x < Chunk.chunk_width and x >= 0 and y < Chunk.chunk_height and z < Chunk.chunk_length and z >= 0) {
        const blk = self.chunk.getBlockChunkPos(x, y, z);
        switch (blk) {
            BlockType.Air, BlockType.Leaves => res = true,
            else => res = false,
        }
    } else if (y >= Chunk.chunk_height) {
        res = true;
    } else {
        const blk = self.world.getBlockAt();
        switch (blk) {
            BlockType.Air, BlockType.Leaves => res = true,
            else => res = false,
        }
    }
    return res;
}

pub fn buildMesh(self: *ChunkMeshBuilder) usize {
    self.mesh.clear();
}

fn addBlockMesh(self: *ChunkMeshBuilder, allocator: std.mem.Allocator, i: c_int, j: c_int, k: c_int) void {
    var neighboors: NeighboringFaces = undefined;
    neighboors.update(i, j, k);

    const i_f = @as(f32, @floatFromInt(i));
    const j_f = @as(f32, @floatFromInt(j));
    const k_f = @as(f32, @floatFromInt(k));

    const block = self.chunk.getBlockChunkPos(i, j, k);
    if (block == BlockType.Air) {
        // don't add anything
        return;
    }

    if (self.shouldMakeFace(neighboors.up)) {
        const tmp = self.upFace(block, i_f, j_f, k_f);
        self.mesh.vertex_positions.appendSlice(allocator, tmp);
    }
}

// the upface of a block of type select
fn upFace(self: *ChunkMeshBuilder, select: BlockType, x: f32, y: f32, z: f32) [48]g.GLfloat {
    return .{
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.0,
        (1.0 + select) * self.texture_vert_factor,
        0,
        1,
        0,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.166667,
        (0.0 + select) * self.texture_vert_factor,
        0,
        1,
        0,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.166667,
        (1.0 + select) * self.texture_vert_factor,
        0,
        1,
        0,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.166667,
        (0.0 + select) * self.texture_vert_factor,
        0,
        1,
        0,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.0,
        (1.0 + select) * self.texture_vert_factor,
        0,
        1,
        0,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.0,
        (0.0 + select) * self.texture_vert_factor,
        0,
        1,
        0,
    };
}

fn downFace(self: *ChunkMeshBuilder, select: BlockType, x: f32, y: f32, z: f32) [48]g.GLfloat {
    return .{
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        1.0,
        (0.0 + select) * self.texture_vert_factor,
        0,
        -1,
        0,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.833333,
        (0.0 + select) * self.texture_vert_factor,
        0,
        -1,
        0, // Ordre de coins ajusté
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        1.0,
        (1.0 + select) * self.texture_vert_factor,
        0,
        -1,
        0,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        1.0,
        (1.0 + select) * self.texture_vert_factor,
        0,
        -1,
        0,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.833333,
        (0.0 + select) * self.texture_vert_factor,
        0,
        -1,
        0,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.833333,
        (1.0 + select) * self.texture_vert_factor,
        0,
        -1,
        0,
    };
}
fn leftFace(self: *ChunkMeshBuilder, select: BlockType, x: f32, y: f32, z: f32) [48]g.GLfloat {
    return .{
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.5,
        (1.0 + select) * self.texture_vert_factor,
        -1,
        0,
        0,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.333333,
        (1.0 + select) * self.texture_vert_factor,
        -1,
        0,
        0,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.5,
        (0.0 + select) * self.texture_vert_factor,
        -1,
        0,
        0,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.333333,
        (0.0 + select) * self.texture_vert_factor,
        -1,
        0,
        0,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.5,
        (0.0 + select) * self.texture_vert_factor,
        -1,
        0,
        0,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.333333,
        (1.0 + select) * self.texture_vert_factor,
        -1,
        0,
        0,
    };
}
fn rightFace(self: *ChunkMeshBuilder, select: BlockType, x: f32, y: f32, z: f32) [48]g.GLfloat {
    return .{
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.833333,
        (1.0 + select) * self.texture_vert_factor,
        1,
        0,
        0, // Ordre de coins ajusté pour l'orientation
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.666667,
        (0.0 + select) * self.texture_vert_factor,
        1,
        0,
        0,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.666667,
        (1.0 + select) * self.texture_vert_factor,
        1,
        0,
        0,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.666667,
        (0.0 + select) * self.texture_vert_factor,
        1,
        0,
        0,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.833333,
        (1.0 + select) * self.texture_vert_factor,
        1,
        0,
        0,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.833333,
        (0.0 + select) * self.texture_vert_factor,
        1,
        0,
        0,
    };
}
fn frontFace(self: *ChunkMeshBuilder, select: BlockType, x: f32, y: f32, z: f32) [48]g.GLfloat {
    return .{
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.166667,
        (1.0 + select) * self.texture_vert_factor,
        0,
        0,
        -1,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.333333,
        (1.0 + select) * self.texture_vert_factor,
        0,
        0,
        -1,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.333333,
        (0.0 + select) * self.texture_vert_factor,
        0,
        0,
        -1,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.333333,
        (0.0 + select) * self.texture_vert_factor,
        0,
        0,
        -1,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.166667,
        (0.0 + select) * self.texture_vert_factor,
        0,
        0,
        -1,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0 + 1.0) * cube_length,
        0.166667,
        (1.0 + select) * self.texture_vert_factor,
        0,
        0,
        -1,
    };
}
fn rearFace(self: *ChunkMeshBuilder, select: BlockType, x: f32, y: f32, z: f32) [48]g.GLfloat {
    return .{
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.666667,
        (1.0 + select) * self.texture_vert_factor,
        0,
        0,
        1, // Ordre de coins inversé pour l'orientation
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.5,
        (0.0 + select) * self.texture_vert_factor,
        0,
        0,
        1,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.5,
        (1.0 + select) * self.texture_vert_factor,
        0,
        0,
        1,
        (x - c_width / 2.0 + 1.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.5,
        (0.0 + select) * self.texture_vert_factor,
        0,
        0,
        1,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.666667,
        (1.0 + select) * self.texture_vert_factor,
        0,
        0,
        1,
        (x - c_width / 2.0) * cube_width,
        (y - c_height / 2.0 + 1.0) * cube_height,
        (z - c_length / 2.0) * cube_length,
        0.666667,
        (0.0 + select) * self.texture_vert_factor,
        0,
        0,
        1,
    };
}
