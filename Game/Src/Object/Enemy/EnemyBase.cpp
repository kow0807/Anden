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

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, 0);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
	stepAnim_ = 0.0f;

	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void EnemyBase::Update(void)
{
	// アニメーション処理
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
	// アニメーション再生
	// 経過時間の取得
	float deltaTime = 1.0f / SceneManager::DEFAULT_FPS;

	// アニメーション時間の進行
	stepAnim_ += (SPEED_ANIM * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		// ループ再生
		stepAnim_ = 0.0f;
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);
}

void EnemyBase::LazyRotation(float goalRot)
{
	// 回転処理
	float degNowAngleY = AsoUtility::Rad2DegF(transform_.rot.y);
	float degGoalAngleY = AsoUtility::Rad2DegF(goalRot);

	// 0度～360度以内に角度をおさめる
	degGoalAngleY = static_cast<float>(AsoUtility::DegIn360(degGoalAngleY));

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	int aroundDir = AsoUtility::DirNearAroundDeg(degNowAngleY, degGoalAngleY);

	// 到達したい角度に回転を加える
	transform_.rot.y += AsoUtility::Deg2RadF(SPEED_ROT * static_cast<float>(aroundDir));

	if (degGoalAngleY - 8.5f < degNowAngleY && degNowAngleY < degGoalAngleY + 8.5f)
	{
		transform_.rot.y = goalRot;
	}

	// 0度～360度以内に角度をおさめる
	transform_.rot.y = static_cast<float>(AsoUtility::RadIn2PI(transform_.rot.y));
}