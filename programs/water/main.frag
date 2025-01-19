#version 330 core

const float PI = 3.141592653589793;
const float refraction_mix_coef = 0.7;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camera_position;
uniform vec3 sun_direction;
uniform float time;

const int NUM_SIDES = 5;

uniform vec3 corner[NUM_SIDES];
uniform vec3 x_side_vector[NUM_SIDES];
uniform vec3 y_side_vector[NUM_SIDES];

uniform sampler2D bottom_texture;
uniform sampler2D wall_texture;

uniform sampler2D bottom_caustics_texture;
uniform sampler2D front_caustics_texture;
uniform sampler2D right_caustics_texture;
uniform sampler2D back_caustics_texture;
uniform sampler2D left_caustics_texture;

uniform sampler2D environment_texture;

in vec3 position;

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

float min(float x, float y) {
    if (x < y) return x;
    return y;
}

float max(float x, float y) {
    return x + y - min(x, y);
}

float dfdx() {
    return cos(position.x / 2.0 + time) / 4.0;
}

float dfdy() {
    return -sin(position.y / 2.0 + time) / 4.0;
}

float get_projection_length(vec3 direction, vec3 v) {
    return length(v) * dot(normalize(direction), normalize(v)) / length(direction);
}

vec3 add_color_from_rectangle(
    vec3 in_color,
    int i,
    sampler2D rectangle_texture,
    sampler2D caustics_texture,
    vec3 ray_point,
    vec3 ray_coef,
    float mix_coef)
{
    
    vec3 currect_corner = corner[i];
    vec3 currecnt_x_side_vector = x_side_vector[i];
    vec3 currect_y_side_vector = y_side_vector[i];
    vec3 rectangle_normal = normalize(cross(currecnt_x_side_vector, currect_y_side_vector));
    vec4 rectangle_plane = get_plane_equation(currect_corner, rectangle_normal);
    float t = intersect_line_with_plane(ray_point, ray_coef, rectangle_plane);
    vec3 p = ray_point + t * ray_coef;
    vec3 angle_p_ray = p - currect_corner;
    vec2 texcoords = vec2(
        get_projection_length(currecnt_x_side_vector, angle_p_ray),
        1 - get_projection_length(currect_y_side_vector, angle_p_ray)
    );

    if (t >= 0 && max(texcoords.x, texcoords.y) <= 1.0 && min(texcoords.x, texcoords.y) >= 0) {
        float ambient_light = 0.2;
        vec3 albedo = texture(rectangle_texture, texcoords).rgb;
        float lightness = ambient_light + texture(caustics_texture, texcoords).r;

        vec3 refracted_color = lightness * albedo;
        return mix(in_color, refracted_color, mix_coef);
    }
    return in_color;
}

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

layout (location = 0) out vec4 out_color;

void main()
{   
    vec3 normal = normalize(vec3(-dfdx(), 1.0, -dfdy()));
    vec3 dir = reflect(position - camera_position, normal);

    float x = atan(dir.z, dir.x) / PI * 0.5 + 0.5;
    float y = -atan(dir.y, length(dir.xz)) / PI + 0.5;
    vec3 color = (texture(environment_texture, vec2(x, y)).rgb) / 2;

    vec3 ray_direction = normalize(position - camera_position);
    float r = 1 / 1.333;
    float c = -dot(normal, ray_direction);
    vec3 refracted_direction = r * ray_direction + (r * c - sqrt(1 - r * r * (1 - c * c))) * normal;

    vec3 ray_coef = normalize(refracted_direction);
    
    color = add_color_from_rectangle(color, 0, bottom_texture, bottom_caustics_texture, position, ray_coef, refraction_mix_coef);
    color = add_color_from_rectangle(color, 1, wall_texture, bottom_caustics_texture, position, ray_coef, refraction_mix_coef);
    color = add_color_from_rectangle(color, 2, wall_texture, bottom_caustics_texture, position, ray_coef, refraction_mix_coef);
    color = add_color_from_rectangle(color, 3, wall_texture, bottom_caustics_texture, position, ray_coef, refraction_mix_coef);
    color = add_color_from_rectangle(color, 4, wall_texture, bottom_caustics_texture, position, ray_coef, refraction_mix_coef);

    color = mix(color, vec3(0.0, 0.1, 1.0), 0.2);

    color = Uncharted2Tonemap(color);
    //gamma correction
    color = pow(color, vec3(1.0 / 2.2));

    out_color = vec4(color, 1.0);
}
