#include "shader.h"

#include "log.h"
#include "util.h"
#include "mema.h"

#include <GL/glew.h>

static char* error_log_buffer = NULL;
GLint error_log_lenght = 0;

int
shader_compile(GLuint shader, const char * source, const char * err[])
{
    int ret = 0;
    gmema_init();

    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_log_lenght);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);

    if(error_log_buffer)
        gfree(error_log_buffer);
    error_log_buffer = gmalloc(sizeof(*error_log_buffer) * error_log_lenght);

    if(error_log_lenght || ret == GL_FALSE)
    {
        glGetShaderInfoLog(shader, error_log_lenght, NULL, error_log_buffer);
        if(err)
            *err = error_log_buffer;
        return 1;
    }

    if(err)
        *err = NULL;
    
    return 0;
}

GLuint 
shader_create_program(GLuint vertext_shader, GLuint fragment_shader)
{
    GLuint program = 0;

    program = glCreateProgram();
    
    glAttachShader(program, vertext_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    return program;
}

int 
shader_create(GLuint * program, const char * vertex_shader_source, const char * fragment_shader_source,
    const char * uniforms_name[], int *uniforms_locations, size_t uniform_len)
{
    size_t i = 0;
    const char *err = NULL;
    GLuint vertex_shader = 0, fragment_shader = 0;
    
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    if(shader_compile(vertex_shader, vertex_shader_source, &err))
    {
        err("cannot compile vertex shader: %s", err);
        return 1;
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if(shader_compile(fragment_shader, fragment_shader_source, &err))
    {
	    glDeleteShader(vertex_shader);
        err("cannot compile fragment shader: %s", err);
        return 1;
    }

    *program = shader_create_program(vertex_shader, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

    for(i = 0; i < uniform_len; i++)
    {
        uniforms_locations[i] = glGetUniformLocation(*program, uniforms_name[i]);
        if(uniforms_locations[i] == -1)
        {
            err("cannot find \"%s\" uniform", uniforms_name[i]);
            return 1;
        }
    }
    
    return 0;
}

int
shader_create_from_file(GLint * program, const char *vertex_shader_filename, 
    const char *fragment_shader_filename, const char *uniform_names[], 
    int *uniforms, size_t uniform_len)
{
    int ret = 0;
    char *vertex_source = NULL, *fragment_source = NULL;

    if((ret = read_file(vertex_shader_filename, &vertex_source, NULL)))
    {
        err("cannot open %s: %s", vertex_shader_filename, strerror(ret));
        return ret;
    }

    if((ret = read_file(fragment_shader_filename, &fragment_source, NULL)))
    {
        err("cannot open %s: %s", fragment_shader_filename, strerror(ret));
        return ret;
    }

    ret = shader_create(program, vertex_source, fragment_source,
        uniform_names, uniforms, uniform_len);
    if(ret)
        return ret;
    
    return 0;
}