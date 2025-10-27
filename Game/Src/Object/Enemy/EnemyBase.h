#pragma once
#include "../Common/Transform.h"

class EnemyBase
{
public:

	// �A�j���[�V�����̍Đ����x
	static constexpr float SPEED_ANIM = 60.0f;

	// ��]��(deg)
	static constexpr float SPEED_ROT = 8.0f;

	EnemyBase(void);
	virtual ~EnemyBase(void);
	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	const Transform& GetTransform(void) const;

protected:

	// �A�j���[�V�����̃A�^�b�`�ԍ�
	int animAttachNo_;

	// �A�j���[�V�����̑��Đ�����
	float animTotalTime_;

	// �Đ����̃A�j���[�V��������
	float stepAnim_;

	// �p�����[�^�ݒ�
	virtual void SetParam(void) = 0;

	// �A�j���[�V����
	void Animation(void);

	// ���f������̊�{���
	Transform transform_;

	// �x����]
	void LazyRotation(float goalRot);
};
