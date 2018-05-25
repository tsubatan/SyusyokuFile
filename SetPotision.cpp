#include "../Header/game.h"
#include "../Header/Player.h"		 // �v���C���[

//----------------------------------------------------------------------------------------------
// �L�����N�^�[�̌����̊Ǘ�
//----------------------------------------------------------------------------------------------
void Playerdirection(PLAYER* player, int g_CameraNum){

	if (player->MatTotal._22 == 1){
		//�p�^�[��0

		if (player->MatTotal._11 == 1){

			//�O
			g_CameraNum = 0;
		}
		else if (player->MatTotal._31 == 1){

			//�E
			g_CameraNum = 1;
		}
		else if (player->MatTotal._11 == -1){

			//���
			g_CameraNum = 2;
		}
		else if (player->MatTotal._31 == -1){

			//��
			g_CameraNum = 3;
		}
	}

	if (player->MatTotal._23 == -1){
		//�p�^�[��1

		if (player->MatTotal._11 == 1){

			//�O
			g_CameraNum = 0;
		}
		else if (player->MatTotal._31 == 1){

			//�E
			g_CameraNum = 1;
		}
		else if (player->MatTotal._11 == -1){

			//���
			g_CameraNum = 2;
		}
		else if (player->MatTotal._31 == -1){

			//��
			g_CameraNum = 3;
		}
	}

	if (player->MatTotal._22 == -1){
		//�p�^�[��2

		if (player->MatTotal._11 == 1){

			//�O
			g_CameraNum = 0;
		}
		else if (player->MatTotal._31 == 1){

			//�E
			g_CameraNum = 1;
		}
		else if (player->MatTotal._11 == -1){

			//���
			g_CameraNum = 2;
		}
		else if (player->MatTotal._31 == -1){

			//��
			g_CameraNum = 3;
		}
	}

	if (player->MatTotal._23 == 1){
		//�p�^�[��3

		if (player->MatTotal._11 == 1){

			//�O
			g_CameraNum = 0;
		}
		else if (player->MatTotal._31 == 1){

			//�E
			g_CameraNum = 1;
		}
		else if (player->MatTotal._11 == -1){

			//���
			g_CameraNum = 2;
		}
		else if (player->MatTotal._31 == -1){

			//��
			g_CameraNum = 3;
		}
	}

	if (player->MatTotal._21 == 1){
		//�p�^�[��4

		if (player->MatTotal._33 == 1){

			//�O
			g_CameraNum = 0;
		}
		else if (player->MatTotal._13 == -1){

			//�E
			g_CameraNum = 1;
		}
		else if (player->MatTotal._33 == -1){

			//���
			g_CameraNum = 2;
		}
		else if (player->MatTotal._13 == 1){

			//��
			g_CameraNum = 3;
		}
	}

	if (player->MatTotal._21 == -1){
		//�p�^�[��5

		if (player->MatTotal._33 == 1){

			//�O
			g_CameraNum = 0;
		}
		else if (player->MatTotal._13 == -1){

			//�E
			g_CameraNum = 1;
		}
		else if (player->MatTotal._33 == -1){

			//���
			g_CameraNum = 2;
		}
		else if (player->MatTotal._13 == 1){

			//��
			g_CameraNum = 3;
		}
	}
}


//----------------------------------------------------------------------------------------------
// �L�����̉�]�ɂ��v���C���[����ɔz�u���Ă���i�v���C���[�����蔻��j�̈ʒu�Y���̏C��
//----------------------------------------------------------------------------------------------
void SetPositionCase(PLAYER* player, HITBOX* hbox){

	if (player->MatTotal._22 == 1){

		//�p�^�[��0
		SetPosition0(player, hbox);
	}

	if (player->MatTotal._23 == -1){

		//�p�^�[��1
		SetPosition1(player, hbox);
	}

	if (player->MatTotal._22 == -1){

		//�p�^�[��2
		SetPosition2(player, hbox);
	}

	if (player->MatTotal._23 == 1){

		//�p�^�[��3
		SetPosition3(player, hbox);
	}

	if (player->MatTotal._21 == 1){

		//�p�^�[��4
		SetPosition4(player, hbox);
	}

	if (player->MatTotal._21 == -1){

		//�p�^�[��5
		SetPosition5(player, hbox);
	}
}


//----------------------------------------------------------------------------------------------
// �L�����̉�]�ɂ��v���C���[����ɔz�u���Ă���i�v���C���[�����蔻��j�̈ʒu�Y���̏C��
// �L�����N�^�[�̍��E��]�ɂ��ʒu�Y���̏C��
//----------------------------------------------------------------------------------------------
void SetPosition0(PLAYER* player, HITBOX* hbox){

	//�O
	hbox->MatTotal[0]._41 = player->MatTotal._41;		//+:���@-:�E
	hbox->MatTotal[0]._42 = player->MatTotal._42 + 1.0;	//+:��  -:��
	hbox->MatTotal[0]._43 = player->MatTotal._43 - 0.5;	//+:��  -:�O

	//�E
	hbox->MatTotal[1]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[1]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[1]._43 = player->MatTotal._43;

	//���
	hbox->MatTotal[2]._41 = player->MatTotal._41;
	hbox->MatTotal[2]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[2]._43 = player->MatTotal._43 + 0.5;

	//��
	hbox->MatTotal[3]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[3]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[3]._43 = player->MatTotal._43;


	//hitbox2
	//�O
	hbox->MatTotal[4]._41 = player->MatTotal._41;
	hbox->MatTotal[4]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[4]._43 = player->MatTotal._43 - 3.0;

	//�E
	hbox->MatTotal[5]._41 = player->MatTotal._41 - 3.0;
	hbox->MatTotal[5]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[5]._43 = player->MatTotal._43;

	//���
	hbox->MatTotal[6]._41 = player->MatTotal._41;
	hbox->MatTotal[6]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[6]._43 = player->MatTotal._43 + 3.0;

	//��
	hbox->MatTotal[7]._41 = player->MatTotal._41 + 3.0;
	hbox->MatTotal[7]._42 = player->MatTotal._42 + 1.0;
	hbox->MatTotal[7]._43 = player->MatTotal._43;

}


//----------------------------------------------------------------------------------------------
// �L�����̉�]�ɂ��v���C���[����ɔz�u���Ă���i�v���C���[�����蔻��j�̈ʒu�Y���̏C��
// �L�����N�^�[�̍��E��]�ɂ��ʒu�Y���̏C��
//----------------------------------------------------------------------------------------------
void SetPosition1(PLAYER* player, HITBOX* hbox){

	//�O
	hbox->MatTotal[0]._41 = player->MatTotal._41;
	hbox->MatTotal[0]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[0]._43 = player->MatTotal._43 - 1.0;

	//�E
	hbox->MatTotal[1]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[1]._42 = player->MatTotal._42;
	hbox->MatTotal[1]._43 = player->MatTotal._43 - 1.0;

	//���
	hbox->MatTotal[2]._41 = player->MatTotal._41;
	hbox->MatTotal[2]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[2]._43 = player->MatTotal._43 - 1.0;

	//��
	hbox->MatTotal[3]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[3]._42 = player->MatTotal._42;
	hbox->MatTotal[3]._43 = player->MatTotal._43 - 1.0;


	//hitbox2
	//�O
	hbox->MatTotal[4]._41 = player->MatTotal._41;
	hbox->MatTotal[4]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[4]._43 = player->MatTotal._43 - 1.0;

	//�E
	hbox->MatTotal[5]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[5]._42 = player->MatTotal._42;
	hbox->MatTotal[5]._43 = player->MatTotal._43 - 1.0;

	//���
	hbox->MatTotal[6]._41 = player->MatTotal._41;
	hbox->MatTotal[6]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[6]._43 = player->MatTotal._43 - 1.0;

	//��
	hbox->MatTotal[7]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[7]._42 = player->MatTotal._42;
	hbox->MatTotal[7]._43 = player->MatTotal._43 - 1.0;
}


//----------------------------------------------------------------------------------------------
// �L�����̉�]�ɂ��v���C���[����ɔz�u���Ă���i�v���C���[�����蔻��j�̈ʒu�Y���̏C��
// �L�����N�^�[�̍��E��]�ɂ��ʒu�Y���̏C��
//----------------------------------------------------------------------------------------------
void SetPosition2(PLAYER* player, HITBOX* hbox){

	//�O
	hbox->MatTotal[0]._41 = player->MatTotal._41;
	hbox->MatTotal[0]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[0]._43 = player->MatTotal._43 + 0.5;

	//�E
	hbox->MatTotal[1]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[1]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[1]._43 = player->MatTotal._43;

	//���
	hbox->MatTotal[2]._41 = player->MatTotal._41;
	hbox->MatTotal[2]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[2]._43 = player->MatTotal._43 - 0.5;

	//��
	hbox->MatTotal[3]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[3]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[3]._43 = player->MatTotal._43;

	//hitbox2
	//�O
	hbox->MatTotal[4]._41 = player->MatTotal._41;
	hbox->MatTotal[4]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[4]._43 = player->MatTotal._43 + 0.5;

	//�E
	hbox->MatTotal[5]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[5]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[5]._43 = player->MatTotal._43;

	//���
	hbox->MatTotal[6]._41 = player->MatTotal._41;
	hbox->MatTotal[6]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[6]._43 = player->MatTotal._43 - 0.5;

	//��
	hbox->MatTotal[7]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[7]._42 = player->MatTotal._42 - 1.0;
	hbox->MatTotal[7]._43 = player->MatTotal._43;
}


//----------------------------------------------------------------------------------------------
// �L�����̉�]�ɂ��v���C���[����ɔz�u���Ă���i�v���C���[�����蔻��j�̈ʒu�Y���̏C��
// �L�����N�^�[�̍��E��]�ɂ��ʒu�Y���̏C��
//----------------------------------------------------------------------------------------------
void SetPosition3(PLAYER* player, HITBOX* hbox){

	//�O
	hbox->MatTotal[0]._41 = player->MatTotal._41;
	hbox->MatTotal[0]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[0]._43 = player->MatTotal._43 + 1.0;

	//�E
	hbox->MatTotal[1]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[1]._42 = player->MatTotal._42;
	hbox->MatTotal[1]._43 = player->MatTotal._43 + 1.0;

	//���
	hbox->MatTotal[2]._41 = player->MatTotal._41;
	hbox->MatTotal[2]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[2]._43 = player->MatTotal._43 + 1.0;

	//��
	hbox->MatTotal[3]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[3]._42 = player->MatTotal._42;
	hbox->MatTotal[3]._43 = player->MatTotal._43 + 1.0;

	//hitbox2
	//�O
	hbox->MatTotal[4]._41 = player->MatTotal._41;
	hbox->MatTotal[4]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[4]._43 = player->MatTotal._43 + 1.0;

	//�E
	hbox->MatTotal[5]._41 = player->MatTotal._41 - 0.5;
	hbox->MatTotal[5]._42 = player->MatTotal._42;
	hbox->MatTotal[5]._43 = player->MatTotal._43 + 1.0;

	//���
	hbox->MatTotal[6]._41 = player->MatTotal._41;
	hbox->MatTotal[6]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[6]._43 = player->MatTotal._43 + 1.0;

	//��
	hbox->MatTotal[7]._41 = player->MatTotal._41 + 0.5;
	hbox->MatTotal[7]._42 = player->MatTotal._42;
	hbox->MatTotal[7]._43 = player->MatTotal._43 + 1.0;
}


//----------------------------------------------------------------------------------------------
// �L�����̉�]�ɂ��v���C���[����ɔz�u���Ă���i�v���C���[�����蔻��j�̈ʒu�Y���̏C��
// �L�����N�^�[�̍��E��]�ɂ��ʒu�Y���̏C��
//----------------------------------------------------------------------------------------------
void SetPosition4(PLAYER* player, HITBOX* hbox){

	//�O
	hbox->MatTotal[0]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[0]._42 = player->MatTotal._42;
	hbox->MatTotal[0]._43 = player->MatTotal._43 - 0.5;

	//�E
	hbox->MatTotal[1]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[1]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[1]._43 = player->MatTotal._43;

	//���
	hbox->MatTotal[2]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[2]._42 = player->MatTotal._42;
	hbox->MatTotal[2]._43 = player->MatTotal._43 + 0.5;

	//��
	hbox->MatTotal[3]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[3]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[3]._43 = player->MatTotal._43;

	//hitbox2
	//�O
	hbox->MatTotal[4]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[4]._42 = player->MatTotal._42;
	hbox->MatTotal[4]._43 = player->MatTotal._43 - 0.5;

	//�E
	hbox->MatTotal[5]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[5]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[5]._43 = player->MatTotal._43;

	//���
	hbox->MatTotal[6]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[6]._42 = player->MatTotal._42;
	hbox->MatTotal[6]._43 = player->MatTotal._43 + 0.5;

	//��
	hbox->MatTotal[7]._41 = player->MatTotal._41 + 1.0;
	hbox->MatTotal[7]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[7]._43 = player->MatTotal._43;
}


//----------------------------------------------------------------------------------------------
// �L�����̉�]�ɂ��v���C���[����ɔz�u���Ă���i�v���C���[�����蔻��j�̈ʒu�Y���̏C��
// �L�����N�^�[�̍��E��]�ɂ��ʒu�Y���̏C��
//----------------------------------------------------------------------------------------------
void SetPosition5(PLAYER* player, HITBOX* hbox){

	//�O
	hbox->MatTotal[0]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[0]._42 = player->MatTotal._42;
	hbox->MatTotal[0]._43 = player->MatTotal._43 - 0.5;

	//�E
	hbox->MatTotal[1]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[1]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[1]._43 = player->MatTotal._43;


	//���
	hbox->MatTotal[2]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[2]._42 = player->MatTotal._42;
	hbox->MatTotal[2]._43 = player->MatTotal._43 + 0.5;

	//��
	hbox->MatTotal[3]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[3]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[3]._43 = player->MatTotal._43;

	//hitbox2
	//�O
	hbox->MatTotal[4]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[4]._42 = player->MatTotal._42;
	hbox->MatTotal[4]._43 = player->MatTotal._43 - 0.5;

	//�E
	hbox->MatTotal[5]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[5]._42 = player->MatTotal._42 - 0.5;
	hbox->MatTotal[5]._43 = player->MatTotal._43;

	//���
	hbox->MatTotal[6]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[6]._42 = player->MatTotal._42;
	hbox->MatTotal[6]._43 = player->MatTotal._43 + 0.5;

	//��
	hbox->MatTotal[7]._41 = player->MatTotal._41 - 1.0;
	hbox->MatTotal[7]._42 = player->MatTotal._42 + 0.5;
	hbox->MatTotal[7]._43 = player->MatTotal._43;
}
