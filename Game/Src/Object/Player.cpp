#include"../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Utility/AsoUtility.h"
#include"../Manager/Camera.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER));
	transform_.pos = { 4000.0f, 70.0f, -4200.0f };
	transform_.topPos = { transform_.pos.x,180.0f,transform_.pos.z };
	transform_.downPos = { transform_.pos.x,60.0f,transform_.pos.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f);
	transform_.scl = { 2.0f,2.0f,2.0f };
	transform_.Update();
	transform_.stamina = STAMINA_MAX;
	state_ = STATE::IDLE;
	passive_ = PASSIVE::NONE;
	active_ = ACTIVE::BELL;
	moveDir_ = AsoUtility::VECTOR_ZERO;

	warpSe_= LoadSoundMem("Data/Sound/warp.mp3");
	stoneSe_ = LoadSoundMem("Data/Sound/stone.mp3");

	isAlive_ = true;
	povFlag_ = false;
	speed_ = 0.0f;
	tabiSpeed_ = 1.0f;
	staminaRecoverySpeed_ = 1.0f;
	bellCount_ = 0;
	lightStoneCount_ = 0;
	kaleidoScopeCount_ = 0;
	compassCount_ = 0;
	charmCount_ = 0;
	mouseWheelRotVol_ = 0;
	movedPos_ = { 4000.0f, 70.0f, -4200.0f };
}

void Player::Update(void)
{
	transform_.topPos.x = transform_.pos.x;
	transform_.topPos.z = transform_.pos.z;
	transform_.downPos.x = transform_.pos.x;
	transform_.downPos.z = transform_.pos.z;

	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);
	auto& ins = InputManager::GetInstance();

	//�p�b�h���ڑ�����Ă���Ƃ�
	if (GetJoypadNum() > 0)
	{
		if (input.Z > 0)
		{
			if (transform_.topPos.y > 100.0f)
			{
				transform_.topPos.y -= 10;
			}
			else
			{
				transform_.topPos.y = 100.0f;
			}
		}
		else
		{
			if (transform_.topPos.y < 180.0f)
			{
				transform_.topPos.y += 10;
			}
			else
			{
				transform_.topPos.y = 180.0f;
			}
		}

		if (transform_.stamina > STAMINA_MAX)
		{
			transform_.stamina = STAMINA_MAX;
		}

		switch (active_)
		{
		case Player::ACTIVE::NONE:
			ChangeActive(ACTIVE::BELL);
			break;
		case Player::ACTIVE::BELL:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 9000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::LIGHT_STONE);
			}
			break;
		case Player::ACTIVE::LIGHT_STONE:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 9000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::KALEIDOSCOPE);
			}
			if (input.POV[0] == 27000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::BELL);
			}
			break;
		case Player::ACTIVE::KALEIDOSCOPE:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 9000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::COMPASS);
			}
			if (input.POV[0] == 27000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::LIGHT_STONE);
			}
			break;
		case Player::ACTIVE::COMPASS:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 27000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::KALEIDOSCOPE);
			}
			break;
		}
	}
	else
	{
		if (ins.IsNew(KEY_INPUT_LCONTROL) || input.Z > 0)
		{
			if (transform_.topPos.y > 100.0f)
			{
				transform_.topPos.y -= 10;
			}
			else
			{
				transform_.topPos.y = 100.0f;
			}
		}
		else
		{
			if (transform_.topPos.y < 180.0f)
			{
				transform_.topPos.y += 10;
			}
			else
			{
				transform_.topPos.y = 180.0f;
			}
		}

		if (transform_.stamina > STAMINA_MAX)
		{
			transform_.stamina = STAMINA_MAX;
		}

		mouseWheelRotVol_ += ins.GetMouseWheelRotV();
		if (mouseWheelRotVol_ < 0)
		{
			mouseWheelRotVol_ = 0;
		}
		if (mouseWheelRotVol_ > 12)
		{
			mouseWheelRotVol_ = 12;
		}
		SelectActiveItem();
	}
	transform_.Update();
}

void Player::AfterUpdate(void)
{
	Move();
	transform_.pos = movedPos_;
}

void Player::Draw(void)
{
	//MV1DrawModel(transform_.modelId);
	DrawStamina(transform_.stamina, STAMINA_MAX);

	//DrawFormatString(10, 280, 0xffffff, "%2f,%2f,%2f", transform_.pos.x, transform_.pos.y, transform_.pos.z);
	//DrawFormatString(10, 90, 0xffffff, "�v���C���[�̃X�s�[�h�F%2f", speed_);
	//switch (state_)
	//{
	//case STATE::IDLE:
	//	DrawString(10, 110, "�v���C���[�̏�ԁF�ҋ@", 0xffffff);
	//	break;

	//case STATE::WALK:
	//	DrawString(10, 110, "�v���C���[�̏�ԁF����", 0xffffff);
	//	break;

	//case STATE::RUN:
	//	DrawString(10, 110, "�v���C���[�̏�ԁF����", 0xffffff);
	//	break;

	//case STATE::FATIGUE:
	//	DrawString(10, 110, "�v���C���[�̏�ԁF��J", 0xffffff);
	//	break;

	//case STATE::SNEAK:
	//	DrawString(10, 110, "�v���C���[�̏�ԁF�X�j�[�N", 0xffffff);
	//	break;
	//}

	//switch (passive_)
	//{
	//case Player::PASSIVE::NONE:
	//	DrawString(10, 130, "�p�b�V�u�A�C�e���F�������Ă��Ȃ�", 0xffffff);
	//	break;
	//case Player::PASSIVE::STRAW_DOLL:
	//	DrawString(10, 130, "�p�b�V�u�A�C�e���F�m�l�`", 0xffffff);
	//	break;
	//case Player::PASSIVE::TABI:
	//	DrawString(10, 130, "�p�b�V�u�A�C�e���F����", 0xffffff);
	//	break;
	//case Player::PASSIVE::GREEN_PURSE:
	//	DrawString(10, 130, "�p�b�V�u�A�C�e���F�΂̋В�", 0xffffff);
	//	break;
	//case Player::PASSIVE::HAND_MIRROR:
	//	DrawString(10, 130, "�p�b�V�u�A�C�e���F���ꂽ�苾", 0xffffff);
	//	break;
	//}

	//DrawFormatString(10, 150, 0xffffff, "��F%d", bellCount_);
	//DrawFormatString(10, 170, 0xffffff, "����΁F%d", lightStoneCount_);
	//DrawFormatString(10, 190, 0xffffff, "�}�E�X�F%d", mouseWheelRotVol_);

	//switch (active_)
	//{
	//case Player::ACTIVE::NONE:
	//	DrawString(10, 210, "�A�N�e�B�u�A�C�e���F�������Ă��Ȃ�", 0xffffff);
	//	break;
	//case Player::ACTIVE::BELL:
	//	DrawString(10, 210, "�A�N�e�B�u�A�C�e���F��", 0xffffff);
	//	break;
	//case Player::ACTIVE::LIGHT_STONE:
	//	DrawString(10, 210, "�A�N�e�B�u�A�C�e���F�����", 0xffffff);
	//	break;
	//case Player::ACTIVE::KALEIDOSCOPE:
	//	DrawString(10, 210, "�A�N�e�B�u�A�C�e���F���؋�", 0xffffff);
	//	break;
	//case Player::ACTIVE::COMPASS:
	//	DrawString(10, 210, "�A�N�e�B�u�A�C�e���F�R���p�X", 0xffffff);
	//	break;
	//}
}

void Player::Release(void)
{
	DeleteSoundMem(warpSe_);
	DeleteSoundMem(stoneSe_);
}

Transform& Player::GetTransform(void)
{
	return transform_;
}

VECTOR Player::GetMoveDir(void) const
{
	return moveDir_;
}

Player::STATE Player::GetState(void)
{
	return state_;
}

void Player::ChangePassive(PASSIVE passive)
{
	// ��Ԃ̍X�V
	passive_ = passive;

	// ��ԕʂ̏���������
	switch (passive_)
	{
	case Player::PASSIVE::NONE:
		tabiSpeed_ = 1.0f;
		staminaRecoverySpeed_ = 1.0f;
		break;
	case Player::PASSIVE::STRAW_DOLL:
		tabiSpeed_ = 1.0f;
		staminaRecoverySpeed_ = 1.0f;
		break;
	case Player::PASSIVE::TABI:
		tabiSpeed_ = 1.2f;
		staminaRecoverySpeed_ = 1.0f;
		break;
	case Player::PASSIVE::GREEN_PURSE:
		tabiSpeed_ = 1.0f;
		staminaRecoverySpeed_ = 3.0f;
		break;
	case Player::PASSIVE::HAND_MIRROR:
		tabiSpeed_ = 0.5f;
		staminaRecoverySpeed_ = 0.5f;
		break;
	}
}

Player::PASSIVE Player::GetPassive(void)
{
	return passive_;
}

void Player::ChangeActive(ACTIVE active)
{
	// ��Ԃ̍X�V
	active_ = active;

	// ��ԕʂ̏���������
	switch (active_)
	{
	case Player::ACTIVE::NONE:
		break;
	case Player::ACTIVE::BELL:
		break;
	case Player::ACTIVE::LIGHT_STONE:
		break;
	case Player::ACTIVE::KALEIDOSCOPE:
		break;
	case Player::ACTIVE::COMPASS:
		break;
	}
}

Player::ACTIVE Player::GetActive(void)
{
	return active_;
}

void Player::SelectActiveItem(void)
{
	auto& ins = InputManager::GetInstance();	
	if (mouseWheelRotVol_ >= 0 && mouseWheelRotVol_ < 3) { ChangeActive(ACTIVE::BELL); }
	if (mouseWheelRotVol_ >= 3 && mouseWheelRotVol_ < 6) { ChangeActive(ACTIVE::LIGHT_STONE); }
	if (mouseWheelRotVol_ >= 6 && mouseWheelRotVol_ < 9) { ChangeActive(ACTIVE::KALEIDOSCOPE); }
	if (mouseWheelRotVol_ >= 9 && mouseWheelRotVol_ < 12) { ChangeActive(ACTIVE::COMPASS); }
}

void Player::StrawDollUpdate(void)
{
	ChangeNextPlayVolumeSoundMem(170, warpSe_);
	PlaySoundMem(warpSe_, DX_PLAYTYPE_BACK);

	movedPos_ = { -1300.0f, 70.0f, -600.0f };
	transform_.pos = { -1300.0f, 70.0f, -600.0f };
	ChangePassive(PASSIVE::NONE);
}

void Player::PickBell(void)
{
	bellCount_++;
}

void Player::UseBell(void)
{
	if (bellCount_ > 0)
	{
		bellCount_--;
	}
}

int Player::GetBellCount(void)
{
	return bellCount_;
}

void Player::PickLightStone(void)
{
	lightStoneCount_++;
}

void Player::UseLightStone(void)
{
	if (lightStoneCount_ > 0)
	{
		ChangeNextPlayVolumeSoundMem(170, stoneSe_);
		PlaySoundMem(stoneSe_, DX_PLAYTYPE_BACK);

		lightStoneCount_--;
	}
}

int Player::GetLightStoneCount(void)
{
	return lightStoneCount_;
}

void Player::PickKaleidoScope(void)
{
	kaleidoScopeCount_++;
}

void Player::UseKaleidoScope(void)
{
	if (kaleidoScopeCount_ > 0)
	{
		ChangeNextPlayVolumeSoundMem(170, warpSe_);
		PlaySoundMem(warpSe_, DX_PLAYTYPE_BACK);

		movedPos_= { -1300.0f, 70.0f, -600.0f };
		transform_.pos = { -1300.0f, 70.0f, -600.0f };
		kaleidoScopeCount_--;
	}
}

int Player::GetKaleidoScopeCount(void)
{
	return kaleidoScopeCount_;
}

void Player::PickCompass(void)
{
	compassCount_++;
}

int Player::GetCompassCount(void)
{
	return compassCount_;
}

void Player::PickCharm(void)
{
	charmCount_++;
}

int Player::GetCharmCount(void)
{
	return charmCount_;
}

void Player::SetIsAlive(bool isAlive)
{
	isAlive_ = isAlive;
}

bool Player::GetIsAlive(void)
{
	return isAlive_;
}

void Player::Collision(std::vector<int> modelIds)
{
	for (const auto& modelId : modelIds)
	{
		// �J�v�Z���Ƃ̏Փ˔���
		auto hits = MV1CollCheck_Capsule(
			modelId, -1,
			transform_.topPos, transform_.downPos, 30.0f);

		// �Փ˂��������̃|���S���ƏՓˉ������܂ŁA
		// �v���C���[�̈ʒu���ړ�������
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];
			// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
			// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
			// �Փ˂����|���S���̖@�������������肷��
			for (int tryCnt = 0; tryCnt < 12; tryCnt++)
			{
				// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
				// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
				int pHit = HitCheck_Capsule_Triangle(
					transform_.topPos, transform_.downPos, 30.0f,
					hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					// �@���̕����ɂ�����Ƃ����ړ�������
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					//// �J�v�Z�����ꏏ�Ɉړ�������
					transform_.topPos.x = movedPos_.x;
					transform_.topPos.z = movedPos_.z;
					transform_.downPos.x = movedPos_.x;
					transform_.downPos.z = movedPos_.z;
					transform_.pos = movedPos_;
					transform_.Update();
					continue;
				}
				break;
			}
		}
		// ���o�����|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
	}
}

void Player::Move(void)
{
	DINPUT_JOYSTATE input;
	moveDir_ = AsoUtility::VECTOR_ZERO;

	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//�p�b�h���ڑ�����Ă���ꍇ
	if (GetJoypadNum() > 0)
	{
		if (input.Y < -500)
		{
			moveDir_ = AsoUtility::DIR_F;
		}
		if (input.Y > 500)
		{
			moveDir_ = AsoUtility::DIR_B;
		}
		if (input.X < -500)
		{
			moveDir_ = AsoUtility::DIR_L;
		}
		if (input.X > 500)
		{
			moveDir_ = AsoUtility::DIR_R;
		}
		//�ҋ@
		if (input.X == 0 && input.Y == 0)
		{
			ChangeState(STATE::IDLE);
			moveDir_ = VScale(moveDir_, speed_);
		}
		//����
		else if (input.Buttons[4] == 0)
		{
			ChangeState(STATE::WALK);
			moveDir_ = VScale(moveDir_, speed_);
		}

		//���Ⴊ��
		if (input.Z > 0)
		{
			ChangeState(STATE::SNEAK);
			moveDir_ = VScale(moveDir_, speed_);
			if (transform_.stamina < STAMINA_MAX)
			{
				transform_.stamina += staminaRecoverySpeed_;
			}
		}
		//�_�b�V��
		else if (input.Buttons[4])
		{
			if (input.X != 0 || input.Y != 0)
			{
				if (transform_.stamina > 0)
				{
					ChangeState(STATE::RUN);
					moveDir_ = VScale(moveDir_, speed_);
					transform_.stamina--;
				}
				//��J
				else
				{
					ChangeState(STATE::FATIGUE);
					moveDir_ = VScale(moveDir_, speed_);
				}
			}
			else if (transform_.stamina < STAMINA_MAX)
			{
				transform_.stamina += staminaRecoverySpeed_;
			}
		}
		else if (transform_.stamina < STAMINA_MAX)
		{
			transform_.stamina += staminaRecoverySpeed_;
		}
	}

	auto& ins = InputManager::GetInstance();

	//�p�b�h���ڑ�����Ă��Ȃ��ꍇ
	if (GetJoypadNum() == 0)
	{
		if (ins.IsNew(KEY_INPUT_W)) { moveDir_ = AsoUtility::DIR_F; }
		if (ins.IsNew(KEY_INPUT_S)) { moveDir_ = AsoUtility::DIR_B; }
		if (ins.IsNew(KEY_INPUT_A)) { moveDir_ = AsoUtility::DIR_L; }
		if (ins.IsNew(KEY_INPUT_D)) { moveDir_ = AsoUtility::DIR_R; }

		//�ҋ@
		if (!ins.IsNew(KEY_INPUT_W) && !ins.IsNew(KEY_INPUT_S) && !ins.IsNew(KEY_INPUT_A) && !ins.IsNew(KEY_INPUT_D))
		{
			ChangeState(STATE::IDLE);
			moveDir_ = VScale(moveDir_, speed_);
		}
		//����
		else if (!ins.IsNew(KEY_INPUT_LSHIFT))
		{
			ChangeState(STATE::WALK);
			moveDir_ = VScale(moveDir_, speed_);
		}

		//���Ⴊ��
		if (ins.IsNew(KEY_INPUT_LCONTROL))
		{
			ChangeState(STATE::SNEAK);
			moveDir_ = VScale(moveDir_, speed_);
			if (transform_.stamina < STAMINA_MAX)
			{
				transform_.stamina += staminaRecoverySpeed_;
			}
		}
		//�_�b�V��
		else if (ins.IsNew(KEY_INPUT_LSHIFT))
		{
			if (ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_D))
			{
				if (transform_.stamina > 0)
				{
					ChangeState(STATE::RUN);
					moveDir_ = VScale(moveDir_, speed_);
					transform_.stamina--;
				}
				//��J
				else
				{
					ChangeState(STATE::FATIGUE);
					moveDir_ = VScale(moveDir_, speed_);
				}
			}
			else if (transform_.stamina < STAMINA_MAX)
			{
				transform_.stamina += staminaRecoverySpeed_;
			}
		}
		else if (transform_.stamina < STAMINA_MAX)
		{
			transform_.stamina += staminaRecoverySpeed_;
		}
	}

	//�ړ�����
	if (!AsoUtility::EqualsVZero(moveDir_))
	{
		//�p�x
		VECTOR angles = mainCamera->GetAngles();

		Quaternion rots = Quaternion::Euler(AsoUtility::Deg2RadF(0.0f), AsoUtility::Deg2RadF(angles.y), AsoUtility::Deg2RadF(0.0f));

		//���݂̌�������i�s�������擾
		VECTOR direction = rots.PosAxis(moveDir_);

		VECTOR movePow = VScale(direction, SPEED);

		movedPos_ = VAdd(transform_.pos, movePow);
		targetPos_ = VAdd(targetPos_, movePow);
	}
}

void Player::DrawStamina(float stamina, int staminaMax)
{
	int r = 0;
	int g = 255;
	int b = 0;

	float ratio = stamina / (float)staminaMax;

	// �΁iGreen�j�͌������A�ԁiRed�j�͑���
	g = static_cast<int>(ratio * 255);
	r = static_cast<int>((1.0f - ratio) * 255);
	b = 0;  // ������0�ɌŒ�

	if (g > 255)
	{
		g = 255;
	}
	if (r < 0)
	{
		r = 0;
	}

	int color = GetColor(r, g, b);

	//�g��`��
	DrawBox(STAMINA_POS_X - STAMINA_SIZE_X, STAMINA_POS_Y, STAMINA_POS_X + STAMINA_SIZE_X, STAMINA_POS_Y + STAMINA_SIZE_Y, 0xffffff, FALSE);

	//�Q�[�W��`��
	DrawBox(STAMINA_POS_X, STAMINA_POS_Y, STAMINA_POS_X + STAMINA_SIZE_X * stamina / staminaMax, STAMINA_POS_Y + STAMINA_SIZE_Y, color, TRUE);

	//�Q�[�W��`��
	DrawBox(STAMINA_POS_X - STAMINA_SIZE_X * stamina / staminaMax, STAMINA_POS_Y, STAMINA_POS_X, STAMINA_POS_Y + STAMINA_SIZE_Y, color, TRUE);
}

void Player::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case STATE::IDLE:
		speed_ = 0.0f * tabiSpeed_;
		break;

	case STATE::WALK:
		speed_ = 1.0f * tabiSpeed_;
		break;

	case STATE::RUN:
		speed_ = 2.4f * tabiSpeed_;
		break;

	case STATE::FATIGUE:
		speed_ = 1.0f * tabiSpeed_;
		break;

	case STATE::SNEAK:
		speed_ = 0.5f * tabiSpeed_;
		break;
	}
}
