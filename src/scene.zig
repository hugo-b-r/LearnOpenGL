const m = @cImport({
    @cInclude("cglm/cglm.h");
});

const conf = @import("configuration.zig");

/// Camera is what the user of the game sees
const Camera = struct { pos: m.vec3, oritentation: m.vec3 };

/// Scene struct to store all objects from a scene
const Scene = struct {
    camera: Camera,
    configuration: conf.Configuration,
};
