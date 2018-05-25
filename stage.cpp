//#include "../Header/game.h"
#define _CRT_SECURE_NO_WARNINGS
#include "../Header/stage.h"

//----------------------------------------------------------------------------------------------
// 2スタートとリザルト
//----------------------------------------------------------------------------------------------
TITLE_NAME TName[2] = {
	{ "csv/menu/Cuve.csv" },
	{ "csv/menu/clear.csv" }
};

//----------------------------------------------------------------------------------------------
// ステージナンバー
//----------------------------------------------------------------------------------------------
STAGE_NAME SName[STAGE_MAX] = {
	{ "csv/map/stage1.csv" },
	{ "csv/map/stage2.csv" },
	{ "csv/map/stage3.csv" },
	{ "csv/map/stage4.csv" },
	{ "csv/map/stage5.csv" }
};

//----------------------------------------------------------------------------------------------
// ステージの状態
// C_START:ゲームスタート　 C_SELECT:ゲームセレクト　 C_GAME:ゲームメイン　 C_END:ゲームリザルト
//----------------------------------------------------------------------------------------------
enum{ C_START, C_SELECT, C_GAME, C_END };


//----------------------------------------------------------------------------------------------
// マップのロード
//----------------------------------------------------------------------------------------------
void MapLoad(MOVEBOX* m_box, int status, int s_num){

	FILE* fp;
	if (status == C_START){

		//ゲームスタート時
		fp = fopen(TName[0].name, "r");
		if (fp == NULL) {

			exit(1);
		}

		fscanf(fp, "%d", &m_box->BoxNum);	//ボックス個数の読み込み

		for (int i = 0; i < m_box->BoxNum; i++){

			fscanf(fp, "%f,%f,%f\n", &m_box->MatTotal[i]._41, &m_box->MatTotal[i]._42, &m_box->MatTotal[i]._43); //ボックス座標
		}
		fclose(fp);
	}
	else if ((status == C_GAME) || (status == C_SELECT)){

		//ステージセレクト　と　ゲームメイン
		fp = fopen(SName[s_num].name, "r");
		if (fp == NULL) {

			exit(1);
		}

		fscanf(fp, "%d", &m_box->BoxNum);	//ボックス個数の読み込み

		for (int i = 0; i < m_box->BoxNum; i++){

			fscanf(fp, "%f,%f,%f\n", &m_box->MatTotal[i]._41, &m_box->MatTotal[i]._42, &m_box->MatTotal[i]._43); //ボックス座標の読み込み
		}
		fclose(fp);
	}
	else if (status == C_END){

		// ゲームリザルト
		fp = fopen(TName[1].name, "r");
		if (fp == NULL) {

			exit(1);
		}

		fscanf(fp, "%d", &m_box->BoxNum);	//ボックス個数の読み込み

		for (int i = 0; i < m_box->BoxNum; i++){

			fscanf(fp, "%f,%f,%f\n", &m_box->MatTotal[i]._41, &m_box->MatTotal[i]._42, &m_box->MatTotal[i]._43); //ボックス座標の読み込み
		}
		fclose(fp);
	}
}
