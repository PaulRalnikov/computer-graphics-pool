#version 330 core

uniform vec3 camera_position;
uniform sampler2D environment_texture;

in vec3 position;

const float PI = 3.141592653589793;
layout (location = 0) out vec4 out_color;

void main()
{
    vec3 dir = position - camera_position;

    float x = atan(dir.z, dir.x) / PI * 0.5 + 0.5;
    float y = -atan(dir.y, length(dir.xz)) / PI + 0.5;

    out_color = vec4(texture(environment_texture, vec2(x, y)).rgb, 1.0);    
}