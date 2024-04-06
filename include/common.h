#pragma once

#include <stdio.h>

#define ERR(x, ...) fprintf(stderr, "App-ERROR: "x"\n", ##__VA_ARGS__)
#define GL_ERR(x, ...) fprintf(stderr, "OpenGL-ERROR: "x"\n", ##__VA_ARGS__)

#define WARN(x, ...) fprintf(stdout, "App-WARNING: "x"\n", ##__VA_ARGS__)

#define GCC_IGN_RES(x) _Pragma("GCC diagnostic ignored \"-Wunused-result\"") \
    x; \
    _Pragma("GCC diagnostic pop")
    