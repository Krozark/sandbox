#ifndef SDLOPENGL
#define SDLOPENGL

#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <SFML/Graphics.hpp>

#include "Constructs.h"

extern unsigned int window_width, window_height, window_bpp;

struct TextureDesc
{
	int width, height;
     GLuint hTexture;
    ~TextureDesc()
    {
        glDeleteTextures(1, &hTexture);
    }
};

// Change viewport
void ViewOrtho(float zNear, float zFar);
void ViewPerspective(float zNear, float zFar);
void ResizeGLScene(GLsizei width, GLsizei height);

bool LoadIMG(const char* filename, TextureDesc &textureInfo);

// Init/shut down
bool InitSDLOpenGL();

// Simple 2D quad drawing function
void Draw2DQuad(const TextureDesc &textureInfo, float tiles);

#endif
