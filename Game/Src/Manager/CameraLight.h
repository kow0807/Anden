#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"


class CameraLight
{
public:

	CameraLight(void);
	~CameraLight(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	//���C�g�̃n���h��
	int handle_;

	// ���C�g�̈ʒu
	VECTOR pos_;

	// ���C�g�̌���
	VECTOR dir_;

	float OutAngle;	//�e���p�x
	float InAngle;	//�������n�߂�p�x
	float Range;	//�e���ő勗��
	float Atten0;	//���C�g�ƒ��_�̋����Ɋ֌W�Ȃ��������闦���w�肷�����
	float Atten1;	//���C�g�̋����ɔ�Ⴕ�Č������闦
	float Atten2;	//���C�g�̋����̓��ɔ�Ⴕ�Č������闦

	// ���C�g�������ʒu�ɖ߂�
	void SetDefault(void);

};

