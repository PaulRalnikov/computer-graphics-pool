#version 330 core

uniform mat4 view_projection_inverse;

const vec2 VERTICES[4] = vec2[4](
    vec2(1.0, -1.0),
    vec2(1.0, 1.0),
    vec2(-1.0, 1.0),
    vec2(-1.0, -1.0)
);

out vec3 position;
void main()
{
    vec2 vertex = VERTICES[gl_VertexID];
    vec4 ndc = vec4(vertex, 0.0, 1.0);
    vec4 clip_space = view_projection_inverse * ndc;
    position = clip_space.xyz / clip_space.w;

    gl_Position = ndc;
}