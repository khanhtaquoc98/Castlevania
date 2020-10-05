#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Torch.h"
#include "Candle.h"
#include "Simon.h"
#include "Items.h"
#include "ItemBigHeart.h"
#include "ItemSmallHeart.h"
#include "ItemChain.h"
#include "ItemDagger.h"
#include "Dagger.h"
#include "SubWeapons.h"
#include "Map.h"

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
#define SCENE_SECTION_INFO_MAP		7
#define SCENE_SECTION_TILE_MAP		8

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_WHIP	4
#define OBJECT_TYPE_TORCH	5

#define OBJECT_ITEM_BIG_HEART		6
#define OBJECT_ITEM_CHAIN		7
#define OBJECT_ITEM_DAGGER		8
#define OBJECT_TYPE_CANDLE		9
#define OBJECT_ITEM_SMALL_HEART	10

#define OBJECT_SUBWEAPON_DAGGER	41

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = CSimon::GetInstance();
		player = (CSimon*)obj;
		obj->SetVisible(true);
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BRICK: 
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		obj = new CBrick(); 
		obj->SetWidth(width);
		obj->SetHeight(height);
		obj->SetVisible(true);
		break; 
	}
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
			obj->SetVisible(true);
		}
		break;
	case OBJECT_TYPE_TORCH:
		{
			obj = new CTorch();
			int IdItem = atoi(tokens[4].c_str());
			obj->SetItem(IdItem);
			obj->SetVisible(true);
			break;
		}
	case OBJECT_TYPE_CANDLE:
		{
			obj = new CCandle();
			int IdItem = atoi(tokens[4].c_str());
			obj->SetItem(IdItem);
			obj->SetVisible(true);
			break;
		}
	case OBJECT_ITEM_BIG_HEART:
		{
			obj = new CItemBigHeart();
			CItems::GetInstance()->AddItem(OBJECT_ITEM_BIG_HEART, obj);
			obj->SetVisible(false);
			break;
		}
	case OBJECT_ITEM_SMALL_HEART:
	{
		obj = new CItemSmallHeart();
		CItems::GetInstance()->AddItem(OBJECT_ITEM_SMALL_HEART, obj);
		obj->SetVisible(false);
		break;
	}
	case OBJECT_ITEM_CHAIN:
	{
		obj = new CItemChain();
		CItems::GetInstance()->AddItem(OBJECT_ITEM_CHAIN, obj);
		obj->SetVisible(false);
		break;
	}
	case OBJECT_ITEM_DAGGER:
	{
		obj = new CItemDagger();
		CItems::GetInstance()->AddItem(OBJECT_ITEM_DAGGER, obj);
		obj->SetVisible(false);
		break;
	}
	case OBJECT_SUBWEAPON_DAGGER:
	{	
		obj = new CDagger();
		obj->SetVisible(false);
		CSubWeapons::GetInstance()->AddSubWeapon(OBJECT_SUBWEAPON_DAGGER, obj);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::_ParseSection_INFO_MAP(string line){
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return; // skip invalid lines 
	widthMap = atoi(tokens[0].c_str());
	colTileImage = atoi(tokens[1].c_str());
	idTexture = atoi(tokens[2].c_str());
}

#define TILE_WIDTH 16;

void CPlayScene::_ParseSection_TILE_MAP(string line) {
	vector<string> tokens = split(line);
	
	for (int i = 0; i < tokens.size(); i++) {
		int index = atoi(tokens[i].c_str());
		RECT Tile;
		int x = i * TILE_WIDTH;
		int y = rowTile;
		Tile.left = index % colTileImage * 16;
		Tile.top = index / colTileImage * 16;
		Tile.right = Tile.left + 16;
		Tile.bottom = Tile.top + 16;
		CMap* map = new CMap(x, y, idTexture, Tile.left, Tile.top, Tile.right, Tile.bottom);
		tileMaps.push_back(map);
	}
	rowTile += 16;
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[INFO_MAP]") {
			section = SCENE_SECTION_INFO_MAP; continue;
		}
		if (line == "[TILE_MAP]") {
			section = SCENE_SECTION_TILE_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_INFO_MAP: _ParseSection_INFO_MAP(line); break;
			case SCENE_SECTION_TILE_MAP: _ParseSection_TILE_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isVisible()) {
			coObjects.push_back(objects[i]);
		}
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isVisible()) {
			objects[i]->Update(dt, &coObjects);
		}
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	if (cx < game->GetScreenWidth() /2 ) {
		cx = 0.0f;
	}
	else if (cx > this->widthMap - game->GetScreenWidth() / 2) {
		cx = this->widthMap - game->GetScreenWidth();
	}
	else
	{
		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;

	}

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
}

void CPlayScene::Render()
{
	for (int i = 0; i < tileMaps.size(); i++)
		tileMaps[i]->Render();
	for (int i = 0; i < objects.size(); i++) {
		if (!dynamic_cast<CSimon*>(objects[i]) && objects[i]->isVisible() == true) 
			objects[i]->Render();
	}
	for (int i = 0; i < objects.size(); i++) {
		if (dynamic_cast<CSimon*>(objects[i]))
			objects[i]->Render();
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{
		/*if (!dynamic_cast<CSimon*>(objects[i]))*/
		delete objects[i];
	}
		
	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CSimon *simon = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_Q:
		simon->SetWeapon(SUBWEAPON_DAGGER);
		break;
	case DIK_Z:
		if (simon->GetState() == SIMON_STATE_JUMP) {
			simon->SetState(SIMON_STATE_JUMP_ATTACK);
		} 
		else if (simon->GetState() == SIMON_STATE_IDLE) {
			if (CGame::GetInstance()->IsKeyDown(DIK_UP) && simon->GetWeapon() != 0) {
				simon->SetState(SIMON_STATE_ATTACK_SUBWEAPON);
			}
			else
			simon->SetState(SIMON_STATE_ATTACK);
		}
		else if (simon->GetState() == SIMON_STATE_SIT)
			simon->SetState(SIMON_STATE_ATTACK_SIT);
		break;
	case DIK_X:
		//if (simon->vy != 0) return;
		if (simon->GetState() == SIMON_STATE_ATTACK || simon->GetState() == SIMON_STATE_ATTACK_SIT) return;
		if (simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_WALKING) {
			if(simon->IsOnGroud() == true)
			simon->SetState(SIMON_STATE_JUMP);
		}
		break;
	case DIK_R: 
		simon->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CSimon *simon = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (simon->GetState() == SIMON_STATE_JUMP && simon->vy != 0) return;
	if (simon->GetState() == SIMON_STATE_JUMP_ATTACK && simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_TIME_ATTACK) == false) return;
	if (simon->GetState() == SIMON_STATE_ATTACK && simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_TIME_ATTACK) == false) return;
	if (simon->GetState() == SIMON_STATE_ATTACK_SUBWEAPON && simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_TIME_ATTACK) == false) return;
	if (simon->GetState() == SIMON_STATE_ATTACK_SIT && simon->animation_set->at(SIMON_ANI_ATTACK_SIT)->IsOver(SIMON_TIME_ATTACK) == false) return;
	if (simon->GetState() == SIMON_STATE_CHANGE_COLOR && simon->animation_set->at(SIMON_ANI_CHANGE_COLOR)->IsOver(SIMON_TIME_CHANGE_COLOR) == false) return;
	

	if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_Z))
	{
			simon->SetOrientation(1);
			simon->SetState(SIMON_STATE_WALKING);
	}
	else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_Z))
	{
		simon->SetOrientation(-1);
		simon->SetState(SIMON_STATE_WALKING);
	}
	else if (game->IsKeyDown(DIK_DOWN)) {
		if (game->IsKeyDown(DIK_RIGHT)) { simon->SetOrientation(1); }
		else if (game->IsKeyDown(DIK_LEFT)) simon->SetOrientation(-1); 
		simon->SetState(SIMON_STATE_SIT);
	}
	else
		simon->SetState(SIMON_STATE_IDLE);
}