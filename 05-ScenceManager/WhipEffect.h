#pragma once
#include "GameObject.h"

#define TIME_END	100

class CWhipEffect : public CGameObject
{
	DWORD timeDisplay;
public:
	bool isEnable;

	CWhipEffect();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

