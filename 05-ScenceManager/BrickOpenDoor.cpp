#include "BrickOpenDoor.h"


void CBrickOpenDoor::Render()
{
	RenderBoundingBox();
}

void CBrickOpenDoor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
