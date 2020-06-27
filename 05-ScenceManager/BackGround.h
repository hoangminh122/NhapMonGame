#pragma once
#include "GameObject.h"

#define BACKGROUND_BBOX_WIDTH  23
#define BACKGROUND_BBOX_HEIGHT 24

class BackGround : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};