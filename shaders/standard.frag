#version 330 core

out vec4 out_color;

uniform vec3 frag_color;

void main()
{
    out_color = vec4(frag_color, 1.0);
}