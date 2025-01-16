#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D environment_texture;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

layout (location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(0.0, 0.0, (normal.x + 1.0) / 2.0, 0.2);
}
