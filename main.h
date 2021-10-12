#ifndef MAIN_H
#define MAIN_H

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define DEBUG_ACTIVE false

#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "compute.h"
#include "batch_renderer.h"

void framebuffer_size_callback( GLFWwindow* window, int width, int height );
void process_input( GLFWwindow* window );

#if DEBUG_ACTIVE
void GLAPIENTRY gl_message_callback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param );
#endif

#endif