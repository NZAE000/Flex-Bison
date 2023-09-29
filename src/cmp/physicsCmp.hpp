#pragma once
#include <numbers>
#include <util/point2d.hpp>


struct PhysicsCmp_t {

	static constexpr auto PI { std::numbers::pi_v<float> };

	// UNITS PER SECOND
	static constexpr float MAX_VLINEAR   { 				    80 }; // MAX Linear velocity to 80 PX per sec
	static constexpr float MAX_VANGULAR  {               PI*.3 }; // ........... PER SECOND
	static constexpr float MAX_ALINEAR   { 		MAX_VLINEAR/.5 }; // ........... PER SECOND
	static constexpr float MAX_AANGULAR  { 	   MAX_VANGULAR/.5 }; // ........... PER SECOND

	// ##########################################################################################

	Point2D_t point {0, 0};			// x, y coordinates
	float vx{0}, vy{0};				// velocity in x and y
	float orientation{0};			// angle orientation (radians)

	float vLinear{0}, vAngular{0}; 	// linear velocity and angular velocity
	float aLinear{0}, aAngular{0};	// linear acceleration and angular acceleration

	float friction{.95}; 			// to drag
};