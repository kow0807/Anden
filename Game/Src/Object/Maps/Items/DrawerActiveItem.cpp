#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/Camera.h"
#include "DrawerActiveItem.h"

DrawerActiveItem::DrawerActiveItem(VECTOR pos, float axis, ACTIVE active, int chestNum, int drawerNum)
{
	transform_.pos = pos;
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(axis), 0.0f);
	active_ = active;
	chestNum_ = chestNum;
	drawerNum_ = drawerNum;
}

DrawerActiveItem::~DrawerActiveItem(void)
{
}

void DrawerActiveItem::Init(void)
{
	switch (active_)
	{
	case DrawerActiveItem::ACTIVE::NONE:
		break;
	case DrawerActiveItem::ACTIVE::BELL:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::BELL));
		break;
	case DrawerActiveItem::ACTIVE::LIGHT_STONE:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::LIGHT_STONE));
		break;
	case DrawerActiveItem::ACTIVE::KALEIDOSCOPE:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::KALEIDOSCOPE));
		break;
	case DrawerActiveItem::ACTIVE::COMPASS:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::COMPASS_BASE));
		compassBasModelId_ = ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::COMPASS_BAS);
		compassBasPos_ = transform_.pos;
		compassBasQuaRot_ = transform_.quaRot;
		compassBasRot_ = compassBasQuaRot_.ToEuler();
		MV1SetScale(compassBasModelId_, { SCALE,SCALE,SCALE });
		break;
	case DrawerActiveItem::ACTIVE::CHARM:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::CHARM));
		break;
	}
	movePow_ = AsoUtility::VECTOR_ZERO;
	dir_ = AsoUtility::VECTOR_ZERO;
	speed_ = 2.0f;
	gravity_ = 0.0f;
	gravityPow_ = AsoUtility::VECTOR_ZERO;
	// ���N���X�̊֐����Ă�
	ItemBase::Init();
}

void DrawerActiveItem::Update(void)
{
	// ���N���X�̊֐����Ă�
	ItemBase::Update();

	compassBasPos_ = transform_.pos;
	compassBasRot_ = compassBasQuaRot_.ToEuler();
	compassBasRot_.y += AsoUtility::Deg2RadF(90.0f);
	MV1SetPosition(compassBasModelId_, compassBasPos_);
	MV1SetRotationXYZ(compassBasModelId_, compassBasRot_);
}

void DrawerActiveItem::Draw(void)
{
	// ���N���X�̊֐����Ă�
	ItemBase::Draw();
	switch (state_)
	{
	case ItemBase::STATE::NONE:
		break;
	case ItemBase::STATE::IN_MAP:
		MV1DrawModel(compassBasModelId_);
		break;
	case ItemBase::STATE::HIT:
		MV1DrawModel(compassBasModelId_);
		break;
	case ItemBase::STATE::PICK:
		break;
	case ItemBase::STATE::USE:
		MV1DrawModel(compassBasModelId_);
		break;
	}
	//DrawSphere3D(transform_.pos, 20.0f, 20, 0xff0000, 0xff0000, false);
}

void DrawerActiveItem::Move(VECTOR pPos, Quaternion pRot)
{
	switch (active_)
	{
	case DrawerActiveItem::ACTIVE::NONE:
		break;
	case DrawerActiveItem::ACTIVE::BELL:
		transform_.pos = pPos;
		transform_.quaRot = pRot;
		break;
	case DrawerActiveItem::ACTIVE::LIGHT_STONE:
		transform_.pos = pPos;
		transform_.quaRot = pRot;
		break;
	case DrawerActiveItem::ACTIVE::KALEIDOSCOPE:
		transform_.pos = pPos;
		transform_.quaRot = pRot;
		break;
	case DrawerActiveItem::ACTIVE::COMPASS:
		break;
	case DrawerActiveItem::ACTIVE::CHARM:
		break;
	}
}

void DrawerActiveItem::BellThrow(int modelId)
{
	// �ړ��O���W
	VECTOR prePos = transform_.pos;

	// ��{�ړ��ʂ̌v�Z
	movePow_ = VScale(dir_, speed_);

	// �d�͌v�Z
	gravity_ += ADD_GRAVITY;
	if (gravity_ > MAX_GRAVITY)
	{
		gravity_ = MAX_GRAVITY;
	}
	gravityPow_ = VScale(AsoUtility::DIR_D, gravity_);

	// �ړ�����W
	VECTOR movedPos = VAdd(transform_.pos, VAdd(movePow_, gravityPow_));

	// �X�e�[�W�Ƃ̏Փ˔���
	auto hits = MV1CollCheck_Sphere(modelId, -1, movedPos, COL_RADIUS);
	for (int i = 0; i < hits.HitNum; i++)
	{

		// �Փˏ��
		auto hit = hits.Dim[i];

		// �@��
		VECTOR N = hit.Normal;

		// ���t���[���̈ړ�����
		auto dir = VNorm(VSub(movedPos, prePos));
		// ���˃x�N�g���v�Z
		dir_ = VAdd(dir, VScale(N, 2.0f * (VDot(VScale(dir, -1.0f), N))));

		// �@���Ə�����̓���
		float upDot = VDot(N, AsoUtility::DIR_U);

		// �n�ʂƏՓ˂��Ă��邩
		bool isGround = upDot > 0.9f;

		// �n�ʏՓˎ��̃K�^�K�^�}��
		if (isGround && speed_ < ADD_GRAVITY * 8.0f)
		{
			// �㉺�^���𖳂���
			dir_.y = 0.0f;
			dir_ = VNorm(dir_);
		}

		// ���ˎ��d�͏�����
		gravity_ = REFRECT_GRAVITY;

		// ���˂������X�s�[�h������������
		speed_ *= 0.8f;

		// �X�s�[�h��������x�x���Ȃ��āA�n�ʂɂ������~������
		if (speed_ < 0.5f && isGround)
		{
			// �Փ˒n�_�O�ɗ��߂�
			movedPos = prePos;
			speed_ = 0.0f;
		}
		else
		{

			// �Փ˂��Ȃ��ꏊ�܂ňړ�������
			for (int i = 0; i < 10; i++)
			{
				auto polygon = hit.Position;
				auto hitCheck = HitCheck_Sphere_Triangle(
					movedPos, COL_RADIUS, polygon[0], polygon[1], polygon[2]);
				if (hitCheck)
				{
					movedPos = VAdd(transform_.pos, VScale(dir_, 1.0f));
				}
			}

		}

		break;

	}

	// �ʒu�X�V
	transform_.pos = movedPos;

	// ����
	speed_ -= DECEL;
	if (speed_ < 0.0f)
	{
		speed_ = 0.0f;
	}
}

void DrawerActiveItem::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}

DrawerActiveItem::ACTIVE DrawerActiveItem::GetActive(void)
{
	return active_;
}

void DrawerActiveItem::ChangeActive(ACTIVE active)
{
	// ��Ԃ̍X�V
	active_ = active;

	// ��ԕʂ̏���������
	switch (active_)
	{
	case DrawerActiveItem::ACTIVE::NONE:
		break;
	case DrawerActiveItem::ACTIVE::BELL:
		break;
	case DrawerActiveItem::ACTIVE::LIGHT_STONE:
		break;
	case DrawerActiveItem::ACTIVE::KALEIDOSCOPE:
		break;
	case DrawerActiveItem::ACTIVE::COMPASS:
		break;
	case DrawerActiveItem::ACTIVE::CHARM:
		break;
	}
}

int DrawerActiveItem::GetChestNum(void)
{
	return chestNum_;
}

int DrawerActiveItem::GetDrawerNum(void)
{
	return drawerNum_;
}

void DrawerActiveItem::ChangeState(STATE state)
{
	// ��Ԃ̍X�V
	state_ = state;

	// ��ԕʂ̏���������
	switch (state_)
	{
	case ItemBase::STATE::IN_MAP:
		break;
	case ItemBase::STATE::HIT:
		break;
	case ItemBase::STATE::PICK:
		break;
	case ItemBase::STATE::USE:
		movePow_ = transform_.quaRot.PosAxis(AsoUtility::DIR_F);
		movePow_ = VScale(movePow_, 30.0f);
		dir_ = VNorm(transform_.quaRot.PosAxis({ 0.0f, 0.2f, 1.0f }));
		speed_ = INITIAL_SPEED;
		gravity_ = INITIAL_GRAVITY;
		break;
	}
}

void DrawerActiveItem::CompassBasUpdate(VECTOR pPos, VECTOR goalPos)
{
	//�p�x
	VECTOR angles = mainCamera->GetAngles();
	Quaternion rots = Quaternion::Euler(AsoUtility::Deg2RadF(angles.x), AsoUtility::Deg2RadF(angles.y), AsoUtility::Deg2RadF(angles.z));

	transform_.quaRot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(rots.y), AsoUtility::AXIS_Y);

	VECTOR relativeTPos = rots.PosAxis({ 40.0f,70.0f,100.0f });
	transform_.pos = VAdd(pPos, relativeTPos);

	VECTOR goalXZ = goalPos;
	goalXZ.y = 0.0f;

	auto posXZ = compassBasPos_;
	posXZ.y = 0.0f;

	VECTOR dir = VNorm(VSub(goalXZ, posXZ));

	Quaternion qua = Quaternion::LookRotation(dir);

	compassBasQuaRot_ = qua;
}

void DrawerActiveItem::SetParam(void)
{
	// ���f������̊�{���
	transform_.scl = { SCALE,SCALE,SCALE };
	transform_.quaRot = Quaternion();
	state_ = STATE::IN_MAP;
	transform_.Update();
}
