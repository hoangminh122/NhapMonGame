#pragma once
#include "GameObject.h"
#include "Game.h"
#define FIRECOL_WALKING_SPEED 0.05f;

#define FIRECOL_BBOX_WIDTH 16
#define FIRECOL_BBOX_HEIGHT 16
#define FIRECOL_BBOX_HEIGHT_DIE 9

#define FIRECOL_STATE_LIVE 101
#define FIRECOL_STATE_DIE 201

#define FIRECOL_ANI_LIVE 0
#define FIRECOL_ANI_DIE 1

class FireCol : public CGameObject
{
	int ani;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	FireCol();
	virtual void SetState(int state);
};