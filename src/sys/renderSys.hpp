#pragma once
#include <util/drawBuffer.hpp>
#include <cstdint>


struct Pointer_t; // Forward declaration

struct RenderSys_t {

	static constexpr uint32_t RED           { 0xFFFF0000 };
    static constexpr uint32_t BACKGRD_COLOR { 0x00000000 };//0x0017202A };

	explicit RenderSys_t(uint32_t w, uint32_t h) 
	: frameBuff{w, h}
	{
		frameBuff.fill(BACKGRD_COLOR);	// fill all frame with specific color
		frameBuff.update();
		frameBuff.capture();			// make copy frame
	}

	void update(const Pointer_t&);
	
	constexpr void setTracer(bool val) noexcept 
	{ 
		if (val == false) frameBuff.capture();
		tracer = val; 
	}

	bool inBounds(float x, float y) const;

private:
	FrameBuffer_t frameBuff;
	bool tracer { false };
};