#version 330 core

in vec2 tex_coords;

out vec4 frag_colour;

uniform sampler2D in_texture;

void main()
{
    frag_colour = texture(in_texture, tex_coords);
}