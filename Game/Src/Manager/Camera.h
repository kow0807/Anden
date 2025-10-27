#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:
	// カメラスピード：NEAR
	static constexpr float SPEED = 4.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// カメラ位置から注視点までの相対座標
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 220.0f };

	// 追従対象からカメラ位置までの相対座標(完全追従)
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 20.0f, -20.0f };

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// 定点カメラ
		FREE,			// フリーモード
		FOLLOW,			// 追従モード
	};

	//カメラの揺れ用
	enum class SHAKE
	{
		UP,
		DOWN,
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFollow(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;
	VECTOR GetTargetPos(void) const;
	VECTOR GetAngles(void) const;
	void SetPos(VECTOR pos);

	VECTOR GetDir(void) const;

	void SetAngles(VECTOR angle);

	// カメラモードの変更
	void ChangeMode(MODE mode);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

	void MouseMove(float* x_m, float* y_m, const float fov_per);

	void ShakeCamera(void);
private:
	SHAKE shake_;
	bool shakeFlag_;
	int shakeCnt_;
	int shakeCnt2_;

	// 追従対象
	const Transform* followTransform_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラの注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	//カメラの角度
	VECTOR angles_;

	// カメラの回転
	Quaternion rot_;

	//カメラの向いている方向
	VECTOR dir_;

	// カメラを初期位置に戻す
	void SetDefault(void);


};