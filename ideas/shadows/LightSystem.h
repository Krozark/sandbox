#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H

#include "Light.h"
#include "ConvexHull.h"
#include <vector>

void MaskShadow(Light* light, ConvexHull* convexHull, float depth);

class LightSystem
{
private:
	std::vector<Light*> lights;
	std::vector<ConvexHull*> convexHulls;

	bool enabled;

public:
	LightSystem();
	~LightSystem();

	void addLight(Light* newLight);
	void addConvexHull(ConvexHull* newConvexHull);
	void clearLights();
	void clearConvexHulls();
	void RenderLights();
};

#endif
