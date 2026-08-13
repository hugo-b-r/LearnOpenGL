const m = @cImport({
    @cInclude("cglm/cglm.h");
});

const std = @import("std");

const Configuration = @import("configuration.zig");

/// Camera is what the user of the game sees
pub const Camera = struct {
    pos: m.vec3,
    orientation: m.vec3,
    pub fn updateProjViewMatrix(self: *Camera) void {
        self.pos = self.pos;
        std.debug.print("updated proj view matrixes", .{});
    }

    pub fn Default() Camera {
        var def_pos: m.vec3 = undefined;
        m.glm_vec3_one(&def_pos);
        var def_orientation: m.vec3 = undefined;
        m.glm_vec3_one(&def_orientation);
        return .{
            .pos = def_pos,
            .orientation = def_orientation,
        };
    }
};

/// Scene struct to store all objects from a scene
pub const Scene = struct {
    camera: Camera,
    configuration: Configuration,
};
