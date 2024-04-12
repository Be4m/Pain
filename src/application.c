#include "application.h"

#include "common.h"
#include "graphics.h"
#include "shaders.h"
#include "shapes.h"
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

    struct mesh icosphere = generate_icosphere(3);

    uint32_t vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * icosphere.n_vertices, icosphere.vertex_buffer, GL_STATIC_DRAW);

    uint32_t vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    uint32_t index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 3 * icosphere.n_triangles, icosphere.index_buffer, GL_STATIC_DRAW);

    free_mesh(&icosphere);

    struct shader_program *shader = SHADER_PROGRAMS[SPRG_Standard];
    glUseProgram(shader->obj);

    mat4 view, proj, model = GLM_MAT4_IDENTITY_INIT;
    camera_produce_projection_matrix(&CAMERA, proj);

    glUniformMatrix4fv(shader->uniform_loc[UNIF_ModelMat], 1, GL_FALSE, (float *)model);
    glUniformMatrix4fv(shader->uniform_loc[UNIF_ProjMat], 1, GL_FALSE, (float *)proj);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    vec3 colour = {1.0f, 0.f, 0.f};
    glUniform3fv(shader->uniform_loc[UNIF_FragColor], 1, colour);

    float last_frame = 0.0f;

    while (!glfwWindowShouldClose(settings->_window)) {
        float current_frame = glfwGetTime();
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        camera_update(&CAMERA, delta_time);

        process_input(settings->_window);
        camera_process_input(settings->_window, &CAMERA);

        camera_produce_view_matrix(&CAMERA, view);
        glUniformMatrix4fv(shader->uniform_loc[UNIF_ViewMat], 1, GL_FALSE, (float *)view);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glDrawElements(GL_TRIANGLES, icosphere.n_triangles * 3, GL_UNSIGNED_INT, 0);

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