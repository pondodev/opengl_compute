#ifndef BATCH_RENDERER_H
#define BATCH_RENDERER_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "dan_math.h"

class BatchRenderer {
public:
    BatchRenderer() {
        // create our vertex buffer and array objects
        glGenBuffers( 1, &vbo );
        glGenBuffers( 1, &ebo );
        glGenVertexArrays( 1, &vao );

        // bind the vbo, ebo, and vao
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
        glBindVertexArray( vao );

        // set attributes
        // position
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, stride, (void*) 0 );
        glEnableVertexAttribArray( 0 );
        // color
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (2 * sizeof(float)) );
        glEnableVertexAttribArray( 1 );

        square_count = 0;
    }

    ~BatchRenderer() {
        glDeleteBuffers( 1, &vbo );
        glDeleteBuffers( 1, &ebo );
        glDeleteVertexArrays( 1, &vao );
    }

    void clear( glm::vec3 color ) {
        // prepare collections for new render
        vbo_data.clear();
        ebo_data.clear();
        square_count = 0;

        // clear the screen
        glClearColor( color.r, color.g, color.b, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
    }

    // takes in 2d ndc pos and 8-bit color
    void add_square( glm::vec2 pos, glm::uvec3 color, float size ) {
        // top left, top right, bottom right, bottom left
        push_vert( pos.x, pos.y, color.r, color.g, color.b );
        push_vert( pos.x+size, pos.y, color.r, color.g, color.b );
        push_vert( pos.x, pos.y-size, color.r, color.g, color.b );
        push_vert( pos.x+size, pos.y-size, color.r, color.g, color.b );

        // add indices to ebo
        unsigned int ebo_offset = square_count * 4;
        // first tri
        ebo_data.push_back( ebo_offset );
        ebo_data.push_back( ebo_offset+1 );
        ebo_data.push_back( ebo_offset+2 );
        // second tri
        ebo_data.push_back( ebo_offset+1 );
        ebo_data.push_back( ebo_offset+2 );
        ebo_data.push_back( ebo_offset+3 );

        square_count++;
    }

    void render( Shader* shader ) {
        shader->use();

        // send data to gl
        auto vbo_ptr = vbo_data.data();
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(vbo_ptr) * vbo_data.size(), vbo_ptr, GL_DYNAMIC_DRAW );

        auto ebo_ptr = ebo_data.data();
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_ptr) * ebo_data.size(), ebo_ptr, GL_DYNAMIC_DRAW );

        // actually render
        glBindVertexArray( vao );
        glDrawElements( GL_TRIANGLES, ebo_data.size(), GL_UNSIGNED_INT, NULL );
    }

private:
    const unsigned int stride = 5 * sizeof(float); // vec2 pos, vec3 color

    unsigned int vbo, ebo, vao;
    std::vector<float> vbo_data;
    std::vector<unsigned int> ebo_data;
    unsigned int square_count;

    void push_vert( float x, float y, float r, float g, float b ) {
        // coords are in ndc
        // TODO: mvp matrix?
        vbo_data.push_back( x );
        vbo_data.push_back( y );

        // convert to 0-1 range
        vbo_data.push_back( inverse_lerp( 0.0, 255.0f, r ) );
        vbo_data.push_back( inverse_lerp( 0.0, 255.0f, g ) );
        vbo_data.push_back( inverse_lerp( 0.0, 255.0f, b ) );
    }
};

#endif
