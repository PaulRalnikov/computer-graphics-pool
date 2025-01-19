#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 sun_direction;

uniform sampler2D albedo_texture;
uniform sampler2D caustics_texture;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

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
    float ambient_light = 0.2;
    vec2 albedo_texcoords = vec2(texcoord.x, 1 - texcoord.y);
    vec3 albedo = texture(albedo_texture, albedo_texcoords).rgb;
    float caustics = texture(caustics_texture, texcoord).r;
    float lightness = ambient_light + max(0.0, dot(normal, sun_direction));

    vec3 color = (caustics + ambient_light) * albedo;
    color = Uncharted2Tonemap(color);
    //gamma correction
    color = pow(color, vec3(1.0 / 2.2));

    out_color = vec4(color, 1.0);
}
