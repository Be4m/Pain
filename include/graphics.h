#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>


int start_graphics(int32_t w, int32_t h, const char *title, GLFWwindow **win);
void graphics_cleanup(void);