#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // program id
    unsigned int id;

    // constructor will read and build the shader
    Shader( const char* vertexPath, const char* fragmentPath ) {
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream v_shader_file;
        std::ifstream f_shader_file;

        // ensure ifstreams can throw exceptions
        v_shader_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        f_shader_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );

        try {
            // open files, read buffers into streams
            v_shader_file.open( vertexPath );
            f_shader_file.open( fragmentPath );
            std::stringstream v_shader_stream, f_shader_stream;
            v_shader_stream << v_shader_file.rdbuf();
            f_shader_stream << f_shader_file.rdbuf();

            v_shader_file.close();
            f_shader_file.close();

            // convert streams into strings
            vertex_code = v_shader_stream.str();
            fragment_code = f_shader_stream.str();
        } catch ( std::ifstream::failure e ) {
            std::cerr << "failed to read shader files" << std::endl;
        }

        const char* v_shader_code = vertex_code.c_str();
        const char* f_shader_code = fragment_code.c_str();

        // compile shaders
        unsigned int vertex, fragment;

        // vertex shader
        vertex = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( vertex, 1, &v_shader_code, NULL );
        glCompileShader( vertex );

        // fragment shader
        fragment = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( fragment, 1, &f_shader_code, NULL );
        glCompileShader( fragment );

        // shader program
        id = glCreateProgram();
        glAttachShader( id, vertex );
        glAttachShader( id, fragment );
        glLinkProgram( id );

        // delete shaders as they are not needed anymore
        glDeleteShader( vertex );
        glDeleteShader( fragment );
    }

    ~Shader() {
        // delete program upon destruction of shader
        glDeleteProgram( id );
    }

    // use/activate the shader
    void use() {
        glUseProgram( id );
    }

    // utility uniform functions
    void setBool( const std::string &name, bool value ) const {
        glUniform1i( glGetUniformLocation( id, name.c_str() ), (int) value );
    }

    void setInt( const std::string &name, int value ) const {
        glUniform1i( glGetUniformLocation( id, name.c_str() ), value );
    }

    void setFloat( const std::string &name, float value ) const {
        glUniform1f( glGetUniformLocation( id, name.c_str() ), value );
    }
};

#endif