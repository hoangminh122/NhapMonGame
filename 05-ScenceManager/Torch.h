#pragma once
#include "GameObject.h"
#include "WhipEffect.h"
#include "DieEffect.h"
#include "Game.h"

#define STATE_TORCH	100
#define STATE_ITEM	200

#define ANI_ITEM_WHIP	0
#define ANI_ITEM_KNIFE	1
#define ANI_ITEM_AXE	2
#define ANI_ITEM_BOMERANG	3
#define ANI_ITEM_SMALL_HEART	4
#define ANI_ITEM_BIG_HEART	5
#define ANI_ITEM_FIRE	6

#define ANI_TORCH	0

#define TORCH_WIDTH	16
#define TORCH_HEIGHT	32
#define	ITEM_WIDTH	16
#define	ITEM_HEIGHT	16

class CTorch : public CGameObject
{
	

	int item;

	int blood;

	CWhipEffect * whipEffect;

	CDieEffect * dieEffect;
public:
	int ani;
	bool isEnable;
	bool isDisplay;
	bool isAttacked;

	CTorch(int _item);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void SetAni(int ani) { this->ani = ani; }
	int GetItem() { return this->item; }

	void SetBlood(int _blood) { this->blood -= _blood; }

	int GetBlood() { return this->blood; }
};

