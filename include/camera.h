#pragma once

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>


struct camera_settings {
    vec3 origin;
    vec3 default_target;
    float movement_speed;
    float mouse_sensitivity;

    float FOV;
    float z_near, z_far;
    float aspect_ratio;
};

struct _camera {
    vec3 position;
    vec3 up_vector;
    vec3 right_vector;
    vec3 direction_vector;
    float pitch, yaw;
    float post_delta_movement_speed;
    float post_delta_mouse_sensitivity;
    struct camera_settings *settings;

    _Bool is_initialized;
};

void camera_update(struct _camera *camera, float delta_time);
void camera_process_input(GLFWwindow *window, struct _camera *camera);
void camera_process_mouse(GLFWwindow *window, float x_pos, float y_pos, struct _camera *camera);

struct _camera create_simple_camera(struct camera_settings *settings);

void camera_produce_view_matrix(struct _camera *camera, mat4 view_mat);
void camera_produce_projection_matrix(struct _camera *camera, mat4 proj_mat);