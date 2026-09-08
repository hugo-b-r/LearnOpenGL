pub const gl = @cImport({
    @cInclude("/home/hugo/Projets/LearnOpenGL/deps/glad/include/glad/glad.h");
    @cInclude("GLFW/glfw3.h");
    // @cInclude("g");
});

pub const cglm = @cImport({
    @cInclude("cglm/cglm.h");
});

pub const stb = @cImport({
    @cInclude("stb/stb_image.h");
});
