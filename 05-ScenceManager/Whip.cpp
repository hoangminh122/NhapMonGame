#include "Whip.h"

CWhip::CWhip() : CGameObject()
{
	this->level = 1;

	this->dame = 1;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(49));
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CGame * game = CGame::GetInstance();
	if (isAttack)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<CTorch*>(coObjects->at(i))) {
				CTorch* torch = dynamic_cast<CTorch*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				torch->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					torch->isAttacked = true;
					torch->SetBlood(GetDame());
					isAttack = false;
				}
			}
		}
		/*for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<CTorch*>(coObjects->at(i))) {
				CTorch* torch = dynamic_cast<CTorch*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				torch->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					torch->isAttacked = true;
					torch->SetBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CCandle*>(coObjects->at(i))) {
				CCandle* candle = dynamic_cast<CCandle*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				candle->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					candle->isAttacked = true;
					candle->SetBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CArmy*>(coObjects->at(i))) {
				CArmy* army = dynamic_cast<CArmy*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				army->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2)==true)
				{
					army->isAttacked = true;
					army->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CZombie*>(coObjects->at(i))) {
				CZombie* zombie = dynamic_cast<CZombie*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				zombie->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					zombie->isAttacked = true;
					zombie->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CBat*>(coObjects->at(i))) {
				CBat* bat = dynamic_cast<CBat*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				bat->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					bat->isAttacked = true;
					bat->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CFlea*>(coObjects->at(i))) {
				CFlea* flea = dynamic_cast<CFlea*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				flea->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					flea->isAttacked = true;
					flea->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CBird*>(coObjects->at(i))) {
				CBird* bird = dynamic_cast<CBird*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				bird->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					bird->isAttacked = true;
					bird->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CToad*>(coObjects->at(i))) {
				CToad* toad = dynamic_cast<CToad*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				toad->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					toad->isAttacked = true;
					toad->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CSkeleton*>(coObjects->at(i))) {
				CSkeleton* skeleton = dynamic_cast<CSkeleton*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				skeleton->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					skeleton->isAttacked = true;
					skeleton->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CBoss*>(coObjects->at(i)))
			{
				CBoss* boss = dynamic_cast<CBoss*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				boss->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					boss->isAttacked = true;
					boss->LostBlood(GetDame());
					isAttack = false;
				}
			}
		}*/
	}
	
}

void CWhip::Render()
{
	if (this->level == 1)
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV1;
		}
		else
			ani = WHIP_ANI_LEFT_LV1;
	}
	/*if (this->level == 1)
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV1;
		}
		else
			ani = WHIP_ANI_LEFT_LV1;
	}
	else if (this->level == 2)
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV2;
		}
		else
			ani = WHIP_ANI_LEFT_LV2;
	}
	else if (this->level == 3)
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV3;
		}
		else
			ani = WHIP_ANI_LEFT_LV3;
	}
	else if (this->level == 4)
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV4;
		}
		else
			ani = WHIP_ANI_LEFT_LV4;
	}
	else
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV5;
		}
		else
			ani = WHIP_ANI_LEFT_LV5;
	}*/
	
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CWhip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (animation_set->at(ani)->GetCurrentFrame() == 2)
	{
		if (this->level == 1)
		{
			left = x;
			top = y;
			right = left + 54;
			bottom = top + 16;
		}
		else
		{
			left = x;
			top = y;
			right = left + 73;
			bottom = top + 16;
		}
	}
	
}

void CWhip::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CWhip::SetDirection(int nx)
{
	this->nx = nx;
}

int CWhip::GetDirection()
{
	return this->nx;
}
//
//void CWhip::LevelUp()
//{
//	if (this->level < 5)
//	{
//		this->level += 1;
//	}
//
//	(this->level > 2) ? this->dame = 2 : this->dame = this->level;
//	
//}
//
//void CWhip::SetLevel(int _level)
//{
//	/*this->level = _level;
//	(this->level > 2) ? this->dame = 2 : this->dame = this->level;*/
//}