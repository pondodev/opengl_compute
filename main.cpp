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
        NULL );

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

    #if DEBUG_ACTIVE
    glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( gl_message_callback, 0 );
    #endif

    #pragma endregion

    #pragma region compute shader setup

    Compute compute_shader( "shader.comp", glm::uvec2( 10, 1 ) );

    compute_shader.use();
    float values[ 10 ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    compute_shader.set_values( values );

    #pragma endregion

    #pragma region rendering setup

    Shader visual_shader( "shader.vert", "shader.frag" );
    BatchRenderer renderer;

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

        // draw
        renderer.clear( glm::vec3( 0.1f, 0.1f, 0.1f ) );

        auto x_offset = glm::sin( glfwGetTime() * 2 ) * 0.2;
        renderer.add_square(
            glm::vec2( 0.0f + x_offset, 0.0f ),
            glm::uvec3( 255, 0, 0 ),
            0.1f );
        renderer.add_square(
            glm::vec2( 0.0f + x_offset, 0.5f ),
            glm::uvec3( 0, 255, 0 ),
            0.1f );
        renderer.add_square(
            glm::vec2( 0.0f + x_offset, -0.5f ),
            glm::uvec3( 0, 0, 255 ),
            0.1f );

        renderer.render( &visual_shader );

        // poll glfw events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers( window );
    }

    #pragma endregion

    // clean up resources upon successful exit
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

#if DEBUG_ACTIVE
// more information at https://www.khronos.org/opengl/wiki/Debug_Output
void GLAPIENTRY gl_message_callback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param ) {
    // parse source
    std::string source_name;
    switch ( source ) {
        case GL_DEBUG_SOURCE_API:
            source_name = "GL_DEBUG_SOURCE_API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            source_name = "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            source_name = "GL_DEBUG_SOURCE_SHADER_COMPILER";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            source_name = "GL_DEBUG_SOURCE_THIRD_PARTY";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            source_name = "GL_DEBUG_SOURCE_APPLICATION";
            break;

        case GL_DEBUG_SOURCE_OTHER:
            source_name = "GL_DEBUG_SOURCE_OTHER";
            break;

        default:
            source_name = "UNKNOWN";
            break;
    }

    // parse type
    std::string type_name;
    switch ( type ) {
        case GL_DEBUG_TYPE_ERROR:
            type_name = "GL_DEBUG_TYPE_ERROR";
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            type_name = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            type_name = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            type_name = "GL_DEBUG_TYPE_PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            type_name = "GL_DEBUG_TYPE_PERFORMANCE";
            break;

        case GL_DEBUG_TYPE_MARKER:
            type_name = "GL_DEBUG_TYPE_MARKER";
            break;

        case GL_DEBUG_TYPE_PUSH_GROUP:
            type_name = "GL_DEBUG_TYPE_PUSH_GROUP";
            break;

        case GL_DEBUG_TYPE_POP_GROUP:
            type_name = "GL_DEBUG_TYPE_POP_GROUP";
            break;

        case GL_DEBUG_TYPE_OTHER:
            type_name = "GL_DEBUG_TYPE_OTHER";
            break;

        default:
            type_name = "UNKNOWN";
            break;
    }

    // parse severity
    std::string severity_name;
    switch ( severity ) {
        case GL_DEBUG_SEVERITY_HIGH:
            severity_name = "GL_DEBUG_SEVERITY_HIGH";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            severity_name = "GL_DEBUG_SEVERITY_MEDIUM";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            severity_name = "GL_DEBUG_SEVERITY_LOW";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            severity_name = "GL_DEBUG_SEVERITY_NOTIFICATION";
            break;

        default:
            severity_name = "UNKNOWN";
            break;
    }

    // print message
    std::cerr << "GL CALLBACK:\n";
    std::cerr << "source: " << source_name << "\n";
    std::cerr << "type: " << type_name << "\n";
    std::cerr << "severity: " << severity_name << "\n";
    std::cerr << "message: " << message << "\n";
    std::cerr << std::endl;
}
#endif
