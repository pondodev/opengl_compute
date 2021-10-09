#ifndef MAIN_H
#define MAIN_H

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "compute.h"

void framebuffer_size_callback( GLFWwindow* window, int width, int height );
void process_input( GLFWwindow* window );

#endif