//#include "../Header/game.h"
#define _CRT_SECURE_NO_WARNINGS
#include "../Header/stage.h"

//----------------------------------------------------------------------------------------------
// 2�X�^�[�g�ƃ��U���g
//----------------------------------------------------------------------------------------------
TITLE_NAME TName[2] = {
	{ "csv/menu/Cuve.csv" },
	{ "csv/menu/clear.csv" }
};

//----------------------------------------------------------------------------------------------
// �X�e�[�W�i���o�[
//----------------------------------------------------------------------------------------------
STAGE_NAME SName[STAGE_MAX] = {
	{ "csv/map/stage1.csv" },
	{ "csv/map/stage2.csv" },
	{ "csv/map/stage3.csv" },
	{ "csv/map/stage4.csv" },
	{ "csv/map/stage5.csv" }
};

//----------------------------------------------------------------------------------------------
// �X�e�[�W�̏��
// C_START:�Q�[���X�^�[�g�@ C_SELECT:�Q�[���Z���N�g�@ C_GAME:�Q�[�����C���@ C_END:�Q�[�����U���g
//----------------------------------------------------------------------------------------------
enum{ C_START, C_SELECT, C_GAME, C_END };


//----------------------------------------------------------------------------------------------
// �}�b�v�̃��[�h
//----------------------------------------------------------------------------------------------
void MapLoad(MOVEBOX* m_box, int status, int s_num){

	FILE* fp;
	if (status == C_START){

		//�Q�[���X�^�[�g��
		fp = fopen(TName[0].name, "r");
		if (fp == NULL) {

			exit(1);
		}

		fscanf(fp, "%d", &m_box->BoxNum);	//�{�b�N�X���̓ǂݍ���

		for (int i = 0; i < m_box->BoxNum; i++){

			fscanf(fp, "%f,%f,%f\n", &m_box->MatTotal[i]._41, &m_box->MatTotal[i]._42, &m_box->MatTotal[i]._43); //�{�b�N�X���W
		}
		fclose(fp);
	}
	else if ((status == C_GAME) || (status == C_SELECT)){

		//�X�e�[�W�Z���N�g�@�Ɓ@�Q�[�����C��
		fp = fopen(SName[s_num].name, "r");
		if (fp == NULL) {

			exit(1);
		}

		fscanf(fp, "%d", &m_box->BoxNum);	//�{�b�N�X���̓ǂݍ���

		for (int i = 0; i < m_box->BoxNum; i++){

			fscanf(fp, "%f,%f,%f\n", &m_box->MatTotal[i]._41, &m_box->MatTotal[i]._42, &m_box->MatTotal[i]._43); //�{�b�N�X���W�̓ǂݍ���
		}
		fclose(fp);
	}
	else if (status == C_END){

		// �Q�[�����U���g
		fp = fopen(TName[1].name, "r");
		if (fp == NULL) {

			exit(1);
		}

		fscanf(fp, "%d", &m_box->BoxNum);	//�{�b�N�X���̓ǂݍ���

		for (int i = 0; i < m_box->BoxNum; i++){

			fscanf(fp, "%f,%f,%f\n", &m_box->MatTotal[i]._41, &m_box->MatTotal[i]._42, &m_box->MatTotal[i]._43); //�{�b�N�X���W�̓ǂݍ���
		}
		fclose(fp);
	}
}
