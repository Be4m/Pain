#include "graphics.h"

#include "shaders.h"


void graphics_cleanup(void)
{
    for (uint32_t i = 0; i < LastShaderProgramUID; i++) {
        if (shader_programs[i] == NULL) {
            continue;
        }

        glDeleteProgram(shader_programs[i]->shader_program);
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

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    *win = window;
    return 0;
}