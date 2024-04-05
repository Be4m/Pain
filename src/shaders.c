#include "shaders.h"

#include "common.h"
#include "shader_store.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

static int compile_programs(sprg_store_t programs);
static int load_shader_sources(shdr_store_t shaders);


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
    uint32_t fragment_shader, vertex_shader, shader_program;
    int32_t success;
    struct shader_program *program;
    char log[512];

    for (uint32_t i = 0; i < SPRG_Last; i++) {
        program = programs[i];
#ifdef _DEBUG
        if (program == NULL) {
            ERR("Couldn't find the definition of the requested shader program (%d).", i);
            return -1;
        }
#endif

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, (const char *const *)&program->shaders.vertex->source, NULL);
        glCompileShader(vertex_shader);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, (const char *const *)&program->shaders.fragment->source, NULL);
        glCompileShader(fragment_shader);

        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex_shader, 512, NULL, log);
            GL_ERR("VERTEX shader failed to compile:\n%s", log);
            return -1;
        }

        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment_shader, 512, NULL, log);
            GL_ERR("FRAGMENT shader failed to compile:\n%s", log);
            return -1;
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

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        free(program->shaders.vertex->source);
        free(program->shaders.fragment->source);
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