#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_BACKGROUND 7
#define OBJECT_TYPE_FIRECOL 5

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);         //duong dan den file texture
	int R = atoi(tokens[2].c_str());         //gan rgb cho  texture
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());                                                       //tao id cho sprite 
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());													// chu y textID == id cua TEXTURES

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)																		//check null texture id
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);                                     //taoj sprite tu textid.,,,
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();                                               //day la 1 animation 1 anh giong sprite ?? chu y              

	int ani_id = atoi(tokens[0].c_str());                                               //id cuar animation
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time   
	{																			       // ??? tai sao lai dung for trong khi co the goi truc tiep
		int sprite_id = atoi(tokens[i].c_str());                                     //sprite duoc danh so thu tu
		int frame_time = atoi(tokens[i+1].c_str());                                  //thuong la = 100
		ani->Add(sprite_id, frame_time);                                             //luu animation theo id array vao CAnimation, vector<LPANIMATION_FRAME> frames;
	}

	CAnimations::GetInstance()->Add(ani_id, ani);									// unordered_map<int, LPANIMATION> animations; array nhieu animation
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());    

	LPANIMATION_SET s = new CAnimationSet();                  //nhieu animations chuyen hanh dong ngoi dung chay,....

	CAnimations *animations = CAnimations::GetInstance();        

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);      //them vao CanimationSets kem id de phan biet voi animationSets khac
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());										// dat id cho object
	float x = atof(tokens[1].c_str());												//????
	float y = atof(tokens[2].c_str());												//???

	int ani_set_id = atoi(tokens[3].c_str());										//GET Id cua animationSets

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();                //T

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before! ");
			return;
		}
		obj = new CSimon(); 
		player = (CSimon*)obj;  
		player->SetPosition(x, y);
		player->SetAnimationSet(animation_sets->Get(ani_set_id));
		break;
	//case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_FIRECOL: obj = new FireCol(); break;
	case OBJECT_TYPE_BACKGROUND: obj = new BackGround(); break;
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);                                                   //set vi tri suat hien dau tien cua object

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);                                           //set animation
	objects.push_back(obj);                                            
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);														//Mo file scene txt da constructor san 

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					                   //Khoi tao ban dau bang -1

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }       //SCENE_SECTION_TEXTURES=2 ??? tai sao bang 2
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }                               //SCENE_SECTION_SPRITES=3 ???
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }							   //SCENE_SECTION_ANIMATIONS=4
		if (line == "[ANIMATION_SETS]") {                                             
			section = SCENE_SECTION_ANIMATION_SETS; continue; }							//SCENE_SECTION_ANIMATION_SETS=5
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }							//SCENE_SECTION_OBJECTS=6
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;				//ve mau
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;                 // them sprite 
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;           // add cho sprite cho animation -> tao animations chua nhieu animation array
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;   //
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;                 //
		}
	}

	f.close();
	
	//textures->Add(ID_TEX_ENTRANCESTAGE, L"textures\\Background_entrance.png", D3DCOLOR_XRGB(36, 24, 140));
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	//CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\Background_entrance.png", D3DCOLOR_XRGB(36, 24, 140));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	if (player != NULL)
	{
		player->SetWhip(1);
	}

}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	cx += 120;


	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() /2;
	cy -= game->GetScreenHeight() /2;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);

	

}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()														//Xoa bo scene dang ton tai
{
	for (int i = 0; i < objects.size(); i++)                                    //Xoa tat ca doi tuong tren scene
		delete objects[i];

	objects.clear();
	player = NULL;                                                            //Doan nay chua hieu  ????
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CSimon *simon = ((CPlayScene*)scence)->player;
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_F:
		simon->SetState(SIMON_STATE_ATTACK);
		break;
	case DIK_Z:
		simon->SetState(SIMON_STATE_DEMO);
		break;
	case DIK_S:
		simon->usingWhip = true;
		simon->SetState(SIMON_STATE_ATTACK);
		break;
	case DIK_A: // reset
		simon->SetState(SIMON_STATE_IDLE);
		//simon->SetLevel(SIMON_LEVEL_BIG);
		simon->SetPosition(50.0f, 0.0f);
		simon->SetSpeed(0, 0);
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CSimon *simon = ((CPlayScene*)scence)->player;

	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	else
		simon->SetState(SIMON_STATE_IDLE);
}