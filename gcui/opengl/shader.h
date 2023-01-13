#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>

int shader_compile(GLuint shader, const char * source, const char * err[]);
GLuint shader_create_program(GLuint vertext_shader, GLuint fragment_shader);

int shader_create(GLuint * program, const char * vertex_shader_source, const char * fragment_shader_source,
    const char * uniforms_name[], int * uniforms_locations, size_t uniform_len);

#endif