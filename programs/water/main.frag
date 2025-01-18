#version 330 core

const float PI = 3.141592653589793;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camera_position;

uniform vec3 bottom_point;
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
    float num = - plane.x * ray_point.x - plane.y * ray_point.y - plane.z * ray_point.z - plane.w;
    float den = plane.x * ray_coef.x + plane.y * ray_coef.y + plane.z * ray_coef.z;
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

void main()
{
    vec3 dir = reflect(position - camera_position, normal);

    float x = atan(dir.z, dir.x) / PI * 0.5 + 0.5;
    float y = -atan(dir.y, length(dir.xz)) / PI + 0.5;
    vec3 color = (texture(environment_texture, vec2(x, y)).rgb) / 2;

    vec4 bottom_plane = get_plane_equation(bottom_point, bottom_normal);
    vec3 ray_coef = get_ray_coef(camera_position, position);
    float t = intersect_line_with_plane(camera_position, ray_coef, bottom_plane);

    if (t < 0)
        out_color = vec4(mix(color, vec3(0.0, 0.1, 1.0), 0.2), 1.0);
    else {
        vec3 p = camera_position + t * ray_coef;
        vec2 bottom_texcoords = vec2(p.x, p.z) / bottom_size;
        vec3 bottom_color = texture(bottom_texture, bottom_texcoords).rgb;
        out_color = vec4(0.0, 0.1, 1.0, 1.0);
    }
}
