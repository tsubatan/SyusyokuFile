#include "../Header/game.h"

void PositionCorrection(PLAYER* player){

	//�s��̒l��1���傫���Ȃ������A1�ɖ߂�
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


	//�s��̒l��-1��菬�����Ȃ������A1�ɖ߂�
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