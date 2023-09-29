#include "physicsSys.hpp"
#include <cmp/physicsCmp.hpp>
#include <algorithm>
#include <cmath>

void PhysicsSys_t::update(PhysicsCmp_t& phycmp, const float dt) const
{
	float newOrien { phycmp.orientation + phycmp.vAngular }; //*dt };

	// SI SE PASA DE 2PI O SI SE VA A UN ANGULO NEGATIVO, DAR LA VUELTA PARA QUEDAR EN EL RANGO DE 0 A 2PI
	if      (newOrien > 2*PhysicsCmp_t::PI)	 phycmp.orientation = newOrien - 2*PhysicsCmp_t::PI;
	else if (newOrien < 0)  				 phycmp.orientation = newOrien + 2*PhysicsCmp_t::PI;
	else									 phycmp.orientation = newOrien;

	// cos = catAd/hipo => catAd = hipo*cos
	phycmp.vx = phycmp.vLinear*std::cos(phycmp.orientation);
	phycmp.vy = phycmp.vLinear*std::sin(phycmp.orientation);

	// x = v*t
	phycmp.point.x += phycmp.vx*dt;
	phycmp.point.y += phycmp.vy*dt;

	// v = a*t 		//(acelerar solo en velocidad lineal, en velocidad angular mucho descontrol)
	phycmp.vLinear  += phycmp.aLinear*dt;
	phycmp.vLinear   = std::clamp(phycmp.vLinear,  -PhysicsCmp_t::MAX_VLINEAR,  PhysicsCmp_t::MAX_VLINEAR);
	//phycmp.vAngular += phycmp.aAngular*dt;
	//phycmp.vAngular  = std::clamp(phycmp.vAngular, -PhysicsCmp_t::MAX_VANGULAR, PhysicsCmp_t::MAX_VANGULAR);

	// Drag
	double drag { phycmp.vLinear*phycmp.friction };
	phycmp.vLinear -= drag*dt;
}