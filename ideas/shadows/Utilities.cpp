#include "Utilities.h"

template <class T> std::string toString(const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

// NOTE: You should probably do some checks to ensure that
// this string contains only numbers. If the string is not
// a valid integer, zero will be returned.
int GetIntVal(std::string strConvert)
{
	return atoi(strConvert.c_str());
}

double GetDoubleVal(std::string strConvert)
{
	return atof(strConvert.c_str());
}

/* template <class T> T GetIntVal(std::string strConvert)          // Template Version - would require making own atoT (string to type T)
{
	return atoi(strConvert.c_str());
}*/

float toRads(float degrees)
{
	return degrees*PI/180;
}

float toDegrees(float rads)
{
	return rads*180/PI;
}

void rangeCheckRads(float &rads)
{
	// Adjust angle to be in range that openGL can render (in radians 0 <= angle < 2*PI, in degrees 0 <= angle < 360)
	if(rads < -2*PI)
		while(rads < -2*PI)
			rads += 2*PI;

	if(rads < 0)
		rads += 2*PI;
	else if(rads > 2*PI)
		while(rads > 2*PI)
			rads -= 2*PI;
}

void rangeCheckDegrees(float &degrees)
{
	// Adjust angle to be in range that openGL can render (in radians 0 <= angle < 2*PI, in degrees 0 <= angle < 360)
	if(degrees < -360)
		while(degrees < -360)
			degrees += 360;

	if(degrees < 0)
		degrees += 360;
	else if(degrees > 360)
		while(degrees > 360) 
			degrees -= 360;
}