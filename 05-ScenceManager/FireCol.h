#pragma once
#include "GameObject.h"

#define FIRECOL_BBOX_WIDTH  16
#define FIRECOL_BBOX_HEIGHT 16

class FireCol : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};