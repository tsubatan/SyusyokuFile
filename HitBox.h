#pragma once
#include <d3dx9.h>

#define HITBOXNUM 8
#define FLGNUM 300

class HITBOX{
public:

	D3DXMATRIX			MatTotal[HITBOXNUM];
	D3DXVECTOR3			angle[HITBOXNUM];
	D3DXVECTOR3			trans[HITBOXNUM];

	HITBOX();
	~HITBOX();

	//前後左右
	bool HitFlg[FLGNUM]; //当たり判定
	bool HitFlg2[FLGNUM];
	int Count;

	void InitFlg();
};