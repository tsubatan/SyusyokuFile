#include "../Header/game.h"
#include "../Header/Player.h"		 // プレイヤー

//----------------------------------------------------------------------------------------------
// キャラクターの向きの管理
//----------------------------------------------------------------------------------------------
void Playerdirection(PLAYER* player, int g_CameraNum){

	if (player->MatTotal._22 == 1){
		//パターン0

		if (player->MatTotal._11 == 1){

			//前
			g_CameraNum = 0;
		}
		else if (player->MatTotal._31 == 1){

			//右
			g_CameraNum = 1;
		}
		else if (player->MatTotal._11 == -1){

			//後ろ
			g_CameraNum = 2;
		}
		else if (player->MatTotal._31 == -1){

			//左
			g_CameraNum = 3;
		}
	}

	if (player->MatTotal._23 == -1){
		//パターン1

		if (player->MatTotal._11 == 1){

			//前
			g_CameraNum = 0;
		}
		else if (player->MatTotal._31 == 1){

			//右
			g_CameraNum = 1;
		}
		else if (player->MatTotal._11 == -1){

			//後ろ
			g_CameraNum = 2;
		}
		else if (player->MatTotal._31 == -1){

			//左
			g_CameraNum = 3;
		}
	}

	if (player->MatTotal._22 == -1){
		//パターン2

		if (player->MatTotal._11 == 1){

			//前
			g_CameraNum = 0;
		}
		else if (player->MatTotal._31 == 1){

			//右
			g_CameraNum = 1;
		}
		else if (player->MatTotal._11 == -1){

			//後ろ
			g_CameraNum = 2;
		}
		else if (player->MatTotal._31 == -1){

			//左
			g_CameraNum = 3;
		}
	}

	if (player->MatTotal._23 == 1){
		//パターン3

		if (player->MatTotal._11 == 1){

			//前
			g_CameraNum = 0;
		}
		else if (player->MatTotal._31 == 1){

			//右
			g_CameraNum = 1;
		}
		else if (player->MatTotal._11 == -1){

			//後ろ
			g_CameraNum = 2;
		}
		else if (player->MatTotal._31 == -1){

			//左
			g_CameraNum = 3;
		}
	}

	if (player->MatTotal._21 == 1){
		//パターン4

		if (player->MatTotal._33 == 1){

			//前
			g_CameraNum = 0;
		}
		else if (player->MatTotal._13 == -1){

			//右
			g_CameraNum = 1;
		}
		else if (player->MatTotal._33 == -1){

			//後ろ
			g_CameraNum = 2;
		}
		else if (player->MatTotal._13 == 1){

			//左
			g_CameraNum = 3;
		}
	}

	if (player->MatTotal._21 == -1){
		//パターン5

		if (player->MatTotal._33 == 1){

			//前
			g_CameraNum = 0;
		}
		else if (player->MatTotal._13 == -1){

			//右
			g_CameraNum = 1;
		}
		else if (player->MatTotal._33 == -1){

			//後ろ
			g_CameraNum = 2;
		}
		else if (player->MatTotal._13 == 1){

			//左
			g_CameraNum = 3;
		}
	}
}


//----------------------------------------------------------------------------------------------
// キャラの回転によるプレイヤー周りに配置している（プレイヤー当たり判定）の位置ズレの修正
//----------------------------------------------------------------------------------------------
void SetPositionCase(PLAYER* player, HITBOX* hbox){

	if (player->MatTotal._22 == 1){

		//パターン0
		SetPosition0(player, hbox);
	}

	if (player->MatTotal._23 == -1){

		//パターン1
		SetPosition1(player, hbox);
	}

	if (player->MatTotal._22 == -1){

		//パターン2
		SetPosition2(player, hbox);
	}

	if (player->MatTotal._23 == 1){

		//パターン3
		SetPosition3(player, hbox);
	}

	if (player->MatTotal._21 == 1){

		//パターン4
		SetPosition4(player, hbox);
	}

	if (player->MatTotal._21 == -1){

		//パターン5
		SetPosition5(player, hbox);
	}
}


//----------------------------------------------------------------------------------------------
// キャラの回転によるプレイヤー周りに配置している（プレイヤー当たり判定）の位置ズレの修正
// キャラクターの左右回転による位置ズレの修正
//----------------------------------------------------------------------------------------------
void SetPosition0(PLAYER* player, HITBOX* hbox){

	//前
	hbox->MatTotal[0]._41 = player->MatTotal._41;		//+:左　-:右
	hbox->MatTotal[0]._42 = player->MatTotal._42 + 1.0;	//+:上  -:下
	hbox->MatTotal[0]._43 = player->MatTotal._43 - 0.5;	//+:後  -:前

	//右
	hbox->MatTotal[1]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[1]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[1]._43 = player->MatTotal._43;

	//後ろ
	hbox->MatTotal[2]._41 = player->MatTotal._41;
	hbox->MatTotal[2]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[2]._43 = player->MatTotal._43 + 0.5;

	//左
	hbox->MatTotal[3]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[3]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[3]._43 = player->MatTotal._43;


	//hitbox2
	//前
	hbox->MatTotal[4]._41 = player->MatTotal._41;
	hbox->MatTotal[4]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[4]._43 = player->MatTotal._43 - 3.0;

	//右
	hbox->MatTotal[5]._41 = player->MatTotal._41 - 3.0;
	hbox->MatTotal[5]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[5]._43 = player->MatTotal._43;

	//後ろ
	hbox->MatTotal[6]._41 = player->MatTotal._41;
	hbox->MatTotal[6]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[6]._43 = player->MatTotal._43 + 3.0;

	//左
	hbox->MatTotal[7]._41 = player->MatTotal._41 + 3.0;
	hbox->MatTotal[7]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[7]._43 = player->MatTotal._43;

}


//----------------------------------------------------------------------------------------------
// キャラの回転によるプレイヤー周りに配置している（プレイヤー当たり判定）の位置ズレの修正
// キャラクターの左右回転による位置ズレの修正
//----------------------------------------------------------------------------------------------
void SetPosition1(PLAYER* player, HITBOX* hbox){

	//前
	hbox->MatTotal[0]._41 = player->MatTotal._41;
	hbox->MatTotal[0]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[0]._43 = player->MatTotal._43 - 1.0;

	//右
	hbox->MatTotal[1]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[1]._42 = player->MatTotal._42;
	hbox->MatTotal[1]._43 = player->MatTotal._43 - 1.0;

	//後ろ
	hbox->MatTotal[2]._41 = player->MatTotal._41;
	hbox->MatTotal[2]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[2]._43 = player->MatTotal._43 - 1.0;

	//左
	hbox->MatTotal[3]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[3]._42 = player->MatTotal._42;
	hbox->MatTotal[3]._43 = player->MatTotal._43 - 1.0;


	//hitbox2
	//前
	hbox->MatTotal[4]._41 = player->MatTotal._41;
	hbox->MatTotal[4]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[4]._43 = player->MatTotal._43 - 1.0;

	//右
	hbox->MatTotal[5]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[5]._42 = player->MatTotal._42;
	hbox->MatTotal[5]._43 = player->MatTotal._43 - 1.0;

	//後ろ
	hbox->MatTotal[6]._41 = player->MatTotal._41;
	hbox->MatTotal[6]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[6]._43 = player->MatTotal._43 - 1.0;

	//左
	hbox->MatTotal[7]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[7]._42 = player->MatTotal._42;
	hbox->MatTotal[7]._43 = player->MatTotal._43 - 1.0;
}


//----------------------------------------------------------------------------------------------
// キャラの回転によるプレイヤー周りに配置している（プレイヤー当たり判定）の位置ズレの修正
// キャラクターの左右回転による位置ズレの修正
//----------------------------------------------------------------------------------------------
void SetPosition2(PLAYER* player, HITBOX* hbox){

	//前
	hbox->MatTotal[0]._41 = player->MatTotal._41;
	hbox->MatTotal[0]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[0]._43 = player->MatTotal._43 + 0.5;

	//右
	hbox->MatTotal[1]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[1]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[1]._43 = player->MatTotal._43;

	//後ろ
	hbox->MatTotal[2]._41 = player->MatTotal._41;
	hbox->MatTotal[2]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[2]._43 = player->MatTotal._43 - 0.5;

	//左
	hbox->MatTotal[3]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[3]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[3]._43 = player->MatTotal._43;

	//hitbox2
	//前
	hbox->MatTotal[4]._41 = player->MatTotal._41;
	hbox->MatTotal[4]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[4]._43 = player->MatTotal._43 + 0.5;

	//右
	hbox->MatTotal[5]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[5]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[5]._43 = player->MatTotal._43;

	//後ろ
	hbox->MatTotal[6]._41 = player->MatTotal._41;
	hbox->MatTotal[6]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[6]._43 = player->MatTotal._43 - 0.5;

	//左
	hbox->MatTotal[7]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[7]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[7]._43 = player->MatTotal._43;
}


//----------------------------------------------------------------------------------------------
// キャラの回転によるプレイヤー周りに配置している（プレイヤー当たり判定）の位置ズレの修正
// キャラクターの左右回転による位置ズレの修正
//----------------------------------------------------------------------------------------------
void SetPosition3(PLAYER* player, HITBOX* hbox){

	//前
	hbox->MatTotal[0]._41 = player->MatTotal._41;
	hbox->MatTotal[0]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[0]._43 = player->MatTotal._43 + 1.0;

	//右
	hbox->MatTotal[1]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[1]._42 = player->MatTotal._42;
	hbox->MatTotal[1]._43 = player->MatTotal._43 + 1.0;

	//後ろ
	hbox->MatTotal[2]._41 = player->MatTotal._41;
	hbox->MatTotal[2]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[2]._43 = player->MatTotal._43 + 1.0;

	//左
	hbox->MatTotal[3]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[3]._42 = player->MatTotal._42;
	hbox->MatTotal[3]._43 = player->MatTotal._43 + 1.0;

	//hitbox2
	//前
	hbox->MatTotal[4]._41 = player->MatTotal._41;
	hbox->MatTotal[4]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[4]._43 = player->MatTotal._43 + 1.0;

	//右
	hbox->MatTotal[5]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[5]._42 = player->MatTotal._42;
	hbox->MatTotal[5]._43 = player->MatTotal._43 + 1.0;

	//後ろ
	hbox->MatTotal[6]._41 = player->MatTotal._41;
	hbox->MatTotal[6]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[6]._43 = player->MatTotal._43 + 1.0;

	//左
	hbox->MatTotal[7]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[7]._42 = player->MatTotal._42;
	hbox->MatTotal[7]._43 = player->MatTotal._43 + 1.0;
}


//----------------------------------------------------------------------------------------------
// キャラの回転によるプレイヤー周りに配置している（プレイヤー当たり判定）の位置ズレの修正
// キャラクターの左右回転による位置ズレの修正
//----------------------------------------------------------------------------------------------
void SetPosition4(PLAYER* player, HITBOX* hbox){

	//前
	hbox->MatTotal[0]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[0]._42 = player->MatTotal._42;
	hbox->MatTotal[0]._43 = player->MatTotal._43 - 0.5;

	//右
	hbox->MatTotal[1]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[1]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[1]._43 = player->MatTotal._43;

	//後ろ
	hbox->MatTotal[2]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[2]._42 = player->MatTotal._42;
	hbox->MatTotal[2]._43 = player->MatTotal._43 + 0.5;

	//左
	hbox->MatTotal[3]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[3]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[3]._43 = player->MatTotal._43;

	//hitbox2
	//前
	hbox->MatTotal[4]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[4]._42 = player->MatTotal._42;
	hbox->MatTotal[4]._43 = player->MatTotal._43 - 0.5;

	//右
	hbox->MatTotal[5]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[5]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[5]._43 = player->MatTotal._43;

	//後ろ
	hbox->MatTotal[6]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[6]._42 = player->MatTotal._42;
	hbox->MatTotal[6]._43 = player->MatTotal._43 + 0.5;

	//左
	hbox->MatTotal[7]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[7]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[7]._43 = player->MatTotal._43;
}


//----------------------------------------------------------------------------------------------
// キャラの回転によるプレイヤー周りに配置している（プレイヤー当たり判定）の位置ズレの修正
// キャラクターの左右回転による位置ズレの修正
//----------------------------------------------------------------------------------------------
void SetPosition5(PLAYER* player, HITBOX* hbox){

	//前
	hbox->MatTotal[0]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[0]._42 = player->MatTotal._42;
	hbox->MatTotal[0]._43 = player->MatTotal._43 - 0.5;

	//右
	hbox->MatTotal[1]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[1]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[1]._43 = player->MatTotal._43;


	//後ろ
	hbox->MatTotal[2]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[2]._42 = player->MatTotal._42;
	hbox->MatTotal[2]._43 = player->MatTotal._43 + 0.5;

	//左
	hbox->MatTotal[3]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[3]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[3]._43 = player->MatTotal._43;

	//hitbox2
	//前
	hbox->MatTotal[4]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[4]._42 = player->MatTotal._42;
	hbox->MatTotal[4]._43 = player->MatTotal._43 - 0.5;

	//右
	hbox->MatTotal[5]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[5]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[5]._43 = player->MatTotal._43;

	//後ろ
	hbox->MatTotal[6]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[6]._42 = player->MatTotal._42;
	hbox->MatTotal[6]._43 = player->MatTotal._43 + 0.5;

	//左
	hbox->MatTotal[7]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[7]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[7]._43 = player->MatTotal._43;
}
