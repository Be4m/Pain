#pragma once

#include <stdint.h>


extern struct shader_asset *shader_assets[];
extern struct shader_program *shader_programs[];

enum SHADER_UID {StandardVertex, StandardFragment, LastShaderUID};
enum SHADER_PROGRAM_UID {Standard, LastShaderProgramUID};

struct shader_asset {
    const char *path;
    char *source;
    enum SHADER_UID idt;
};

struct shader_program {
    uint32_t shader_program;
    enum SHADER_PROGRAM_UID idt;
    struct {
        enum SHADER_UID vertex;
        enum SHADER_UID fragment;
    } shaders;
};