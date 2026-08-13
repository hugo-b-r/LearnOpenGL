const g = @cImport({
    @cInclude("/home/hugo/Projets/LearnOpenGL/deps/glad/include/glad/glad.h");
    @cInclude("GLFW/glfw3.h");
    // @cInclude("g");
});

const m = @cImport({
    @cInclude("cglm/cglm.h");
});

const std = @import("std");

const ErrorTypes = error{ GLFWError, GLADError };

pub const OpenGLWindow = struct {
    window: ?*g.GLFWwindow = null,

    pub fn init(windowWidth: i32, windowHeight: i32) ErrorTypes!OpenGLWindow {
        var window: ?*g.GLFWwindow = null;
        _ = g.glfwInit();
        g.glfwWindowHint(g.GLFW_CONTEXT_VERSION_MAJOR, 3);
        g.glfwWindowHint(g.GLFW_CONTEXT_VERSION_MINOR, 3);
        g.glfwWindowHint(g.GLFW_OPENGL_PROFILE, g.GLFW_OPENGL_CORE_PROFILE);

        window = g.glfwCreateWindow(@as(c_int, windowWidth), @as(c_int, windowHeight), "LearnOpenGL", null, null);
        if (window == null) {
            std.log.info("Failed to create GLFW Window", .{});
            g.glfwTerminate();
            return ErrorTypes.GLFWError;
        }
        g.glfwMakeContextCurrent(window);
        if (g.gladLoadGLLoader(@ptrCast(&g.glfwGetProcAddress)) == 0) {
            std.log.err("Failed to initialize GLAD", .{});
            return ErrorTypes.GLADError;
        }
        std.log.info("Window has been created", .{});

        g.glfwSetInputMode(window, g.GLFW_CURSOR, g.GLFW_CURSOR_DISABLED);

        return OpenGLWindow{ .window = window };
    }

    fn registerCallbacks(self: *OpenGLWindow) void {
        _ = g.glfwSetFramebufferSizeCallback(self.window, @ptrCast(&frameBufferSizeCallback));
    }

    fn frameBufferSizeCallback() void {}

    pub fn deinit(self: *OpenGLWindow) void {
        if (self.window != null) {
            g.glfwDestroyWindow(self.window);
        }
        g.glfwTerminate();
    }

    pub fn shouldClose(self: *const OpenGLWindow) u8 {
        return @intCast(g.glfwWindowShouldClose(self.window));
    }
};

test "création de fenetre" {
    var Fen = try OpenGLWindow.init(1000, 600);
    defer Fen.deinit();

    while (Fen.shouldClose() == 0) {
        g.glClearColor(0.2, 0.3, 0.3, 1.0);
        g.glClear(g.GL_COLOR_BUFFER_BIT);

        g.glfwSwapBuffers(Fen.window);
        g.glfwPollEvents();
    }
}

test "création de fenetre et ajout des callbacks" {
    var Fen = try OpenGLWindow.init(1000, 600);
    defer Fen.deinit();

    Fen.registerCallbacks();

    while (Fen.shouldClose() == 0) {
        g.glClearColor(0.2, 0.3, 0.3, 1.0);
        g.glClear(g.GL_COLOR_BUFFER_BIT);

        g.glfwSwapBuffers(Fen.window);
        g.glfwPollEvents();
    }
}
