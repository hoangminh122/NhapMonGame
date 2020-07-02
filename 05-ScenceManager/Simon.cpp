#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Simon.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CSimon::CSimon() : CGameObject()
{
	MARIO_BIG_BBOX_WIDTH_ATTACK = 60;
	level = SIMON_LEVEL_BIG;
	untouchable = 0;
	SetState(SIMON_STATE_IDLE);

	whip = new CWhip();
	usingWhip = false;
}
int CSimon::upBox = 0;
void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	// update whip
	whip->Update(dt, coObjects);
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		x = x;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//if (dynamic_cast<FireCol *>(e->obj)) // if e->obj is Goomba 
			//{
			//	FireCol *fireCol = dynamic_cast<FireCol *>(e->obj);
			//	//CMario *mario = dynamic_cast<CMario *>(e->obj);
			//	//CMario *mario = ((CPlayScene*)scence)->player;
			//	// jump on top >> kill Goomba and deflect a bit 
			//	if (e->nx < 0 || e->ny < 0)
			//	{
			//		/*	if (fireCol->GetState() != FIRECOL_STATE_DIE)
			//			{*/
			//			/*int b=CGameObject::GetState();
			//			int a=this->GetState();
			//			DebugOut(L"[INFO] Switching to scene %d", a);*/
			//			//this->SetState(MARIO_STATE_ATTACK);

			//		fireCol->SetState(FIRECOL_STATE_DIE);
			//		//vy = -MARIO_JUMP_DEFLECT_SPEED;
			//	//}
			//	}

			//} // if FireCol
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{

						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -SIMON_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > SIMON_LEVEL_SMALL)
							{
								level = SIMON_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(SIMON_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				DebugOut(L"[INFO] Switching to scene %d", p->GetSceneId());
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSimon::Render()
{
	ani = SIMON_ANI_IDLE_RIGHT;

	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else if (state == SIMON_STATE_SIT)
	{
		if (nx > 0)
		{
			if (isAttack)
				ani = SIMON_ANI_SIT_ATTACK_RIGHT;
			else
				ani = SIMON_ANI_SIT_RIGHT;
		}
		else
		{
			if (isAttack)
				ani = SIMON_ANI_SIT_ATTACK_LEFT;
			else
				ani = SIMON_ANI_SIT_LEFT;
		}

		isSit = false;
	}
	else if (state == SIMON_STATE_HURT)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_HURT_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_HURT_LEFT;
		}
	}
	else if (state == SIMON_STATE_JUMP)
	{
		if (isJump)
		{
			if (nx > 0)
			{
				if (isAttack)
					ani = SIMON_ANI_ATTACK_RIGHT;
				else
					ani = SIMON_ANI_JUMP_RIGHT;
			}
			else
			{
				if (isAttack)
					ani = SIMON_ANI_ATTACK_LEFT;
				else
					ani = SIMON_ANI_JUMP_LEFT;
			}
		}
		else return;
	}
	else if (state == SIMON_STATE_GO_UP && isGoUp == true)
	{
		if (nx > 0)
		{
			if (isAttack)
				ani = SIMON_ANI_ATTACK_UP_RIGHT;
			else
				ani = SIMON_ANI_GO_UP_RIGHT;
		}
		else
		{
			if (isAttack)
				ani = SIMON_ANI_ATTACK_UP_LEFT;
			else
				ani = SIMON_ANI_GO_UP_LEFT;
		}
	}
	else if (state == SIMON_STATE_GO_DOWN && isGoDown == true)
	{
		if (nx > 0)
		{
			if (isAttack)
				ani = SIMON_ANI_ATTACK_DOWN_RIGHT;
			else
				ani = SIMON_ANI_GO_DOWN_RIGHT;
		}
		else
		{
			if (isAttack)
				ani = SIMON_ANI_ATTACK_DOWN_LEFT;
			else
				ani = SIMON_ANI_GO_DOWN_LEFT;
		}
	}
	else
	{
		if (vx == 0)
		{
			if (isAttack)
			{
				if (nx > 0)
					ani = SIMON_ANI_ATTACK_RIGHT;
				else ani = SIMON_ANI_ATTACK_LEFT;
			}
			else if (isLevelUp)
			{
				if (nx > 0)
					ani = SIMON_ANI_LEVELUP_RIGHT;
				else ani = SIMON_ANI_LEVELUP_LEFT;
			}
			else if (isHurt)
			{
				if (nx > 0)
					ani = SIMON_ANI_HURT_RIGHT;
				else ani = SIMON_ANI_HURT_LEFT;
			}
			else
			{
				if (isGoUp == true && isGoDown == false)
				{
					if (nx > 0)
						ani = SIMON_ANI_IDLE_GO_UP_RIGHT;
					else ani = SIMON_ANI_IDLE_GO_UP_LEFT;
				}
				else if (isGoUp == false && isGoDown == true)
				{
					if (nx > 0)
						ani = SIMON_ANI_IDLE_GO_DOWN_RIGHT;
					else ani = SIMON_ANI_IDLE_GO_DOWN_LEFT;
				}
				else
				{
					if (nx > 0)
						ani = SIMON_ANI_IDLE_RIGHT;
					else ani = SIMON_ANI_IDLE_LEFT;
				}

			}

		}
		else if (vx > 0)
			ani = SIMON_ANI_WALKING_RIGHT;
		else ani = SIMON_ANI_WALKING_LEFT;
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(x, y, alpha);


	if (isAttack && usingWhip)
	{
		if (state == SIMON_STATE_SIT || state == SIMON_STATE_JUMP)
		{
			whip->SetDirection(nx);
			if (whip->GetDirection() > 0)
			{
				whip->SetPosition(x - 14, y + 5);
			}
			else
			{
				if (whip->GetLevel() > 1)
				{
					whip->SetPosition(x - 40, y + 5);
				}
				else
				{
					whip->SetPosition(x - 20, y + 5);
				}
			}
		}
		else
		{
			whip->SetDirection(nx);
			if (whip->GetDirection() > 0)
			{
				whip->SetPosition(x - 15, y + 2);
			}
			else
			{
				if (whip->GetLevel() > 1)
				{
					whip->SetPosition(x - 40, y + 2);
				}
				else
				{
					whip->SetPosition(x - 20, y + 2);
				}

			}
		}

		whip->Render();

		whip->isAttack = true;
	}

	RenderBoundingBox();
}

void CSimon::Reset()
{
	SetState(SIMON_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	/*this->blood = 16;*/

}
void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		this->isAttack = 0;
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		this->isAttack = 0;
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		this->isAttack = 0;
		vy = -SIMON_JUMP_SPEED_Y;
	case SIMON_STATE_IDLE:

		vx = 0;
		break;
	case SIMON_STATE_DIE:
		this->isAttack = 0;
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_ATTACK:
		isAttack = true;
		if (!usingWhip) {
			float simon_x = 0, simon_y = 0;
			GetPosition(simon_x, simon_y);
			//tao 1 doi tuong vu khi
			GetPosition(simon_x, simon_y);
			weapon = new CWeapon(simon_x, simon_y, GetDirection(), this->stateWeapon);
			weapon->isAttack = true;

		}
		vy = 0.05f;
		break;
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{

	left = x;
	top = y;

	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;

	if (state == SIMON_STATE_JUMP)
	{
		left = x;
		top = y + 7;

		right = x + SIMON_BBOX_WIDTH;
		bottom = y + 23;
	}
}

