#include "../Header/Player.h"


PLAYER::PLAYER(){

	D3DXMATRIX MatTotal = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		2.0f, 15.0f, -4.0f, 1.0f
	};

	angle = { 0.0f, 0.0f, 0.0f };
	trans = { 0.0f, 0.0f, 0.0f };
}


PLAYER::~PLAYER(){

}