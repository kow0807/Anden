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
	// カメラの初期設定
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

	// 入力状態を取得
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

	//カメラの向きベクトルを計算（注視点-カメラ位置）
	VECTOR dir_ = VSub(targetPos_, pos_);	//カメラが向いている方向

}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
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
		//パッドが接続されていない場合
		if (GetJoypadNum() == 0)
		{
			MouseMove(&angles_.y, &angles_.x, 1.f);
		}
		SetBeforeDrawFollow();
		break;
	}

	//ライトの設定を更新
	dir_ = VNorm(VSub(targetPos_, pos_));
	SetLightPosition(VGet(pos_.x, pos_.y, pos_.z));
	SetLightDirection(VGet(dir_.x, dir_.y, dir_.z));

	auto type = GetLightType();

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
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

	//カメラ座標を中心として、注視点を回転させる
	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		//今回の回転量を合成
		Quaternion rotPow = Quaternion();
		//rotPow = rotPow.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(axisDeg.z), AsoUtility::AXIS_Z));
		rotPow = rotPow.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(axisDeg.x), AsoUtility::AXIS_X));
		rotPow = rotPow.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(axisDeg.y), AsoUtility::AXIS_Y));


		//カメラの回転に合成
		rot_ = rot_.Mult(rotPow);

		//位置情報に回転情報を反映させる
		VECTOR rotLocalPos = rot_.PosAxis(RELATIVE_C2T_POS);

		//注視点更新
		targetPos_ = VAdd(pos_, rotLocalPos);

		//カメラの上方向更新
		cameraUp_ = rot_.GetUp();

		rot_.z = 0.0f;
	}

	//移動処理
	if (!AsoUtility::EqualsVZero(moveDir))
	{
		//現在の向きから進行方向を取得
		VECTOR direction = rot_.PosAxis(moveDir);

		VECTOR movePow = VScale(direction, SPEED);

		pos_ = VAdd(pos_, movePow);
		targetPos_ = VAdd(targetPos_, movePow);

	}
}

void Camera::SetBeforeDrawFollow(void)
{
	// 追従対象の位置
	VECTOR followPos = followTransform_->topPos;

	// 追従対象の向き
	//Quaternion followRot = followTransform_->quaRot;

	Quaternion rotX = Quaternion::AngleAxis(AsoUtility::Deg2RadF(angles_.x), AsoUtility::AXIS_X);
	Quaternion rotY = Quaternion::AngleAxis(AsoUtility::Deg2RadF(angles_.y), AsoUtility::AXIS_Y);
	Quaternion mixRot = rotY.Mult(rotX);

	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = rotY.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	// カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);

	// カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = mixRot.PosAxis(RELATIVE_C2T_POS);

	// 注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);

	// カメラの上方向
	//cameraUp_ = mixRot.PosAxis(rot_.GetUp());
	cameraUp_ = AsoUtility::DIR_U;

	angles_.z = 0.0f;

	auto& ins = InputManager::GetInstance();

	//パッドが接続されている場合
	if (GetJoypadNum() > 0)
	{
		DINPUT_JOYSTATE input;

		// 入力状態を取得
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

	//パッドが接続されていない場合
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

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
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

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	// カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// カメラはX軸に傾いているが、
	// この傾いた状態を角度ゼロ、傾き無しとする
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
