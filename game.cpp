//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	�Q�[������
//!	@author	
//*****************************************************************************
#define _CRT_SECURE_NO_WARNIN
#define _CRT_SECURE_NO_WARNINGS
#define GRAVITY 0.5
#define baisoku 1
#define MOVESPED 0.1
#define PARTICLE_MAX 500	// �p�[�e�B�N���̍ő吔
#define PARTICLE_NUM 70	// �p�[�e�B�N���̎g�p��

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
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
CDirectXGraphics	*g_DXGrobj = NULL;		// DirectX Graphics�I�u�W�F�N�g
CDirect3DXFile		*g_DXXFileObj = NULL, *g_DXXFileObj_skydome = NULL;	// �w�t�@�C���I�u�W�F�N�g
CDirect3DXFile		*g_DXXFileObj_hit = NULL;		//�{�b�N�X�̓����蔻��
CDirect3DXFile		*g_DXXFileObj_player = NULL;	//�v���C���[�̓����蔻��
CDirect3DXFile		*g_DXXFileObj_moon = NULL;		//��

CSkinnedMesh		g_oneskin;

//CBoundingSphere		*cbs, *BSphereSkydome;
CDebugFont			d_font;

//COBB
//COBB	stage_obj[STAGENUM_Z][STAGENUM_Y][STAGENUM_X], m_box_obj[M_BOX_NUM]; //�X�e�[�W�A������
COBB	m_box_obj[M_BOX_NUM]; //�X�e�[�W�A������
COBB	player_obj;
COBB	g_HitBox_obj[HITBOXNUM];
COBB	moon_obj;

D3DXMATRIX			g_MatView;			// �J�����s��
D3DXMATRIX			g_Matbul;			// �J�����s��
D3DXMATRIX			g_MatProjection;	// �v���W�F�N�V�����ϊ��s��
D3DXMATRIX			g_MatOrthor;		// ���s���e�ϊ��s��
D3DXMATRIX			g_MatWorld;			// ���[���h�ϊ��s��

// �X�J�C�h�[��
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

HANDLE				g_hEventHandle;		// �C�x���g�n���h��
bool				g_EndFlag = false;	// �I���t���O
std::thread			g_gamemainthread;	// �Q�[�����C���X���b�h


// add_start(2018/05/10)

//LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;
LPDIRECT3DTEXTURE9	select_tex;	//�X�e�[�W�Z���N�g
LPDIRECT3DTEXTURE9	star_tex;		//��
LPDIRECT3DTEXTURE9	star_tex_ora;	//��(�I�����W�F)
LPDIRECT3DTEXTURE9	star_tex_whi;	//��(���F)
LPDIRECT3DTEXTURE9	serect_button;		// �Z���N�g�{�^��
LPDIRECT3DTEXTURE9	serect_button_r;	// �Z���N�g�{�^���i�E�j
LPDIRECT3DTEXTURE9	serect_button_l;	// �Z���N�g�{�^���i���j

//�e�N�X�`���\����
struct VERTEX2D{
	float x, y, z;
	float rhw;
	D3DCOLOR diffuse;
	float tu, tv;
};
// add_end(2018/05/10)
//---------------------------------------------------------------
//�O���[�o���ϐ�
//---------------------------------------------------------------
enum{ STAND, MOVE_UP, MOVE_DOWN, MOVE_RIGHT, MOVE_LEFT };
enum{ START, SELECT, GAME, END };
//���
int g_GAMESTATUS = START;  //�Q�[��
int CameraStatus = START;  //�J����

bool DebugFlg = false;
int cnt = 0;
float up_speed = 0.0f;
using namespace std;

int StageNum = 0;

//�����m�̓����蔻��
int hit_count[M_BOX_NUM];
bool Hit_BoxFlg[M_BOX_NUM];
bool Clear_Obj[M_BOX_NUM];

//���������鎞�̏��
bool up_move = false;
bool down_move = false;
bool right_move = false;
bool left_move = false;

//�A�j���[�V�����֌W
int	g_NowAnimSet = 0;
int g_status = STAND;
bool g_Aniflg = false;

//�v���C���[�����蔻��
//�v���C���[�ƃX�e�[�W
bool hit_Player_Stage = false;

//�v���C���[�Ɣ�
bool player_up_move = false;
bool player_down_move = false;
bool player_right_move = false;
bool player_left_move = false;

//��]�ݒ�
int g_TurnCount_Y = 0; //Y��
int g_TurnCount_Z = 0; //Z��
int TurnFlg = 0;
//�����ݒ�
int g_GCount = 0;

//�v���C���[�ƒn�ʂ̓����蔻��
bool g_HitGroundFlg = false;

//�J�������
int g_CameraNum = 0;	//�J�����̏��
int g_CameraUpDownCount = 0;	//�J�����̏㉺�ړ�


bool ChangeDFlg = false;
bool HitInitFlg = true;

//LoadMap
bool LoadFlg = true;

//��]���Ă���Ƃ��Ƀ{�^���������Ȃ�����
bool StopFlg = false;
bool StopMove = false;

//clear
bool ClearFlg = false;

//ENDCamera
int end_cnt = 0;

// �p�[�e�B�N���X�e�[�^�X
float particle_x[PARTICLE_MAX];	// x���W
float particle_y[PARTICLE_MAX];	// y���W
float particle_z[PARTICLE_MAX];	// z���W
float particle_a[PARTICLE_MAX];	// �傫��
float particle_speed[PARTICLE_MAX];	// �ړ����x
float particle_vanish[PARTICLE_MAX]; // �p�[�e�B�N�������������鎞�̒l
bool particle_reset_flg[PARTICLE_MAX];	//ture

// �p�[�e�B�N���J�E���g
int p_time=0;

//==============================================================================
//!	@fn		GameInit
//!	@brief	�Q�[����������
//!	@param	�C���X�^���X�l
//!	@param	�E�C���h�E�n���h���l
//!	@param	�E�C���h�E�T�C�Y���i�␳�ς݂̒l�j
//!	@param	�E�C���h�E�T�C�Y�����i�␳�ς݂̒l�j
//!	@param�@	�t���X�N���[���t���O�@true ; �t���X�N���[���@false : �E�C���h�E
//!	@retval	true �����@false ���s
//==============================================================================
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	bool sts;
	srand((unsigned)time(NULL)); //�����h�֐��@�p�[�e�B�N���̍��W�Ɏg�p

	g_DXGrobj = new CDirectXGraphics();	// DirectX Graphics�I�u�W�F�N�g����
	sts = g_DXGrobj->Init(hwnd, fullscreen, width, height);	// DirectX Graphics�I�u�W�F�N�g������

	if (!sts){
		MessageBox(hwnd, "ERROR!!", "DirectX �������G���[", MB_OK);
		return false;
	}

	//--------------------------------------------------------
	// �w�t�@�C���I�u�W�F�N�g����
	//--------------------------------------------------------
	//--------------------------------------------------------
	//�X�J�C�h�[��
	//--------------------------------------------------------
	g_DXXFileObj_skydome = new CDirect3DXFile();
	sts = g_DXXFileObj_skydome->LoadXFile("assets/skydome.x", g_DXGrobj->GetDXDevice());

	//--------------------------------------------------------
	//stage�u���b�N
	//--------------------------------------------------------
	g_DXXFileObj = new CDirect3DXFile();
	sts = g_DXXFileObj->LoadXFile("assets/cube.x", g_DXGrobj->GetDXDevice());

	//--------------------------------------------------------
	//hitbox�u���b�N
	//--------------------------------------------------------
	g_DXXFileObj_hit = new CDirect3DXFile();
	sts = g_DXXFileObj_hit->LoadXFile("assets/hitbox.x", g_DXGrobj->GetDXDevice());

	//--------------------------------------------------------
	//�v���C���[�̓����蔻��
	//--------------------------------------------------------
	g_DXXFileObj_player = new CDirect3DXFile();
	sts = g_DXXFileObj_player->LoadXFile("assets/player_hit.x", g_DXGrobj->GetDXDevice());

	//--------------------------------------------------------
	//�v���C���[�̓����蔻��
	//--------------------------------------------------------
	g_DXXFileObj_moon = new CDirect3DXFile();
	sts = g_DXXFileObj_moon->LoadXFile("assets/moon.x", g_DXGrobj->GetDXDevice());

	//--------------------------------------------------------
	//�L�����N�^�[
	//--------------------------------------------------------

	sts = g_oneskin.LoadXFile("assets/girl/girl.x", g_DXGrobj->GetDXDevice());

	if (!sts){
		MessageBox(hwnd, "ERROR!!", "girl.x �ǂݍ��݃G���[", MB_OK);
		return false;
	}
	// �A�j���[�V�����Z�b�g���O�ɂ���
	g_oneskin.ChangeAnimation(g_NowAnimSet);
	//--------------------------------------------------------
	// �w�t�@�C���I�u�W�F�N�g���� END
	//--------------------------------------------------------


	if (!sts){
		MessageBox(hwnd, "ERROR!!", "cube3.x �ǂݍ��݃G���[", MB_OK);
		MessageBox(hwnd, "ERROR!!", "skydome.x �ǂݍ��݃G���[", MB_OK);
		return false;
	}

	// �v���W�F�N�V�����ϊ��s��쐬
	D3DXMatrixPerspectiveFovLH(&g_MatProjection,
		D3DX_PI / 2,					// ����p
		(float)width / (float)height,	// �A�X�y�N�g��
		1.0f,							// �j�A�v���[��
		5000.0f);						// �t�@�[�v���[��

	D3DXMatrixOrthoLH(&g_MatOrthor,
		width,
		height,
		1.f,
		1000.f);

	// �ˉe�ϊ��s����Œ�p�C�v���C���փZ�b�g
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);
	// �y�o�b�t�@�L��
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ���C�g�L��
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_LIGHTING, true);
	// �J�����O������
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// �����Z�b�g
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	// �e�N�X�`���[�Z�b�g
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/StageSelect.png", &select_tex);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/Star.png", &star_tex);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/Star_ora.png", &star_tex_ora);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/Star_whi.png", &star_tex_whi);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/select_button.png", &serect_button);
	//D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/test.png", &serect_button);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/select_button_r.png", &serect_button_r);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "tex/select_button_l.png", &serect_button_l);

	// DirectT Input������
	InitInput(hinst, hwnd);

	//----------------------------------------------------------------
	//  �O���[�o���ϐ�������
	//----------------------------------------------------------------
	for (int i = 0; i < M_BOX_NUM; i++){
		Hit_BoxFlg[i] = false;
		hit_count[i] = 0;
		Clear_Obj[i] = false;
	}

	// �p�[�e�B�N���̍��W�̏�����
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
	// COBB������
	//----------------------------------------------------------------
	//�X�e�[�Wfor (int n = 0; n < STAGENUM_Y; n++){
	
	//������
	for (int i = 0; i < M_BOX_NUM; i++){

		m_box_obj[i].Init(g_DXXFileObj->GetMesh(), g_DXGrobj->GetDXDevice());
	}

	//�v���C���[
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
	//�f�o�b�O�t�H���g ������
	d_font.CreateFontA(g_DXGrobj->GetDXDevice());

	//----------------------------------------------------------------
	// Init [END]
	//----------------------------------------------------------------


	//�{�[����BBOX(������)
	g_oneskin.InitBBox(g_DXGrobj->GetDXDevice(), g_oneskin.GetFrameRoot());

	// �C�x���g�n���h������
	g_hEventHandle = CreateEvent(NULL, false, false, NULL);
	if (g_hEventHandle == NULL){
		MessageBox(hwnd, "ERROR!!", "CreateEvent �G���[", MB_OK);
		return false;
	}
	// �X���b�h����(�Q�[�����C��)
	g_gamemainthread = std::thread(GameMain);

	return	true;
}


//----------------------------------------------------------------------------------------------
// �G���h�J���������Z�b�g
//----------------------------------------------------------------------------------------------
void ResetCamera(CameraPos* c_pos){

	end_cnt = 0;
	c_pos->angle.y = 0;
	c_pos->Init();
}


//==============================================================================
//!	@fn		GameInput
//!	@brief	�Q�[�����͌��m����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameInput(){
	UpdateInput();				// ���͂����m����
}


//==============================================================================
//!	@fn		GameUpdate
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameUpdate(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos){

	Playerdirection(player,g_CameraNum); //�����ݒ�

	if (GetKeyboardTrigger(DIK_F5)){
		if (DebugFlg) DebugFlg = false;
		else if (DebugFlg == false) DebugFlg = true;
	}

	//������
	for (int i = 0; i < M_BOX_NUM; i++){

		MakeWorldMatrix(g_MatWorld, m_box->MatTotal[i], m_box->angle[i], m_box->trans[i]);
	}
	if (LoadFlg){

		MapLoad(m_box, g_GAMESTATUS, StageNum);
		LoadFlg = false;

	}

	//�o�E���f�B���O�{�b�N�X
	//������
	for (int i = 0; i < M_BOX_NUM; i++){

		m_box_obj[i].Update(m_box->MatTotal[i]);
	}

	//�L�����N�^�[
	MakeWorldMatrix(g_MatWorld, player->MatTotal, player->angle, player->trans);
	g_oneskin.UpdateFrameMatrices(g_oneskin.GetFrameRoot(), &player->MatTotal);

	//�v���C���[
	player_obj.Update(player->MatTotal);

	//�{�[����BBOX
	g_oneskin.UpdateBBox(g_oneskin.GetFrameRoot());

	//HItBox
	for (int i = 0; i < HITBOXNUM; i++){

		MakeWorldMatrix(hbox->MatTotal[i], hbox->angle[i], hbox->trans[i]);
	}


	SetPositionCase(player,hbox); //hitbox�̍��W�Ƀv���C���[���W������
	for (int i = 0; i < HITBOXNUM; i++){

		g_HitBox_obj[i].Update(hbox->MatTotal[i]);
	}

	//��
	MakeWorldMatrix(g_MatWorld, mn->MatTotal, mn->angle, mn->trans);
	moon_obj.Update(mn->MatTotal);

	if (g_GAMESTATUS == GAME){

		KeyInput(player, c_pos, m_box); //�L�[����
	}

	//�J����
	MakeWorldMatrix(g_MatWorld, c_pos->MatTotal, c_pos->angle, c_pos->trans); //�J�������W
	CameraAngle(player,c_pos); //�J�����A���O��

	//�X�J�C�h�[��
	MakeWorldMatrix(g_MatWorld, g_SkyMat, g_Skyangle, g_Skytrans);
}


//----------------------------------------------------------------------------------------------
// �X�^�[�g���
//----------------------------------------------------------------------------------------------
void StartRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn){
	HRESULT  hr;

	// �^�[�Q�b�g�o�b�t�@�̃N���A�A�y�o�b�t�@�̃N���A
	g_DXGrobj->GetDXDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	// �`��̊J�n���c�h�q�d�b�s�w�ɒʒm
	g_DXGrobj->GetDXDevice()->BeginScene();
	//---���`�揈����---

	//==================================================
	//�������������������� 3D�`�� ����������������������
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);

	DrawSkydome();
	DrawCube(player,m_box, mn);

	ParticleDraw();

	//==================================================
	//�������������������� 2D�`�� ����������������������
	//==================================================
	//g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatOrthor);

	

	//---���`�揈����---
	g_DXGrobj->GetDXDevice()->EndScene();	// �`��̏I����҂�
	hr = g_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// �o�b�N�o�b�t�@����v���C�}���o�b�t�@�֓]��
	if (hr != D3D_OK){
		g_DXGrobj->GetDXDevice()->Reset(&g_DXGrobj->GetDXD3dpp());
	}
}


//==============================================================================
//!	@fn		GameRender
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos){
	HRESULT  hr;

	// �^�[�Q�b�g�o�b�t�@�̃N���A�A�y�o�b�t�@�̃N���A
	g_DXGrobj->GetDXDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	// �`��̊J�n���c�h�q�d�b�s�w�ɒʒm
	g_DXGrobj->GetDXDevice()->BeginScene();

	//==================================================
	//�������������������� 3D�`�� ����������������������
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);

	//�X�J�C�h�[���`��
	DrawSkydome();
	//�X�e�[�W�A�������`��
	DrawCube(player,m_box, mn);
	//�L�����`��
	PlayerDraw();
	//HitBox
	//HitDraw(hbox);

	//�o�E���f�B���O�X�t�B�A�`��
	if (DebugFlg){

		DebugFont(player, m_box, hbox,c_pos);
		//Cobb_Draw(m_box);

	}
	// �p�[�e�B�N���`��
	ParticleDraw();

	//�f�o�b�O�t�H���g
	//DebugFont(stagebox, m_box,hbox);

	//==================================================
	//�������������������� 2D�`�� ����������������������
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatOrthor);

	TextureDarw();

	g_DXGrobj->GetDXDevice()->EndScene();	// �`��̏I����҂�
	hr = g_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// �o�b�N�o�b�t�@����v���C�}���o�b�t�@�֓]��
	if (hr != D3D_OK){
		g_DXGrobj->GetDXDevice()->Reset(&g_DXGrobj->GetDXD3dpp());
	}

	HitFunction(player,m_box, hbox,c_pos);
}


//----------------------------------------------------------------------------------------------
// ���U���g���
//----------------------------------------------------------------------------------------------
void EndRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn){

	HRESULT  hr;
	// �^�[�Q�b�g�o�b�t�@�̃N���A�A�y�o�b�t�@�̃N���A
	g_DXGrobj->GetDXDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	// �`��̊J�n���c�h�q�d�b�s�w�ɒʒm
	g_DXGrobj->GetDXDevice()->BeginScene();
	//---���`�揈����---

	//==================================================
	//�������������������� 3D�`�� ����������������������
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);

	DrawSkydome();
	DrawCube(player, m_box, mn);
	// �p�[�e�B�N���`��
	ParticleDraw();

	//---���`�揈����---
	g_DXGrobj->GetDXDevice()->EndScene();	// �`��̏I����҂�
	hr = g_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// �o�b�N�o�b�t�@����v���C�}���o�b�t�@�֓]��
	if (hr != D3D_OK){
		g_DXGrobj->GetDXDevice()->Reset(&g_DXGrobj->GetDXD3dpp());
	}
}

//----------------------------------------------------------------------------------------------
// �Z���N�g���
//----------------------------------------------------------------------------------------------
void selectRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos){

	HRESULT  hr;
	// �^�[�Q�b�g�o�b�t�@�̃N���A�A�y�o�b�t�@�̃N���A
	g_DXGrobj->GetDXDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	// �`��̊J�n���c�h�q�d�b�s�w�ɒʒm
	g_DXGrobj->GetDXDevice()->BeginScene();
	//---���`�揈����---
	//==================================================
	//�������������������� 3D�`�� ����������������������
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);
	DrawSkydome();
	DrawCube(player, m_box, mn);
	// �p�[�e�B�N���`��
	SerectButton();
	ParticleDraw();

	//==================================================
	//�������������������� 2D�`�� ����������������������
	//==================================================
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatOrthor);
	TextureDarw();
	//DebugFont(player, m_box, hbox, c_pos);
	//---���`�揈����---
	g_DXGrobj->GetDXDevice()->EndScene();	// �`��̏I����҂�
	hr = g_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// �o�b�N�o�b�t�@����v���C�}���o�b�t�@�֓]��
	if (hr != D3D_OK){
		g_DXGrobj->GetDXDevice()->Reset(&g_DXGrobj->GetDXD3dpp());
	}
}


//==============================================================================
//!	@fn		GameMain
//!	@brief	�Q�[�����[�v����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameMain()
{
	PLAYER* player = new PLAYER;
	MOVEBOX*  m_box = new MOVEBOX;	   //������
	HITBOX* hbox = new HITBOX;		   //hitbox
	MOON* mn = new MOON;
	CameraPos* c_pos = new CameraPos; // �J�����̍��W

	while (1){
		DWORD sts = WaitForSingleObject(g_hEventHandle, 1000);	// �C�x���g�t���O���Z�b�g�����̂�҂i1000ms�Ń^�C���A�E�g�j
		if (sts == WAIT_FAILED){

			break;
		}
		else if (sts == WAIT_TIMEOUT){
			if (g_EndFlag){

				break;
			}
			continue;
		}

		GameInput();		// ����
		GameUpdate(player, m_box, hbox, mn,c_pos);	// �X�V

		switch (g_GAMESTATUS){	// �`��
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

			//�f�o�b�O�p
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
//!	@brief	�Q�[���I������
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameExit()
{
	g_gamemainthread.join();					// �Q�[�����C���X���b�h�̏I����҂�
	CloseHandle(g_hEventHandle);				// �C�x���g�n���h���N���[�Y

	if (g_DXXFileObj != NULL){

		delete g_DXXFileObj;					//	X�t�@�C���I�u�W�F�N�g�̉��
	}

	if (g_DXXFileObj_skydome != NULL){

		delete g_DXXFileObj_skydome;					//	X�t�@�C���I�u�W�F�N�g�̉��
	}

	if (g_DXXFileObj_hit != NULL){

		delete g_DXXFileObj_hit;					//	X�t�@�C���I�u�W�F�N�g�̉��
	}

	if (g_DXGrobj != NULL){

		g_DXGrobj->Exit();
		// DirectX�̃O���t�B�b�N�I�u�W�F�N�g�̏I������
	}

	if (g_DXGrobj != NULL){						// DirectX�̃O���t�B�b�N�I�u�W�F�N�g�̉��

		delete g_DXGrobj;
		g_DXGrobj = NULL;
	}

	if (g_DXXFileObj_moon != NULL){

		delete g_DXXFileObj_moon;					//	X�t�@�C���I�u�W�F�N�g�̉��
	}

	// DirectT Input�I������
	UninitInput();
}

//==============================================================================
//!	@fn		GameSetEvent
//!	@brief	�Q�[�����[�v���̃C�x���g�I�u�W�F�N�g���Z�b�g����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameSetEvent(){
	if (!g_EndFlag){

		SetEvent(g_hEventHandle);	// �I���t���O���Z�b�g����Ă��Ȃ���΃C�x���g���Z�b�g����
	}
}

//==============================================================================
//!	@fn		GameSetEndFlag
//!	@brief	�Q�[�����[�v�̏I���t���O���Z�b�g����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameSetEndFlag(){

	g_EndFlag = true;				// �I���t���O���I���ɂ���
}

//******************************************************************************
//	End of file.
//******************************************************************************


//----------------------------------------------------------------------------------------------
// �f�o�b�O�t�H���g
//----------------------------------------------------------------------------------------------
void DebugFont(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, CameraPos* c_pos){

	char str[256];

	if (g_GAMESTATUS == SELECT){

		sprintf(str, "[�J�������W]");
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

		//�������Ă����
		switch (g_CameraNum){
		case 0:
			sprintf(str, "�O (0)");
			break;

		case 1:
			sprintf(str, "�E (1)");
			break;

		case 2:
			sprintf(str, "�� (2)");
			break;

		case 3:
			sprintf(str, "�� (3)");
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

			// �v���C���[���
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


			// �J�������
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

			// �v���C���[���
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


			// �J�������
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
// �J�����A���O��
//----------------------------------------------------------------------------------------------
void CameraAngle(PLAYER* player, CameraPos* c_pos){

	if (CameraStatus == START){

		//�^�C�g�����̃J�����A���O��
		D3DXMatrixLookAtLH(&g_MatView,
			&D3DXVECTOR3(50.0f, -20.0f, 0.0f),		// ���_
			&D3DXVECTOR3(2.0f, -20.0f, -10.0f),		// �����_
			&D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	}
	else if (CameraStatus == SELECT){

		//�X�e�[�W�Z���N�g���̃J�����A���O��

		c_pos->g_up.x = c_pos->MatTotal._21;
		c_pos->g_up.y = c_pos->MatTotal._22;
		c_pos->g_up.z = c_pos->MatTotal._23;

		c_pos->g_lookat.x = c_pos->MatTotal._41;
		c_pos->g_lookat.y = c_pos->MatTotal._42;
		c_pos->g_lookat.z = c_pos->MatTotal._43;

		c_pos->g_eye.x = c_pos->g_lookat.x + c_pos->MatTotal._31 * 30;
		c_pos->g_eye.y = c_pos->g_lookat.y + c_pos->MatTotal._32 * 30;
		c_pos->g_eye.z = c_pos->g_lookat.z + c_pos->MatTotal._33 * 30;

		// �J�����ϊ��s��쐬
		D3DXMatrixLookAtLH(&g_MatView,
			&c_pos->g_eye,				// ���_
			&c_pos->g_lookat,			// �����_
			&c_pos->g_up);				// �����

		c_pos->angle.y = -0.5;	//�J�����̉�]

		if (GetKeyboardTrigger(DIK_RETURN)){
			c_pos->angle.y = 0.0f;
		}
	}
	else if (CameraStatus == GAME){

		//�Q�[�����̃J�����A���O��
		PositionCorrection(player); //�v���C���[�̍��W����𒼂�

		/*if (player->MatTotal._22 == 1){

			//�p�^�[��0
			CameraAngleNum_0(player,c_pos);
		}

		if (player->MatTotal._23 == -1){

			//�p�^�[��1
			CameraAngleNum_1(player, c_pos);
		}

		if (player->MatTotal._22 == -1){

			//�p�^�[��2
			CameraAngleNum_2(player, c_pos);
		}

		if (player->MatTotal._23 == 1){

			//�p�^�[��3
			CameraAngleNum_3(player, c_pos);
		}

		if (player->MatTotal._21 == 1){

			//�p�^�[��4
			CameraAngleNum_4(player, c_pos);
		}

		if (player->MatTotal._21 == -1){

			//�p�^�[��5
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

		// �J�����ϊ��s��쐬
		D3DXMatrixLookAtLH(&g_MatView,
			&c_pos->g_eye,				// ���_
			&c_pos->g_lookat,			// �����_
			&c_pos->g_up);				// �����
	}
	else if (CameraStatus == END){

		//�Q�[���N���A�[���̃J�����A���O��
		if (end_cnt > 0 && end_cnt<50){
			//���o�J����
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

		// �J�����ϊ��s��쐬
		D3DXMatrixLookAtLH(&g_MatView,
			&c_pos->g_eye,				// ���_
			&c_pos->g_lookat,			// �����_
			&c_pos->g_up);				// �����
		end_cnt++;
	}


	// �J�����s����Œ�p�C�v���C���փZ�b�g
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_VIEW, &g_MatView);
}

//�p�^�[��0
void CameraAngleNum_0(PLAYER* player, CameraPos* c_pos){

	/*if ((player->MatTotal._11 == 1) && (player->MatTotal._33 == 1)){

		// �O
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

		//���
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

		//��
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

		//�E
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


//�p�^�[��1
void CameraAngleNum_1(PLAYER* player, CameraPos* c_pos){

	if ((player->MatTotal._11 == 1) && (player->MatTotal._32 == 1)){

		// �O
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

		//���
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

		//��
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

		//�E
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


//�p�^�[��2
void CameraAngleNum_2(PLAYER* player, CameraPos* c_pos){
	if ((player->MatTotal._11 == 1) && (player->MatTotal._33 == -1)){

		// �O
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

		//���
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

		//��
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

		//�E
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


//�p�^�[��3
void CameraAngleNum_3(PLAYER* player, CameraPos* c_pos){

	if ((player->MatTotal._11 == 1) && (player->MatTotal._32 == -1)){

		// �O
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

		//���
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

		//��
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

		//�E
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


//�p�^�[��4
void CameraAngleNum_4(PLAYER* player, CameraPos* c_pos){

	if ((player->MatTotal._12 == -1) && (player->MatTotal._33 == 1)){

		// �O
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

		//���
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

		//��
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

		//�E
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


//�p�^�[��5
void CameraAngleNum_5(PLAYER* player, CameraPos* c_pos){

	if ((player->MatTotal._12 == 1) && (player->MatTotal._33 == 1)){

		// �O
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

		//���
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

		//��
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

		//�E
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
// �L�[����
//----------------------------------------------------------------------------------------------
void KeyInput(PLAYER* player, CameraPos* c_pos, MOVEBOX* m_box){

	if (g_TurnCount_Y == 0){
		//�v���C���[����
		if (GetKeyboardPress(DIK_UP) && !StopFlg){


			if (g_status == STAND){

				g_Aniflg = false;
			}
			//��ړ�
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
			//���ړ�
			g_status = MOVE_DOWN;
		}
		else if (GetKeyboardRelease(DIK_DOWN)){

			if (g_status == MOVE_DOWN){

				g_Aniflg = false;
			}
			g_status = STAND;
		}*/
	}

	if (!GetKeyboardPress(DIK_UP)){ //�ړ����Ă��Ȃ����A���E�Ɍ�����

		if (GetKeyboardTrigger(DIK_RIGHT)){

			if (g_status == STAND){

				g_Aniflg = false;
			}
			//�E��]
			g_status = MOVE_RIGHT;
		}
		else if (g_TurnCount_Y > 10){

			if (g_status == MOVE_RIGHT){

				g_Aniflg = false;
			}
			g_status = STAND; //������Ԃɂ���
			g_TurnCount_Y = 0; //��]�̃J�E���g�̃��Z�b�g
		}

		if (GetKeyboardTrigger(DIK_LEFT)){

			if (g_status == STAND){

				g_Aniflg = false;
			}
			//����]
			g_status = MOVE_LEFT;
		}
		else if (g_TurnCount_Y > 10){

			if (g_status == MOVE_LEFT){

				g_Aniflg = false;
			}
			g_status = STAND; //������Ԃɂ���
			g_TurnCount_Y = 0; //��]�̃J�E���g�̃��Z�b�g
		}
	}

	PlayerStatus(player, c_pos, g_status);
}


//----------------------------------------------------------------------------------------------
// �v���C���[�̏�ԑJ��
//----------------------------------------------------------------------------------------------
void PlayerStatus(PLAYER* player, CameraPos* c_pos, int g_status){

	//�v���C���[�̍s��
	switch (g_status){

	case STAND:
		g_NowAnimSet = 5;
		AnimationSet(g_NowAnimSet);

		//�v���C���[
		player->trans.x = 0;
		player->trans.z = 0;

		//�v���C���[��Ԃ̏�����
		player_up_move = false;
		player_down_move = false;
		player_right_move = false;
		player_left_move = false;


		//�J����
		c_pos->trans.x = 0;
		c_pos->trans.y = 0;
		c_pos->trans.z = 0;
		break;

	case MOVE_UP:
		g_NowAnimSet = 4;
		AnimationSet(g_NowAnimSet);

		//�v���C���[
		player->trans.z = -0.1f*baisoku;
		player_up_move = true;

		//�J����
		c_pos->trans.z = -0.1f*baisoku;
		break;

	case MOVE_DOWN:
		g_NowAnimSet = 4;
		AnimationSet(g_NowAnimSet);

		//�v���C���[
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

		// ���[�V�����X�V
		g_oneskin.GetAnimationController()->AdvanceTime(0.02, NULL);
	}
	else {

		//�A�j���[�V�����I��
		g_Aniflg = false;
	}
}


//----------------------------------------------------------------------------------------------
// �A�j���[�V�����̕ύX
//----------------------------------------------------------------------------------------------
void AnimationSet(int g_NowAnimSet){

	if (!g_Aniflg){

		g_oneskin.ChangeAnimation(g_NowAnimSet);	// ���݂̃A�j���[�V����
		g_Aniflg = true;
	}
}


//----------------------------------------------------------------------------------------------
// �X�e�[�W�ɔz�u���Ă�I�u�W�F�N�g�̕`��
//----------------------------------------------------------------------------------------------
void DrawCube(PLAYER* player, MOVEBOX* m_box, MOON* mn){

	//������
	for (int i = 0; i < m_box->BoxNum; i++){

		if (g_GAMESTATUS == GAME){

			if (player->MatTotal._22 >= 1){
				//�p�^�[��
				//     3
				//     2
				//   4 1 5
				//     0

				// �p�^�[��0
				if (player->MatTotal._11 >= 1 && player->MatTotal._33 >= 1){

					// �O
					if ((player->MatTotal._43 + 1.5f) > m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._11 <= -1 && player->MatTotal._33 <= -1){

					// ���
					if ((player->MatTotal._43 - 1.5f) < m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._13 <= -1 && player->MatTotal._31 >= 1){

					// �E
					if ((player->MatTotal._41 + 1.5f) > m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._13 >= 1 && player->MatTotal._31 <= -1){

					// ��
					if ((player->MatTotal._41 - 1.0f) < m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else{

					// �؂�ւ����̕`��
					// �������ȊO���ƑS�ď����邩��
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
				}
			}
			else if (player->MatTotal._23 <= -1){
				

				// �p�^�[��1
				if (player->MatTotal._11 >= 1 && player->MatTotal._32 >= 1){

					// �O
					if ((player->MatTotal._42 + 1.5f) > m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._11 <= -1 && player->MatTotal._32 <= -1){

					// ���
					if ((player->MatTotal._42 - 1.5f) < m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._12 <= -1 && player->MatTotal._31 >= 1){

					// �E
					if ((player->MatTotal._41 + 1.5f) > m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._12 >= 1 && player->MatTotal._31 <= -1){

					// ��
					if ((player->MatTotal._41 - 1.0f) < m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else{

					// �؂�ւ����̕`��
					// �������ȊO���ƑS�ď����邩��
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
				}
			}
			else if (player->MatTotal._22 <= -1){


				// �p�^�[��2
				if (player->MatTotal._11 >= 1 && player->MatTotal._33 <= -1){

					// �O
					if ((player->MatTotal._43 - 1.5f) < m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._11 <= -1 && player->MatTotal._33 >= 1){

					// ���
					if ((player->MatTotal._43 + 1.5f) > m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._13 >= 1 && player->MatTotal._31 >= 1){

					// �E
					if ((player->MatTotal._41 + 1.5f) > m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._13 <= -1 && player->MatTotal._31 <= -1){

					// ��
					if ((player->MatTotal._41 - 1.0f) < m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else{

					// �؂�ւ����̕`��
					// �������ȊO���ƑS�ď����邩��
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
				}
			}
			else if (player->MatTotal._23 >= 1){


				// �p�^�[��3
				if (player->MatTotal._11 >= 1 && player->MatTotal._32 <= -1){

					// �O
					if ((player->MatTotal._42 - 1.5f) < m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._11 <= -1 && player->MatTotal._32 >= 1){

					// ���
					if ((player->MatTotal._42 + 1.5f) > m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._12 >= 1 && player->MatTotal._31 >= 1){

					// �E
					if ((player->MatTotal._41 + 1.5f) > m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._12 <= -1 && player->MatTotal._31 <= -1){

					// ��
					if ((player->MatTotal._41 - 1.0f) < m_box->MatTotal[i]._41){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else{

					// �؂�ւ����̕`��
					// �������ȊO���ƑS�ď����邩��
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
				}
			}
			else if (player->MatTotal._21 >= 1){


				// �p�^�[��4
				if (player->MatTotal._12 <= -1 && player->MatTotal._33 >= 1){

					// �O
					if ((player->MatTotal._43 + 1.5f) > m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._12 >= 1 && player->MatTotal._33 <= -1){

					// ���
					if ((player->MatTotal._43 - 1.5f) < m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._13 <= -1 && player->MatTotal._32 >= -1){

					// �E
					if ((player->MatTotal._42 - 1.5f) < m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._13 >= 1 && player->MatTotal._32 >= 1){

					// ��
					if ((player->MatTotal._42 + 1.5f) > m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else{

					// �؂�ւ����̕`��
					// �������ȊO���ƑS�ď����邩��
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
				}
			}
			else if (player->MatTotal._21 <= -1){


				// �p�^�[��5
				if (player->MatTotal._12 >= 1 && player->MatTotal._33 >= 1){

					// �O
					if ((player->MatTotal._43 + 1.5f) > m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._12 <= -1 && player->MatTotal._33 <= -1){

					// ���
					if ((player->MatTotal._43 - 1.5f) < m_box->MatTotal[i]._43){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._13 <= -1 && player->MatTotal._32 >= 1){

					// �E
					if ((player->MatTotal._42 + 1.5f) > m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else if (player->MatTotal._13 >= 1 && player->MatTotal._32 <= -1){

					// ��
					if ((player->MatTotal._42 - 1.5f) < m_box->MatTotal[i]._42){

						g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
						g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
					}
				}
				else{

					// �؂�ւ����̕`��
					// �������ȊO���ƑS�ď����邩��
					g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
					g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
				}
			}

		}
		else{

			g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_box->MatTotal[i]);	// ���[���h�ϊ��s����Z�b�g
			g_DXXFileObj->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
		}
	}

	if (g_GAMESTATUS == GAME){

		g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &mn->MatTotal);	// ���[���h�ϊ��s����Z�b�g
		g_DXXFileObj_moon->Draw(g_DXGrobj->GetDXDevice());	// �w�t�@�C���`��
	}
}


//----------------------------------------------------------------------------------------------
// �X�J�C�h�[���̕`��
//----------------------------------------------------------------------------------------------
void DrawSkydome(){

	//�X�J�C�h�[��
	D3DXMatrixIdentity(&g_SkyMat);
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &g_SkyMat);   // ���[���h�ϊ��s����Z�b�g 
	g_DXXFileObj_skydome->Draw(g_DXGrobj->GetDXDevice());       // �w�t�@�C���`��

}


//----------------------------------------------------------------------------------------------
// �v���C���[�̕`��
//----------------------------------------------------------------------------------------------
void PlayerDraw(){

	g_oneskin.DrawFrame(g_DXGrobj->GetDXDevice(), g_oneskin.GetFrameRoot());	// �����X�L���`��
}


//----------------------------------------------------------------------------------------------
// COBB�̕`��i�u���b�N�̓����蔻��j
//----------------------------------------------------------------------------------------------
void Cobb_Draw(MOVEBOX* m_box){

	//COBB�`��
	//������OBB
	for (int i = 0; i < m_box->BoxNum; i++){

		m_box_obj[i].Draw(g_DXGrobj->GetDXDevice());
	}

	//�v���C���[
	//g_oneskin.DrawBBox(g_DXGrobj->GetDXDevice(), g_oneskin.GetFrameRoot()); //�{�[��
	//player_obj.Draw(g_DXGrobj->GetDXDevice());

	//hitbox
	for (int i = 0; i < HITBOXNUM; i++){

		g_HitBox_obj[i].Draw(g_DXGrobj->GetDXDevice());
	}

	moon_obj.Draw(g_DXGrobj->GetDXDevice());
}


//----------------------------------------------------------------------------------------------
// �����蔻��̏���
//----------------------------------------------------------------------------------------------
void HitFunction(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, CameraPos* c_pos){

	//�t���O������
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

	//�v���C���[��������
	if (hit_Player_Stage){

		//(1)
		player->trans.y = 0;
		g_HitGroundFlg = true;
		g_TurnCount_Z = 0;
		StopFlg = false;

		//�J�����̂���𒼂�
		test(player, c_pos);
	}
	else{

		StopFlg = true; //�{�^�����~�߂�
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


	// �v���C���[�ƃX�e�[�W�i���̓����蔻��j
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


	//�v���C���[�ƃ{�b�N�X�̓����蔻��
	for (int i = 0; i < m_box->BoxNum; i++){

		if (hbox->HitFlg[i]){

			if (GetKeyboardPress(DIK_SPACE)){

				if (GetKeyboardPress(DIK_UP)){

					if (player->MatTotal._22 == 1){

						// �{�b�N�X����������
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
						// �{�b�N�X�����������I��
					}
				}
			}
			else{

				// �v���C���[�Ɣ��������������A�v���C���[����]������
				HitTurn(player, c_pos, hbox);
			}
		}
		else if (!hbox->HitFlg[i]){

			if (g_HitGroundFlg){

				// �v���C���[���n�ʂɓ����������A�����̏������~�߂�
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

		//�p�^�[��0
		c_pos->MatTotal._41 = player->MatTotal._41;
		c_pos->MatTotal._42 = player->MatTotal._42 + 4;
		c_pos->MatTotal._43 = player->MatTotal._43;
	}


	if (player->MatTotal._23 == -1){

		//�p�^�[��1
		c_pos->MatTotal._41 = player->MatTotal._41;
		c_pos->MatTotal._42 = player->MatTotal._42;
		c_pos->MatTotal._43 = player->MatTotal._43 - 4;
	}


	if (player->MatTotal._22 == -1){

		//�p�^�[��2
		c_pos->MatTotal._41 = player->MatTotal._41;
		c_pos->MatTotal._42 = player->MatTotal._42 - 4;
		c_pos->MatTotal._43 = player->MatTotal._43;
	}


	if (player->MatTotal._23 == 1){

		//�p�^�[��3
		c_pos->MatTotal._41 = player->MatTotal._41;
		c_pos->MatTotal._42 = player->MatTotal._42;
		c_pos->MatTotal._43 = player->MatTotal._43 + 4;
	}


	if (player->MatTotal._21 == 1){

		//�p�^�[��4
		c_pos->MatTotal._41 = player->MatTotal._41 + 4;
		c_pos->MatTotal._42 = player->MatTotal._42;
		c_pos->MatTotal._43 = player->MatTotal._43;
	}


	if (player->MatTotal._21 == -1){

		//�p�^�[��5
		c_pos->MatTotal._41 = player->MatTotal._41 - 4;
		c_pos->MatTotal._42 = player->MatTotal._42;
		c_pos->MatTotal._43 = player->MatTotal._43;
	}
}


void PlayerTurn(PLAYER* player, CameraPos* c_pos, int g_status){

	if (g_GAMESTATUS == GAME){

		//���E��]
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

	//��
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

	//�c��](�u���b�N�ɓ���������)
	//HitBox�������������̉�]����
	if (hbox->Count == 0){

		StopFlg = true; //�{�^�����~�߂�
		//�v���C���[
		player->angle.x = 90;
		c_pos->angle.x = 90;
		player->trans.y = 1.0;
		hbox->Count++;
	}
	else{
		StopFlg = false;
		//�v���C���[
		player->angle.x = 0;
		c_pos->angle.x = 0;
		player->trans.y = 0;
		hbox->Count = 0;
		HitInitFlg = true;
		g_HitGroundFlg = false;
	}
}


void DownTurn(PLAYER* player, CameraPos* c_pos){

	//�c��](�u���b�N�ɓ�����Ȃ�������)
	if (g_status == MOVE_UP){

		if (g_TurnCount_Z == 0){

			//�v���C���[
			player->angle.x = -90; 
			c_pos->angle.x = -90;
			g_TurnCount_Z++;
		}
		else{

			//�v���C���[
			player->angle.x = 0;
			player->trans.y = 0;

			//�J����
			c_pos->angle.x = 0;
		}
	}

	if (g_status == MOVE_DOWN){

		if (g_TurnCount_Z == 0){

			//�v���C���[
			//g_PlayerTrans.y = -1.0f;
			player->angle.x = 90;
			g_TurnCount_Z++;
		}
		else{

			//�v���C���[
			player->angle.x = 0;
			player->trans.y = 0;
		}
	}
}


//----------------------------------------------------------------------------------------------
// COBB�̕`��i�v���C���[�̓����蔻��j
//----------------------------------------------------------------------------------------------
void HitDraw(HITBOX* hbox){

	for (int i = 0; i < HITBOXNUM; i++){
		//for (int i = 0; i < 1; i++){

		g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &hbox->MatTotal[i]);
		g_DXXFileObj_hit->Draw(g_DXGrobj->GetDXDevice());
	}
}




//----------------------------------------------------------------------------------------------
// 2D�e�N�X�`���̕`��ݒ�
//----------------------------------------------------------------------------------------------
void TextureDarw(){
	
	//2D�e�N�X�`���[���W
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
// 2D�e�N�X�`���ł̃p�[�e�B�N���`��
//----------------------------------------------------------------------------------------------
void ParticleDraw(){

	for (int i = 0; i < PARTICLE_NUM; i++){

		if (particle_reset_flg[i] == true){

			//2D�e�N�X�`���[���W
			particle_x[i] = 100.0f + rand() % 1200;
			particle_y[i] = -(rand() % 200);
			particle_z[i] = 0.0f;
			particle_a[i] = 5.0f; // �p�[�e�B�N���̑傫���w��
			particle_speed[i] = rand() % 3; // �������x
			particle_vanish[i] = rand() % 5 +2; // �p�[�e�B�N���������l(�傫���قǂ���������)
			particle_reset_flg[i] = false;
		}
	}

	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);

	for (int i = 0; i < PARTICLE_NUM; i++){

		int color_pass = rand() % 7;
		//int color_pass = 0;

		// �p�[�e�B�N�����W
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

			//���W�̈ړ�
			particle_y[i] += 3.0f + particle_speed[i]; //������
			particle_x[i] -= 2.0f + particle_speed[i]; //������
			particle_a[i] -= particle_vanish[i]/100; //�p�[�e�B�N���̑傫�������X�ɏ���������
			if ((particle_y[i]>SCREEN_Y + particle_a[i]) || particle_a[i] < 0){

				particle_reset_flg[i] = true;	//���W��߂�
			}
		} 
	}
}


//----------------------------------------------------------------------------------------------
// 2D�e�N�X�`���[�ł̃{�^���\��
//----------------------------------------------------------------------------------------------
void SerectButton(){

	//2D�e�N�X�`���[���W
	float tex_x = SCREEN_X / 2;
	float tex_y = SCREEN_Y / 2;
	float tex_z = 0.0f;
	float tex_a = 200.0f;

	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);
	//D3DCOLOR color2 = D3DCOLOR_XRGB(0, 0, 0);

	if (GetKeyboardPress(DIK_RIGHT)){

		// �E�{�^�������������̏���
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

		// ���{�^�������������̏���
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

		// �{�^���������ĂȂ����̏���

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