//! Shaders are the most important part of an opengl program. This is the
//! algorithm that paint colours for each pixel. Here is a simple data structure
//! to manipulate ones from opengl.

const std = @import("std");
const g = @import("c.zig").gl;

const Dir = std.Io.Dir;
const Io = std.Io;

const Shader = @This();

/// The program id created by the init method.
id: g.GLuint,

/// Initializes a tuple of vertex shader and fragment shader. Must be used in
/// a valid opengl context.
/// Never forget to deinit as you need to destroy the shader program.
pub fn init(vertex_path: []const u8, fragment_path: []const u8, io: Io, allocator: std.mem.Allocator) !Shader {
    std.log.info("Reading Vertex shader's file", .{});
    const vert_code = try readFileInAllocedBuffer(io, allocator, vertex_path);
    defer allocator.free(vert_code);

    const frag_code = try readFileInAllocedBuffer(io, allocator, fragment_path);
    defer allocator.free(frag_code);

    var success: c_int = 1;
    var infoLog: [512]u8 = undefined;

    // create vertex shader
    //
    const vertex_shader_id = g.glCreateShader(g.GL_VERTEX_SHADER);

    g.glShaderSource(vertex_shader_id, 1, &vert_code.ptr, null);
    g.glCompileShader(vertex_shader_id);
    g.glGetShaderiv(vertex_shader_id, g.GL_COMPILE_STATUS, &success);
    if (success == 0) {
        g.glGetShaderInfoLog(vertex_shader_id, 512, null, &infoLog);
        std.log.err("Error when compiling vertex shader : {s}", .{infoLog});
    } else {
        std.log.info("Success compiling vertex shader", .{});
    }

    // same for fragment shader
    //
    const fragment_shader_id = g.glCreateShader(g.GL_FRAGMENT_SHADER);

    g.glShaderSource(fragment_shader_id, 1, &frag_code.ptr, null);
    g.glCompileShader(fragment_shader_id);
    g.glGetShaderiv(fragment_shader_id, g.GL_COMPILE_STATUS, &success);
    if (success == 0) {
        g.glGetShaderInfoLog(fragment_shader_id, 512, null, &infoLog);
        std.log.err("Error when compiling fragment shader : {s}", .{infoLog});
    } else {
        std.log.info("Success compiling fragment shader", .{});
    }

    // create the shader program
    //
    const id = g.glCreateProgram();
    // attach vertex
    g.glAttachShader(id, vertex_shader_id);
    // attach fragment
    g.glAttachShader(id, fragment_shader_id);

    g.glLinkProgram(id);
    g.glGetProgramiv(id, g.GL_LINK_STATUS, &success);
    if (success == 0) {
        g.glGetShaderInfoLog(fragment_shader_id, 512, null, &infoLog);
        std.log.err("Error when linking shader program : {s}", .{infoLog});
    } else {
        std.log.info("Success linking shader program", .{});
    }

    // Now the vertex and fragment shaders are linked to the program. This is safe to delete them.
    g.glDeleteShader(vertex_shader_id);
    g.glDeleteShader(fragment_shader_id);

    // save the shader program id for later
    return .{
        .id = id,
    };
}

/// The destructor for this.
pub fn deinit(self: *const Shader) void {
    // we need to delete the program we created
    g.glDeleteProgram(self.id);
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
    const buf = try readFileInAllocedBuffer(io, allocator, "../src/opengl.zig");
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
