//*****************************************************************************
//!	@file	game.h
//!	@brief	�Q�[������
//!	@note	
//!	@author
//*****************************************************************************
#pragma once
#include "../Header/Player.h"		 // �v���C���[
#include "../Header/MoveBox.h"		 // �u���b�N
#include "../Header/HitBox.h"		 // �����蔻��p
#include "../Header/moon.h"			 // ���i�S�[���j
#include "../Header/stage.h"		 // 
#include "../Header/CameraPos.h"
#include "../Header/input.h"

#define		SCREEN_X		1280
#define		SCREEN_Y		720
#define		STAGE_NUM		4

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);
void GameMain();
void GameExit();
void GameSetEvent();
void GameSetEndFlag();
void GameInput();
void GameUpdate(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos);
void GameRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos); //�`��
//�ǉ�
//void AddInit();
void DebugFont(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, CameraPos* c_pos); //�f�o�b�O
void KeyInput(PLAYER* player, CameraPos* c_pos, MOVEBOX* m_box);
void CameraAngle(PLAYER* player, CameraPos* c_pos); //�J����


//Draw
void DrawCube(PLAYER* player, MOVEBOX* m_box, MOON* mn);
void DrawSkydome();
void Cobb_Draw(MOVEBOX* m_box);
void PlayerDraw();

//�����蔻��
void HitFunction(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, CameraPos* c_pos);

//�v���C���[�̏��
void PlayerStatus(PLAYER* player, CameraPos* c_pos, int g_status);
void AnimationSet(int g_NowAnimSet);

//��]
void DownTurn(PLAYER* player, CameraPos* c_pos);	//�n�ʂ��Ȃ��������̃v���C���[�̉�]
void HitTurn(PLAYER* player,CameraPos* c_pos,HITBOX* hbox);	//�u���b�N�ɓ����������̃v���C���[�̉�]
void PlayerTurn(PLAYER* player, CameraPos* c_pos, int g_status);	//�v���C���[�̍��E��]

//�X�e�[�W�̃v���C���[�ƌ��̔z�u
void InitMat(PLAYER* player, MOON* mn, CameraPos* c_pos,int StageNum);
void InitCameraMat(CameraPos* c_pos);
//�J�����Ƀv���C���[�̍s������ʒu�Y���𒼂�
void CameraMatCorrection(PLAYER* player, CameraPos* c_pos);

//�J����
//�p�^�[��1
void CameraAngleNum_0(PLAYER* player, CameraPos* c_pos);
void CameraAngleNum_1(PLAYER* player, CameraPos* c_pos);
void CameraAngleNum_2(PLAYER* player, CameraPos* c_pos);
void CameraAngleNum_3(PLAYER* player, CameraPos* c_pos);
//�p�^�[��2			 
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

//�L�����̌���
void Playerdirection(PLAYER* player, int g_CameraNum);

//�J��
void StartRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn); //�`��
void EndRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn);
void selectRender(PLAYER* player, MOVEBOX* m_box, HITBOX* hbox, MOON* mn, CameraPos* c_pos);

void ResetCamera(CameraPos* c_pos);

//�ʒu�̏C��
void PositionCorrection(PLAYER* player);

// �f�o�b�O�p
void DebugCameraAngle(PLAYER* player, CameraPos* c_pos);
void test(PLAYER* player,CameraPos* c_pos);


// 2D�e�N�X�`��
void TextureDarw();
void SerectButton();

// �p�[�e�B�N���e�N�X�`��
void ParticleDraw();
//******************************************************************************
//	End of file.
//******************************************************************************
