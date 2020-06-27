#include "FireCol.h"
FireCol::FireCol()
{
	ani = 0;
	SetState(FIRECOL_STATE_LIVE);
}

void FireCol::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	/*left = x;
	top = y;
	right = x + FIRECOL_BBOX_WIDTH;

	if (state == FIRECOL_STATE_DIE)
		bottom = y + FIRECOL_BBOX_HEIGHT_DIE;
	else
		bottom = y + FIRECOL_BBOX_HEIGHT;*/
	left = x;
	top = y;
	right = x + FIRECOL_BBOX_WIDTH;
	bottom = y + FIRECOL_BBOX_HEIGHT;
}

void FireCol::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	/*x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}*/
}

void FireCol::Render()
{
	CGame *game = CGame::GetInstance();
	//ani = FIRECOL_ANI_LIVE;
	if (state == FIRECOL_STATE_DIE) {
		if (game->IsKeyDown(DIK_F)) {
			ani = FIRECOL_ANI_DIE;
			y =130;
		}
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void FireCol::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRECOL_STATE_DIE: {
		//ani = 1;
		CGame *game = CGame::GetInstance();
		if (game->IsKeyDown(DIK_F)) {


			//y += FIRECOL_BBOX_HEIGHT - FIRECOL_BBOX_HEIGHT_DIE + 1;
			y += 100;
		}
		/*vx = 0;
		vy = 0;*/
		break;
	}
	case FIRECOL_STATE_LIVE:
		vx = 0;
		/*vx = -FIRECOL_WALKING_SPEED;*/
	}

}
