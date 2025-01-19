#version 330 core

uniform vec3 camera_position;
uniform sampler2D environment_texture;

in vec3 position;

const float PI = 3.141592653589793;
layout (location = 0) out vec4 out_color;

vec3 TonemapRaw(vec3 x){
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}
vec3 Uncharted2Tonemap(vec3 color){
    float W = 11.2;
    return TonemapRaw(color) / TonemapRaw(vec3(W));
}

void main()
{
    vec3 dir = position - camera_position;

    float x = atan(dir.z, dir.x) / PI * 0.5 + 0.5;
    float y = -atan(dir.y, length(dir.xz)) / PI + 0.5;

    vec3 color = texture(environment_texture, vec2(x, y)).rgb;

    color = Uncharted2Tonemap(color);
    //gamma correction
    color = pow(color, vec3(1.0 / 2.2));
    out_color = vec4(color, 1.0);    
}