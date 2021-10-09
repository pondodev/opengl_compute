#ifndef COMPUTE_H
#define COMPUTE_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Compute {
    public:
    unsigned int id;

    Compute( const char* path ) {
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
    }

    ~Compute() {
        glad_glDeleteProgram( id );
    }

    void use() {
        glUseProgram( id );
    }
};

#endif
