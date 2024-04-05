#include "graphics.h"

#include "shaders.h"

static void framebuffer_size_callback(GLFWwindow *window, int32_t new_width, int32_t new_height);


void graphics_cleanup(void)
{
    for (uint32_t i = 0; i < SPRG_Last; i++) {
        if (SHADER_PROGRAMS[i] == NULL) {
            continue;
        }

        glDeleteProgram(SHADER_PROGRAMS[i]->obj);
    }

    glfwTerminate();
}

int start_graphics(int32_t w, int32_t h, const char *title, GLFWwindow **win)
{
    GLFWwindow *window;

    if (glfwInit() != GLFW_TRUE) {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!(window = glfwCreateWindow(w, h, title, NULL, NULL))) {
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        return -1;
    }
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);

    *win = window;
    return 0;
}

static void framebuffer_size_callback(GLFWwindow *window, int32_t new_width, int32_t new_height)
{
    glViewport(0, 0, new_width, new_height);
}