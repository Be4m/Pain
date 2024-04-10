#version 330 core

layout (location = 0) in vec3 vert_coord;
layout (location = 1) in vec3 in_normal_vec;
layout (location = 2) in vec2 in_tex_coords;

out vec3 normal_vec;
out vec3 frag_pos;
out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vert_coord, 1.0);
    frag_pos = vec3(model * vec4(vert_coord, 1.0));
    normal_vec = mat3(transpose(inverse(model))) * in_normal_vec;
    tex_coords = in_tex_coords;
}