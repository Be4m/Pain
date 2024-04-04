#version 330 core

out vec4 frag_colour;

in vec2 tex_coords;

uniform sampler2D in_texture;

void main()
{
    frag_colour = texture(in_texture, tex_coords);
}