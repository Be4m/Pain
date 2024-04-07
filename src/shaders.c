#include "shaders.h"

#include "common.h"
#include "shader_store.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <GL/glew.h>

static int compile_programs(sprg_store_t programs);
static int load_shader_sources(shdr_store_t shaders);
static inline const char *shader_uid2str(enum SHADER_UID idt);


int create_shader_programs(sprg_store_t programs, shdr_store_t shaders)
{
    if (load_shader_sources(shaders) != 0) {
        ERR("Failed to load shader sources from the binary.");
        return -1;
    }

    if (compile_programs(programs) != 0) {
        GL_ERR("Failed to compile shader programs.");
        return -1;
    }

    return 0;
}

static int compile_programs(sprg_store_t programs)
{
    int32_t fragment_shader, vertex_shader, shader_program;
    int32_t success;
    struct shader_program *program;
    char log[512];

    int32_t compiled_shader_map[SHDR_Last] = { [0 ... SHDR_Last - 1] = -1};

    for (uint32_t i = 0; i < SPRG_Last; i++) {
        program = programs[i];
#ifdef _DEBUG
        if (program == NULL) {
            ERR("Couldn't find the definition of the requested shader program (%d).", i);
            return -1;
        }
#endif

        if (compiled_shader_map[program->shaders.vertex->shader_uid] == -1) {
            vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex_shader, 1, (const char *const *)&program->shaders.vertex->source, NULL);
            glCompileShader(vertex_shader);

            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertex_shader, 512, NULL, log);
                GL_ERR("VERTEX shader (%s) failed to compile:\n%s", shader_uid2str(program->shaders.vertex->shader_uid), log);
                return -1;
            }

            compiled_shader_map[program->shaders.vertex->shader_uid] = vertex_shader;
        } else {
            vertex_shader = compiled_shader_map[program->shaders.vertex->shader_uid];
        }

        if (compiled_shader_map[program->shaders.fragment->shader_uid] == -1) {
            fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment_shader, 1, (const char *const *)&program->shaders.fragment->source, NULL);
            glCompileShader(fragment_shader);

            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragment_shader, 512, NULL, log);
                GL_ERR("FRAGMENT shader (%s) failed to compile:\n%s", shader_uid2str(program->shaders.fragment->shader_uid), log);
                return -1;
            }

            compiled_shader_map[program->shaders.fragment->shader_uid] = fragment_shader;
        } else {
            fragment_shader = compiled_shader_map[program->shaders.fragment->shader_uid];
        }
        
        shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);

        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader_program, 512, NULL, log);
            GL_ERR("Failed to link shader program (%d):\n%s", program->idt, log);
            return -1;
        }

        program->obj = shader_program;

        free(program->shaders.vertex->source);
        free(program->shaders.fragment->source);
    }

    for (uint32_t u = 0; u < SHDR_Last; u++) {
        int32_t shader = compiled_shader_map[u];

        glDeleteShader(shader);
        glDeleteShader(shader);
    }

    return 0;
}

static int load_shader_sources(shdr_store_t shaders)
{
    const char *p;
    struct shader_asset *shader;
    char *buffer;
    uint32_t buf_len, n_chunks;

    for (uint32_t i = 0; i < SHDR_Last; i++) {
        shader = shaders[i];

        buffer = (char *)malloc(SHADER_SIZ_CHUNK);
        buf_len = 0;
        n_chunks = 1;

        for (p = shader->binary.start_ptr; p != shader->binary.end_ptr; p++) {
            // PREPROCESSING SHADER SOURCES
            // 1) Remove carriage return characters.
            if (*p == '\r') {
                continue;
            }

            buf_len++;
            if (buf_len > SHADER_SIZ_CHUNK && buf_len % SHADER_SIZ_CHUNK == 1) {
                n_chunks++;

                if ((buffer = realloc(buffer, n_chunks * SHADER_SIZ_CHUNK)) == NULL) {
                    return -1;
                }
            }

            buffer[buf_len - 1] = *p;
        }

        buffer[buf_len] = '\0';
        shader->source = buffer;
    }

    return 0;
}

void shader_program_load_uniform_locations(struct shader_program *program)
{
    struct uniform *unif;
    _Bool first_el_found = false;

    glUseProgram(program->obj);

    for (uint32_t j = 0; j < UNIF_Last; j++) {
        unif = &program->uniforms[j];
        if (!unif->idt && !first_el_found) {
            first_el_found = true;
        } else if (!unif->idt && first_el_found) {
            continue;
        }

        unif->location = glGetUniformLocation(program->obj, unif->name);
    }
}

static inline const char *shader_uid2str(enum SHADER_UID idt)
{
    switch (idt) {
    case SHDR_StandardVert: return "SHDR_StandardVert";
    case SHDR_StandardFrag: return "SHDR_StandardFrag";
    case SHDR_StandardTextureVert: return "SHDR_StandardTextureVert";
    case SHDR_StandardTextureFrag: return "SHDR_StandardTextureFrag";
    case SHDR_SimpleLightingFrag: return "SHDR_SimpleLightingFrag";
    default:
        return "Unrecognized Shader";
    }
}