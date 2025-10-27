#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource.h"
#include "../../../Manager/Camera.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/Transform.h"
#include "Drawer.h"

Drawer::Drawer(const Transform& transform, 
    VECTOR localPos, float localRot) : transformParent_(transform)
{
	transform_.pos = localPos;
	transform_.quaRot = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(localRot), 0.0f);
    ResourceManager& ins = ResourceManager::GetInstance();
    transform_.SetModel(
        ins.LoadModelDuplicate(ResourceManager::SRC::DRAWER));

}

Drawer::~Drawer(void)
{
}

void Drawer::Init(void)
{
	// モデル制御の基本情報
	transform_.scl = { SCALE, SCALE, SCALE };
	// 回転なし
	//transform_.quaRot = Quaternion();
	// ローカル回転(正面をZの正方向に合わせる)
	transform_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	state_ = STATE::CLOSE;

	moveCnt_ = 0.0f;

	// 親モデルと同期
	SyncParent(transform_, { 0.0f, 0.0f, 0.0f });

	//現在の向きから進行方向を取得
	VECTOR direction = transform_.quaRot.PosAxis(AsoUtility::DIR_F);

	openPos_ = VSub(transform_.pos, VScale(direction, 90.0f));
	closePos_ = transform_.pos;
}

void Drawer::Update(void)
{
	//現在の向きから進行方向を取得
	VECTOR direction = transform_.quaRot.PosAxis(AsoUtility::DIR_B);

	VECTOR movePow = VScale(direction, 1.0f);

	moveCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	switch (state_)
	{
	case Drawer::STATE::CLOSE:
		break;
	case Drawer::STATE::CLOSE_MOVE:
		if (moveCnt_ > 0.0f)
		{
			transform_.pos = VSub(transform_.pos, movePow);
		}
		else
		{
			ChangeState(STATE::CLOSE);
		}
		break;
	case Drawer::STATE::OPEN:
		break;
	case Drawer::STATE::OPEN_MOVE:
		if (moveCnt_ > 0.0f)
		{
			transform_.pos = VAdd(transform_.pos, movePow);
		}
		else
		{
			ChangeState(STATE::OPEN);
		}
		break;
	}

	transform_.Update();
}

void Drawer::Draw(void)
{
	VECTOR direction = transform_.quaRot.PosAxis(AsoUtility::DIR_F);

	MV1DrawModel(transform_.modelId);
	//DrawSphere3D({ transform_.pos.x - direction.x * 50.0f,transform_.pos.y - 40.0f,transform_.pos.z - direction.z * 50.0f }, 20.0f, 20, 0xff0000, 0xff0000, false);
}

void Drawer::Release(void)
{
}

Transform& Drawer::GetTransform(void)
{
	return transform_;
}

const VECTOR& Drawer::GetPos(void) const
{
    return transform_.pos;
}

void Drawer::Move(void)
{
	Quaternion rots = transform_.quaRot;

	//現在の向きから進行方向を取得
	VECTOR direction = rots.PosAxis(AsoUtility::DIR_B);

	VECTOR movePow = VScale(direction, 0.5f);

	transform_.pos = VAdd(transform_.pos, movePow);
}

bool Drawer::IsHitPlayer(VECTOR pPos, std::vector<int> otherModelIds)
{
	float disPow1 = 0.0f;
	float disPow2;
	bool isHitOther = false;

	for (const auto& otherModelId : otherModelIds)
	{
		VECTOR pos1 = { pPos.x,pPos.y + 14.0f ,pPos.z };
		VECTOR pos2 = mainCamera->GetTargetPos();

		auto info1 = MV1CollCheck_Line(transform_.modelId, -1, pos1, pos2);
		auto info2 = MV1CollCheck_Line(otherModelId, -1, pos1, pos2);

		//自分のモデルとプレイヤーの距離
		VECTOR diff1 = VSub(
			info1.HitPosition,
			pos1);
		disPow1 = sqrtf(diff1.x * diff1.x + diff1.y * diff1.y + diff1.z * diff1.z);

		//相手のモデルとプレイヤーの距離
		VECTOR diff2 = VSub(
			info2.HitPosition,
			pos1);
		disPow2 = sqrtf(diff2.x * diff2.x + diff2.y * diff2.y + diff2.z * diff2.z);

		if (disPow1 <= disPow2 && info1.HitFlag == 1)
		{
			isHitOther = false;
		}
		else
		{
			isHitOther = true;
		}

		if (isHitOther == true)
		{
			return false;
		}
	}
	if (isHitOther == false)
	{
		return true;
	}
}

Drawer::STATE Drawer::GetState(void)
{
	return state_;
}

void Drawer::ChangeState(STATE state)
{
	// 状態の更新
	state_ = state;

	// 状態別の初期化処理
	switch (state_)
	{
	case Drawer::STATE::CLOSE:
		transform_.pos = closePos_;
		break;
	case Drawer::STATE::CLOSE_MOVE:
		moveCnt_ = 1.5f;
		break;
	case Drawer::STATE::OPEN:
		transform_.pos = openPos_;
		break;
	case Drawer::STATE::OPEN_MOVE:
		moveCnt_ = 1.5f;
		break;
	}

}

void Drawer::SyncParent(Transform& transform, VECTOR addAxis)
{
	// 親の回転情報を取得
	Quaternion parentRot = transformParent_.quaRot;

	// 親回転からの相対回転
	Quaternion rot = Quaternion::Identity();

	rot = rot.Mult(transform_.quaRot);

	// 稼働分の回転を加える
	rot = rot.Mult(Quaternion::Euler(addAxis));

	// 親の回転と相対回転を合成
	transform.quaRot = parentRot.Mult(rot);

	// 親との相対座標を、親の回転情報に合わせて回転
	VECTOR localRotPos = Quaternion::PosAxis(parentRot, transform_.pos);

	// 相対座標をワールド座標に直す
	transform.pos = VAdd(transformParent_.pos, VScale(localRotPos, SCALE));

	// モデル制御の基本情報更新
	transform.Update();
}
