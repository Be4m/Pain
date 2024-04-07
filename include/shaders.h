#pragma once

#include "shader_store.h"

#include <stdint.h>

#define SHADER_SIZ_CHUNK 256


int create_shader_programs(sprg_store_t programs, shdr_store_t shaders);