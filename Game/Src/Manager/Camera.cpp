#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Object/Common/Transform.h"
#include "../Object/Player.h"
#include "../Manager/InputManager.h"
#include "Camera.h"

Camera::Camera(void)
{
	shake_ = SHAKE::DOWN;
	mode_ = MODE::NONE;
	shakeFlag_ = false;
	shakeCnt_ = 0;
	shakeCnt2_ = 0;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	angles_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();
	dir_ = { 0.0f,0.0f,0.0f };
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// �J�����̏����ݒ�
	SetDefault();


	//float OutAngle = DX_PI_F / 4.0f;
	//float OutAngle = 0.5f;
	float OutAngle = 0.8f;

	//float InAngle = DX_PI_F / 4.0f;
	//float InAngle = 0.2f;
	float InAngle = 0.3f;
	float Range = 5000.0f;
	//float Atten0 = 0.0f;
	float Atten0 = 0.191586f;
	float Atten1 = 0.001662f;
	//float Atten1 = 0.0006f;
	float Atten2 = 0.0f;

	ChangeLightTypeSpot(
		VGet(pos_.x, pos_.y, pos_.y),
		VGet(dir_.x, dir_.y, dir_.z),
		OutAngle,
		InAngle,
		Range,
		Atten0,
		Atten1,
		Atten2);

}

void Camera::Update(void)
{
	DINPUT_JOYSTATE input;

	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (input.Rx > 100)
	{
		angles_.y += 1.5f;
	}
	if (input.Rx < -100)
	{
		angles_.y -= 1.5f;
	}
	if (input.Ry > 100 && angles_.x < 60.0f)
	{
		angles_.x += 1.5f;
	}
	if (input.Ry < -100 && angles_.x > -60.0f)
	{
		angles_.x -= 1.5f;
	}

	//�J�����̌����x�N�g�����v�Z�i�����_-�J�����ʒu�j
	VECTOR dir_ = VSub(targetPos_, pos_);	//�J�����������Ă������

}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		//�p�b�h���ڑ�����Ă��Ȃ��ꍇ
		if (GetJoypadNum() == 0)
		{
			MouseMove(&angles_.y, &angles_.x, 1.f);
		}
		SetBeforeDrawFollow();
		break;
	}

	//���C�g�̐ݒ���X�V
	dir_ = VNorm(VSub(targetPos_, pos_));
	SetLightPosition(VGet(pos_.x, pos_.y, pos_.z));
	SetLightDirection(VGet(dir_.x, dir_.y, dir_.z));

	auto type = GetLightType();

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// �������Ȃ�
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();

	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }

	const float ROT_POW = 1.0f;
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y = -ROT_POW; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y = ROT_POW; }
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x = -ROT_POW; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x = ROT_POW; }

	//MouseMove(&axisDeg.y, &axisDeg.x, 1.f);

	//�J�������W�𒆐S�Ƃ��āA�����_����]������
	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		//����̉�]�ʂ�����
		Quaternion rotPow = Quaternion();
		//rotPow = rotPow.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(axisDeg.z), AsoUtility::AXIS_Z));
		rotPow = rotPow.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(axisDeg.x), AsoUtility::AXIS_X));
		rotPow = rotPow.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(axisDeg.y), AsoUtility::AXIS_Y));


		//�J�����̉�]�ɍ���
		rot_ = rot_.Mult(rotPow);

		//�ʒu���ɉ�]���𔽉f������
		VECTOR rotLocalPos = rot_.PosAxis(RELATIVE_C2T_POS);

		//�����_�X�V
		targetPos_ = VAdd(pos_, rotLocalPos);

		//�J�����̏�����X�V
		cameraUp_ = rot_.GetUp();

		rot_.z = 0.0f;
	}

	//�ړ�����
	if (!AsoUtility::EqualsVZero(moveDir))
	{
		//���݂̌�������i�s�������擾
		VECTOR direction = rot_.PosAxis(moveDir);

		VECTOR movePow = VScale(direction, SPEED);

		pos_ = VAdd(pos_, movePow);
		targetPos_ = VAdd(targetPos_, movePow);

	}
}

void Camera::SetBeforeDrawFollow(void)
{
	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followTransform_->topPos;

	// �Ǐ]�Ώۂ̌���
	//Quaternion followRot = followTransform_->quaRot;

	Quaternion rotX = Quaternion::AngleAxis(AsoUtility::Deg2RadF(angles_.x), AsoUtility::AXIS_X);
	Quaternion rotY = Quaternion::AngleAxis(AsoUtility::Deg2RadF(angles_.y), AsoUtility::AXIS_Y);
	Quaternion mixRot = rotY.Mult(rotX);

	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = rotY.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	// �J�����ʒu�̍X�V
	pos_ = VAdd(followPos, relativeCPos);

	// �J�����ʒu���璍���_�܂ł̑��΍��W
	VECTOR relativeTPos = mixRot.PosAxis(RELATIVE_C2T_POS);

	// �����_�̍X�V
	targetPos_ = VAdd(pos_, relativeTPos);

	// �J�����̏����
	//cameraUp_ = mixRot.PosAxis(rot_.GetUp());
	cameraUp_ = AsoUtility::DIR_U;

	angles_.z = 0.0f;

	auto& ins = InputManager::GetInstance();

	//�p�b�h���ڑ�����Ă���ꍇ
	if (GetJoypadNum() > 0)
	{
		DINPUT_JOYSTATE input;

		// ���͏�Ԃ��擾
		GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

		if (input.Z <= 0
			&& (input.Buttons[4] > 0 && (input.X != 0 || input.Y != 0)))
		{
			if (followTransform_->stamina > 0)
			{
				shakeFlag_ = true;
			}
		}
		else
		{
			shakeFlag_ = false;
			shakeCnt_ = 0;
		}
	}

	//�p�b�h���ڑ�����Ă��Ȃ��ꍇ
	if (GetJoypadNum() == 0)
	{
		if (!ins.IsNew(KEY_INPUT_LCONTROL)
			&& (ins.IsNew(KEY_INPUT_LSHIFT) && ins.IsNew(KEY_INPUT_W)
				|| ins.IsNew(KEY_INPUT_LSHIFT) && ins.IsNew(KEY_INPUT_A)
				|| ins.IsNew(KEY_INPUT_LSHIFT) && ins.IsNew(KEY_INPUT_S)
				|| ins.IsNew(KEY_INPUT_LSHIFT) && ins.IsNew(KEY_INPUT_D)))
		{
			if (followTransform_->stamina > 0)
			{
				shakeFlag_ = true;
			}
		}
		else
		{
			shakeFlag_ = false;
			shakeCnt_ = 0;
		}
	}


	if (followTransform_->stamina == 0) {
		shakeFlag_ = false;
		shakeCnt_ = 0;
	}

	if (shakeFlag_ == true)
	{
		switch (shake_)
		{
		case SHAKE::UP:
			angles_.x -= 0.3f;
			shakeCnt_++;
			if (shakeCnt_ > 15)
			{
				shake_ = SHAKE::DOWN;
			}
			break;
		case SHAKE::DOWN:
			angles_.x += 0.3f;
			shakeCnt_++;
			if ((0 <= shakeCnt_ && shakeCnt_ < 15) || shakeCnt_ > 30)
			{
				shake_ = SHAKE::UP;
				shakeCnt_ = 0;
			}
			break;
		}
	}
}

void Camera::Draw(void)
{
	//DrawFormatString(0, 16, 0xffffff, "CameraPos = (%2f,%2f,%2f)", pos_.x, pos_.y, pos_.z);
	//DrawFormatString(0, 32, 0xffffff, "CameraAngles = (%2f,%2f,%2f)", angles_.x, angles_.y, angles_.z);
	//DrawFormatString(0, 48, 0xffffff, "CameraTPos = (%2f,%2f,%2f)", targetPos_.x, targetPos_.y, targetPos_.z);
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

void Camera::SetPos(VECTOR pos)
{
	pos_ = pos;
}

VECTOR Camera::GetDir(void) const
{
	return dir_;
}

void Camera::SetAngles(VECTOR angle)
{
	angles_ = angle;
}

void Camera::ChangeMode(MODE mode)
{

	// �J�����̏����ݒ�
	SetDefault();

	// �J�������[�h�̕ύX
	mode_ = mode;

	// �ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetMouseDispFlag(TRUE);
		break;
	case Camera::MODE::FREE:
		SetMouseDispFlag(TRUE);
		break;
	case Camera::MODE::FOLLOW:
		SetMouseDispFlag(FALSE);
		break;
	}

	

}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	// �J�����̏����
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// �J������X���ɌX���Ă��邪�A
	// ���̌X������Ԃ��p�x�[���A�X�������Ƃ���
	rot_ = Quaternion::Identity();

}

void Camera::MouseMove(float* x_m, float* y_m, const float fov_per)
{
	int x_t, y_t;
	GetMousePoint(&x_t, &y_t);
	*x_m += float(std::clamp(x_t - Application::SCREEN_SIZE_X / 2, -120, 120)) * fov_per / GetFPS() * 5;
	*y_m += float(std::clamp(y_t - Application::SCREEN_SIZE_Y / 2, -120, 120)) * fov_per / GetFPS() * 5;
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
	if (angles_.x > 60.0f)
	{
		angles_.x = 60.0f;
	}
	if (angles_.x < -60.0f)
	{
		angles_.x = -60.0f;
	}
}

void Camera::ShakeCamera(void)
{
	shakeCnt2_++;

	if (shakeCnt2_ % 2 < 1)
	{
		angles_.y -= 2.0f;
	}
	if (shakeCnt2_ % 2 >= 1)
	{
		angles_.y += 2.0f;
	}
}
