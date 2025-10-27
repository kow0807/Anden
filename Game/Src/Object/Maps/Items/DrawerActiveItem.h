#pragma once
#include "ItemBase.h"

class DrawerActiveItem : public ItemBase
{
public:

	enum class ACTIVE
	{
		NONE,
		BELL,
		LIGHT_STONE,
		KALEIDOSCOPE,
		COMPASS,
		CHARM
	};


	// �Փ˗p���̔��a
	const float COL_RADIUS = 20.0f;

	// �����X�s�[�h
	const float INITIAL_SPEED = 15.0f;

	// ����
	const float DECEL = 0.005f;

	// �����d��
	const float INITIAL_GRAVITY = 0.0f;

	// ���ˎ������d��
	const float REFRECT_GRAVITY = 2.0f;

	// ���Z�d��
	const float ADD_GRAVITY = 9.8f / 40.0f;

	// �ő�d��
	const float MAX_GRAVITY = 30.0f;

	DrawerActiveItem(VECTOR pos, float axis, ACTIVE active,int chestNum,int drawerNum);
	~DrawerActiveItem(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Move(VECTOR pPos, Quaternion pRot);

	void BellThrow(int modelId);

	void SetPos(VECTOR pos);

	ACTIVE GetActive(void);
	void ChangeActive(ACTIVE active);

	int GetChestNum(void);
	int GetDrawerNum(void);

	void ChangeState(STATE state)override;

	//�R���p�X�̐j�̓���
	void CompassBasUpdate(VECTOR pPos, VECTOR goalPos);
protected:

	int compassBasModelId_;
	VECTOR compassBasPos_;
	VECTOR compassBasRot_;
	Quaternion compassBasQuaRot_;

	ACTIVE active_;

	int chestNum_;
	int drawerNum_;

	float speed_;
	VECTOR movePow_;
	float gravity_;
	VECTOR gravityPow_;

	VECTOR dir_;

	// �p�����[�^�ݒ�
	void SetParam(void) override;
};
