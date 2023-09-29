#pragma once
#include <numbers>
#include <tuple>

struct Point2D_t;	// forward declaration

namespace CALC {

constexpr auto PI { std::numbers::pi_v<float> };

float suma(float, float);
float resta(float, float);
float multi(float, float);
float divi(float, float);

std::tuple<float, float, float>
distanceToPoint(Point2D_t const&, Point2D_t const&);

float
adjustBestAngle(float& angle);

float
calculateAngle(float distX, float distY);

float
alignAngle(float originOrien, float targetOrien);

std::tuple<float, float>
distanceXY(float distance, float orientation);

float
degreesToRadian(float degrees);

float
radianToDegrees(float radian);

void
clampAngle(float& angle);

} // namespace CALC