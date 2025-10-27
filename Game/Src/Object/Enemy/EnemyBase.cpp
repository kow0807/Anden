#include"../../Application.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Utility/AsoUtility.h"
#include"../../Manager/Camera.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(void)
{
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Init(void)
{
	SetParam();

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, 0);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
	stepAnim_ = 0.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void EnemyBase::Update(void)
{
	// �A�j���[�V��������
	Animation();

	transform_.Update();
}

void EnemyBase::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void EnemyBase::Release(void)
{
	MV1DeleteModel(transform_.modelId);
}

const Transform& EnemyBase::GetTransform(void) const
{
	return transform_;
}

void EnemyBase::Animation(void)
{
	// �A�j���[�V�����Đ�
	// �o�ߎ��Ԃ̎擾
	float deltaTime = 1.0f / SceneManager::DEFAULT_FPS;

	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (SPEED_ANIM * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		// ���[�v�Đ�
		stepAnim_ = 0.0f;
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);
}

void EnemyBase::LazyRotation(float goalRot)
{
	// ��]����
	float degNowAngleY = AsoUtility::Rad2DegF(transform_.rot.y);
	float degGoalAngleY = AsoUtility::Rad2DegF(goalRot);

	// 0�x�`360�x�ȓ��Ɋp�x�������߂�
	degGoalAngleY = static_cast<float>(AsoUtility::DegIn360(degGoalAngleY));

	// ��]�����Ȃ����̉�]�������擾����(���v���:1�A�����v���:-1)
	int aroundDir = AsoUtility::DirNearAroundDeg(degNowAngleY, degGoalAngleY);

	// ���B�������p�x�ɉ�]��������
	transform_.rot.y += AsoUtility::Deg2RadF(SPEED_ROT * static_cast<float>(aroundDir));

	if (degGoalAngleY - 8.5f < degNowAngleY && degNowAngleY < degGoalAngleY + 8.5f)
	{
		transform_.rot.y = goalRot;
	}

	// 0�x�`360�x�ȓ��Ɋp�x�������߂�
	transform_.rot.y = static_cast<float>(AsoUtility::RadIn2PI(transform_.rot.y));
}