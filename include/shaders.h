#pragma once

#include "shader_store.h"

#include <stdint.h>

#define SHADER_SIZ 256


int compile_shaders(struct shader_asset *assets[], struct shader_program *programs[]);
int load_shaders(struct shader_asset *assets[]);