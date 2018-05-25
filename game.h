//*****************************************************************************
//!	@file	game.h
//!	@brief	ゲーム処理
//!	@note	
//!	@author
//*****************************************************************************
#pragma once
#include "../Header/Player.h"		 // プレイヤー
#include "../Header/MoveBox.h"		 // ブロック
#include "../Header/HitBox.h"		 // 当たり判定用
#include "../Header/moon.h"			 // 月（ゴール）
#include "../Header/stage.h"		 // 
#include "../Header/CameraPos.h"
#include "../Header/input.h"

#define		SCREEN_X		1280
#define		SCREEN_Y		720
#define		STAGE_NUM		4

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);
void GameMain();
void GameExit();
void GameSetEvent();
void GameSetEndFlag();
void GameInput();
void GameUpdate(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos);
void GameRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos); //描画
//追加
//void AddInit();
void DebugFont(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, CameraPos* c_pos); //デバッグ
void KeyInput(PLAYER* player, CameraPos* c_pos, MOVEBOX* m_box);
void CameraAngle(PLAYER* player, CameraPos* c_pos); //カメラ


//Draw
void DrawCube(PLAYER* player, MOVEBOX* m_box, MOON* mn);
void DrawSkydome();
void Cobb_Draw(MOVEBOX* m_box);
void PlayerDraw();

//当たり判定
void HitFunction(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, CameraPos* c_pos);

//プレイヤーの状態
void PlayerStatus(PLAYER* player, CameraPos* c_pos, int g_status);
void AnimationSet(int g_NowAnimSet);

//回転
void DownTurn(PLAYER* player, CameraPos* c_pos);	//地面がなかった時のプレイヤーの回転
void HitTurn(PLAYER* player,CameraPos* c_pos,HITBOX* hbox);	//ブロックに当たった時のプレイヤーの回転
void PlayerTurn(PLAYER* player, CameraPos* c_pos, int g_status);	//プレイヤーの左右回転

//ステージのプレイヤーと月の配置
void InitMat(PLAYER* player, MOON* mn, CameraPos* c_pos,int StageNum);
void InitCameraMat(CameraPos* c_pos);
//カメラにプレイヤーの行列を入れ位置ズレを直す
void CameraMatCorrection(PLAYER* player, CameraPos* c_pos);

//カメラ
//パターン1
void CameraAngleNum_0(PLAYER* player, CameraPos* c_pos);
void CameraAngleNum_1(PLAYER* player, CameraPos* c_pos);
void CameraAngleNum_2(PLAYER* player, CameraPos* c_pos);
void CameraAngleNum_3(PLAYER* player, CameraPos* c_pos);
//パターン2			 
void CameraAngleNum_4(PLAYER* player, CameraPos* c_pos);
void CameraAngleNum_5(PLAYER* player, CameraPos* c_pos);

//HitBox
void SetPositionCase(PLAYER* player,HITBOX* hbox);
void SetPosition0(PLAYER* player,HITBOX* hbox);
void SetPosition1(PLAYER* player,HITBOX* hbox);
void SetPosition2(PLAYER* player,HITBOX* hbox);
void SetPosition3(PLAYER* player,HITBOX* hbox);
void SetPosition4(PLAYER* player,HITBOX* hbox);
void SetPosition5(PLAYER* player,HITBOX* hbox);
void HitDraw(HITBOX* hbox);

//キャラの向き
void Playerdirection(PLAYER* player, int g_CameraNum);

//遷移
void StartRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn); //描画
void EndRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn);
void selectRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos);

void ResetCamera(CameraPos* c_pos);

//位置の修正
void PositionCorrection(PLAYER* player);

// デバッグ用
void DebugCameraAngle(PLAYER* player, CameraPos* c_pos);
void test(PLAYER* player,CameraPos* c_pos);


// 2Dテクスチャ
void TextureDarw();
void SerectButton();

// パーティクルテクスチャ
void ParticleDraw();
//******************************************************************************
//	End of file.
//******************************************************************************
