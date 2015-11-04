#include "LightSystem.h"
#include <assert.h>

void MaskShadow(Light* light, ConvexHull* convexHull, float depth)
{
	// ----------------------------- Determine the Shadow Boundaries -----------------------------

	const int numVertices = convexHull->vertices.size();

	std::vector<bool> backFacing(numVertices);

	for (int i = 0; i < numVertices; i++)
    {
		Vec2f firstVertex(convexHull->getWorldVertex(i).x, convexHull->getWorldVertex(i).y);
        int secondIndex = (i + 1) % convexHull->vertices.size();
        Vec2f secondVertex(convexHull->getWorldVertex(secondIndex).x, convexHull->getWorldVertex(secondIndex).y);
        Vec2f middle = (firstVertex + secondVertex) / 2;

		Vec2f L = light->center - middle;

        Vec2f N;
        N.x = - (secondVertex.y - firstVertex.y);
        N.y = secondVertex.x - firstVertex.x;
                
		if (N.dot(L) > 0)
            backFacing[i] = false;
        else
            backFacing[i] = true;
    }

	int firstBoundaryIndex = 0;
	int secondBoundaryIndex = 0;

	for (int i = 0; i < numVertices; i++)
    {
        int currentEdge = i;
        int nextEdge = (i + 1) % numVertices;

        if (backFacing[currentEdge] && !backFacing[nextEdge])
            firstBoundaryIndex = nextEdge;

        if (!backFacing[currentEdge] && backFacing[nextEdge])
            secondBoundaryIndex = nextEdge;
    }

	// ----------------------------- Drawing the shadow geometry -----------------------------

	// Go through the points
	glColor4f(0.0f, 0.0f, 0.0f, ambientLight);

	glBegin(GL_TRIANGLE_STRIP);

	if(firstBoundaryIndex <= secondBoundaryIndex)
		for(; firstBoundaryIndex <= secondBoundaryIndex; firstBoundaryIndex++)
		{
			int index = Wrap(firstBoundaryIndex, static_cast<signed>(convexHull->vertices.size()));
			Vec2f ray(convexHull->getWorldVertex(index).x - light->center.x,
			convexHull->getWorldVertex(index).y - light->center.y);

			// Get ray to right length
			assert(ray.magnitude() != 0); // Might divide by 0, watch out

			ray *= light->radius/ray.magnitude();

			glVertex3f(convexHull->getWorldVertex(index).x, convexHull->getWorldVertex(index).y, depth);
			glVertex3f(convexHull->getWorldVertex(index).x + ray.x, convexHull->getWorldVertex(index).y + ray.y, depth);	
		}
	else 
		for(; firstBoundaryIndex <= secondBoundaryIndex + static_cast<signed>(convexHull->vertices.size()); firstBoundaryIndex++)
		{
			int index = Wrap(firstBoundaryIndex, static_cast<signed>(convexHull->vertices.size()));
			Vec2f ray(convexHull->getWorldVertex(index).x - light->center.x,
			convexHull->getWorldVertex(index).y - light->center.y);

			// Get ray to right length
			assert(ray.magnitude() != 0); // Might divide by 0, watch out

			ray *= light->radius/ray.magnitude();
			
			glVertex3f(convexHull->getWorldVertex(index).x, convexHull->getWorldVertex(index).y, depth);
			glVertex3f(convexHull->getWorldVertex(index).x + ray.x, convexHull->getWorldVertex(index).y + ray.y, depth);	
		}

	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

LightSystem::LightSystem()
	: enabled(false)
{
}

LightSystem::~LightSystem()
{
}

void LightSystem::addLight(Light* newLight)
{
	lights.push_back(newLight);
}

void LightSystem::addConvexHull(ConvexHull* newConvexHull)
{
	convexHulls.push_back(newConvexHull);
}

void LightSystem::clearLights()
{
	for(unsigned int i = 0; i < lights.size(); i++)
		delete lights[i];

	lights.clear();
}

void LightSystem::clearConvexHulls()
{
	for(unsigned int i = 0; i < convexHulls.size(); i++)
		delete convexHulls[i];

	convexHulls.clear();
}

void LightSystem::RenderLights()
{
	// Draw darkness
	glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);

	glColor4f(0.0f, 0.0f, 0.0f, ambientLight);

	const float window_width_float = static_cast<float>(window_width);
	const float window_height_float = static_cast<float>(window_height);

	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, -1.0f);
		glVertex3f(window_width_float, 0.0f, -1.0f);
		glVertex3f(window_width_float, window_height_float, -1.0f);
		glVertex3f(0.0f, window_height_float, -1.0f);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_NEVER, 0x1, 0x1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	
	for(unsigned int i = 0; i < lights.size(); i++)
		for(unsigned int k = 0; k < convexHulls.size(); k++)
			MaskShadow(lights[i], convexHulls[k], lights[i]->depth);

	// Remove shadow mask from the shapes if one overlaps onto the other
	//glStencilFunc(GL_NEVER, 0x1, 0x1); // Redundant
	glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

	for(unsigned int i = 0; i < convexHulls.size(); i++)
		convexHulls[i]->RenderHull();

	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	for(unsigned int i = 0; i < lights.size(); i++)
		lights[i]->renderLightAlpha();

	glClear(GL_STENCIL_BUFFER_BIT);
	
	glDisable(GL_STENCIL_TEST);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE); // This function owns!!!

	glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
