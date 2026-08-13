const g = @import("c.zig").gl;
const m = @cImport("c.zig").cglm;
const Scene = @import("scene.zig").Scene;
const Camera = @import("scene.zig").Camera;
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

pub fn initWindow(windowWidth: i32, windowHeight: i32, scene: *Scene) ErrorTypes!OpenGLWindow {
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

    g.glfwSetWindowUserPointer(gl_window, @ptrCast(scene));
    g.glfwSetInputMode(gl_window, g.GLFW_CURSOR, g.GLFW_CURSOR_DISABLED);

    return OpenGLWindow{ .gl_window = gl_window };
}

fn registerCallbacks(self: *OpenGLWindow) void {
    _ = g.glfwSetFramebufferSizeCallback(self.gl_window, @ptrCast(@alignCast(&frameBufferSizeCallback)));
    _ = g.glfwSetKeyCallback(self.gl_window, @ptrCast(@alignCast(&keyCallback)));
}

fn frameBufferSizeCallback(win: ?*g.GLFWwindow, width: c_int, height: c_int) callconv(.c) void {
    const s: *Scene = @ptrCast(@alignCast(g.glfwGetWindowUserPointer(win)));
    s.configuration.window_height = @intCast(height);
    s.configuration.window_width = @intCast(width);
    s.camera.updateProjViewMatrix();
    g.glViewport(0, 0, width, height);
}

fn keyCallback(win: ?*g.GLFWwindow, key: c_int, scancode: c_int, action: c_int, mods: c_int) callconv(.c) void {
    _ = mods; // unused
    _ = scancode; //unused

    const s: *Scene = @ptrCast(@alignCast(g.glfwGetWindowUserPointer(win)));
    if (key == g.GLFW_KEY_Z and action == g.GLFW_PRESS) {
        // to be fixed : check opengl polygone mode using some sort of
        // app variable that has been linked to the gl window
        var polyg_mode: g.GLint = undefined;
        g.glGetIntegerv(g.GL_POLYGON_MODE, &polyg_mode);

        if (polyg_mode == g.GL_LINE) {
            g.glPolygonMode(g.GL_FRONT_AND_BACK, g.GL_FILL);
        } else {
            g.glPolygonMode(g.GL_FRONT_AND_BACK, g.GL_LINE);
        }
    }
    if (key == g.GLFW_KEY_ESCAPE and action == g.GLFW_PRESS) {
        s.configuration.mouse_free = !(s.configuration.mouse_free);
        if (s.configuration.mouse_free) {
            g.glfwSetInputMode(win, g.GLFW_CURSOR, g.GLFW_CURSOR_NORMAL);
        } else {
            g.glfwSetInputMode(win, g.GLFW_CURSOR, g.GLFW_CURSOR_DISABLED);
        }
    }
    if (key == g.GLFW_KEY_F3 and action == g.GLFW_PRESS) {
        s.configuration.show_debug_infos =
            !(s.configuration.show_debug_infos);
        std.log.debug("Show or hide now debug infos", .{});
    }
}

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
    var sc: Scene = .{
        .camera = Camera.Default(),
        .configuration = .{},
    };
    var Fen = try OpenGLWindow.initWindow(1000, 600, &sc);
    defer Fen.deinit();

    const reference_time = g.glfwGetTime();
    while (g.glfwGetTime() < reference_time + 1) {
        g.glClearColor(0.2, 0.3, 0.3, 1.0);
        g.glClear(g.GL_COLOR_BUFFER_BIT);

        g.glfwSwapBuffers(Fen.gl_window);
        g.glfwPollEvents();
    }
    std.debug.print("the window is closed now, should have passed", .{});
}

test "création de fenetre et ajout des callbacks" {
    var sc: Scene = .{
        .camera = Camera.Default(),
        .configuration = .{},
    };
    var Fen = try OpenGLWindow.initWindow(1000, 600, &sc);
    defer Fen.deinit();

    Fen.registerCallbacks();

    const reference_time = g.glfwGetTime();
    while (g.glfwGetTime() < reference_time + 1) {
        g.glClearColor(0.2, 0.3, 0.3, 1.0);
        g.glClear(g.GL_COLOR_BUFFER_BIT);

        g.glfwSwapBuffers(Fen.gl_window);
        g.glfwPollEvents();
    }
    std.debug.print("the window is closed now, should have passed", .{});
}
