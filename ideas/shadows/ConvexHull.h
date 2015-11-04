#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include "SDLOpenGL.h"
#include "Utilities.h"
#include "Constructs.h"
#include <vector>

struct ConvexHullVertex
{
	Point2f position;

	// Other information can be added later
};

class ConvexHull
{	
public:
	std::vector<ConvexHullVertex> vertices;
	std::vector<Vec2f> normals;

	Point2f worldCenter;

	float depth;
	float shadowDepthOffset;

	ConvexHull();
	~ConvexHull();

	bool LoadShape(const char* fileName);
	Point2f getWorldVertex(unsigned int index);

	void CalculateNormals();

	void RenderHull();
};

#endif

