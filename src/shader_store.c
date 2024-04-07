#include "shader_store.h"

#include <stdint.h>
#include <stdlib.h>

// #define SHDR_BIN_SYM_PREFIX binary_shaders
#define STANDARD_UNIFORMS [UNIF_ModelMat] = {.idt = UNIF_ModelMat, .name = "model"}, \
    [UNIF_ViewMat] = {.idt = UNIF_ViewMat, .name = "view" }, \
    [UNIF_ProjMat] = {.idt = UNIF_ProjMat, .name = "projection" }

#define SHDR_BIN_SYM(uid, suffix) binary_shaders_##uid##_##suffix
#define DECLARE_SHADER_ASSET(UID, uid) extern char SHDR_BIN_SYM(uid, start)[]; extern char SHDR_BIN_SYM(uid, end)[]; \
    struct shader_asset s_##UID = {.shader_uid = UID, .binary = {.start_ptr = (const char *const)SHDR_BIN_SYM(uid, start), .end_ptr = (const char *const)SHDR_BIN_SYM(uid, end) \
    }, .source = NULL}
#define DECLARE_SHADER_PROGRAM(UID, vert, frag, ...) struct shader_program s_##UID = {.idt = UID, .shaders = {.vertex = vert, .fragment = frag}, \
    .uniforms = __VA_ARGS__, .obj = -1}


DECLARE_SHADER_ASSET(SHDR_StandardVert, standard_vert);
DECLARE_SHADER_ASSET(SHDR_StandardFrag, standard_frag);
DECLARE_SHADER_ASSET(SHDR_StandardTextureVert, standard_texture_vert);
DECLARE_SHADER_ASSET(SHDR_StandardTextureFrag, standard_texture_frag);

struct shader_asset *SHADER_ASSETS[SHDR_Last] = {
    [SHDR_StandardVert] = &s_SHDR_StandardVert,
    [SHDR_StandardFrag] = &s_SHDR_StandardFrag,
    [SHDR_StandardTextureVert] = &s_SHDR_StandardTextureVert,
    [SHDR_StandardTextureFrag] = &s_SHDR_StandardTextureFrag
};

DECLARE_SHADER_PROGRAM(SPRG_Standard,
    &s_SHDR_StandardVert, &s_SHDR_StandardFrag,
    {
        STANDARD_UNIFORMS,
        [UNIF_FragColor] = {.idt = UNIF_FragColor, .name = "frag_color"}
    }
);

DECLARE_SHADER_PROGRAM(SPRG_StandardTexture,
    &s_SHDR_StandardTextureVert, &s_SHDR_StandardTextureFrag,
    {
        STANDARD_UNIFORMS
    }
);

struct shader_program *SHADER_PROGRAMS[SPRG_Last] = {
    [SPRG_Standard] = &s_SPRG_Standard,
    [SPRG_StandardTexture] = &s_SPRG_StandardTexture
};