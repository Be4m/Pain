#include "shader_store.h"

#include <stdint.h>
#include <stdlib.h>

#define DECLARE_SHADER(x, y) struct shader_asset __##x = {.idt = x, .path = "shaders/"y}
#define DECLARE_SHADER_PROGRAM(x, ...) struct shader_program __##x = {.idt = x, .shaders = {__VA_ARGS__}}


DECLARE_SHADER(StandardVertex, "standard.vert");
DECLARE_SHADER(StandardFragment, "standard.frag");
struct shader_asset *shader_assets[LastShaderUID] = {
    [StandardVertex] = &__StandardVertex,
    [StandardFragment] = &__StandardFragment
};

DECLARE_SHADER_PROGRAM(Standard, .vertex = StandardVertex, .fragment = StandardFragment);
struct shader_program *shader_programs[LastShaderProgramUID] = {
    [Standard] = &__Standard
};