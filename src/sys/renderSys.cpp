#include "renderSys.hpp"
#include <pointer.hpp>
#include <cmath>


void RenderSys_t::update(const Pointer_t& pointer)
{
	auto const& phycmp { pointer.phycmp };
	if (!tracer) frameBuff.fill();

	frameBuff.drawFillRectangle(
		static_cast<uint32_t>(std::round(phycmp.point.x)), 
		static_cast<uint32_t>(std::round(phycmp.point.y)), 1, 1, RED);
	
	frameBuff.update();
}

bool RenderSys_t::inBounds(float x, float y) const
{
	if ((x < 0 || x >= static_cast<float>(frameBuff.width)) || (y < 0 || y >= static_cast<float>(frameBuff.height))) return false;
	
	return true;
}