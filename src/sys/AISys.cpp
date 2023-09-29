#include "AISys.hpp"
#include <pointer.hpp>
#include <util/point2d.hpp>
#include <cmp/physicsCmp.hpp>
#include <cmp/AICmp.hpp>
#include <algorithm>
#include <util/calculation.hpp>


AISys_t::SteerTarget_t AISys_t::
arrive(PhysicsCmp_t const& phycmp, Point2D_t const& pointT, float arrivalTime, float arrivalRadius) const
{
// DISTANCE TARGET
	auto [disxT, disyT, distanceT] = CALC::distanceToPoint({phycmp.point.x, phycmp.point.y}, {pointT.x, pointT.y});
	
	if (distanceT <= arrivalRadius) return {}; // Check if AI is on target

// ANGULAR VELOCITY
	/*float orienTarget  { CALC::calculateAngle(disxT, disyT) };
	float vAngToTarget { CALC::alignAngle(phycmp.orientation, orienTarget) };

	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float vAngSpecificToTarget { vAngToTarget / arrivalTime };
	float angularSteer = std::clamp(vAngSpecificToTarget, -PhysicsCmp_t::MAX_VANGULAR, PhysicsCmp_t::MAX_VANGULAR);
	*/
	float angularSteer {0};

// ANGULAR ACCELERATION
	/*float aAngTspecific { (vAngSpecificToTarget - phycmp->vAngular) / aicmp.arrivalTime };
	aAngTspecific = std::clamp(aAngTspecific, -PhysicsCmp_t::MAX_AANGULAR, PhysicsCmp_t::MAX_AANGULAR);
	*/

// LINEAR VELOCITY
	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float vLinTSpecific { distanceT / arrivalTime };
	vLinTSpecific = std::clamp(vLinTSpecific, -PhysicsCmp_t::MAX_VLINEAR, PhysicsCmp_t::MAX_VLINEAR);

// LINEAR ACCELERATION
	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float aLinTSpecific { (vLinTSpecific - phycmp.vLinear) / arrivalTime  };
	float linearSteer   { std::clamp(aLinTSpecific, -PhysicsCmp_t::MAX_ALINEAR, PhysicsCmp_t::MAX_ALINEAR) };

	return { linearSteer, angularSteer }; // ONLY KINEMATIC ON THIS MOMENT!!
}


void AISys_t::update(Pointer_t& pointer) const
{
	auto& phycmp { pointer.phycmp };
	auto& aicmp  { pointer.aicmp  };

	//phycmp.aLinear = phycmp.vAngular = 0;
	SteerTarget_t steer = arrive(phycmp, aicmp.pointTarget, aicmp.arrivalTime, aicmp.arrivalRadius);

	phycmp.aLinear  = steer.linear;
	phycmp.vAngular = steer.angular;

	if (!steer.linear && !steer.angular) aicmp.targetActive = false;

}