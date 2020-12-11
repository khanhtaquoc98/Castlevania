#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Simon.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Whip.h"
#include "Map.h"


class CPlayScene: public CScene
{
protected: 
	CSimon *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;
	vector<LMap> tileMaps;

	int spaceHUD; //Chiều cao của bảng 

	int widthMap;
	int idTexture;
	int colTileImage;
	int rowTile = 0;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_INFO_MAP(string line);
	void _ParseSection_TILE_MAP(string line);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CSimon * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

