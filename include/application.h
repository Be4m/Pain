#pragma once

#include <stdint.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


struct app_settings {
    int32_t win_width;
    int32_t win_height;
    const char *win_title;

    GLFWwindow *const _window;
};

void create_application(struct app_settings *settings);
void application_run(struct app_settings *settings);
void close_application(int32_t code);

void mouse_move_callback(GLFWwindow *window, double x_pos, double y_pos);