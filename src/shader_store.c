#include "shader_store.h"

#include <stdint.h>
#include <stdlib.h>

#define DECLARE_SHADER(x, y) struct shader_asset __##x = {.idt = x, .path = "shaders/"y}
#define DECLARE_SHADER_VERT_FRAG_PAIR(x, y) DECLARE_SHADER(x##Vert, y".vert"); \
    DECLARE_SHADER(x##Frag, y".frag")
#define DECLARE_BASIC_SHADER_PROGRAM(x) struct shader_program __##x = {.idt = x, .shaders = {.vertex = x##Vert, .fragment = x##Frag}}
#define DECLARE_SHADER_PROGRAM(x, ...) struct shader_program __##x = {.idt = x, .shaders = {__VA_ARGS__}}


DECLARE_SHADER_VERT_FRAG_PAIR(Standard, "standard");
DECLARE_SHADER_VERT_FRAG_PAIR(SimpleTexture, "simple_texture");

struct shader_asset *shader_assets[LastShaderUID] = {
    [StandardVert] = &__StandardVert,
    [StandardFrag] = &__StandardFrag,
    [SimpleTextureVert] = &__SimpleTextureVert,
    [SimpleTextureFrag] = &__SimpleTextureFrag
};

DECLARE_BASIC_SHADER_PROGRAM(Standard);
DECLARE_BASIC_SHADER_PROGRAM(SimpleTexture);

struct shader_program *shader_programs[LastShaderProgramUID] = {
    [Standard] = &__Standard,
    [SimpleTexture] = &__SimpleTexture
};