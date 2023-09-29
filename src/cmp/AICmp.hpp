#pragma once
#include <util/point2d.hpp>

struct AICmp_t {

	Point2D_t pointTarget {  0, 0 };		// x, y coordinates to target
	bool  targetActive    { false };
	float arrivalRadius   {   1.0 };
	float arrivalTime     {   1.0 };
};