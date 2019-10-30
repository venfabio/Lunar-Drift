#pragma once

const unsigned int GLCONFIG_MAJOR_VERSION = 4;
const unsigned int GLCONFIG_MINOR_VERSION = 0;

const unsigned int GLCONFIG_COLOR_BITS = 24;
const unsigned int GLCONFIG_ALPHA_BITS = 8;
const unsigned int GLCONFIG_DEPTH_BITS = 24;
const unsigned int GLCONFIG_STENCIL_BITS = 8;

const bool USE_MULTITHREADING = false;


#define GLEW_STATIC
#include <GL/glew.h>

#ifdef _WIN32
#include <GL/wglew.h>
#endif

#include <glm/glm.hpp>

#pragma comment (lib, "opengl32.lib")
#ifdef _WIN32
#pragma comment (lib, "glew32s.lib")
#endif
