#version 330 core

in vec3 normal_vec;
in vec3 frag_pos;
in vec2 tex_coords;

out vec4 frag_color;

uniform vec3 object_color;

uniform vec3 light_color;
uniform vec3 light_pos;

uniform vec3 eye_pos;

uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;

void main()
{
    float ambient_intensity = 0.1f;
    vec3 ambient = light_color * ambient_intensity * vec3(texture(diffuse_tex, tex_coords));

    vec3 norm = normalize(normal_vec);
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color * vec3(texture(diffuse_tex, tex_coords));

    float specular_strength = 0.5f;
    vec3 view_dir = normalize(eye_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 256);
    vec3 specular = specular_strength * spec * light_color * vec3(texture(specular_tex, tex_coords));

    vec3 compute = (ambient + diffuse + specular);
    frag_color = vec4(compute, 1.0);
}