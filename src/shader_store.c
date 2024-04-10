#include "shader_store.h"

#include <stdint.h>
#include <stdlib.h>

// #define SHDR_BIN_SYM_PREFIX binary_shaders
#define STANDARD_UNIFORMS [UNIF_ModelMat] = "model", \
    [UNIF_ViewMat] = "view", \
    [UNIF_ProjMat] =  "projection"

#define SHDR_BIN_SYM(uid, suffix) binary_shaders_##uid##_##suffix
#define DECLARE_SHADER_ASSET(UID, uid, ...) extern char SHDR_BIN_SYM(uid, start)[]; extern char SHDR_BIN_SYM(uid, end)[]; \
    struct shader_asset s_##UID = {.shader_uid = UID, .binary = {.start_ptr = (const char *const)SHDR_BIN_SYM(uid, start), .end_ptr = (const char *const)SHDR_BIN_SYM(uid, end) \
    }, .uniforms = __VA_ARGS__, .source = NULL}
#define DECLARE_SHADER_PROGRAM(UID, vert, frag) struct shader_program s_##UID = {.idt = UID, .shaders = {.vertex = vert, .fragment = frag}, \
    .uniform_loc = {[0 ... UNIF_Last - 1] = -1}, .obj = -1}


DECLARE_SHADER_ASSET(SHDR_StandardVert, standard_vert, {STANDARD_UNIFORMS});
DECLARE_SHADER_ASSET(SHDR_StandardFrag, standard_frag,
    {
        [UNIF_FragColor] = "frag_color"
    }
);
DECLARE_SHADER_ASSET(SHDR_SimpleFrag, simple_frag, {});
DECLARE_SHADER_ASSET(SHDR_StandardTextureVert, standard_texture_vert, {STANDARD_UNIFORMS});
DECLARE_SHADER_ASSET(SHDR_StandardTextureFrag, standard_texture_frag, {});
DECLARE_SHADER_ASSET(SHDR_SimpleLightingVert, simple_lighting_vert, {STANDARD_UNIFORMS});
DECLARE_SHADER_ASSET(SHDR_SimpleLightingFrag, simple_lighting_frag,
    {
        [UNIF_SimpleLighting_ObjColor] = "object_color",
        [UNIF_SimpleLighting_LightColor] = "light_color",
        [UNIF_SimpleLighting_LightPos] = "light_pos",
        [UNIF_SimpleLighting_EyePos] = "eye_pos",
        [UNIF_SimpleLighting_SpecularMap] = "specular_tex",
        [UNIF_SimpleLighting_DiffuseMap] = "diffuse_tex"
    }
);

struct shader_asset *SHADER_ASSETS[SHDR_Last] = {
    [SHDR_StandardVert] = &s_SHDR_StandardVert,
    [SHDR_StandardFrag] = &s_SHDR_StandardFrag,
    [SHDR_SimpleFrag] = &s_SHDR_SimpleFrag,
    [SHDR_StandardTextureVert] = &s_SHDR_StandardTextureVert,
    [SHDR_StandardTextureFrag] = &s_SHDR_StandardTextureFrag,
    [SHDR_SimpleLightingVert] = &s_SHDR_SimpleLightingVert,
    [SHDR_SimpleLightingFrag] = &s_SHDR_SimpleLightingFrag
};

DECLARE_SHADER_PROGRAM(SPRG_Standard, &s_SHDR_StandardVert, &s_SHDR_StandardFrag);
DECLARE_SHADER_PROGRAM(SPRG_StandardTexture, &s_SHDR_StandardTextureVert, &s_SHDR_StandardTextureFrag);
DECLARE_SHADER_PROGRAM(SPRG_SimpleLighting, &s_SHDR_SimpleLightingVert, &s_SHDR_SimpleLightingFrag);
DECLARE_SHADER_PROGRAM(SPRG_Simple, &s_SHDR_StandardVert, &s_SHDR_SimpleFrag);

struct shader_program *SHADER_PROGRAMS[SPRG_Last] = {
    [SPRG_Standard] = &s_SPRG_Standard,
    [SPRG_StandardTexture] = &s_SPRG_StandardTexture,
    [SPRG_SimpleLighting] = &s_SPRG_SimpleLighting,
    [SPRG_Simple] = &s_SPRG_Simple,
};