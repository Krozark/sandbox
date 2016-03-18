#include "SFMLOpenGL.h"

#include <iostream>

unsigned int window_width = 640;
unsigned int window_height = 480;
unsigned int window_bpp = 32;
bool inOrtho = false;

void ViewOrtho(float zNear, float zFar)
{
	if(!inOrtho)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, window_width, 0, window_height, zNear, zFar);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		inOrtho = true;
	}
}

void ViewPerspective(float zNear, float zFar)
{
	if(inOrtho)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (window_width/window_height), zNear, zFar);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		inOrtho = false;
	}
}

void ResizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
		height=1;										// Making Height Equal One

	glViewport(0, 0, width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

bool LoadIMG(const char* filename,TextureDesc& textureInfo)
{
     
    sf::Image image;
    if (!image.loadFromFile(filename))
    {
        return false;
    }
    
	int w = image.getSize().x; 
	int h = image.getSize().y; 
			
	// Now generate the OpenGL texture object 
    GLuint texture = 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
			
	GLenum glError = glGetError(); 
			
	if(glError)
		return false;

    textureInfo.width = w;
    textureInfo.height = h;
    textureInfo.hTexture = texture;

	return true;
}

bool InitSDLOpenGL()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	ViewPerspective(0.1f, 100.0f);

	ResizeGLScene(window_width, window_height);

	glEnable(GL_CULL_FACE); // Enable backface culling
	glEnable(GL_BLEND); // Enable alpha blending
	glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
	//glEnable(GL_NORMALIZE); // Normalizes normals when scaling to lighting isn't messed up

	// Default blend function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ------------------------------- Check for Initialization Errors -------------------------------

	if(glGetError() != GL_NO_ERROR)
		return false;

	return true;
}

void Draw2DQuad(const TextureDesc& texture, float tiles)
{
	float halfWidth = texture.width/2.0f;
	float halfHeight = texture.height/2.0f;

	glBindTexture(GL_TEXTURE_2D, texture.hTexture);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, 0.0f);
		glTexCoord2f(tiles, 0.0f); glVertex3f(halfWidth, -halfHeight, 0.0f);
		glTexCoord2f(tiles, tiles); glVertex3f(halfWidth, halfHeight, 0.0f);
		glTexCoord2f(0.0f, tiles); glVertex3f(-halfWidth, halfHeight, 0.0f);
	glEnd();
}
