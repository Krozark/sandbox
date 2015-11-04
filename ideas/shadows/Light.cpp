#include "Light.h"

#include <assert.h>

Light::Light()
	: intensity(1.0f),
	radius(160.0f),
	depth(-1.0f),
	center(0.0f, 0.0f) // Vec2f contructor
{
}

Light::~Light()
{
}

void Light::renderLightAlpha()
{
	assert(intensity > 0.0f && intensity <= 1.0f);
 
	int numSubdivisions = 32;
    
	glBegin(GL_TRIANGLE_FAN);

	glColor4f(0.0f, 0.0f, 0.0f, intensity);

	glVertex3f(center.x, center.y, depth);
      
	// Set edge colour for rest of shape
	glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
      
	for (float angle = 0.0f; angle <= PI*2; angle += ((PI*2)/numSubdivisions) )
		glVertex3f(radius * cosf(angle) + center.x, radius * sinf(angle) + center.y, depth);  
      
	glVertex3f(center.x + radius, center.y, depth);

	glEnd();
}
