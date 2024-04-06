#include "application.h"

#include "common.h"
#include "graphics.h"
#include "shaders.h"
#include "shapes.c"
#include "camera.h"

#include <stdlib.h>

#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

static struct _camera CAMERA;

static void process_input(GLFWwindow *window);


void close_application(int32_t code)
{
    graphics_cleanup();

    exit(code);
}

void application_run(struct app_settings *settings)
{
    struct camera_settings cam_settings = {
        .origin = {0.0f},
        .default_target = {0.0f, 0.0f, -1.0f},

        .movement_speed = 5.0f,
        .mouse_sensitivity = 8.0f,

        .FOV = 45.0f,
        .z_near = 0.1f,
        .z_far = 100.0f,
        .aspect_ratio = (float)settings->win_width/(float)settings->win_height
    };
    CAMERA = create_simple_camera(&cam_settings);
 
    uint32_t vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), CUBE_VERTICES, GL_STATIC_DRAW);

    uint32_t vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    glBindVertexArray(vertex_array_obj);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    int32_t width, height, n_channels;
    unsigned char *img_data = stbi_load("textures/universa.jpg", &width, &height, &n_channels, 0);
    
    uint32_t texture_obj;
    glGenTextures(1, &texture_obj);
    glBindTexture(GL_TEXTURE_2D, texture_obj);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    struct shader_program *shader_prog = SHADER_PROGRAMS[SPRG_StandardTexture];
    shader_program_load_uniform_locations(shader_prog);

    mat4 model = GLM_MAT4_IDENTITY_INIT, proj;
    camera_produce_projection_matrix(&CAMERA, proj);

    glUniformMatrix4fv(shader_prog->uniforms[UNIF_ModelMat].location, 1, GL_FALSE, (float *)model);
    glUniformMatrix4fv(shader_prog->uniforms[UNIF_ProjMat].location, 1, GL_FALSE, (float *)proj);

    float last_frame = 0.0f;

    while (!glfwWindowShouldClose(settings->_window)) {
        float current_frame = glfwGetTime();
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        camera_update(&CAMERA, delta_time);

        process_input(settings->_window);
        camera_process_input(settings->_window, &CAMERA);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // CAMERA
        mat4 view;
        camera_produce_view_matrix(&CAMERA, view);
        glUniformMatrix4fv(shader_prog->uniforms[UNIF_ViewMat].location, 1, GL_FALSE, (float *)view);

        // MAIN LOOP
        glfwPollEvents();
        glfwSwapBuffers(settings->_window);
    }

    close_application(0);
}

void create_application(struct app_settings *settings)
{
    if (start_graphics(settings->win_width, settings->win_height, settings->win_title, (GLFWwindow **)&settings->_window) != 0) {
        ERR("Something went wrong while trying to initialise graphics.");
        close_application(1);
    }

    if (create_shader_programs(SHADER_PROGRAMS, SHADER_ASSETS) != 0) {
        ERR("Something went wrong while trying to create shader programs.");
        close_application(1);
    }
}

static void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouse_move_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    camera_process_mouse(window, (float)x_pos, (float)y_pos, &CAMERA);
}