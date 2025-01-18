#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 sun_direction;

uniform sampler2D albedo_texture;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

layout (location = 0) out vec4 out_color;

void main()
{
    float ambient_light = 0.2;
    vec3 albedo = texture(albedo_texture, texcoord).rgb;
    float lightness = ambient_light + max(0.0, dot(normal, sun_direction));

    out_color = vec4(lightness * albedo, 1.0);
}
