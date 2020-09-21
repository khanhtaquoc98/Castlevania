#include "Map.h"

void CMap::Render()
{
	LPDIRECT3DTEXTURE9 Texture = CTextures::GetInstance()->Get(idTexture);
	CGame::GetInstance()->Draw(x, y, -1, Texture, left, top, right, bottom);
}
