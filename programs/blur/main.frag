#version 330 core

uniform sampler2D in_texture;
uniform vec2 dir;

in vec2 texcoord;

layout (location = 0) out vec4 out_color;

void main()
{
    int N = 5;
    float radius = 5.0;
    float w = 0.0;
    vec4 clr = vec4(0.0);

    for (int x = -N; x <= N; ++x) {
        float c = exp(-float(x*x) / (radius*radius));
        clr += c * texture(in_texture, texcoord + dir * x);
        w += c;
    }
    
    out_color = clr / w;
}