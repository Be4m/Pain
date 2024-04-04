#include "application.h"

#include "common.h"
#include "graphics.h"
#include "shaders.h"

#include <stdlib.h>

#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>


void close_application(int32_t code)
{
    graphics_cleanup();

    exit(code);
}

void application_run(struct app_settings *settings)
{
    while (!glfwWindowShouldClose(settings->_window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    if (load_shaders(shader_assets) != 0 || compile_shaders(shader_assets, shader_programs) != 0) {
        ERR("Something went wrong while trying to create shaders.");
        close_application(1);
    }
}