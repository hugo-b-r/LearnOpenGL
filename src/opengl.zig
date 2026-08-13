const g = @import("c.zig").gl;
const m = @cImport("c.zig").cglm;
const GenericWindow = @import("generic_window.zig").GenericWindow;

const std = @import("std");

const ErrorTypes = error{ GLFWError, GLADError };

pub const OpenGLWindow = @This();

gl_window: ?*g.GLFWwindow = null,

pub fn window(self: *OpenGLWindow) GenericWindow {
    return .{ .ptr = self, .vtable = &.{
        .initWindow = initWindow,
        .shouldClose = shouldClose,
        .addBlockFace = undefined,
        .removeBlockFace = undefined,
        .deinitWindow = deinit,
    } };
}

pub fn initWindow(windowWidth: i32, windowHeight: i32) ErrorTypes!OpenGLWindow {
    var gl_window: ?*g.GLFWwindow = null;
    _ = g.glfwInit();
    g.glfwWindowHint(g.GLFW_CONTEXT_VERSION_MAJOR, 3);
    g.glfwWindowHint(g.GLFW_CONTEXT_VERSION_MINOR, 3);
    g.glfwWindowHint(g.GLFW_OPENGL_PROFILE, g.GLFW_OPENGL_CORE_PROFILE);

    gl_window = g.glfwCreateWindow(@as(c_int, windowWidth), @as(c_int, windowHeight), "LearnOpenGL", null, null);
    if (gl_window == null) {
        std.log.info("Failed to create GLFW Window", .{});
        g.glfwTerminate();
        return ErrorTypes.GLFWError;
    }
    g.glfwMakeContextCurrent(gl_window);
    if (g.gladLoadGLLoader(@ptrCast(&g.glfwGetProcAddress)) == 0) {
        std.log.err("Failed to initialize GLAD", .{});
        return ErrorTypes.GLADError;
    }
    std.log.info("Window has been created", .{});

    g.glfwSetInputMode(gl_window, g.GLFW_CURSOR, g.GLFW_CURSOR_DISABLED);

    return OpenGLWindow{ .gl_window = gl_window };
}

fn registerCallbacks(self: *OpenGLWindow) void {
    _ = g.glfwSetFramebufferSizeCallback(self.gl_window, @ptrCast(&frameBufferSizeCallback));
}

fn frameBufferSizeCallback() void {}

pub fn deinit(self: *OpenGLWindow) void {
    if (self.gl_window != null) {
        g.glfwDestroyWindow(self.gl_window);
    }
    g.glfwTerminate();
}

pub fn shouldClose(self: *const OpenGLWindow) u8 {
    return @intCast(g.glfwWindowShouldClose(self.gl_window));
}

// Testing!

test "création de fenetre" {
    var Fen = try OpenGLWindow.initWindow(1000, 600);
    defer Fen.deinit();

    const reference_time = g.glfwGetTime();
    while (g.glfwGetTime() < reference_time + 1) {
        g.glClearColor(0.2, 0.3, 0.3, 1.0);
        g.glClear(g.GL_COLOR_BUFFER_BIT);

        g.glfwSwapBuffers(Fen.gl_window);
        g.glfwPollEvents();
    }
}

test "création de fenetre et ajout des callbacks" {
    var Fen = try OpenGLWindow.initWindow(1000, 600);
    defer Fen.deinit();

    Fen.registerCallbacks();

    const reference_time = g.glfwGetTime();
    while (g.glfwGetTime() < reference_time + 1) {
        g.glClearColor(0.2, 0.3, 0.3, 1.0);
        g.glClear(g.GL_COLOR_BUFFER_BIT);

        g.glfwSwapBuffers(Fen.gl_window);
        g.glfwPollEvents();
    }
}
