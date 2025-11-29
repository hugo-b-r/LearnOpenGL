#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // Simple mapping of position to color for demonstration
    vertexColor = vec3((aPos.x + 1.0) * 0.5, (aPos.y + 1.0) * 0.5, 0.5);
}
