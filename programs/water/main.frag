#version 330 core

const float PI = 3.141592653589793;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camera_position;

uniform sampler2D environment_texture;

in vec3 position;
in vec3 normal;
// in vec2 texcoord;

layout (location = 0) out vec4 out_color;

void main()
{
    vec3 dir = -reflect(camera_position - position, normal);

    float x = atan(dir.z, dir.x) / PI * 0.5 + 0.5;
    float y = -atan(dir.y, length(dir.xz)) / PI + 0.5;

    out_color = vec4((texture(environment_texture, vec2(x, y)).rgb) / 2, 1.0);
}
