#version 330 core

layout (location = 0) in vec3 in_position;

uniform mat4 model;

uniform vec3 corner;
uniform vec3 x_side_vector;
uniform vec3 y_side_vector;

uniform vec3 sun_direction;
uniform float time;

out vec3 refracted_direction;

//ray: (t >= 0 is ok, t < 0 is not ok)
//  x = position.x + t * ray_coef.x
//  y = position.y + t * ray_coef.y
//  z = position.z + t * ray_coef.z
//plane:
//  x * plane.x + y * plane.y + z * plane.z + plane.w = 0;
//returns t from line equation
float intersect_line_with_plane(vec3 position, vec3 ray_coef, vec4 plane) {
    float num = - dot(vec3(plane), position) - plane.w;
    float den = dot(vec3(plane), ray_coef);
    if (den != 0.0)
        return num / den;
    return -1;
}

vec4 get_plane_equation(vec3 point, vec3 normal) {
    return vec4(normal, -dot(normal, point));
}

float min(float x, float y) {
    if (x < y) return x;
    return y;
}

float max(float x, float y) {
    return x + y - min(x, y);
}

float dfdx(float x) {
    return cos(x / 2.0 + time) / 4.0;
}

float dfdy(float y) {
    return -sin(y / 2.0 + time) / 4.0;
}

float get_projection_length(vec3 direction, vec3 v) {
    return length(v) * dot(normalize(direction), normalize(v)) / length(direction);
}

void main()
{
    vec3 position = (model * vec4(in_position, 1.0)).xyz;
    vec3 normal = normalize(vec3(-dfdx(position.x), 1.0, -dfdy(position.y)));

    vec3 sun_ray_direction = -sun_direction;
    float r = 1 / 1.333;
    float c = -dot(normal, sun_ray_direction);
    refracted_direction = normalize(r * sun_ray_direction + (r * c - sqrt(1 - r * r * (1 - c * c))) * normal);

    vec3 plane_normal = normalize(cross(x_side_vector, y_side_vector));
    vec4 plane = get_plane_equation(corner, plane_normal);
    float t = intersect_line_with_plane(position, refracted_direction, plane);
    vec3 p = position + t * refracted_direction;
    vec3 corner_p_ray = p - corner;
    vec2 texcoords = vec2(
        get_projection_length(x_side_vector, corner_p_ray),
        get_projection_length(y_side_vector, corner_p_ray)
    );

    gl_Position = vec4(texcoords, 1.0, 1.0);
}
