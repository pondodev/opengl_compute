#include "main.h"

int main() {
    #pragma region glfw setup

    // init glfw and some settings
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    // create window object
    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        "compute shader test",
        NULL,
        NULL);

    // ensure creation was successful 
    if ( window == NULL ) {
        std::cerr << "failed to create glfw window" << std::endl;
        glfwTerminate();

        return -1;
    }

    // set context
    glfwMakeContextCurrent( window );

    // load glad before we make any opengl calls
    if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
        std::cerr << "failed to initialise glad" << std::endl;

        return -1;
    }

    // set gl viewport size, and set glfw callback for window resize
    glViewport( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

    #pragma endregion

    #pragma region compute shader setup

    Compute compute_shader( "shader.comp", glm::uvec2( 10, 1 ) );

    compute_shader.use();
    float values[ 10 ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    compute_shader.set_values( values );

    #pragma endregion

    #pragma region vertex data

    Shader visual_shader( "shader.vert", "shader.frag" );

    // TODO: refactor into a generic call that renders the tris in a batch
    // TODO: supply MVP matrices for proper transforms
    // verts we will draw to screen
    float verts[] = {
        // positions         // colors
        // first tri
        -0.1f, -0.1f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.1f, -0.1f, 0.0f,   1.0f, 1.0f, 1.0f,
        0.1f, 0.1f, 0.0f,    1.0f, 1.0f, 1.0f,

        // second tri
        -0.1f, -0.1f, 0.0f,  1.0f, 1.0f, 1.0f,
        -0.1f, 0.1f, 0.0f,   1.0f, 1.0f, 1.0f,
        0.1f, 0.1f, 0.0f,    1.0f, 1.0f, 1.0f
    };

    // create our vertex buffer and array objects
    unsigned int vbo, vao;
    glGenBuffers( 1, &vbo );
    glGenVertexArrays( 1, &vao );

    // bind the vbo and vao, send vert data for object to gpu, set up attributes on vao
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindVertexArray( vao );
    glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW );

    // position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0 );
    glEnableVertexAttribArray( 0 );
    // color attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)) );
    glEnableVertexAttribArray( 1 );

    #pragma endregion

    #pragma region render loop

    while ( !glfwWindowShouldClose( window ) ) {
        // input
        process_input( window );

        // update
        compute_shader.use();
        compute_shader.dispatch();
        compute_shader.wait();

        auto data = compute_shader.get_values();
        for ( auto d : data ) {
            std::cout << d << " ";
        }
        std::cout << std::endl;

        // rendering
        glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        // render quad
        visual_shader.use();
        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLES, 0, 6 );

        // poll glfw events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers( window );
    }

    #pragma endregion

    // clean up resources upon successful exit
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    glfwTerminate();

    return 0;
}

// callback function to handle when the window resizes
void framebuffer_size_callback( GLFWwindow* window, int width, int height ) {
    glViewport( 0, 0, width, height );
}

// handle all input here
void process_input( GLFWwindow* window ) {
    // close window on pressing esc
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
        glfwSetWindowShouldClose( window, true );
    }
}
