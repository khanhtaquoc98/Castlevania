#include "StairBottom.h"


void CStairBottom::Render()
{
	RenderBoundingBox();
}

void CStairBottom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
