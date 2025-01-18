#version 330 core

const float PI = 3.141592653589793;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camera_position;
uniform float time;

uniform vec3 bottom_angle;
uniform vec3 bottom_normal;
uniform vec2 bottom_size;
uniform sampler2D bottom_texture;

uniform sampler2D environment_texture;

in vec3 position;
in vec3 normal;

//ray: (t >= 0 is ok, t < 0 is not ok)
//  x = ray_point.x + t * ray_coef.x
//  y = ray_point.y + t * ray_coef.y
//  z = ray_point.z + t * ray_coef.z
//plane:
//  x * plane.x + y * plane.y + z * plane.z + plane.w = 0;
//returns t from line equation
float intersect_line_with_plane(vec3 ray_point, vec3 ray_coef, vec4 plane) {
    float num = - dot(vec3(plane), ray_point) - plane.w;
    float den = dot(vec3(plane), ray_coef);
    if (den != 0.0)
        return num / den;
    return -1;
}

vec4 get_plane_equation(vec3 point, vec3 normal) {
    return vec4(normal, -dot(normal, point));
}

vec3 get_ray_coef(vec3 point_start, vec3 point_end){
    return normalize(point_end - point_start);
}

layout (location = 0) out vec4 out_color;

float min(float x, float y) {
    if (x < y) return x;
    return y;
}

float max(float x, float y) {
    return x + y - min(x, y);
}

float dfdx() {
    return cos(position.x + time);
}

float dfdy() {
    return -sin(position.y + time);
}

void main()
{
    vec3 normal2 = normalize(vec3(-dfdx(), 1.0, -dfdy()));
    // normal2 = normal;
    vec3 dir = reflect(position - camera_position, normal2);

    float x = atan(dir.z, dir.x) / PI * 0.5 + 0.5;
    float y = -atan(dir.y, length(dir.xz)) / PI + 0.5;
    vec3 color = (texture(environment_texture, vec2(x, y)).rgb) / 2;

    vec3 ray_direction = normalize(position - camera_position);
    float r = 1 / 1.333;
    float c = -dot(normal2, ray_direction);
    vec3 refracted_direction = r * ray_direction + (r * c - sqrt(1 - r * r * (1 - c * c))) * normal2;

    vec3 ray_coef = normalize(refracted_direction);
    vec4 bottom_plane = get_plane_equation(bottom_angle, bottom_normal);
    float t = intersect_line_with_plane(camera_position, ray_coef, bottom_plane);
    vec3 p = camera_position + t * ray_coef;
    vec2 bottom_texcoords = vec2(p.x, p.z) / bottom_size;
    // color = vec3(0.0, 0.1, 1.0);

    vec3 refracted_color;
    if (t >= 0 && max(bottom_texcoords.x, bottom_texcoords.y) <= 1.0 && min(bottom_texcoords.x, bottom_texcoords.y) >= 0) {
        refracted_color = texture(bottom_texture, bottom_texcoords).rgb;
    } else {
        x = atan(refracted_direction.z, refracted_direction.x) / PI * 0.5 + 0.5;
        y = -atan(refracted_direction.y, length(refracted_direction.xz)) / PI + 0.5;
        refracted_color = (texture(environment_texture, vec2(x, y)).rgb) / 2;;
    }
    color = mix(color, refracted_color, 0.8);
    color = mix(color, vec3(0.0, 0.1, 1.0), 0.2);
    out_color = vec4(color, 1.0);
}
