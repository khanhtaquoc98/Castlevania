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
#include "ItemAxe.h"
#include "ItemBigHeart.h"
#include "ItemSmallHeart.h"
#include "ItemChain.h"
#include "ItemDagger.h"
#include "ItemHolyWater.h"
#include "ItemCross.h"
#include "ItemInvisibility.h"
#include "ItemDouble.h"
#include "Dagger.h"
#include "HolyWater.h"
#include "Axe.h"
#include "SubWeapons.h"
#include "Map.h"
#include "Leopard.h"
#include "Brick4Leopard.h"
#include "Brick4MoneyRed.h"
#include "ItemMoneyBagRed.h"
#include "StairBottom.h"
#include "StairTop.h"
#include "BrickHide.h"
#include "PointEffect.h"
#include "ItemMoneyBagPurple.h"
#include "ItemMoneyBagYellow.h"
#include "WallPieces.h"
#include "Board.h"
#include "Boss.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
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
#define OBJECT_ITEM_HOLY_WATER 16
#define OBJECT_ITEM_AXE 17
#define OBJECT_ITEM_CROSS 18
#define OBJECT_ITEM_INVISIBILITY 19
#define OBJECT_ITEM_DOUBLE 199
#define OBJECT_TYPE_WALL_PIECES		30

#define OBJECT_SUBWEAPON_DAGGER	41
#define OBJECT_SUBWEAPON_HOYLYWATER	72
#define OBJECT_SUBWEAPON_AXE	73

#define OBJECT_TYPE_PORTAL	50

#define OBJECT_TYPE_LEOPARD	11
#define OBJECT_TYPE_BRICK_4_LEOPARD	12

#define OBJECT_TYPE_STAIR_BOTTOM 20
#define OBJECT_TYPE_STAIR_TOP	21

#define OBJECT_TYPE_BRICK_4_MONEY_RED 80
#define OBJECT_TYPE_ITEM_MONEY_RED 81
#define OBJECT_TYPE_ITEM_MONEY_YELLOW 82
#define OBJECT_TYPE_ITEM_MONEY_PURPLE 83
#define OBJECT_TYPE_BRICK_HIDE	84
#define OBJECT_TYPE_POINT_EFFECT	99
#define OBJECT_TYPE_BOSS	51

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
		int frame_time = atoi(tokens[i + 1].c_str());
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

	CAnimations* animations = CAnimations::GetInstance();

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
	float y = atof(tokens[2].c_str()) + spaceHUD;

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		if (player != NULL)
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
		float r = atoi(tokens[4].c_str());
		float b = atoi(tokens[5].c_str()) + spaceHUD;
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
	case OBJECT_TYPE_WALL_PIECES:
	{
		obj = new CWallPiece();
		obj->SetVisible(false);
		CWallPieces::GetInstance()->AddPiece((CWallPiece*)obj);
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
	case OBJECT_ITEM_CROSS: {
		obj = new CItemCross();
		CItems::GetInstance()->AddItem(OBJECT_ITEM_CROSS, obj);
		obj->SetVisible(false);
		break;
	}
	case OBJECT_ITEM_INVISIBILITY: {
		obj = new CItemInvisibility();
		CItems::GetInstance()->AddItem(OBJECT_ITEM_INVISIBILITY, obj);
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
	case OBJECT_ITEM_AXE: {
		obj = new CItemAxe();
		CItems::GetInstance()->AddItem(OBJECT_ITEM_AXE, obj);
		obj->SetVisible(false);
		break;
	}
	case OBJECT_ITEM_HOLY_WATER:
	{
		obj = new CItemHolyWater();
		CItems::GetInstance()->AddItem(OBJECT_ITEM_HOLY_WATER, obj);
		obj->SetVisible(false);
		break;
	}
	case OBJECT_ITEM_DOUBLE: {
		obj = new CItemDouble();
		CItems::GetInstance()->AddItem(OBJECT_ITEM_DOUBLE, obj);
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
	case OBJECT_SUBWEAPON_HOYLYWATER:
	{
		obj = new CHolyWater();
		obj->SetVisible(false);
		CSubWeapons::GetInstance()->AddSubWeapon(OBJECT_SUBWEAPON_HOYLYWATER, obj);
		break;
	}
	case OBJECT_SUBWEAPON_AXE:
	{
		obj = new CAxe();
		obj->SetVisible(false);
		CSubWeapons::GetInstance()->AddSubWeapon(OBJECT_SUBWEAPON_AXE, obj);
		break;
	}
	case OBJECT_TYPE_LEOPARD:
	{
		obj = new CLeopard();
		obj->SetVisible(true);
		break;
	}
	case OBJECT_TYPE_BRICK_4_LEOPARD:
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		obj = new CBrick4Leopard();
		obj->SetWidth(width);
		obj->SetHeight(height);
		obj->SetVisible(true);
		break;
	}
	case OBJECT_TYPE_STAIR_BOTTOM:
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		int nx = atoi(tokens[6].c_str());
		float simonXGoUpStair = atoi(tokens[7].c_str());
		obj = new CStairBottom();
		obj->SetWidth(width);
		obj->SetHeight(height);
		obj->nx = nx;
		obj->SetVisible(true);
		obj->simonXStair = simonXGoUpStair;
		break;
	}
	case OBJECT_TYPE_STAIR_TOP:
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		int nx = atoi(tokens[6].c_str());
		float simonXDownStair = atoi(tokens[7].c_str());
		obj = new CStairTop();
		obj->SetWidth(width);
		obj->SetHeight(height);
		obj->nx = nx;
		obj->SetVisible(true);
		obj->simonXStair = simonXDownStair;
		break;
	}
	case OBJECT_TYPE_BRICK_4_MONEY_RED:
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		obj = new CBrick4MoneyRed();
		int IdItem = atoi(tokens[6].c_str());
		obj->SetItem(IdItem);
		obj->SetWidth(width);
		obj->SetHeight(height);
		obj->SetVisible(true);
		break;
	}
	case OBJECT_TYPE_ITEM_MONEY_RED:
	{
		obj = new CItemMoneyBagRed();
		int yCheck = atoi(tokens[4].c_str());
		CItems::GetInstance()->AddItem(OBJECT_TYPE_ITEM_MONEY_RED, obj);
		obj->SetVisible(false);
		obj->SetyCheck(yCheck);
		break;
	}
	case OBJECT_TYPE_ITEM_MONEY_PURPLE:
	{
		obj = new CItemMoneyBagPurple();
		CItems::GetInstance()->AddItem(OBJECT_TYPE_ITEM_MONEY_PURPLE, obj);
		obj->SetVisible(false);
		break;
	}
	case OBJECT_TYPE_ITEM_MONEY_YELLOW:
	{
		obj = new CItemMoneyBagYellow();
		CItems::GetInstance()->AddItem(OBJECT_TYPE_ITEM_MONEY_YELLOW, obj);
		obj->SetVisible(false);
		break;
	}
	case OBJECT_TYPE_POINT_EFFECT:
	{
		obj = new PointEffect();
		int typePoint = atoi(tokens[4].c_str());
		PointEffects::GetInstance()->AddPointEffect(typePoint, obj);
		obj->SetVisible(false);
		break;
	}
	case OBJECT_TYPE_BRICK_HIDE:
	{
		obj = new CBrickHide();
		int IdItem = atoi(tokens[4].c_str());
		obj->SetItem(IdItem);
		obj->SetVisible(true);
		break;
	}

	case OBJECT_TYPE_BOSS:
	{
		obj = new CBoss(player);
		obj->SetVisible(true);
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

void CPlayScene::_ParseSection_INFO_MAP(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return; // skip invalid lines 
	widthMap = atoi(tokens[0].c_str());
	colTileImage = atoi(tokens[1].c_str());
	idTexture = atoi(tokens[2].c_str());
	spaceHUD = atoi(tokens[3].c_str());
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
		CMap* map = new CMap(x, y + spaceHUD, idTexture, Tile.left, Tile.top, Tile.right, Tile.bottom);
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
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
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

	/*if (CGame::GetInstance()->GetScene() == 3) {
		player->SetState(SIMON_STATE_IDLE_DOWNSTAIR);
	}*/
	HUD = Board::GetInstance();
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

	CGame* game = CGame::GetInstance();
	if (cx < game->GetScreenWidth() / 2) {
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
	HUD->Update(dt);

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
}

void CPlayScene::Render()
{
	for (int i = 0; i < tileMaps.size(); i++)
		tileMaps[i]->Render();

	for (int i = 0; i < objects.size(); i++) {
		if (!dynamic_cast<CSimon*>(objects[i]) && objects[i]->isVisible() == true && !dynamic_cast<CBrickHide*>(objects[i]))
			objects[i]->Render();
	}
	for (int i = 0; i < objects.size(); i++) {
		if (dynamic_cast<CSimon*>(objects[i]))
			objects[i]->Render();
		if(dynamic_cast<CBrickHide*>(objects[i]) && objects[i]->visible)
			objects[i]->Render();
	}

	HUD->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<CSimon*>(objects[i]))
		delete objects[i];
	}

	CWallPieces::GetInstance()->Clear();
	CItems::GetInstance()->Clear();

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode) {
	CSimon* simon = ((CPlayScene*)scence)->GetPlayer();



}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CSimon* simon = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_1:
		CGame::GetInstance()->SwitchScene(1);
		break;
	case DIK_2:
		CGame::GetInstance()->SwitchScene(2);
		break;
	case DIK_3:
		CGame::GetInstance()->SwitchScene(3);
		break;
	case DIK_4:
		CGame::GetInstance()->SwitchScene(31);
		break;
	case DIK_5:
		CGame::GetInstance()->SwitchScene(32);
		break;
	case DIK_6:
		CGame::GetInstance()->SwitchScene(41);
		break;
	case DIK_7:
		CGame::GetInstance()->SwitchScene(42);
		break;
	case DIK_8:
		CGame::GetInstance()->SwitchScene(5);
		break;
	case DIK_A:
		simon->SetWeapon(SUBWEAPON_DAGGER);
		break;
	case DIK_S:
		simon->SetWeapon(SUBWEAPON_HOLYWATER);
		break;
	case DIK_D:
		simon->SetWeapon(SUBWEAPON_AXE);
		break;
	case DIK_Z:
		if (simon->IsOnStair()) {
			if (simon->GetState() == SIMON_STATE_IDLE_UPSTAIR || simon->GetState() == SIMON_STATE_GO_UPSTAIR)
			{
				simon->SetState(SIMON_STATE_ATTACK_UPSTAIR);
				simon->animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->Reset();
				simon->animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->SetAniStartTime(GetTickCount());
			}
			if (simon->GetState() == SIMON_STATE_IDLE_DOWNSTAIR || simon->GetState() == SIMON_STATE_GO_DOWNSTAIR)
			{
				simon->SetState(SIMON_STATE_ATTACK_DOWNSTAIR);
				simon->animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->Reset();
				simon->animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->SetAniStartTime(GetTickCount());
			}
		}
		else {
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
		}
		break;
	case DIK_X:
		//if (simon->vy != 0) return;
		if (simon->GetState() == SIMON_STATE_ATTACK || simon->GetState() == SIMON_STATE_ATTACK_SIT) return;
		if (simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_WALKING) {
			if (simon->IsOnGroud() == true)
				simon->SetState(SIMON_STATE_JUMP);
		}
		break;
	case DIK_UP:
		if (simon->IsOnStair() && simon->GetState() == SIMON_STATE_GO_UPSTAIR) {
			simon->animation_set->at(SIMON_ANI_GO_UPSTAIR)->SetAniStartTime(GetTickCount());
			simon->SetState(SIMON_STATE_GO_UPSTAIR);
		}
		else if (simon->IsOnStair() == false && simon->canGoUpStair) {
			if (simon->x != simon->simonGoStair) {
				simon->x > simon->simonGoStair ? simon->SetOrientation(-1) : simon->SetOrientation(1);
				simon->vx = 0.0001;
				simon->SetState(SIMON_STATE_WALKING);
			}
			//simon->x = simon->simonGoStair;
		}
		else if (simon->IsOnStair() && (simon->GetState() == SIMON_STATE_GO_DOWNSTAIR || simon->GetState() == SIMON_STATE_IDLE_DOWNSTAIR)) {
			simon->nxCanGoStair *= -1;
			simon->SetState(SIMON_STATE_IDLE_UPSTAIR);
		}
		break;
	case DIK_DOWN:
	{
		if (simon->IsOnStair()) {
			if (simon->GetState() == SIMON_STATE_GO_DOWNSTAIR) {
				simon->animation_set->at(SIMON_ANI_GO_DOWNSTAIR)->SetAniStartTime(GetTickCount());
				simon->SetState(SIMON_STATE_GO_DOWNSTAIR);
			}
			else if ((simon->GetState() == SIMON_STATE_GO_UPSTAIR || simon->GetState() == SIMON_STATE_IDLE_UPSTAIR)) {
				simon->nxCanGoStair *= -1;
				//simon->animation_set->at(SIMON_ANI_IDLE_DOWNSTAIR)->SetAniStartTime(GetTickCount());
				simon->SetState(SIMON_STATE_IDLE_DOWNSTAIR);
			}
		}
		else if (simon->IsOnStair() == false && simon->canGoDownStair) {
			if (simon->x != simon->simonGoStair) {
				simon->x > simon->simonGoStair ? simon->SetOrientation(-1) : simon->SetOrientation(1);
				simon->vx = 0.0001;
				simon->SetState(SIMON_STATE_WALKING);
			} else
			simon->SetState(SIMON_STATE_GO_DOWNSTAIR);
		}
		
		break;
	}
	case DIK_R:
		simon->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CSimon* simon = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (simon->GetState() == SIMON_STATE_JUMP && simon->vy != 0) return;
	if (simon->GetState() == SIMON_STATE_JUMP_ATTACK && simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_TIME_ATTACK) == false) return;
	if (simon->GetState() == SIMON_STATE_ATTACK && simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_TIME_ATTACK) == false) return;
	if (simon->GetState() == SIMON_STATE_ATTACK_SUBWEAPON && simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_TIME_ATTACK) == false) return;
	if (simon->GetState() == SIMON_STATE_ATTACK_SIT && simon->animation_set->at(SIMON_ANI_ATTACK_SIT)->IsOver(SIMON_TIME_ATTACK) == false) return;
	if (simon->GetState() == SIMON_STATE_CHANGE_COLOR && simon->animation_set->at(SIMON_ANI_CHANGE_COLOR)->IsOver(SIMON_TIME_CHANGE_COLOR) == false) return;
	if (simon->GetState() == SIMON_STATE_GO_UPSTAIR && simon->animation_set->at(SIMON_ANI_GO_UPSTAIR)->IsOver(200) == false) return;
	if (simon->GetState() == SIMON_STATE_GO_DOWNSTAIR && simon->animation_set->at(SIMON_ANI_GO_DOWNSTAIR)->IsOver(200) == false) return;
	if (simon->GetState() == SIMON_STATE_HURT_DEATH && simon->animation_set->at(SIMON_ANI_HURT_DEATH)->IsOver(300) == false) return;
	if (simon->GetState() == SIMON_STATE_ATTACK_DOWNSTAIR && simon->animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->IsOver(300) == false) return;
	if (simon->GetState() == SIMON_STATE_ATTACK_UPSTAIR && simon->animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->IsOver(300) == false) return;

	if (simon->IsOnStair() == false) {
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
		else if (game->IsKeyDown(DIK_DOWN) ) {
			if (simon->canGoDownStair == false && simon->canGoUpStair == false) {
				if (game->IsKeyDown(DIK_RIGHT)) { simon->SetOrientation(1); }
				else if (game->IsKeyDown(DIK_LEFT)) simon->SetOrientation(-1);
				simon->SetState(SIMON_STATE_SIT);
			}
			else {
				if (simon->canGoDownStair) {
					simon->SetOnStair(true);
					simon->x = simon->simonGoStair;
					simon->SetState(SIMON_STATE_GO_DOWNSTAIR);
				}
			}
		}
		else if (game->IsKeyDown(DIK_UP) && simon->canGoUpStair == true) {
			simon->x = simon->simonGoStair;
			simon->SetOnStair(true);
			simon->SetState(SIMON_STATE_GO_UPSTAIR);
		}
		else 
			simon->SetState(SIMON_STATE_IDLE);
	}
	else {
		simon->canGoDownStair = true;
		simon->canGoUpStair = true;
		if (game->IsKeyDown(DIK_UP) ) {
			if (simon->GetState() == SIMON_STATE_IDLE_DOWNSTAIR || simon->GetState() == SIMON_STATE_GO_DOWNSTAIR) {
				simon->nxCanGoStair *= -1;
			}
			simon->SetState(SIMON_STATE_GO_UPSTAIR);
		}
		else if (game->IsKeyDown(DIK_DOWN)) {
			if (simon->GetState() == SIMON_STATE_IDLE_UPSTAIR || simon->GetState() == SIMON_STATE_GO_UPSTAIR) {
				simon->nxCanGoStair *= -1;
			}
			simon->SetState(SIMON_STATE_GO_DOWNSTAIR);
		}
		else 
		{
			if (simon->GetState() == SIMON_STATE_GO_UPSTAIR || simon->GetState() == SIMON_STATE_ATTACK_UPSTAIR) {
				simon->SetState(SIMON_STATE_IDLE_UPSTAIR);
			}
			else if (simon->GetState() == SIMON_STATE_GO_DOWNSTAIR || simon->GetState() == SIMON_STATE_ATTACK_DOWNSTAIR) {
				simon->SetState(SIMON_STATE_IDLE_DOWNSTAIR);
			}
		}
	}
}