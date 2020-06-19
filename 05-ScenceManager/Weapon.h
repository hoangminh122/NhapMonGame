#pragma once
#include "GameObject.h"
#include "Game.h"

#define WEAPON_FLY_SPEED 0.1f;

#define WEAPON_STATE_NONE	0
#define WEAPON_STATE_KNIFE	200
#define WEAPON_STATE_AXE	300
#define WEAPON_STATE_BOMERANG	400
#define WEAPON_STATE_FIRE	500

#define WEAPON_ANI_KNIFE_RIGHT	0
#define WEAPON_ANI_KNIFE_LEFT	1

#define WEAPON_ANI_AXE	2

#define WEAPON_ANI_BOMERANG	3

#define WEAPON_ANI_FIRE	4


class CWeapon : public CGameObject
{
	int ani;
	int state;
	int dame = 0;

	DWORD timeAttack;

public:
	bool isAttack = false;

	bool isEnable;

	CWeapon(int state);

	CWeapon(float x, float y, int nx, int state);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void SetPosition(float x, float y);
	void GetPosition(float &x, float &y);
	void SetDirection(int nx);
	int GetDirection();
	int GetCurrentFrame() { return animation_set->at(ani)->GetCurrentFrame(); }
	void SetCurrentFrame(int frame) { animation_set->at(ani)->SetCurrentFrame(frame); }

	int GetState() { return this->state; }
	virtual void SetState(int state);

	int GetDame() { return this->dame; }

};



#pragma once
