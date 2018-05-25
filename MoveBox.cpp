#include "../Header/MoveBox.h"


MOVEBOX::MOVEBOX(){

		for (int i = 0; i < M_BOX_NUM; i++){

			MatTotal[i] = {

				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 4.0f, 0.0f, 1.0f
			};

			angle[i] = { 0.0f, 0.0f, 0.0f };
			trans[i] = { 0.0f, 0.0f, 0.0f };

			up_flg[i] = false;
			down_flg[i] = false;
			right_flg[i] = false;
			left_flg[i] = false;
		}

		BoxNum = 0; //ボックスの総合数
}


MOVEBOX::~MOVEBOX(){

}

void MOVEBOX::Init(){

	for (int i = 0; i < M_BOX_NUM; i++){

		MatTotal[0] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 8.0f, 0.0f, 1.0f
		};
	}

	angle[0] = { 0.0f, 0.0f, 0.0f };
	trans[0]= { 0.0f, 0.0f, 0.0f };
}