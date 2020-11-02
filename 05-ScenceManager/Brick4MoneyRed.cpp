#include "Brick4MoneyRed.h"


void CBrick4MoneyRed::Render()
{
	RenderBoundingBox();
}

void CBrick4MoneyRed::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
