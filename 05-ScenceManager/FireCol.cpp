#include "FireCol.h"

void FireCol::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void FireCol::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + FIRECOL_BBOX_WIDTH;
	b = y + FIRECOL_BBOX_HEIGHT;
}