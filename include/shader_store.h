#pragma once

#include <stdint.h>


typedef struct shader_program *sprg_store_t[];
typedef struct shader_asset *shdr_store_t[];

extern shdr_store_t SHADER_ASSETS;
extern sprg_store_t SHADER_PROGRAMS;

enum SHADER_UID {SHDR_StandardVert, SHDR_StandardFrag, SHDR_StandardTextureVert, SHDR_StandardTextureFrag, SHDR_Last};
enum SHADER_PROGRAM_UID {SPRG_Standard, SPRG_StandardTexture, SPRG_Last};

enum UNIFORM_UID {UNIF_ModelMat, UNIF_ViewMat, UNIF_ProjMat, UNIF_FragColor, UNIF_Last};

struct shader_asset {
    struct {
        const char *const start_ptr;
        const char *const end_ptr;
    } binary;
    char *source;
    enum SHADER_UID shader_uid;

    const char *uniforms[UNIF_Last];
};

struct shader_program {
    int32_t obj;
    enum SHADER_PROGRAM_UID idt;
    struct {
        const struct shader_asset *vertex;
        const struct shader_asset *fragment;
    } shaders;

    int32_t uniform_loc[UNIF_Last];
};