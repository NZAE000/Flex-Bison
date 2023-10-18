#include "drawBuffer.hpp"

uint32_t* FrameBuffer_t::getPosition(uint32_t x, uint32_t y) const noexcept 
{ 
	return frameBuff.get() + this->width*y + x; 
}

void FrameBuffer_t::fill() noexcept
{
    const auto* fbcpy = fBuffCopy.get();
    std::copy(fbcpy, fbcpy + (width*height), frameBuff.get());
}

void FrameBuffer_t::fill(uint32_t color) noexcept
{
    auto* buff = frameBuff.get();
    std::fill(buff, buff + (width*height), color);
}

void FrameBuffer_t::capture() noexcept
{
    const auto* buff = frameBuff.get();
    std::copy(buff, buff + (width*height), fBuffCopy.get());
}

void FrameBuffer_t::
drawSprite(uint32_t x, uint32_t y, uint32_t widthSpr, uint32_t heightSpr, const uint32_t* ptrToSprite, uint32_t pixels_off) noexcept
{
	auto* ptrToBuff = getPosition(x, y);

    while(heightSpr--) {
        for (uint32_t w=0; w<widthSpr; ++w)
        {
            // Draw only if transparency != 0 (not blending)
            if (*ptrToSprite & 0xFF000000) *ptrToBuff = *ptrToSprite;
            ++ptrToSprite; ++ptrToBuff;
        }
        ptrToSprite += pixels_off;
        ptrToBuff   += this->width - widthSpr;
    }
}

void FrameBuffer_t::
drawAlignedLine(uint32_t x, uint32_t y, uint32_t lengthLine, uint32_t displacement, uint32_t color) noexcept
{
    uint32_t* ptr_toScr = getPosition(x, y);
    while(lengthLine-- > 0) {
        *ptr_toScr = color;
        ptr_toScr += displacement;
    }
}


void FrameBuffer_t::
drawFillRectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) noexcept
{   
    uint32_t displacement {0};
    do {
        drawAlignedLine(x, y++, width, 1, color);
    } while (++displacement < height);
}
