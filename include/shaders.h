#pragma once

#include "shader_store.h"

#include <stdint.h>

#include <GL/glew.h>

#define SHADER_SIZ_CHUNK 256

#define SHDR_SET_VEC3(program, unif, vec3) glUniformMatrix4fv(program##->uniforms[unif].location, 1, vec3)


int create_shader_programs(sprg_store_t programs, shdr_store_t shaders);