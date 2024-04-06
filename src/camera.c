#include "camera.h"


void camera_produce_view_matrix(struct _camera *camera, mat4 view_mat)
{
    glm_look(camera->position, camera->direction_vector, camera->up_vector, view_mat);
}

void camera_produce_projection_matrix(struct _camera *camera, mat4 proj_mat)
{
    glm_perspective(glm_rad(camera->settings->FOV), camera->settings->aspect_ratio,
        camera->settings->z_near, camera->settings->z_far, proj_mat);
}

void camera_update(struct _camera *camera, float delta_time)
{
    glm_vec3_crossn(camera->direction_vector, camera->up_vector, camera->right_vector);
    camera->post_delta_movement_speed = camera->settings->movement_speed * delta_time;
    camera->post_delta_mouse_sensitivity = camera->settings->mouse_sensitivity * delta_time;
}

void camera_process_input(GLFWwindow *window, struct _camera *camera)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_muladds(camera->direction_vector, camera->post_delta_movement_speed, camera->position);
    } else if (glfwGetKey(window, GLFW_KEY_S)) {
        vec3 backwards_vec;
        glm_vec3_negate_to(camera->direction_vector, backwards_vec);
        glm_vec3_muladds(backwards_vec, camera->post_delta_movement_speed, camera->position);
    } else if (glfwGetKey(window, GLFW_KEY_A)) {
        vec3 left_vec;
        glm_vec3_negate_to(camera->right_vector, left_vec);
        glm_vec3_muladds(left_vec, camera->post_delta_movement_speed, camera->position);
    } else if (glfwGetKey(window, GLFW_KEY_D)) {
        glm_vec3_muladds(camera->right_vector, camera->post_delta_movement_speed, camera->position);
    }

    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        vec3 up_vector;
        glm_vec3_scale(camera->up_vector, camera->post_delta_movement_speed, up_vector);
        glm_vec3_add(camera->position, up_vector, camera->position);
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        vec3 down_vector;
        glm_vec3_negate_to(camera->up_vector, down_vector);
        glm_vec3_scale(down_vector, camera->post_delta_movement_speed, down_vector);
        glm_vec3_add(camera->position, down_vector, camera->position);
    }
}

void camera_process_mouse(GLFWwindow *window, float x_pos, float y_pos, struct _camera *camera)
{
    static float last_x_pos = 0.0f;
    static float last_y_pos = 0.0f;
    if (!camera->is_initialized) {
        last_x_pos = x_pos;
        last_y_pos = y_pos;
        camera->is_initialized = true;
    }

    float x_offset = x_pos - last_x_pos;
    float y_offset = y_pos - last_y_pos;
    last_x_pos = x_pos;
    last_y_pos = y_pos;

    camera->yaw += x_offset * camera->post_delta_mouse_sensitivity;

    camera->pitch -= y_offset * camera->post_delta_mouse_sensitivity;
    if (camera->pitch > 89.0f) {
        camera->pitch = 89.0f;
    } else if (camera->pitch < -89.0f) {
        camera->pitch = -89.0f;
    }

    glm_vec3_copy((vec3){
        cos(glm_rad(camera->yaw)),
        sin(glm_rad(camera->pitch)),
        sin(glm_rad(camera->yaw))
    }, camera->direction_vector);
}

struct _camera create_simple_camera(struct camera_settings *settings)
{
    struct _camera _ = {
        .position = {0.0f},
        .settings = settings,

        .up_vector = {0.0f, 1.0f, 0.0f},
        .right_vector = {1.0f, 0.0f, 0.0f},

        .is_initialized = false,
    };

    glm_vec3_sub(settings->default_target, settings->origin, _.direction_vector);
    glm_vec3_normalize(_.direction_vector);

    return _;
}