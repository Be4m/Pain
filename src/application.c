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
    glEnableVertexAttribArray(0);

    struct shader_program *obj_shader_prog = SHADER_PROGRAMS[SPRG_SimpleLighting];
    struct shader_program *light_shader_prog = SHADER_PROGRAMS[SPRG_Standard];

    mat4 proj; camera_produce_projection_matrix(&CAMERA, proj);
    vec3 light_color = {0.0f, 1.0, 0.0f};

    glUseProgram(obj_shader_prog->obj);

    mat4 obj_model = GLM_MAT4_IDENTITY_INIT;
    glUniformMatrix4fv(obj_shader_prog->uniform_loc[UNIF_ModelMat], 1, GL_FALSE, (float *)obj_model);
    glUniformMatrix4fv(obj_shader_prog->uniform_loc[UNIF_ProjMat], 1, GL_FALSE, (float *)proj);

    vec3 obj_color = {.2f, 0.2f, 0.8f};
    glUniform3fv(obj_shader_prog->uniform_loc[UNIF_SimpleLighting_ObjColor], 1, obj_color);
    glUniform3fv(obj_shader_prog->uniform_loc[UNIF_SimpleLighting_LightColor], 1, light_color);

    glUseProgram(light_shader_prog->obj);

    mat4 light_model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(light_model, (vec3){0.3f, 1.8f, 3.0f});
    glUniformMatrix4fv(light_shader_prog->uniform_loc[UNIF_ModelMat], 1, GL_FALSE, (float *)light_model);
    glUniformMatrix4fv(light_shader_prog->uniform_loc[UNIF_ProjMat], 1, GL_FALSE, (float *)proj);

    glUniform3fv(light_shader_prog->uniform_loc[UNIF_FragColor], 1, light_color);

    float last_frame = 0.0f;

    while (!glfwWindowShouldClose(settings->_window)) {
        float current_frame = glfwGetTime();
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        camera_update(&CAMERA, delta_time);

        process_input(settings->_window);
        camera_process_input(settings->_window, &CAMERA);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // CAMERA
        mat4 view;
        camera_produce_view_matrix(&CAMERA, view);

        glUseProgram(obj_shader_prog->obj);
        glUniformMatrix4fv(obj_shader_prog->uniform_loc[UNIF_ViewMat], 1, GL_FALSE, (float *)view);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUseProgram(light_shader_prog->obj);
        glUniformMatrix4fv(light_shader_prog->uniform_loc[UNIF_ViewMat], 1, GL_FALSE, (float *)view);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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