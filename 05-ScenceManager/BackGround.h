#pragma once
#include "GameObject.h"

#define BACKGROUND_BBOX_WIDTH  100
#define BACKGROUND_BBOX_HEIGHT 900

class BackGround : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};