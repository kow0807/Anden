#pragma once
#include <vector>
#include "../../Common/Transform.h"

class ItemBase
{
public:

	enum class STATE
	{
		NONE,
		IN_MAP,
		HIT,
		PICK,
		USE
	};

	// �A�C�e���̑傫��
	static constexpr float SCALE = 0.3f;

	// �R���X�g���N�^
	ItemBase(void);
	// �f�X�g���N�^
	virtual ~ItemBase(void);
	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	Transform& GetTransform(void);

	bool IsHitPlayer(VECTOR pPos, std::vector<int> otherModelIds);

	virtual void ChangeState(STATE state);
	STATE GetState(void);

protected:

	// �p�����[�^�ݒ�
	virtual void SetParam(void) = 0;

	STATE state_;

	// ���f������̊�{���
	Transform transform_;
};

