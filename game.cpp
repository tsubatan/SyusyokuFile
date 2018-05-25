//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	ゲーム処理
//!	@author	
//*****************************************************************************
#define _CRT_SECURE_NO_WARNIN
#define _CRT_SECURE_NO_WARNINGS
#define GRAVITY 0.5
#define baisoku 1
#define MOVESPED 0.1
#define PARTICLE_MAX 500	// パーティクルの最大数
#define PARTICLE_NUM 70	// パーティクルの使用数

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <thread>
#include <math.h>
#include <vector>
#include "../Header/CDirectXGraphics.h"
#include "../Header/CDirect3DXFile.h"
#include "../Header/game.h"
#include "../Header/mathutil.h"
#include "../Header/CBoundingSphere.h"
#include "../Header/CDebugFont.h"
#include "../Header/COBB.h"
#include "../Header/CSkinnedMesh.h"

using namespace std;
//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
CDirectXGraphics	*g_DXGrobj = NULL;		// DirectX Graphicsオブジェクト
CDirect3DXFile		*g_DXXFileObj = NULL, *g_DXXFileObj_skydome = NULL;	// Ｘファイルオブジェクト
CDirect3DXFile		*g_DXXFileObj_hit = NULL;		//ボックスの当たり判定
CDirect3DXFile		*g_DXXFileObj_player = NULL;	//プレイヤーの当たり判定
CDirect3DXFile		*g_DXXFileObj_moon = NULL;		//月

CSkinnedMesh		g_oneskin;

//CBoundingSphere		*cbs, *BSphereSkydome;
CDebugFont			d_font;

//COBB
//COBB	stage_obj[STAGENUM_Z][STAGENUM_Y][STAGENUM_X], m_box_obj[M_BOX_NUM]; //ステージ、動く箱
COBB	m_box_obj[M_BOX_NUM]; //ステージ、動く箱
COBB	player_obj;
COBB	g_HitBox_obj[HITBOXNUM];
COBB	moon_obj;

D3DXMATRIX			g_MatView;			// カメラ行列
D3DXMATRIX			g_Matbul;			// カメラ行列
D3DXMATRIX			g_MatProjection;	// プロジェクション変換行列
D3DXMATRIX			g_MatOrthor;		// 平行投影変換行列
D3DXMATRIX			g_MatWorld;			// ワールド変換行列

// スカイドーム
D3DXMATRIX			g_SkyMat = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

D3DXVECTOR3			g_Skyangle = { 0.0f, 0.0f, 0.0f };;
D3DXVECTOR3			g_Skytrans = { 0.0f, 0.0f, 0.0f };;

D3DXVECTOR3			g_CameraEye;
D3DXVECTOR3			g_CameraLookat;
D3DXVECTOR3			g_CameraUp;

HANDLE				g_hEventHandle;		// イベントハンドル
bool				g_EndFlag = false;	// 終了フラグ
std::thread			g_gamemainthread;	// ゲームメインスレッド


// add_start(2018/05/10)

//LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;
LPDIRECT3DTEXTURE9	select_tex;	//ステージセレクト
LPDIRECT3DTEXTURE9	star_tex;		//星
LPDIRECT3DTEXTURE9	star_tex_ora;	//星(オレンジ色)
LPDIRECT3DTEXTURE9	star_tex_whi;	//星(白色)
LPDIRECT3DTEXTURE9	serect_button;		// セレクトボタン
LPDIRECT3DTEXTURE9	serect_button_r;	// セレクトボタン（右）
LPDIRECT3DTEXTURE9	serect_button_l;	// セレクトボタン（左）

//テクスチャ構造体
struct VERTEX2D{
	float x, y, z;
	float rhw;
	D3DCOLOR diffuse;
	float tu, tv;
};
// add_end(2018/05/10)
//---------------------------------------------------------------
//グローバル変数
//---------------------------------------------------------------
enum{ STAND, MOVE_UP, MOVE_DOWN, MOVE_RIGHT, MOVE_LEFT };
enum{ START, SELECT, GAME, END };
//状態
int g_GAMESTATUS = START;  //ゲーム
int CameraStatus = START;  //カメラ

bool DebugFlg = false;
int cnt = 0;
float up_speed = 0.0f;
using namespace std;

int StageNum = 0;

//箱同士の当たり判定
int hit_count[M_BOX_NUM];
bool Hit_BoxFlg[M_BOX_NUM];
bool Clear_Obj[M_BOX_NUM];

//箱が動いる時の状態
bool up_move = false;
bool down_move = false;
bool right_move = false;
bool left_move = false;

//アニメーション関係
int	g_NowAnimSet = 0;
int g_status = STAND;
bool g_Aniflg = false;

//プレイヤー当たり判定
//プレイヤーとステージ
bool hit_Player_Stage = false;

//プレイヤーと箱
bool player_up_move = false;
bool player_down_move = false;
bool player_right_move = false;
bool player_left_move = false;

//回転設定
int g_TurnCount_Y = 0; //Y軸
int g_TurnCount_Z = 0; //Z軸
int TurnFlg = 0;
//落下設定
int g_GCount = 0;

//プレイヤーと地面の当たり判定
bool g_HitGroundFlg = false;

//カメラ状態
int g_CameraNum = 0;	//カメラの状態
int g_CameraUpDownCount = 0;	//カメラの上下移動


bool ChangeDFlg = false;
bool HitInitFlg = true;

//LoadMap
bool LoadFlg = true;

//回転しているときにボタンを押せなくする
bool StopFlg = false;
bool StopMove = false;

//clear
bool ClearFlg = false;

//ENDCamera
int end_cnt = 0;

// パーティクルステータス
float particle_x[PARTICLE_MAX];	// x座標
float particle_y[PARTICLE_MAX];	// y座標
float particle_z[PARTICLE_MAX];	// z座標
float particle_a[PARTICLE_MAX];	// 大きさ
float particle_speed[PARTICLE_MAX];	// 移動速度
float particle_vanish[PARTICLE_MAX]; // パーティクルを小さくする時の値
bool particle_reset_flg[PARTICLE_MAX];	//ture

// パーティクルカウント
int p_time=0;

//==============================================================================
//!	@fn		GameInit
//!	@brief	ゲーム初期処理
//!	@param	インスタンス値
//!	@param	ウインドウハンドル値
//!	@param	ウインドウサイズ幅（補正済みの値）
//!	@param	ウインドウサイズ高さ（補正済みの値）
//!	@param　	フルスクリーンフラグ　true ; フルスクリーン　false : ウインドウ
//!	@retval	true 成功　false 失敗
//==============================================================================
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	bool sts;
	srand((unsigned)time(NULL)); //ランド関数　パーティクルの座標に使用

	g_DXGrobj = new CDirectXGraphics();	// DirectX Graphicsオブジェクト生成
	sts = g_DXGrobj->Init(hwnd, fullscreen, width, height);	// DirectX Graphicsオブジェクト初期化

	if (!sts){
		MessageBox(hwnd, "ERROR!!", "DirectX 初期化エラー", MB_OK);
		return false;
	}

	//--------------------------------------------------------
	// Ｘファイルオブジェクト生成
	//--------------------------------------------------------
	//--------------------------------------------------------
	//スカイドーム
	//--------------------------------------------------------
	g_DXXFileObj_skydome = new CDirect3DXFile();
	sts = g_DXXFileObj_skydome->LoadXFile("assets/skydome.x", g_DXGrobj->GetDXDevice());

	//--------------------------------------------------------
	//stageブロック
	//--------------------------------------------------------
	g_DXXFileObj = new CDirect3DXFile();
	sts = g_DXXFileObj->LoadXFile("assets/cube.x", g_DXGrobj->GetDXDevice());

	//--------------------------------------------------------
	//hitboxブロック
	//--------------------------------------------------------
	g_DXXFileObj_hit = new CDirect3DXFile();
	sts = g_DXXFileObj_hit->LoadXFile("assets/hitbox.x", g_DXGrobj->GetDXDevice());

	//--------------------------------------------------------
	//プレイヤーの当たり判定
	//--------------------------------------------------------
	g_DXXFileObj_player = new CDirect3DXFile();
	sts = g_DXXFileObj_player->LoadXFile("assets/player_hit.x", g_DXGrobj->GetDXDevice());

	//--------------------------------------------------------
	//プレイヤーの当たり判定
	//--------------------------------------------------------
	g_DXXFileObj_moon = new CDirect3DXFile();
	sts = g_DXXFileObj_moon->LoadXFile("assets/moon.x", g_DXGrobj->GetDXDevice());

	//--------------------------------------------------------
	//キャラクター
	//--------------------------------------------------------

	sts = g_oneskin.LoadXFile("assets/girl/girl.x", g_DXGrobj->GetDXDevice());

	if (!sts){
		MessageBox(hwnd, "ERROR!!", "girl.x 読み込みエラー", MB_OK);
		return false;
	}
	// アニメーションセットを０にする
	g_oneskin.ChangeAnimation(g_NowAnimSet);
	//--------------------------------------------------------
	// Ｘファイルオブジェクト生成 END
	//--------------------------------------------------------


	if (!sts){
		MessageBox(hwnd, "ERROR!!", "cube3.x 読み込みエラー", MB_OK);
		MessageBox(hwnd, "ERROR!!", "skydome.x 読み込みエラー", MB_OK);
		return false;
	}

	// プロジェクション変換行列作成
	D3DXMatrixPerspectiveFovLH(&g_MatProjection,
		D3DX_PI / 2,					// 視野角
		(float)width / (float)height,	// アスペクト比
		1.0f,							// ニアプレーン
		5000.0f);						// ファープレーン

	D3DXMatrixOrthoLH(&g_MatOrthor,
		width,
		height,
		1.f,
		1000.f);

	// 射影変換行列を固定パイプラインへセット
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);
	// Ｚバッファ有効
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ライト有効
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_LIGHTING, true);
	// カリング無効化
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// 環境光セット
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	// テクスチャーセット
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/StageSelect.png", &select_tex);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/Star.png", &star_tex);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/Star_ora.png", &star_tex_ora);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/Star_whi.png", &star_tex_whi);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/select_button.png", &serect_button);
	//D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/test.png", &serect_button);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/select_button_r.png", &serect_button_r);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/select_button_l.png", &serect_button_l);

	// DirectT Input初期化
	InitInput(hinst, hwnd);

	//----------------------------------------------------------------
	//  グローバル変数初期化
	//----------------------------------------------------------------
	for (int i = 0; i < M_BOX_NUM; i++){
		Hit_BoxFlg[i] = false;
		hit_count[i] = 0;
		Clear_Obj[i] = false;
	}

	// パーティクルの座標の初期化
	for (int i = 0; i < PARTICLE_MAX; i++){

		particle_x[i] = 0.0f;
		particle_y[i] = 0.0f;
		particle_z[i] = 0.0f;
		particle_a[i] = 0.0f;
		particle_speed[i] = 0.0f;
		particle_vanish[i] = 0.0f;
		particle_reset_flg[i] = true;
	}
	//----------------------------------------------------------------
	// END
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// COBB初期化
	//----------------------------------------------------------------
	//ステージfor (int n = 0; n < STAGENUM_Y; n++){
	
	//動く箱
	for (int i = 0; i < M_BOX_NUM; i++){

		m_box_obj[i].Init(g_DXXFileObj->GetMesh(), g_DXGrobj->GetDXDevice());
	}

	//プレイヤー
	player_obj.Init(g_DXXFileObj_player->GetMesh(), g_DXGrobj->GetDXDevice());

	//hitboxCOBB
	for (int i = 0; i < HITBOXNUM; i++){

		g_HitBox_obj[i].Init(g_DXXFileObj_hit->GetMesh(), g_DXGrobj->GetDXDevice());
	}

	moon_obj.Init(g_DXXFileObj->GetMesh(), g_DXGrobj->GetDXDevice());
	//----------------------------------------------------------------
	//	END
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// Init [START]
	//----------------------------------------------------------------
	//デバッグフォント 初期化
	d_font.CreateFontA(g_DXGrobj->GetDXDevice());

	//----------------------------------------------------------------
	// Init [END]
	//----------------------------------------------------------------


	//ボーンのBBOX(初期化)
	g_oneskin.InitBBox(g_DXGrobj->GetDXDevice(), g_oneskin.GetFrameRoot());

	// イベントハンドル生成
	g_hEventHandle = CreateEvent(NULL, false, false, NULL);
	if (g_hEventHandle == NULL){
		MessageBox(hwnd, "ERROR!!", "CreateEvent エラー", MB_OK);
		return false;
	}
	// スレッド生成(ゲームメイン)
	g_gamemainthread = std::thread(GameMain);

	return	true;
}


//----------------------------------------------------------------------------------------------
// エンドカメラをリセット
//----------------------------------------------------------------------------------------------
void ResetCamera(CameraPos* c_pos){

	end_cnt = 0;
	c_pos->angle.y = 0;
	c_pos->Init();
}


//==============================================================================
//!	@fn		GameInput
//!	@brief	ゲーム入力検知処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameInput(){
	UpdateInput();				// 入力を検知する
}


//==============================================================================
//!	@fn		GameUpdate
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameUpdate(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos){

	Playerdirection(player,g_CameraNum); //方向設定

	if (GetKeyboardTrigger(DIK_F5)){
		if (DebugFlg) DebugFlg = false;
		else if (DebugFlg == false) DebugFlg = true;
	}

	//動く箱
	for (int i = 0; i < M_BOX_NUM; i++){

		MakeWorldMatrix(g_MatWorld, m_box->MatTotal[i], m_box->angle[i], m_box->trans[i]);
	}
	if (LoadFlg){

		MapLoad(m_box, g_GAMESTATUS, StageNum);
		LoadFlg = false;

	}

	//バウンディングボックス
	//動く箱
	for (int i = 0; i < M_BOX_NUM; i++){

		m_box_obj[i].Update(m_box->MatTotal[i]);
	}

	//キャラクター
	MakeWorldMatrix(g_MatWorld, player->MatTotal, player->angle, player->trans);
	g_oneskin.UpdateFrameMatrices(g_oneskin.GetFrameRoot(), &player->MatTotal);

	//プレイヤー
	player_obj.Update(player->MatTotal);

	//ボーンのBBOX
	g_oneskin.UpdateBBox(g_oneskin.GetFrameRoot());

	//HItBox
	for (int i = 0; i < HITBOXNUM; i++){

		MakeWorldMatrix(hbox->MatTotal[i], hbox->angle[i], hbox->trans[i]);
	}


	SetPositionCase(player,hbox); //hitboxの座標にプレイヤー座標を入れる
	for (int i = 0; i < HITBOXNUM; i++){

		g_HitBox_obj[i].Update(hbox->MatTotal[i]);
	}

	//月
	MakeWorldMatrix(g_MatWorld, mn->MatTotal, mn->angle, mn->trans);
	moon_obj.Update(mn->MatTotal);

	if (g_GAMESTATUS == GAME){

		KeyInput(player, c_pos, m_box); //キー入力
	}

	//カメラ
	MakeWorldMatrix(g_MatWorld, c_pos->MatTotal, c_pos->angle, c_pos->trans); //カメラ座標
	CameraAngle(player,c_pos); //カメラアングル

	//スカイドーム
	MakeWorldMatrix(g_MatWorld, g_SkyMat, g_Skyangle, g_Skytrans);
}


//----------------------------------------------------------------------------------------------
// スタート画面
//----------------------------------------------------------------------------------------------
void StartRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn){
	HRESULT  hr;

	// ターゲットバッファのクリア、Ｚバッファのクリア
	g_DXGrobj->GetDXDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	// 描画の開始をＤＩＲＥＣＴＸに通知
	g_DXGrobj->GetDXDevice()->BeginScene();
	//---↓描画処理↓---

	//==================================================
	//↓↓↓↓↓↓↓↓↓↓ 3D描画 ↓↓↓↓↓↓↓↓↓↓↓
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);

	DrawSkydome();
	DrawCube(player,m_box, mn);

	ParticleDraw();

	//==================================================
	//↓↓↓↓↓↓↓↓↓↓ 2D描画 ↓↓↓↓↓↓↓↓↓↓↓
	//==================================================
	//g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatOrthor);

	

	//---↑描画処理↑---
	g_DXGrobj->GetDXDevice()->EndScene();	// 描画の終了を待つ
	hr = g_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// バックバッファからプライマリバッファへ転送
	if (hr != D3D_OK){
		g_DXGrobj->GetDXDevice()->Reset(&g_DXGrobj->GetDXD3dpp());
	}
}


//==============================================================================
//!	@fn		GameRender
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos){
	HRESULT  hr;

	// ターゲットバッファのクリア、Ｚバッファのクリア
	g_DXGrobj->GetDXDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	// 描画の開始をＤＩＲＥＣＴＸに通知
	g_DXGrobj->GetDXDevice()->BeginScene();

	//==================================================
	//↓↓↓↓↓↓↓↓↓↓ 3D描画 ↓↓↓↓↓↓↓↓↓↓↓
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);

	//スカイドーム描画
	DrawSkydome();
	//ステージ、動く箱描画
	DrawCube(player,m_box, mn);
	//キャラ描画
	PlayerDraw();
	//HitBox
	//HitDraw(hbox);

	//バウンディングスフィア描画
	if (DebugFlg){

		DebugFont(player, m_box, hbox,c_pos);
		//Cobb_Draw(m_box);

	}
	// パーティクル描画
	ParticleDraw();

	//デバッグフォント
	//DebugFont(stagebox, m_box,hbox);

	//==================================================
	//↓↓↓↓↓↓↓↓↓↓ 2D描画 ↓↓↓↓↓↓↓↓↓↓↓
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatOrthor);

	TextureDarw();

	g_DXGrobj->GetDXDevice()->EndScene();	// 描画の終了を待つ
	hr = g_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// バックバッファからプライマリバッファへ転送
	if (hr != D3D_OK){
		g_DXGrobj->GetDXDevice()->Reset(&g_DXGrobj->GetDXD3dpp());
	}

	HitFunction(player,m_box, hbox,c_pos);
}


//----------------------------------------------------------------------------------------------
// リザルト画面
//----------------------------------------------------------------------------------------------
void EndRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn){

	HRESULT  hr;
	// ターゲットバッファのクリア、Ｚバッファのクリア
	g_DXGrobj->GetDXDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	// 描画の開始をＤＩＲＥＣＴＸに通知
	g_DXGrobj->GetDXDevice()->BeginScene();
	//---↓描画処理↓---

	//==================================================
	//↓↓↓↓↓↓↓↓↓↓ 3D描画 ↓↓↓↓↓↓↓↓↓↓↓
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);

	DrawSkydome();
	DrawCube(player, m_box, mn);
	// パーティクル描画
	ParticleDraw();

	//---↑描画処理↑---
	g_DXGrobj->GetDXDevice()->EndScene();	// 描画の終了を待つ
	hr = g_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// バックバッファからプライマリバッファへ転送
	if (hr != D3D_OK){
		g_DXGrobj->GetDXDevice()->Reset(&g_DXGrobj->GetDXD3dpp());
	}
}

//----------------------------------------------------------------------------------------------
// セレクト画面
//----------------------------------------------------------------------------------------------
void selectRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos){

	HRESULT  hr;
	// ターゲットバッファのクリア、Ｚバッファのクリア
	g_DXGrobj->GetDXDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	// 描画の開始をＤＩＲＥＣＴＸに通知
	g_DXGrobj->GetDXDevice()->BeginScene();
	//---↓描画処理↓---
	//==================================================
	//↓↓↓↓↓↓↓↓↓↓ 3D描画 ↓↓↓↓↓↓↓↓↓↓↓
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);
	DrawSkydome();
	DrawCube(player, m_box, mn);
	// パーティクル描画
	SerectButton();
	ParticleDraw();

	//==================================================
	//↓↓↓↓↓↓↓↓↓↓ 2D描画 ↓↓↓↓↓↓↓↓↓↓↓
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatOrthor);
	TextureDarw();
	//DebugFont(player, m_box, hbox, c_pos);
	//---↑描画処理↑---
	g_DXGrobj->GetDXDevice()->EndScene();	// 描画の終了を待つ
	hr = g_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// バックバッファからプライマリバッファへ転送
	if (hr != D3D_OK){
		g_DXGrobj->GetDXDevice()->Reset(&g_DXGrobj->GetDXD3dpp());
	}
}


//==============================================================================
//!	@fn		GameMain
//!	@brief	ゲームループ処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameMain()
{
	PLAYER* player = new PLAYER;
	MOVEBOX*  m_box = new MOVEBOX;	   //動く箱
	HITBOX* hbox = new HITBOX;		   //hitbox
	MOON* mn = new MOON;
	CameraPos* c_pos = new CameraPos; // カメラの座標

	while (1){
		DWORD sts = WaitForSingleObject(g_hEventHandle, 1000);	// イベントフラグがセットされるのを待つ（1000msでタイムアウト）
		if (sts == WAIT_FAILED){

			break;
		}
		else if (sts == WAIT_TIMEOUT){
			if (g_EndFlag){

				break;
			}
			continue;
		}

		GameInput();		// 入力
		GameUpdate(player, m_box, hbox, mn,c_pos);	// 更新

		switch (g_GAMESTATUS){	// 描画
		case START:
			StartRender(player,m_box, hbox, mn);
			
			if (GetKeyboardTrigger(DIK_RETURN)){
				LoadFlg = true;
				g_GAMESTATUS = SELECT;
				CameraStatus = SELECT;
				StageNum = 0;
			}
			ResetCamera(c_pos);
			break;

		case SELECT:
			selectRender(player,m_box, hbox, mn,c_pos);
			if (GetKeyboardTrigger(DIK_RIGHT)){

				LoadFlg = true;
				StageNum++;
				if (StageNum > STAGE_NUM){
					StageNum = 0;
				}
			}
			else if (GetKeyboardTrigger(DIK_LEFT)){

				LoadFlg = true;
				StageNum--;
				if (StageNum < 0){
					StageNum = STAGE_NUM;
				}
			}
			else if (GetKeyboardTrigger(DIK_RETURN)){

				LoadFlg = true;
				g_GAMESTATUS = GAME;
				CameraStatus = GAME;
				InitMat(player,mn,c_pos,StageNum);
			}
			break;

		case GAME:
			GameRender(player,m_box, hbox, mn,c_pos);
			if (ClearFlg){
				LoadFlg = true;
				g_GAMESTATUS = END;
				CameraStatus = END;
				ResetCamera(c_pos);
			}

			//デバッグ用
			/*if (GetKeyboardTrigger(DIK_RETURN)){

				LoadFlg = true;
				g_GAMESTATUS = END;
				CameraStatus = END;
				ResetCamera(c_pos);
			}*/
			break;

		case END:
			EndRender(player,m_box, hbox, mn);
			if (GetKeyboardTrigger(DIK_RETURN)){
				ClearFlg = false;
				LoadFlg = true;
				g_GAMESTATUS = START;
				CameraStatus = START;
			}
			break;
		}
	}
}


//==============================================================================
//!	@fn		GameExit
//!	@brief	ゲーム終了処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameExit()
{
	g_gamemainthread.join();					// ゲームメインスレッドの終了を待つ
	CloseHandle(g_hEventHandle);				// イベントハンドルクローズ

	if (g_DXXFileObj != NULL){

		delete g_DXXFileObj;					//	Xファイルオブジェクトの解放
	}

	if (g_DXXFileObj_skydome != NULL){

		delete g_DXXFileObj_skydome;					//	Xファイルオブジェクトの解放
	}

	if (g_DXXFileObj_hit != NULL){

		delete g_DXXFileObj_hit;					//	Xファイルオブジェクトの解放
	}

	if (g_DXGrobj != NULL){

		g_DXGrobj->Exit();
		// DirectXのグラフィックオブジェクトの終了処理
	}

	if (g_DXGrobj != NULL){						// DirectXのグラフィックオブジェクトの解放

		delete g_DXGrobj;
		g_DXGrobj = NULL;
	}

	if (g_DXXFileObj_moon != NULL){

		delete g_DXXFileObj_moon;					//	Xファイルオブジェクトの解放
	}

	// DirectT Input終了処理
	UninitInput();
}

//==============================================================================
//!	@fn		GameSetEvent
//!	@brief	ゲームループ内のイベントオブジェクトをセットする
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameSetEvent(){
	if (!g_EndFlag){

		SetEvent(g_hEventHandle);	// 終了フラグがセットされていなければイベントをセットする
	}
}

//==============================================================================
//!	@fn		GameSetEndFlag
//!	@brief	ゲームループの終了フラグをセットする
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameSetEndFlag(){

	g_EndFlag = true;				// 終了フラグをオンにする
}

//******************************************************************************
//	End of file.
//******************************************************************************


//----------------------------------------------------------------------------------------------
// デバッグフォント
//----------------------------------------------------------------------------------------------
void DebugFont(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, CameraPos* c_pos){

	char str[256];

	if (g_GAMESTATUS == SELECT){

		sprintf(str, "[カメラ座標]");
		d_font.DarwText(100, 100, str);
		sprintf(str, "Mat._11: %.2f",c_pos->MatTotal._11);
		d_font.DarwText(100, 120, str);
		sprintf(str, "Mat._21: %.2f", c_pos->MatTotal._21);
		d_font.DarwText(100, 140, str);
		sprintf(str, "Mat._31: %.2f", c_pos->MatTotal._31);
		d_font.DarwText(100, 160, str); 
		sprintf(str, "Mat._41: %.2f", c_pos->MatTotal._41);
		d_font.DarwText(100, 180, str);

		sprintf(str, "Mat._12: %.2f", c_pos->MatTotal._12);
		d_font.DarwText(250, 120, str);
		sprintf(str, "Mat._22: %.2f", c_pos->MatTotal._22);
		d_font.DarwText(250, 140, str);
		sprintf(str, "Mat._32: %.2f", c_pos->MatTotal._32);
		d_font.DarwText(250, 160, str);
		sprintf(str, "Mat._42: %.2f", c_pos->MatTotal._42);
		d_font.DarwText(250, 180, str);

		sprintf(str, "Mat._13: %.2f", c_pos->MatTotal._13);
		d_font.DarwText(400, 120, str);
		sprintf(str, "Mat._23: %.2f", c_pos->MatTotal._23);
		d_font.DarwText(400, 140, str);
		sprintf(str, "Mat._33: %.2f", c_pos->MatTotal._33);
		d_font.DarwText(400, 160, str);
		sprintf(str, "Mat._43: %.2f", c_pos->MatTotal._43);
		d_font.DarwText(400, 180, str);

		sprintf(str, "Mat._14: %.2f", c_pos->MatTotal._14);
		d_font.DarwText(550, 120, str);
		sprintf(str, "Mat._24: %.2f", c_pos->MatTotal._24);
		d_font.DarwText(550, 140, str);
		sprintf(str, "Mat._34: %.2f", c_pos->MatTotal._34);
		d_font.DarwText(550, 160, str);
		sprintf(str, "Mat._44: %.2f", c_pos->MatTotal._44);
		d_font.DarwText(550, 180, str);
	}
	else if (g_GAMESTATUS == GAME){

		//今向いてる方向
		switch (g_CameraNum){
		case 0:
			sprintf(str, "前 (0)");
			break;

		case 1:
			sprintf(str, "右 (1)");
			break;

		case 2:
			sprintf(str, "後 (2)");
			break;

		case 3:
			sprintf(str, "左 (3)");
			break;
		}
		d_font.DarwText(20, 20, str);

		sprintf(str, "MoveBox:%d", m_box->BoxNum);
		d_font.DarwText(20, 40, str);


		if (GetKeyboardTrigger(DIK_F6)){

			if (ChangeDFlg){

				ChangeDFlg = false;
			}
			else{
				ChangeDFlg = true;
			}
		}

		if (ChangeDFlg){

			// プレイヤー情報
			sprintf(str, "%f", player->MatTotal._11);
			d_font.DarwText(20, 60, str);
			sprintf(str, "%f", player->MatTotal._21);
			d_font.DarwText(20, 80, str);
			sprintf(str, "%f", player->MatTotal._31);
			d_font.DarwText(20, 100, str);
			sprintf(str, "%f", player->MatTotal._41);
			d_font.DarwText(20, 120, str);

			sprintf(str, "%f", player->MatTotal._12);
			d_font.DarwText(130, 60, str);
			sprintf(str, "%f", player->MatTotal._22);
			d_font.DarwText(130, 80, str);
			sprintf(str, "%f", player->MatTotal._32);
			d_font.DarwText(130, 100, str);
			sprintf(str, "%f", player->MatTotal._42);
			d_font.DarwText(130, 120, str);

			sprintf(str, "%f", player->MatTotal._13);
			d_font.DarwText(240, 60, str);
			sprintf(str, "%f", player->MatTotal._23);
			d_font.DarwText(240, 80, str);
			sprintf(str, "%f", player->MatTotal._33);
			d_font.DarwText(240, 100, str);
			sprintf(str, "%f", player->MatTotal._43);
			d_font.DarwText(240, 120, str);


			// カメラ情報
			sprintf(str, "%f", player->MatTotal._11);
			d_font.DarwText(20, 160, str);
			sprintf(str, "%f", player->MatTotal._21);
			d_font.DarwText(20, 180, str);
			sprintf(str, "%f", player->MatTotal._31);
			d_font.DarwText(20, 200, str);
			sprintf(str, "%f", player->MatTotal._41);
			d_font.DarwText(20, 220, str);

			sprintf(str, "%f", player->MatTotal._12);
			d_font.DarwText(130, 160, str);
			sprintf(str, "%f", player->MatTotal._22);
			d_font.DarwText(130, 180, str);
			sprintf(str, "%f", player->MatTotal._32);
			d_font.DarwText(130, 200, str);
			sprintf(str, "%f", player->MatTotal._42);
			d_font.DarwText(130, 220, str);

			sprintf(str, "%f", player->MatTotal._13);
			d_font.DarwText(240, 160, str);
			sprintf(str, "%f", player->MatTotal._23);
			d_font.DarwText(240, 180, str);
			sprintf(str, "%f", player->MatTotal._33);
			d_font.DarwText(240, 200, str);
			sprintf(str, "%f", player->MatTotal._43);
			d_font.DarwText(240, 220, str);
		}
		else{

			// プレイヤー情報
			sprintf(str, "PlayerMat._11 : %.2f", player->MatTotal._11);
			d_font.DarwText(20, 60, str);
			sprintf(str, "PlayerMat._21 : %.2f", player->MatTotal._21);
			d_font.DarwText(20, 80, str);
			sprintf(str, "PlayerMat._31 : %.2f", player->MatTotal._31);
			d_font.DarwText(20, 100, str);
			sprintf(str, "PlayerMat._41 : %.2f", player->MatTotal._41);
			d_font.DarwText(20, 120, str);

			sprintf(str, "PlayerMat._12 : %.2f", player->MatTotal._12);
			d_font.DarwText(250, 60, str);
			sprintf(str, "PlayerMat._22 : %.2f", player->MatTotal._22);
			d_font.DarwText(250, 80, str);
			sprintf(str, "PlayerMat._32 : %.2f", player->MatTotal._32);
			d_font.DarwText(250, 100, str);
			sprintf(str, "PlayerMat._42 : %.2f", player->MatTotal._42);
			d_font.DarwText(250, 120, str);

			sprintf(str, "PlayerMat._13 : %.2f", player->MatTotal._13);
			d_font.DarwText(480, 60, str);
			sprintf(str, "PlayerMat._23 : %.2f", player->MatTotal._23);
			d_font.DarwText(480, 80, str);
			sprintf(str, "PlayerMat._33 : %.2f", player->MatTotal._33);
			d_font.DarwText(480, 100, str);
			sprintf(str, "PlayerMat._43 : %.2f", player->MatTotal._43);
			d_font.DarwText(480, 120, str);


			// カメラ情報
			sprintf(str, "CameraMat._11 : %.2f", c_pos->MatTotal._11);
			d_font.DarwText(20, 160, str);
			sprintf(str, "CameraMat._21 : %.2f", c_pos->MatTotal._21);
			d_font.DarwText(20, 180, str);
			sprintf(str, "CameraMat._31 : %.2f", c_pos->MatTotal._31);
			d_font.DarwText(20, 200, str);
			sprintf(str, "CameraMat._41 : %.2f", c_pos->MatTotal._41);
			d_font.DarwText(20, 220, str);

			sprintf(str, "CameraMat._12 : %.2f", c_pos->MatTotal._12);
			d_font.DarwText(250, 160, str);
			sprintf(str, "CameraMat._22 : %.2f", c_pos->MatTotal._22);
			d_font.DarwText(250, 180, str);
			sprintf(str, "CameraMat._32 : %.2f", c_pos->MatTotal._32);
			d_font.DarwText(250, 200, str);
			sprintf(str, "CameraMat._42 : %.2f", c_pos->MatTotal._42);
			d_font.DarwText(250, 220, str);

			sprintf(str, "CameraMat._13 : %.2f", c_pos->MatTotal._13);
			d_font.DarwText(480, 160, str);
			sprintf(str, "CameraMat._23 : %.2f", c_pos->MatTotal._23);
			d_font.DarwText(480, 180, str);
			sprintf(str, "CameraMat._33 : %.2f", c_pos->MatTotal._33);
			d_font.DarwText(480, 200, str);
			sprintf(str, "CameraMat._43 : %.2f", c_pos->MatTotal._43);
			d_font.DarwText(480, 220, str);
		}

		/*for (int i = 0; i < HITBOXNUM; i++){

			if (hbox->HitFlg[i]){

				sprintf(str, "HitFlf[%d] : true", i);
				d_font.DarwText(100, 240 + (i * 20), str);
			}
			else {

				sprintf(str, "HitFlf[%d] : false", i);
				d_font.DarwText(100, 240 + (i * 20), str);
			}
		}*/
	} // GAME
}


//----------------------------------------------------------------------------------------------
// カメラアングル
//----------------------------------------------------------------------------------------------
void CameraAngle(PLAYER* player, CameraPos* c_pos){

	if (CameraStatus == START){

		//タイトル時のカメラアングル
		D3DXMatrixLookAtLH(&g_MatView,
			&D3DXVECTOR3(50.0f, -20.0f, 0.0f),		// 視点
			&D3DXVECTOR3(2.0f, -20.0f, -10.0f),		// 注視点
			&D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	}
	else if (CameraStatus == SELECT){

		//ステージセレクト時のカメラアングル

		c_pos->g_up.x = c_pos->MatTotal._21;
		c_pos->g_up.y = c_pos->MatTotal._22;
		c_pos->g_up.z = c_pos->MatTotal._23;

		c_pos->g_lookat.x = c_pos->MatTotal._41;
		c_pos->g_lookat.y = c_pos->MatTotal._42;
		c_pos->g_lookat.z = c_pos->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + c_pos->MatTotal._31 * 30;
		c_pos->g_eye.y = c_pos->g_lookat.y + c_pos->MatTotal._32 * 30;
		c_pos->g_eye.z = c_pos->g_lookat.z + c_pos->MatTotal._33 * 30;

		// カメラ変換行列作成
		D3DXMatrixLookAtLH(&g_MatView,
			&c_pos->g_eye,				// 視点
			&c_pos->g_lookat,			// 注視点
			&c_pos->g_up);				// 上向き

		c_pos->angle.y = -0.5;	//カメラの回転

		if (GetKeyboardTrigger(DIK_RETURN)){
			c_pos->angle.y = 0.0f;
		}
	}
	else if (CameraStatus == GAME){

		//ゲーム時のカメラアングル
		PositionCorrection(player); //プレイヤーの座標ずれを直す

		/*if (player->MatTotal._22 == 1){

			//パターン0
			CameraAngleNum_0(player,c_pos);
		}

		if (player->MatTotal._23 == -1){

			//パターン1
			CameraAngleNum_1(player, c_pos);
		}

		if (player->MatTotal._22 == -1){

			//パターン2
			CameraAngleNum_2(player, c_pos);
		}

		if (player->MatTotal._23 == 1){

			//パターン3
			CameraAngleNum_3(player, c_pos);
		}

		if (player->MatTotal._21 == 1){

			//パターン4
			CameraAngleNum_4(player, c_pos);
		}

		if (player->MatTotal._21 == -1){

			//パターン5
			CameraAngleNum_5(player, c_pos);
		}*/

		c_pos->g_up.x = c_pos->MatTotal._21;
		c_pos->g_up.y = c_pos->MatTotal._22;
		c_pos->g_up.z = c_pos->MatTotal._23;

		c_pos->g_lookat.x = c_pos->MatTotal._41;
		c_pos->g_lookat.y = c_pos->MatTotal._42;
		c_pos->g_lookat.z = c_pos->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 7;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 7;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 7;

		// カメラ変換行列作成
		D3DXMatrixLookAtLH(&g_MatView,
			&c_pos->g_eye,				// 視点
			&c_pos->g_lookat,			// 注視点
			&c_pos->g_up);				// 上向き
	}
	else if (CameraStatus == END){

		//ゲームクリアー時のカメラアングル
		if (end_cnt > 0 && end_cnt<50){
			//演出カメラ
			c_pos->angle.y = 1.8f; 
			c_pos->MatTotal._43 += 0.4f;
			c_pos->MatTotal._41 += 0.5f;
		}
		else{

			c_pos->angle.y = 0;
		}

		c_pos->g_up.x = c_pos->MatTotal._21;
		c_pos->g_up.y = c_pos->MatTotal._22;
		c_pos->g_up.z = c_pos->MatTotal._23;

		c_pos->g_lookat.x = c_pos->MatTotal._41;
		c_pos->g_lookat.y = c_pos->MatTotal._42;
		c_pos->g_lookat.z = c_pos->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + c_pos->MatTotal._31 * 20;
		c_pos->g_eye.y = c_pos->g_lookat.y + c_pos->MatTotal._32 * 20;
		c_pos->g_eye.z = c_pos->g_lookat.z + c_pos->MatTotal._33 * 20;
		DebugCameraAngle(player,c_pos);

		// カメラ変換行列作成
		D3DXMatrixLookAtLH(&g_MatView,
			&c_pos->g_eye,				// 視点
			&c_pos->g_lookat,			// 注視点
			&c_pos->g_up);				// 上向き
		end_cnt++;
	}


	// カメラ行列を固定パイプラインへセット
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_VIEW, &g_MatView);
}

//パターン0
void CameraAngleNum_0(PLAYER* player, CameraPos* c_pos){

	/*if ((player->MatTotal._11 == 1) && (player->MatTotal._33 == 1)){

		// 前
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41;
		c_pos->g_lookat.y = player->MatTotal._42 + 5;
		c_pos->g_lookat.z = player->MatTotal._43 - 2;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._11 == -1) && (player->MatTotal._33 == -1)){

		//後ろ
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41;
		c_pos->g_lookat.y = player->MatTotal._42 + 5;
		c_pos->g_lookat.z = player->MatTotal._43 + 2;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._13 == 1) && (player->MatTotal._31 == -1)){

		//左
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 + 2;
		c_pos->g_lookat.y = player->MatTotal._42 + 5;
		c_pos->g_lookat.z = player->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._13 == -1) && (player->MatTotal._31 == 1)){

		//右
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 - 2;
		c_pos->g_lookat.y = player->MatTotal._42 + 5;
		c_pos->g_lookat.z = player->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}*/

	c_pos->g_up.x = c_pos->MatTotal._21;
	c_pos->g_up.y = c_pos->MatTotal._22;
	c_pos->g_up.z = c_pos->MatTotal._23;

	c_pos->g_lookat.x = c_pos->MatTotal._41;
	c_pos->g_lookat.y = c_pos->MatTotal._42;
	c_pos->g_lookat.z = c_pos->MatTotal._43;

	c_pos->g_eye.x = c_pos->g_lookat.x + c_pos->MatTotal._31 * 7;
	c_pos->g_eye.y = c_pos->g_lookat.y + c_pos->MatTotal._32 * 7;
	c_pos->g_eye.z = c_pos->g_lookat.z + c_pos->MatTotal._33 * 7;
}


//パターン1
void CameraAngleNum_1(PLAYER* player, CameraPos* c_pos){

	if ((player->MatTotal._11 == 1) && (player->MatTotal._32 == 1)){

		// 前
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41;
		c_pos->g_lookat.y = player->MatTotal._42 - 2;
		c_pos->g_lookat.z = player->MatTotal._43 - 5;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._11 == -1) && (player->MatTotal._32 == -1)){

		//後ろ
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41;
		c_pos->g_lookat.y = player->MatTotal._42 + 2;
		c_pos->g_lookat.z = player->MatTotal._43 - 5;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._12 == 1) && (player->MatTotal._31 == -1)){

		//左
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 + 2;
		c_pos->g_lookat.y = player->MatTotal._42;
		c_pos->g_lookat.z = player->MatTotal._43 - 5;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._12 == -1) && (player->MatTotal._31 == 1)){

		//右
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 - 2;
		c_pos->g_lookat.y = player->MatTotal._42;
		c_pos->g_lookat.z = player->MatTotal._43 - 5;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}
}


//パターン2
void CameraAngleNum_2(PLAYER* player, CameraPos* c_pos){
	if ((player->MatTotal._11 == 1) && (player->MatTotal._33 == -1)){

		// 前
		c_pos->g_up.x = c_pos->MatTotal._21;
		c_pos->g_up.y = c_pos->MatTotal._22;
		c_pos->g_up.z = c_pos->MatTotal._23;

		c_pos->g_lookat.x = c_pos->MatTotal._41;
		c_pos->g_lookat.y = c_pos->MatTotal._42;
		c_pos->g_lookat.z = c_pos->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + c_pos->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + c_pos->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + c_pos->MatTotal._33 * 10;
	}

	if ((player->MatTotal._11 == -1) && (player->MatTotal._33 == 1)){

		//後ろ
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41;
		c_pos->g_lookat.y = player->MatTotal._42 - 5;
		c_pos->g_lookat.z = player->MatTotal._43 - 2;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._13 == -1) && (player->MatTotal._31 == -1)){

		//左
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 + 2;
		c_pos->g_lookat.y = player->MatTotal._42 - 5;
		c_pos->g_lookat.z = player->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._13 == 1) && (player->MatTotal._31 == 1)){

		//右
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 - 2;
		c_pos->g_lookat.y = player->MatTotal._42 - 5;
		c_pos->g_lookat.z = player->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}
}


//パターン3
void CameraAngleNum_3(PLAYER* player, CameraPos* c_pos){

	if ((player->MatTotal._11 == 1) && (player->MatTotal._32 == -1)){

		// 前
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41;
		c_pos->g_lookat.y = player->MatTotal._42 + 2;
		c_pos->g_lookat.z = player->MatTotal._43 + 5;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._11 == -1) && (player->MatTotal._32 == 1)){

		//後ろ
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41;
		c_pos->g_lookat.y = player->MatTotal._42 - 2;
		c_pos->g_lookat.z = player->MatTotal._43 + 5;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._12 == -1) && (player->MatTotal._31 == -1)){

		//左
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 + 2;
		c_pos->g_lookat.y = player->MatTotal._42;
		c_pos->g_lookat.z = player->MatTotal._43 + 5;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._12 == 1) && (player->MatTotal._31 == 1)){

		//右
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 - 2;
		c_pos->g_lookat.y = player->MatTotal._42;
		c_pos->g_lookat.z = player->MatTotal._43 + 5;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}
}


//パターン4
void CameraAngleNum_4(PLAYER* player, CameraPos* c_pos){

	if ((player->MatTotal._12 == -1) && (player->MatTotal._33 == 1)){

		// 前
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 + 5;
		c_pos->g_lookat.y = player->MatTotal._42;
		c_pos->g_lookat.z = player->MatTotal._43 - 2;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._12 == 1) && (player->MatTotal._33 == -1)){

		//後ろ
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 + 5;
		c_pos->g_lookat.y = player->MatTotal._42;
		c_pos->g_lookat.z = player->MatTotal._43 + 2;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._13 == 1) && (player->MatTotal._32 == 1)){

		//左
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 + 5;
		c_pos->g_lookat.y = player->MatTotal._42 - 2;
		c_pos->g_lookat.z = player->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._13 == -1) && (player->MatTotal._32 == -1)){

		//右
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 + 5;
		c_pos->g_lookat.y = player->MatTotal._42 + 2;
		c_pos->g_lookat.z = player->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}
}


//パターン5
void CameraAngleNum_5(PLAYER* player, CameraPos* c_pos){

	if ((player->MatTotal._12 == 1) && (player->MatTotal._33 == 1)){

		// 前
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 - 5;
		c_pos->g_lookat.y = player->MatTotal._42;
		c_pos->g_lookat.z = player->MatTotal._43 - 2;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._12 == -1) && (player->MatTotal._33 == -1)){

		//後ろ
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 - 5;
		c_pos->g_lookat.y = player->MatTotal._42;
		c_pos->g_lookat.z = player->MatTotal._43 + 2;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._13 == 1) && (player->MatTotal._32 == -1)){

		//左
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 - 5;
		c_pos->g_lookat.y = player->MatTotal._42 + 2;
		c_pos->g_lookat.z = player->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}

	if ((player->MatTotal._13 == -1) && (player->MatTotal._32 == 1)){

		//右
		c_pos->g_up.x = player->MatTotal._21;
		c_pos->g_up.y = player->MatTotal._22;
		c_pos->g_up.z = player->MatTotal._23;

		c_pos->g_lookat.x = player->MatTotal._41 - 5;
		c_pos->g_lookat.y = player->MatTotal._42 - 2;
		c_pos->g_lookat.z = player->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + player->MatTotal._31 * 10;
		c_pos->g_eye.y = c_pos->g_lookat.y + player->MatTotal._32 * 10;
		c_pos->g_eye.z = c_pos->g_lookat.z + player->MatTotal._33 * 10;
	}
}


//----------------------------------------------------------------------------------------------
// キー入力
//----------------------------------------------------------------------------------------------
void KeyInput(PLAYER* player, CameraPos* c_pos, MOVEBOX* m_box){

	if (g_TurnCount_Y == 0){
		//プレイヤー操作
		if (GetKeyboardPress(DIK_UP) && !StopFlg){


			if (g_status == STAND){

				g_Aniflg = false;
			}
			//上移動
			g_status = MOVE_UP;
		}
		else if (GetKeyboardRelease(DIK_UP)){

			if (g_status == MOVE_UP){

				g_Aniflg = false;
			}
			g_status = STAND;
		}

		/*if (GetKeyboardPress(DIK_DOWN) && !StopFlg && StopMove){

			if (g_status == STAND){

				g_Aniflg = false;
			}
			//下移動
			g_status = MOVE_DOWN;
		}
		else if (GetKeyboardRelease(DIK_DOWN)){

			if (g_status == MOVE_DOWN){

				g_Aniflg = false;
			}
			g_status = STAND;
		}*/
	}

	if (!GetKeyboardPress(DIK_UP)){ //移動していない時、左右に向ける

		if (GetKeyboardTrigger(DIK_RIGHT)){

			if (g_status == STAND){

				g_Aniflg = false;
			}
			//右回転
			g_status = MOVE_RIGHT;
		}
		else if (g_TurnCount_Y > 10){

			if (g_status == MOVE_RIGHT){

				g_Aniflg = false;
			}
			g_status = STAND; //立ち状態にする
			g_TurnCount_Y = 0; //回転のカウントのリセット
		}

		if (GetKeyboardTrigger(DIK_LEFT)){

			if (g_status == STAND){

				g_Aniflg = false;
			}
			//左回転
			g_status = MOVE_LEFT;
		}
		else if (g_TurnCount_Y > 10){

			if (g_status == MOVE_LEFT){

				g_Aniflg = false;
			}
			g_status = STAND; //立ち状態にする
			g_TurnCount_Y = 0; //回転のカウントのリセット
		}
	}

	PlayerStatus(player, c_pos, g_status);
}


//----------------------------------------------------------------------------------------------
// プレイヤーの状態遷移
//----------------------------------------------------------------------------------------------
void PlayerStatus(PLAYER* player, CameraPos* c_pos, int g_status){

	//プレイヤーの行動
	switch (g_status){

	case STAND:
		g_NowAnimSet = 5;
		AnimationSet(g_NowAnimSet);

		//プレイヤー
		player->trans.x = 0;
		player->trans.z = 0;

		//プレイヤー状態の初期化
		player_up_move = false;
		player_down_move = false;
		player_right_move = false;
		player_left_move = false;


		//カメラ
		c_pos->trans.x = 0;
		c_pos->trans.y = 0;
		c_pos->trans.z = 0;
		break;

	case MOVE_UP:
		g_NowAnimSet = 4;
		AnimationSet(g_NowAnimSet);

		//プレイヤー
		player->trans.z = -0.1f*baisoku;
		player_up_move = true;

		//カメラ
		c_pos->trans.z = -0.1f*baisoku;
		break;

	case MOVE_DOWN:
		g_NowAnimSet = 4;
		AnimationSet(g_NowAnimSet);

		//プレイヤー
		c_pos->trans.z = 0.1f*baisoku;
		player_down_move = true;
		break;

	case MOVE_RIGHT:
		g_NowAnimSet = 5;
		AnimationSet(g_NowAnimSet);

		//g_PlayerTrans.z = 0.1f;
		player_right_move = true;
		PlayerTurn(player, c_pos, MOVE_RIGHT);
		//CameraStatusNum(MOVE_RIGHT);
		break;

	case MOVE_LEFT:
		g_NowAnimSet = 5;
		AnimationSet(g_NowAnimSet);

		//g_PlayerTrans.z = -0.1f;
		player_left_move = true;
		PlayerTurn(player, c_pos, MOVE_LEFT);
		break;
	}

	if (!g_oneskin.isMotionEnd()) {

		// モーション更新
		g_oneskin.GetAnimationController()->AdvanceTime(0.02, NULL);
	}
	else {

		//アニメーション終了
		g_Aniflg = false;
	}
}


//----------------------------------------------------------------------------------------------
// アニメーションの変更
//----------------------------------------------------------------------------------------------
void AnimationSet(int g_NowAnimSet){

	if (!g_Aniflg){

		g_oneskin.ChangeAnimation(g_NowAnimSet);	// 現在のアニメーション
		g_Aniflg = true;
	}
}


//----------------------------------------------------------------------------------------------
// ステージに配置してるオブジェクトの描画
//----------------------------------------------------------------------------------------------
void DrawCube(PLAYER* player, MOVEBOX* m_box, MOON* mn){

	//動く箱
	for (int i = 0; i < m_box->BoxNum; i++){

		if (g_GAMESTATUS == GAME){

			if (player->MatTotal._22 >= 1){
				//パターン
				//     3
				//     2
				//   4 1 5
				//     0

				// パターン0
				if (player->MatTotal._11 >= 1 && player->MatTotal._33 >= 1){

					// 前
					if ((player->MatTotal._43 + 1.5f) > m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._11 <= -1 && player->MatTotal._33 <= -1){

					// 後ろ
					if ((player->MatTotal._43 - 1.5f) < m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._13 <= -1 && player->MatTotal._31 >= 1){

					// 右
					if ((player->MatTotal._41 + 1.5f) > m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._13 >= 1 && player->MatTotal._31 <= -1){

					// 左
					if ((player->MatTotal._41 - 1.0f) < m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else{

					// 切り替え時の描画
					// 条件式以外だと全て消えるから
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
				}
			}
			else if (player->MatTotal._23 <= -1){
				

				// パターン1
				if (player->MatTotal._11 >= 1 && player->MatTotal._32 >= 1){

					// 前
					if ((player->MatTotal._42 + 1.5f) > m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._11 <= -1 && player->MatTotal._32 <= -1){

					// 後ろ
					if ((player->MatTotal._42 - 1.5f) < m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._12 <= -1 && player->MatTotal._31 >= 1){

					// 右
					if ((player->MatTotal._41 + 1.5f) > m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._12 >= 1 && player->MatTotal._31 <= -1){

					// 左
					if ((player->MatTotal._41 - 1.0f) < m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else{

					// 切り替え時の描画
					// 条件式以外だと全て消えるから
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
				}
			}
			else if (player->MatTotal._22 <= -1){


				// パターン2
				if (player->MatTotal._11 >= 1 && player->MatTotal._33 <= -1){

					// 前
					if ((player->MatTotal._43 - 1.5f) < m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._11 <= -1 && player->MatTotal._33 >= 1){

					// 後ろ
					if ((player->MatTotal._43 + 1.5f) > m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._13 >= 1 && player->MatTotal._31 >= 1){

					// 右
					if ((player->MatTotal._41 + 1.5f) > m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._13 <= -1 && player->MatTotal._31 <= -1){

					// 左
					if ((player->MatTotal._41 - 1.0f) < m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else{

					// 切り替え時の描画
					// 条件式以外だと全て消えるから
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
				}
			}
			else if (player->MatTotal._23 >= 1){


				// パターン3
				if (player->MatTotal._11 >= 1 && player->MatTotal._32 <= -1){

					// 前
					if ((player->MatTotal._42 - 1.5f) < m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._11 <= -1 && player->MatTotal._32 >= 1){

					// 後ろ
					if ((player->MatTotal._42 + 1.5f) > m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._12 >= 1 && player->MatTotal._31 >= 1){

					// 右
					if ((player->MatTotal._41 + 1.5f) > m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._12 <= -1 && player->MatTotal._31 <= -1){

					// 左
					if ((player->MatTotal._41 - 1.0f) < m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else{

					// 切り替え時の描画
					// 条件式以外だと全て消えるから
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
				}
			}
			else if (player->MatTotal._21 >= 1){


				// パターン4
				if (player->MatTotal._12 <= -1 && player->MatTotal._33 >= 1){

					// 前
					if ((player->MatTotal._43 + 1.5f) > m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._12 >= 1 && player->MatTotal._33 <= -1){

					// 後ろ
					if ((player->MatTotal._43 - 1.5f) < m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._13 <= -1 && player->MatTotal._32 >= -1){

					// 右
					if ((player->MatTotal._42 - 1.5f) < m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._13 >= 1 && player->MatTotal._32 >= 1){

					// 左
					if ((player->MatTotal._42 + 1.5f) > m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else{

					// 切り替え時の描画
					// 条件式以外だと全て消えるから
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
				}
			}
			else if (player->MatTotal._21 <= -1){


				// パターン5
				if (player->MatTotal._12 >= 1 && player->MatTotal._33 >= 1){

					// 前
					if ((player->MatTotal._43 + 1.5f) > m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._12 <= -1 && player->MatTotal._33 <= -1){

					// 後ろ
					if ((player->MatTotal._43 - 1.5f) < m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._13 <= -1 && player->MatTotal._32 >= 1){

					// 右
					if ((player->MatTotal._42 + 1.5f) > m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else if (player->MatTotal._13 >= 1 && player->MatTotal._32 <= -1){

					// 左
					if ((player->MatTotal._42 - 1.5f) < m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
					}
				}
				else{

					// 切り替え時の描画
					// 条件式以外だと全て消えるから
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
				}
			}

		}
		else{

			g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ワールド変換行列をセット
			g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
		}
	}

	if (g_GAMESTATUS == GAME){

		g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &mn->MatTotal);	// ワールド変換行列をセット
		g_DXXFileObj_moon->Draw(g_DXGrobj->GetDXDevice());	// Ｘファイル描画
	}
}


//----------------------------------------------------------------------------------------------
// スカイドームの描画
//----------------------------------------------------------------------------------------------
void DrawSkydome(){

	//スカイドーム
	D3DXMatrixIdentity(&g_SkyMat);
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &g_SkyMat);   // ワールド変換行列をセット 
	g_DXXFileObj_skydome->Draw(g_DXGrobj->GetDXDevice());       // Ｘファイル描画

}


//----------------------------------------------------------------------------------------------
// プレイヤーの描画
//----------------------------------------------------------------------------------------------
void PlayerDraw(){

	g_oneskin.DrawFrame(g_DXGrobj->GetDXDevice(), g_oneskin.GetFrameRoot());	// ワンスキン描画
}


//----------------------------------------------------------------------------------------------
// COBBの描画（ブロックの当たり判定）
//----------------------------------------------------------------------------------------------
void Cobb_Draw(MOVEBOX* m_box){

	//COBB描画
	//動く箱OBB
	for (int i = 0; i < m_box->BoxNum; i++){

		m_box_obj[i].Draw(g_DXGrobj->GetDXDevice());
	}

	//プレイヤー
	//g_oneskin.DrawBBox(g_DXGrobj->GetDXDevice(), g_oneskin.GetFrameRoot()); //ボーン
	//player_obj.Draw(g_DXGrobj->GetDXDevice());

	//hitbox
	for (int i = 0; i < HITBOXNUM; i++){

		g_HitBox_obj[i].Draw(g_DXGrobj->GetDXDevice());
	}

	moon_obj.Draw(g_DXGrobj->GetDXDevice());
}


//----------------------------------------------------------------------------------------------
// 当たり判定の処理
//----------------------------------------------------------------------------------------------
void HitFunction(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, CameraPos* c_pos){

	//フラグ初期化
	for (int i = 0; i < M_BOX_NUM; i++){

		hit_count[i] = 0;
	}
	hit_Player_Stage = false;

	if (HitInitFlg){

		hbox->InitFlg();
		HitInitFlg = false;
	}

	StopMove = false;

for (int i = 0; i < m_box->BoxNum; i++){


		bool sts = player_obj.Collision(m_box_obj[i]);

		if (sts){

			hit_Player_Stage = true;
		}
	}

	//プレイヤー落下処理
	if (hit_Player_Stage){

		//(1)
		player->trans.y = 0;
		g_HitGroundFlg = true;
		g_TurnCount_Z = 0;
		StopFlg = false;

		//カメラのずれを直す
		test(player, c_pos);
	}
	else{

		StopFlg = true; //ボタンを止める
		if (!g_HitGroundFlg){

			//(2)
			player->trans.y = -GRAVITY;
		}
		else{

			//(3)
			DownTurn(player,c_pos);
			g_GCount++;
		}

	}

	if (g_GCount == 3){

		//(4)
		g_HitGroundFlg = false;
		g_GCount = 0;
	}


	// プレイヤーとステージ（箱の当たり判定）
	for (int i = 0; i < m_box->BoxNum; i++){
		bool sts = g_HitBox_obj[g_CameraNum].Collision(m_box_obj[i]);
		int BackNum = g_CameraNum + 2;
		if (BackNum == 4){
			BackNum = 0;
		}
		else if (BackNum == 5){
			BackNum = 1;
		}

		bool sts2 = g_HitBox_obj[BackNum].Collision(m_box_obj[i]);
		bool sts3 = g_HitBox_obj[g_CameraNum].Collision(moon_obj);

		if (sts){

			hbox->HitFlg[i] = true;
		}
		if (sts2){

			StopMove = true;
		}
		if (sts3){

			ClearFlg = true;
		}
	}


	//プレイヤーとボックスの当たり判定
	for (int i = 0; i < m_box->BoxNum; i++){

		if (hbox->HitFlg[i]){

			if (GetKeyboardPress(DIK_SPACE)){

				if (GetKeyboardPress(DIK_UP)){

					if (player->MatTotal._22 == 1){

						// ボックスを押す処理
						switch (g_CameraNum){
						case 0:
							m_box->trans[i].z = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 1:
							m_box->trans[i].x = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 2:
							m_box->trans[i].z = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 3:
							m_box->trans[i].x = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;
						}
					}//player->MatTotal._22==1
					else if (player->MatTotal._23 == -1){

						switch (g_CameraNum){
						case 0:
							m_box->trans[i].y = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 1:
							m_box->trans[i].x = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 2:
							m_box->trans[i].y = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 3:
							m_box->trans[i].x = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;
						}
					}//player->MatTotal._23==-1
					else if (player->MatTotal._22 == -1){

						switch (g_CameraNum){
						case 0:
							m_box->trans[i].z = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 1:
							m_box->trans[i].x = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 2:
							m_box->trans[i].z = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 3:
							m_box->trans[i].x = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;
						}
					}
					else if (player->MatTotal._23 == 1){

						switch (g_CameraNum){
						case 0:
							m_box->trans[i].y = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 1:
							m_box->trans[i].x = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 2:
							m_box->trans[i].y = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 3:
							m_box->trans[i].x = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;
						}
					}//player->MatTotal._23==-1
					else if (player->MatTotal._21 == 1){

						switch (g_CameraNum){
						case 0:
							m_box->trans[i].z = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 1:
							m_box->trans[i].y = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 2:
							m_box->trans[i].z = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 3:
							m_box->trans[i].y = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;
						}
					}//player->MatTotal._21 == 1
					else if (player->MatTotal._21 == -1){

						switch (g_CameraNum){
						case 0:
							m_box->trans[i].z = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 1:
							m_box->trans[i].y = -0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 2:
							m_box->trans[i].z = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;

						case 3:
							m_box->trans[i].y = 0.1f*baisoku;
							hbox->HitFlg[i] = false;
							break;
						}
						// ボックスを押す処理終了
					}
				}
			}
			else{

				// プレイヤーと箱が当たった時、プレイヤーを回転させる
				HitTurn(player, c_pos, hbox);
			}
		}
		else if (!hbox->HitFlg[i]){

			if (g_HitGroundFlg){

				// プレイヤーが地面に当たった時、落下の処理を止める
				m_box->trans[i].x = 0;
				m_box->trans[i].y = 0;
				m_box->trans[i].z = 0;
			}
		}
	}// for i
}


//
void test(PLAYER* player, CameraPos* c_pos){
	
	if (player->MatTotal._22 == 1){

		//パターン0
		c_pos->MatTotal._41 = player->MatTotal._41;
		c_pos->MatTotal._42 = player->MatTotal._42 + 4;
		c_pos->MatTotal._43 = player->MatTotal._43;
	}


	if (player->MatTotal._23 == -1){

		//パターン1
		c_pos->MatTotal._41 = player->MatTotal._41;
		c_pos->MatTotal._42 = player->MatTotal._42;
		c_pos->MatTotal._43 = player->MatTotal._43 - 4;
	}


	if (player->MatTotal._22 == -1){

		//パターン2
		c_pos->MatTotal._41 = player->MatTotal._41;
		c_pos->MatTotal._42 = player->MatTotal._42 - 4;
		c_pos->MatTotal._43 = player->MatTotal._43;
	}


	if (player->MatTotal._23 == 1){

		//パターン3
		c_pos->MatTotal._41 = player->MatTotal._41;
		c_pos->MatTotal._42 = player->MatTotal._42;
		c_pos->MatTotal._43 = player->MatTotal._43 + 4;
	}


	if (player->MatTotal._21 == 1){

		//パターン4
		c_pos->MatTotal._41 = player->MatTotal._41 + 4;
		c_pos->MatTotal._42 = player->MatTotal._42;
		c_pos->MatTotal._43 = player->MatTotal._43;
	}


	if (player->MatTotal._21 == -1){

		//パターン5
		c_pos->MatTotal._41 = player->MatTotal._41 - 4;
		c_pos->MatTotal._42 = player->MatTotal._42;
		c_pos->MatTotal._43 = player->MatTotal._43;
	}
}


void PlayerTurn(PLAYER* player, CameraPos* c_pos, int g_status){

	if (g_GAMESTATUS == GAME){

		//左右回転
		if (g_TurnCount_Y == 0){

			if (g_status == MOVE_RIGHT){

				player->angle.y = 9;
				c_pos->angle.y = 9;
			}
			if (g_status == MOVE_LEFT){

				player->angle.y = -9;
				c_pos->angle.y = -9;
			}
		}
		else if (g_TurnCount_Y == 10){

			player->angle.y = 0;
			c_pos->angle.y = 0;
		}

		g_TurnCount_Y++;
	}

	//元
	/*if (g_TurnCount_Y == 0){

		if (g_status == MOVE_RIGHT){

			player->angle.y = 90;
		}
		if (g_status == MOVE_LEFT){

			player->angle.y = -90;
		}
	}
	else if (g_TurnCount_Y == 1){

		player->angle.y = 0;
		//g_TurnCount_Y = 0;
	}

	g_TurnCount_Y++;*/
}


void HitTurn(PLAYER* player, CameraPos* c_pos, HITBOX* hbox){

	//縦回転(ブロックに当たった時)
	//HitBoxが当たった時の回転処理
	if (hbox->Count == 0){

		StopFlg = true; //ボタンを止める
		//プレイヤー
		player->angle.x = 90;
		c_pos->angle.x = 90;
		player->trans.y = 1.0;
		hbox->Count++;
	}
	else{
		StopFlg = false;
		//プレイヤー
		player->angle.x = 0;
		c_pos->angle.x = 0;
		player->trans.y = 0;
		hbox->Count = 0;
		HitInitFlg = true;
		g_HitGroundFlg = false;
	}
}


void DownTurn(PLAYER* player, CameraPos* c_pos){

	//縦回転(ブロックに当たらなかった時)
	if (g_status == MOVE_UP){

		if (g_TurnCount_Z == 0){

			//プレイヤー
			player->angle.x = -90; 
			c_pos->angle.x = -90;
			g_TurnCount_Z++;
		}
		else{

			//プレイヤー
			player->angle.x = 0;
			player->trans.y = 0;

			//カメラ
			c_pos->angle.x = 0;
		}
	}

	if (g_status == MOVE_DOWN){

		if (g_TurnCount_Z == 0){

			//プレイヤー
			//g_PlayerTrans.y = -1.0f;
			player->angle.x = 90;
			g_TurnCount_Z++;
		}
		else{

			//プレイヤー
			player->angle.x = 0;
			player->trans.y = 0;
		}
	}
}


//----------------------------------------------------------------------------------------------
// COBBの描画（プレイヤーの当たり判定）
//----------------------------------------------------------------------------------------------
void HitDraw(HITBOX* hbox){

	for (int i = 0; i < HITBOXNUM; i++){
		//for (int i = 0; i < 1; i++){

		g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &hbox->MatTotal[i]);
		g_DXXFileObj_hit->Draw(g_DXGrobj->GetDXDevice());
	}
}




//----------------------------------------------------------------------------------------------
// 2Dテクスチャの描画設定
//----------------------------------------------------------------------------------------------
void TextureDarw(){
	
	//2Dテクスチャー座標
	float tex_x;
	float tex_y;
	float tex_z;
	float tex_a;

	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);
	if (g_GAMESTATUS == START){

		tex_x = 200.0f;
		tex_y = 100.0f;
		tex_z = 0.0f;
		tex_a = 300.0f;

		VERTEX2D vx[4] = {
			{ tex_x - tex_a, tex_y - tex_a, 0, 1, color, 0, 0 },
			{ tex_x + tex_a, tex_y - tex_a, 0, 1, color, 1, 0 },
			{ tex_x + tex_a, tex_y + tex_a, 0, 1, color, 1, 1 },
			{ tex_x - tex_a, tex_y + tex_a, 0, 1, color, 0, 1 },
		};

		g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		g_DXGrobj->GetDXDevice()->SetTexture(0, star_tex);
		g_DXGrobj->GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx, sizeof(VERTEX2D));
	}

	if (g_GAMESTATUS == SELECT){

		tex_x = 200.0f;
		tex_y = 100.0f;
		tex_z = 0.0f;
		tex_a = 200.0f;

		VERTEX2D vx[4] = {
			{ tex_x - tex_a, tex_y - tex_a, 0, 1, color, 0, 0 },
			{ tex_x + tex_a, tex_y - tex_a, 0, 1, color, 1, 0 },
			{ tex_x + tex_a, tex_y + tex_a, 0, 1, color, 1, 1 },
			{ tex_x - tex_a, tex_y + tex_a, 0, 1, color, 0, 1 },
		};

		g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		g_DXGrobj->GetDXDevice()->SetTexture(0, select_tex);
		g_DXGrobj->GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx, sizeof(VERTEX2D));
	}
}


//----------------------------------------------------------------------------------------------
// 2Dテクスチャでのパーティクル描画
//----------------------------------------------------------------------------------------------
void ParticleDraw(){

	for (int i = 0; i < PARTICLE_NUM; i++){

		if (particle_reset_flg[i] == true){

			//2Dテクスチャー座標
			particle_x[i] = 100.0f + rand() % 1200;
			particle_y[i] = -(rand() % 200);
			particle_z[i] = 0.0f;
			particle_a[i] = 5.0f; // パーティクルの大きさ指定
			particle_speed[i] = rand() % 3; // 落下速度
			particle_vanish[i] = rand() % 5 +2; // パーティクルを消す値(大きいほどすぐ消える)
			particle_reset_flg[i] = false;
		}
	}

	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);

	for (int i = 0; i < PARTICLE_NUM; i++){

		int color_pass = rand() % 7;
		//int color_pass = 0;

		// パーティクル座標
		VERTEX2D vx[4] = {
			{ (particle_x[i] + i * 10) - particle_a[i], particle_y[i] - particle_a[i], 0, 1, color, 0, 0 },
			{ (particle_x[i] + i * 10) + particle_a[i], particle_y[i] - particle_a[i], 0, 1, color, 1, 0 },
			{ (particle_x[i] + i * 10) + particle_a[i], particle_y[i] + particle_a[i], 0, 1, color, 1, 1 },
			{ (particle_x[i] + i * 10) - particle_a[i], particle_y[i] + particle_a[i], 0, 1, color, 0, 1 },
		};

		if (color_pass == 0 || color_pass == 1){

			g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
			g_DXGrobj->GetDXDevice()->SetTexture(0, star_tex_ora);
			g_DXGrobj->GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx, sizeof(VERTEX2D));
		}
		else if (color_pass == 2){

			g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
			g_DXGrobj->GetDXDevice()->SetTexture(0, star_tex_whi);
			g_DXGrobj->GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx, sizeof(VERTEX2D));
		}
		else{

			g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
			g_DXGrobj->GetDXDevice()->SetTexture(0, star_tex);
			g_DXGrobj->GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx, sizeof(VERTEX2D));
		}
	}

	for (int i = 0; i < PARTICLE_NUM; i++){

		if (particle_reset_flg[i] == false){

			//座標の移動
			particle_y[i] += 3.0f + particle_speed[i]; //左向き
			particle_x[i] -= 2.0f + particle_speed[i]; //下向き
			particle_a[i] -= particle_vanish[i]/100; //パーティクルの大きさを徐々に小さくする
			if ((particle_y[i]>SCREEN_Y + particle_a[i]) || particle_a[i] < 0){

				particle_reset_flg[i] = true;	//座標を戻す
			}
		} 
	}
}


//----------------------------------------------------------------------------------------------
// 2Dテクスチャーでのボタン表示
//----------------------------------------------------------------------------------------------
void SerectButton(){

	//2Dテクスチャー座標
	float tex_x = SCREEN_X / 2;
	float tex_y = SCREEN_Y / 2;
	float tex_z = 0.0f;
	float tex_a = 200.0f;

	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);
	//D3DCOLOR color2 = D3DCOLOR_XRGB(0, 0, 0);

	if (GetKeyboardPress(DIK_RIGHT)){

		// 右ボタンを押した時の処理
		VERTEX2D vx[4] = {
			{ tex_x - tex_a * 3, tex_y - tex_a, 0, 1, color, 0, 0 },
			{ tex_x + tex_a * 3, tex_y - tex_a, 0, 1, color, 1, 0 },
			{ tex_x + tex_a * 3, tex_y + tex_a, 0, 1, color, 1, 1 },
			{ tex_x - tex_a * 3, tex_y + tex_a, 0, 1, color, 0, 1 },
		};

		g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		g_DXGrobj->GetDXDevice()->SetTexture(0, serect_button_r);
		g_DXGrobj->GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx, sizeof(VERTEX2D));
	}
	else if (GetKeyboardPress(DIK_LEFT)){

		// 左ボタンを押した時の処理
		VERTEX2D vx[4] = {
			{ tex_x - tex_a * 3, tex_y - tex_a, 0, 1, color, 0, 0 },
			{ tex_x + tex_a * 3, tex_y - tex_a, 0, 1, color, 1, 0 },
			{ tex_x + tex_a * 3, tex_y + tex_a, 0, 1, color, 1, 1 },
			{ tex_x - tex_a * 3, tex_y + tex_a, 0, 1, color, 0, 1 },
		};

		g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		g_DXGrobj->GetDXDevice()->SetTexture(0, serect_button_l);
		g_DXGrobj->GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx, sizeof(VERTEX2D));
	}
	else{

		// ボタンを押してない時の処理

		VERTEX2D vx[4] = {
			{ tex_x - tex_a * 3, tex_y - tex_a, 0, 1, color, 0, 0 },
			{ tex_x + tex_a * 3, tex_y - tex_a, 0, 1, color, 1, 0 },
			{ tex_x + tex_a * 3, tex_y + tex_a, 0, 1, color, 1, 1 },
			{ tex_x - tex_a * 3, tex_y + tex_a, 0, 1, color, 0, 1 },
		};

		g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		g_DXGrobj->GetDXDevice()->SetTexture(0, serect_button);
		g_DXGrobj->GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vx, sizeof(VERTEX2D));
	}
}