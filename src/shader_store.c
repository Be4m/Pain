#include "shader_store.h"

#include <stdint.h>
#include <stdlib.h>

#define SHDR_BIN_SYM_PREFIX binary_shaders

#define SHDR_BIN_SYM(uid, suffix) binary_shaders_##uid##_##suffix
#define DECLARE_SHADER_ASSET(UID, uid) extern char SHDR_BIN_SYM(uid, start)[]; extern char SHDR_BIN_SYM(uid, end)[]; \
    struct shader_asset s_##UID = {.idt = UID, .binary = {.start_ptr = (const char *const)SHDR_BIN_SYM(uid, start), .end_ptr = (const char *const)SHDR_BIN_SYM(uid, end) \
    }, .source = NULL}
#define DECLARE_SHADER_PROGRAM(UID, ...) struct shader_program s_##UID = {.idt = UID, .shaders = {__VA_ARGS__}, .obj = -1}


DECLARE_SHADER_ASSET(SHDR_StandardVert, standard_vert);
DECLARE_SHADER_ASSET(SHDR_StandardFrag, standard_frag);
DECLARE_SHADER_ASSET(SHDR_SimpleTextureVert, simple_texture_vert);
DECLARE_SHADER_ASSET(SHDR_SimpleTextureFrag, simple_texture_frag);

struct shader_asset *SHADER_ASSETS[SHDR_Last] = {
    [SHDR_StandardVert] = &s_SHDR_StandardVert,
    [SHDR_StandardFrag] = &s_SHDR_StandardFrag,
    [SHDR_SimpleTextureVert] = &s_SHDR_SimpleTextureVert,
    [SHDR_SimpleTextureFrag] = &s_SHDR_SimpleTextureFrag
};

DECLARE_SHADER_PROGRAM(SPRG_Standard,
    .vertex = &s_SHDR_StandardVert,
    .fragment = &s_SHDR_StandardFrag
);

DECLARE_SHADER_PROGRAM(SPRG_SimpleTexture,
    .vertex = &s_SHDR_SimpleTextureVert,
    .fragment = &s_SHDR_SimpleTextureFrag
);

struct shader_program *SHADER_PROGRAMS[SPRG_Last] = {
    [SPRG_Standard] = &s_SPRG_Standard,
    [SPRG_SimpleTexture] = &s_SPRG_SimpleTexture
};