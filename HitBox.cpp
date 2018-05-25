#include "../Header/HitBox.h"

HITBOX::HITBOX(){

	for (int i = 0; i < HITBOXNUM; i++){

		MatTotal[i] = {			// �ώZ�s��i�P�ʍs��ŏ������j

			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			10.0f, 10.0f, 15.0f, 1.0f
		};

		angle[i] = { 0.0f, 0.0f, 0.0f };
		trans[i] = { 0.0f, 0.0f, 0.0f };
	}

	Count = 0;
}


HITBOX::~HITBOX(){

}


void HITBOX::InitFlg(){

	for (int i = 0; i < FLGNUM; i++){

		HitFlg[i] = false;
		HitFlg2[i] = false;
	}
}