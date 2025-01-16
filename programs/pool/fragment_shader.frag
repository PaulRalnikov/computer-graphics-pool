#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D albedo_texture;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

layout (location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(texture(albedo_texture, texcoord).rgb, 1.0);
}
