const std = @import("std");
const g = @import("c.zig").gl;

const Dir = std.Io.Dir;
const Io = std.Io;

const Shader = @This();

id: g.GLint,
/// Initializes a tuple of vertex shader and fragment shader. Must be used in
/// a valid opengl context.
pub fn init(vertex_path: []const u8, fragment_path: []const u8, io: Io, allocator: std.mem.Allocator) !Shader {
    std.log.info("Reading Vertex shader's file", .{});
    const vert_buffer = try readFileInAllocedBuffer(io, allocator, vertex_path);
    defer allocator.free(vert_buffer);

    const frag_buffer = try readFileInAllocedBuffer(io, allocator, fragment_path);
    defer allocator.free(frag_buffer);

    return .{
        .id = 0,
    };
}

/// This function reads a file into a buffer and returns it. The buffer is
/// allocated at the right size but never freed here. It needs to be done by
/// YOU the caller using the passed allocator.
fn readFileInAllocedBuffer(io: Io, allocator: std.mem.Allocator, path: []const u8) ![]u8 {
    var file = try std.Io.Dir.cwd().openFile(io, path, .{});
    defer file.close(io);

    const filesize = try file.length(io);
    const buffer = try allocator.alloc(u8, filesize);
    // defer allocator.free(frag_buffer);

    _ = file.readPositionalAll(io, buffer, 0) catch |err| {
        std.log.err("read {s} failed: {}", .{ path, err });
    };

    return buffer;
}

test "reading file and allocating the buffer" {
    const io: std.Io = std.testing.io;
    const allocator: std.mem.Allocator = std.testing.allocator;
    const buf = try readFileInAllocedBuffer(io, allocator, "./src/opengl.zig");
    defer allocator.free(buf);

    const reference_buf = @embedFile("./opengl.zig");
    const other = reference_buf[0 .. reference_buf.len - 1];
    // std.debug.print("{s}", .{other});
    // std.debug.print("\n---\n", .{});
    // std.debug.print("{s}", .{buf});

    var equal = true;
    outer_for: for (0..reference_buf.len - 1) |i| {
        if (other[i] != buf[i]) {
            equal = false;
            break :outer_for;
        }
    }
    // could not make std.mem.eql work
    std.debug.assert(equal);
}
