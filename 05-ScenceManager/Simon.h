#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Weapon.h"

#define SIMON_WALKING_SPEED		0.15f 
//0.1f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_ATTACK			600
#define SIMON_STATE_DEMO			700
#define SIMON_STATE_DIE				400

#define SIMON_ANI_IDLE_RIGHT		0

#define SIMON_ANI_JUMP_RIGHT		4
#define SIMON_ANI_JUMP_LEFT			5

#define SIMON_ANI_WALKING_RIGHT		2
#define SIMON_ANI_WALKING_LEFT		3

#define SIMON_ANI_BIG_IDLE_RIGHT		0
#define SIMON_ANI_BIG_IDLE_LEFT			1
#define SIMON_ANI_SMALL_IDLE_RIGHT		2
#define SIMON_ANI_SMALL_IDLE_LEFT			3

#define SIMON_ANI_BIG_WALKING_RIGHT			4
#define SIMON_ANI_BIG_WALKING_LEFT			5
#define SIMON_ANI_SMALL_WALKING_RIGHT		6
#define SIMON_ANI_SMALL_WALKING_LEFT		7

#define SIMON_ANI_DIE				8
#define SIMON_ANI_ATTACK				9

#define	SIMON_LEVEL_SMALL	1
#define	SIMON_LEVEL_BIG		2

#define SIMON_BIG_BBOX_WIDTH  15             //15
#define SIMON_BIG_BBOX_HEIGHT 29            //27

#define SIMON_SMALL_BBOX_WIDTH  13
#define SIMON_SMALL_BBOX_HEIGHT 15       

#define SIMON_UNTOUCHABLE_TIME 5000

#define SIMON_BBOX_WIDTH			16
#define SIMON_BBOX_HEIGHT			30

class CSimon : public CGameObject
{
	float start_x;
	float start_y;

	int MARIO_BIG_BBOX_WIDTH_ATTACK;
	int level;
	int ani;
	int untouchable;
	DWORD untouchable_start;

	int stateWeapon;							//trang thai vu khi
	CWeapon * weapon;							//tao doi tuong vu khi

	CWhip *whip;

public:

	// check type attack
	bool usingWhip;

	static int upBox;
	static int isAttack;
	CSimon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void Reset();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void SetWhip(int _level) { whip->SetLevel(_level); }
	int GetDirection() { return this->nx; }    //get nx

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};