#include "ConvexHull.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

ConvexHull::ConvexHull()
	: worldCenter(0.0f, 0.0f),
	depth(0.0f),
	shadowDepthOffset(0.0f)
{
}

ConvexHull::~ConvexHull()
{
}

bool ConvexHull::LoadShape(const char* fileName)
{
	std::ifstream load(fileName);

	if(!load)
	{
		load.close();
		std::cout << "Load failed!" << std::endl;
		
		return false;
	}
	else
	{
		while(true) // While not at end of file
		{
			std::string firstElement, secondElement;

			load >> firstElement >> secondElement;

			if(firstElement.size() == 0 || secondElement.size() == 0)
			{
				load.close();
				break;
			}

			ConvexHullVertex newVertex;
			newVertex.position.x = static_cast<float>(GetDoubleVal(firstElement));
			newVertex.position.y = static_cast<float>(GetDoubleVal(secondElement));

			vertices.push_back(newVertex);
		}
	}

	// Calculate normals
	CalculateNormals();

	return true;
}

Point2f ConvexHull::getWorldVertex(unsigned int index)
{
	assert(index >= 0 && index < vertices.size());
	return Point2f(vertices[index].position.x + worldCenter.x, vertices[index].position.y + worldCenter.y);
}

void ConvexHull::CalculateNormals()
{
	if(normals.size() != vertices.size())
		normals.resize(vertices.size());

	for(unsigned int i = 0; i < vertices.size(); i++) // Dots are wrong
	{
        unsigned int s = vertices.size();
		int index2 = Wrap(i + 1, s);

		normals[i].x = vertices[index2].position.x - vertices[i].position.x;
		normals[i].y = vertices[index2].position.y - vertices[i].position.y;
	}
}

void ConvexHull::RenderHull()
{
	glBegin(GL_TRIANGLE_FAN);

	glVertex3f(worldCenter.x, worldCenter.y, depth);
	
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		Point2f pos(getWorldVertex(i));
		glVertex3f(pos.x, pos.y, depth);
	}

	Point2f pos(getWorldVertex(0));
	glVertex3f(pos.x, pos.y, depth);

	glEnd();
}
