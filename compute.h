#ifndef COMPUTE_H
#define COMPUTE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Compute {
    public:
    unsigned int id;
    unsigned int out_tex;

    Compute( const char* path, glm::uvec2 size ) {
        work_size = size;

        // read in shader code
        std::string compute_code;
        std::ifstream file;

        file.exceptions( std::ifstream::failbit | std::ifstream::badbit );

        try {
            file.open( path );
            std::stringstream file_stream;
            file_stream << file.rdbuf();
            file.close();

            compute_code = file_stream.str();
        } catch ( std::ifstream::failure e ) {
            std::cerr << "failed to read compute shader file" << std::endl;
        }

        const char* c_shader_code = compute_code.c_str();

        // compile shader
        unsigned int shader;

        shader = glCreateShader( GL_COMPUTE_SHADER );
        glShaderSource( shader, 1, &c_shader_code, NULL );
        glCompileShader( shader );

        // check for errors
        int success;
        char info_log[ 512 ];
        glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
        if ( !success ) {
            glGetShaderInfoLog( shader, 512, NULL, info_log );
            std::cerr << "compute shader failed to compile:\n" << info_log << std::endl;
        }

        // create program
        id = glCreateProgram();
        glAttachShader( id, shader );
        glLinkProgram( id );

        // check for linking errors
        glGetProgramiv( id, GL_LINK_STATUS, &success );
        if ( !success ) {
            glGetProgramInfoLog( id, 512, NULL, info_log );
            std::cerr << "failed to link compute program:\n" << info_log << std::endl;
        }

        // cleanup
        glDeleteShader( shader );

        // create input/output textures
        glGenTextures( 1, &out_tex );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, out_tex );

        // i cant imagine we would need this because we shouldnt ever
        // render this texture. guess we'll see huh
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

        // create empty texture
        float values[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        glTexImage2D( GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, values);
        glBindImageTexture( 0, out_tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F );
    }

    ~Compute() {
        glDeleteProgram( id );
    }

    void use() {
        glUseProgram( id );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, out_tex );
    }

    void dispatch() {
        // just keep it simple, 2d work group
        glDispatchCompute( work_size.x, work_size.y, 1 );
    }

    void wait() {
        glMemoryBarrier( GL_ALL_BARRIER_BITS );
    }

private:
    glm::vec2 work_size;
};

#endif
