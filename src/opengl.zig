const g = @cImport({
    @cInclude("GLFW/glfw3.h");
    @cInclude("glad/glad.h");
    // @cInclude("g");
});

const m = @cImport({
    @cInclude("cglm/cglm.h");
});

const std = @import("std");

const ErrorTypes = error{pasMarche};

const OpenGLWindow = struct {
    window: ?*g.GLFWwindow = null,

    pub fn init(windowWidth: i32, windowHeight: i32) ErrorTypes!OpenGLWindow {
        var window: ?*g.GLFWwindow = null;
        _ = g.glfwInit();
        window = g.glfwCreateWindow(@as(c_int, windowWidth), @as(c_int, windowHeight), "LearnOpenGL", null, null);
        if (window == null) {
            std.log.info("Failed to create GLFW Window", .{});
            g.glfwTerminate();
            return ErrorTypes.pasMarche;
        }
        g.glfwMakeContextCurrent(window);
        return OpenGLWindow{ .window = window };
    }
};

test "création de fenetre" {
    _ = try OpenGLWindow.init(500, 300);
}
