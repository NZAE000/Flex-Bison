#include "calculation.hpp"
#include "point2d.hpp"
#include <stdexcept>
#include <cmath>

namespace CALC {

float suma(float n1, float n2){
    return n1 + n2;
}

float resta(float n1, float n2){
    return n1 - n2;
}

float multi(float n1, float n2){
    return n1 * n2;
}

float divi(float n1, float n2){
    if (!n2) throw std::runtime_error("division by zero");
    return n1 / n2;
}

std::tuple<float, float, float>
distanceToPoint(Point2D_t const& pOrigin, Point2D_t const& pTarget)
{
	float disxT     { pTarget.x - pOrigin.x };
	float disyT     { pTarget.y - pOrigin.y };
	float distanceT { std::sqrt(disxT*disxT + disyT*disyT) };

	return std::tuple(disxT, disyT, distanceT);
}

float
adjustBestAngle(float& angle)
{
	while (angle > PI)  angle -= 2*PI;
	while (angle < -PI) angle += 2*PI;
	return angle;
}

float
calculateAngle(float distX, float distY)
{
	float orien { std::atan2(distY, distX) };
	if (orien < 0) orien += 2*PI; // not negative radian
	return orien;
}

float
alignAngle(float originOrien, float targetOrien)
{
	float orienToTarget  { targetOrien - originOrien };

	// SEARCH BEST RUTE
	adjustBestAngle(orienToTarget);

	return orienToTarget;
}

std::tuple<float, float>
distanceXY(float distance, float orientation)
{
	float xDist { distance*std::cos(orientation) };
	float yDist { distance*std::sin(orientation) };

	return { std::round(xDist), std::round(yDist) };
}

float
degreesToRadian(float degrees)
{
	return (degrees*PI)/180;
}

float
radianToDegrees(float radian)
{
	return (radian*180)/PI;
}

void
clampAngle(float& angle)
{
	if      (angle > 2*PI) angle -= 2*PI;
	else if (angle < 0)    angle += 2*PI;
}

} // namespace CALC