#include "Brick4Leopard.h"

void CBrick4Leopard::Render()
{
	//animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBrick4Leopard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}