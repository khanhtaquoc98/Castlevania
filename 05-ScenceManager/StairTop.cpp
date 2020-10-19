#include "StairTop.h"


void CStairTop::Render()
{
	RenderBoundingBox();
}

void CStairTop::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
