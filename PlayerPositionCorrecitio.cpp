#include "../Header/game.h"

void PositionCorrection(PLAYER* player){

	//行列の値が1より大きくなった時、1に戻す
	if (player->MatTotal._11 > 1){

		player->MatTotal._11 = 1;
	}
	if (player->MatTotal._12 > 1){

		player->MatTotal._12 = 1;
	}
	if (player->MatTotal._13 > 1){

		player->MatTotal._13 = 1;
	}


	if (player->MatTotal._21 > 1){

		player->MatTotal._21 = 1;
	}
	if (player->MatTotal._22 > 1){

		player->MatTotal._22 = 1;
	}
	if (player->MatTotal._23 > 1){

		player->MatTotal._23 = 1;
	}


	if (player->MatTotal._31 > 1){

		player->MatTotal._31 = 1;
	}
	if (player->MatTotal._32 > 1){

		player->MatTotal._32 = 1;
	}
	if (player->MatTotal._33 > 1){

		player->MatTotal._33 = 1;
	}


	//行列の値が-1より小さくなった時、1に戻す
	if (player->MatTotal._11 < -1){

		player->MatTotal._11 = -1;
	}
	if (player->MatTotal._12 < -1){

		player->MatTotal._12 = -1;
	}
	if (player->MatTotal._13 < -1){

		player->MatTotal._13 = -1;
	}


	if (player->MatTotal._21 < -1){

		player->MatTotal._21 = -1;
	}
	if (player->MatTotal._22 < -1){

		player->MatTotal._22 = -1;
	}
	if (player->MatTotal._23 < -1){

		player->MatTotal._23 = -1;
	}


	if (player->MatTotal._31 < -1){

		player->MatTotal._31 = -1;
	}
	if (player->MatTotal._32< -1){

		player->MatTotal._32 = -1;
	}
	if (player->MatTotal._33 < -1){

		player->MatTotal._33 = -1;
	}
}