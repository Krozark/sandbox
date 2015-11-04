#ifndef UTILITIES_H
#define UTILITIES_H

#include <fstream>
#include <sstream>

const float PI = 3.14159265f; 

template <class T> std::string toString(const T& t);

// NOTE: You should probably do some checks to ensure that
// this string contains only numbers. If the string is not
// a valid integer, zero will be returned.
int GetIntVal(std::string strConvert);
double GetDoubleVal(std::string strConvert);

float toRads(float degrees);
float toDegrees(float rads);

void rangeCheckRads(float &rads);

void rangeCheckDegrees(float &degrees);

// Useful for having a counter loop around on itself again
template<typename T>
T Wrap(const T& val, const T& size)
{
	if(val < 0)
		return size + val;

	if(val >= size)
		return val - size;

	return val;
}

#endif
