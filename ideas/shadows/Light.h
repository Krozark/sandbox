#ifndef LIGHT_H
#define LIGHT_H

#include "SDLOpenGL.h"
#include "Utilities.h"
#include "Constructs.h"

const float ambientLight = 0.3f;

class Light
{
public:
	float intensity;
	float radius;
	float depth;
	Vec2f center;

	Light();
	~Light();

	void renderLightAlpha();
};

#endif
