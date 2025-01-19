#version 330 core

uniform vec3 x_side_vector;
uniform vec3 y_side_vector;

in vec3 refracted_direction;

layout (location = 0) out vec4 out_color;

void main() {
    vec3 normal = normalize(cross(x_side_vector, y_side_vector));
    float lightness = max(0.0, dot(-refracted_direction, normal));
    out_color = vec4(lightness * 5.0, 0.0, 0.0, 1.0);
}