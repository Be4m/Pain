#include "shaders.h"

#include "common.h"
#include "shader_store.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>


static struct shader_asset *get_shader_asset(struct shader_asset *assets[], enum SHADER_UID idt)
{
    for (uint32_t i = 0; i < LastShaderUID; i++) {
        if (assets[i]->idt == idt) {
            return assets[i];
        }
    }

    return NULL;
}

int compile_shaders(struct shader_asset *assets[], struct shader_program *programs[])
{
    uint32_t fragment_shader, vertex_shader, shader_program;
    int32_t success;
    struct shader_program *prog;
    struct shader_asset *vert_shader_asset, *frag_shader_asset;
    char log[512];

    for (uint32_t i = 0; i < LastShaderProgramUID; i++) {
        prog = programs[i];
#ifdef _DEBUG
        if (prog == NULL) {
            ERR("Couldn't find the definition of requested shader program (%d).", i);
            return -1;
        }
#endif

        vert_shader_asset = get_shader_asset(assets, prog->shaders.vertex);
        frag_shader_asset = get_shader_asset(assets, prog->shaders.fragment);
#ifdef _DEBUG
        if (vert_shader_asset == NULL) {
            ERR("Could not find a shader asset associated with id: %d", prog->shaders.vertex);
            return -1;
        }
        if (frag_shader_asset == NULL) {
            ERR("Could not find a shader asset associated with id: %d", prog->shaders.fragment);
            return -1;
        }
#endif

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, (const char *const *)&vert_shader_asset->source, NULL);
        glCompileShader(vertex_shader);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, (const char *const *)&frag_shader_asset->source, NULL);
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
            GL_ERR("Failed to link shader program (%d):\n%s", prog->idt, log);
            return -1;
        }

        prog->shader_program = shader_program;
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    return 0;
}

int load_shaders(struct shader_asset *assets[])
{
    FILE *f;
    char c, *buffer = malloc(SHADER_SIZ * sizeof(char));
    uint32_t len = 0;

    for (uint32_t i = 0; i < LastShaderUID; i++) {
        if ((f = fopen(assets[i]->path, "r")) == NULL) {
            return -1;
        }

        len = 0;
        while ((c = fgetc(f)) != EOF) {
            len++;
            if (len > SHADER_SIZ) {
                GCC_IGN_RES(realloc(buffer, (len / SHADER_SIZ) * SHADER_SIZ + SHADER_SIZ));
            }

            buffer[len - 1] = c;
        }

        fclose(f);
        buffer[len] = '\0';
        assets[i]->source = buffer;
    }

    return 0;
}