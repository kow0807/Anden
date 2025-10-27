#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:
	// �J�����X�s�[�h�FNEAR
	static constexpr float SPEED = 4.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// �J�����ʒu���璍���_�܂ł̑��΍��W
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 220.0f };

	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 20.0f, -20.0f };

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// ��_�J����
		FREE,			// �t���[���[�h
		FOLLOW,			// �Ǐ]���[�h
	};

	//�J�����̗h��p
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

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

	void MouseMove(float* x_m, float* y_m, const float fov_per);

	void ShakeCamera(void);
private:
	SHAKE shake_;
	bool shakeFlag_;
	int shakeCnt_;
	int shakeCnt2_;

	// �Ǐ]�Ώ�
	const Transform* followTransform_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����̒����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	//�J�����̊p�x
	VECTOR angles_;

	// �J�����̉�]
	Quaternion rot_;

	//�J�����̌����Ă������
	VECTOR dir_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);


};