#pragma once
#include "GameObject.h"
#include <d3dx9.h>
#include "Textures.h"
#include "Game.h"

using namespace std;

class CMap {
	int left, top, right, bottom, x, y, idTexture;
public:
	CMap(int x, int y, int idTexture, int l, int t, int r, int b) {
		this->x = x;
		this->y = y;
		this->idTexture = idTexture;
		this->left = l;
		this->top = t;
		this->right = r;
		this->bottom = b;
	}
	void Render();

}; typedef CMap* LMap;