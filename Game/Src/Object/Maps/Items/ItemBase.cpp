#include "../../../Manager/SceneManager.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Application.h"
#include "ItemBase.h"

ItemBase::ItemBase(void)
{

}

ItemBase::~ItemBase(void)
{
}

void ItemBase::Init(void)
{
	SetParam();
}

void ItemBase::Update(void)
{
	transform_.Update();
}

void ItemBase::Draw(void)
{
	switch (state_)
	{
	case ItemBase::STATE::NONE:
		break;
	case ItemBase::STATE::IN_MAP:
		MV1DrawModel(transform_.modelId);
		break;
	case ItemBase::STATE::HIT:
		MV1DrawModel(transform_.modelId);
		DrawString(Application::SCREEN_SIZE_X / 2 - 65, Application::SCREEN_SIZE_Y / 2 + 35, "アイテムを拾う", 0xffffff);
		break;
	case ItemBase::STATE::PICK:
		break;
	case ItemBase::STATE::USE:
		MV1DrawModel(transform_.modelId);
		break;
	}
}

void ItemBase::Release(void)
{
}

Transform& ItemBase::GetTransform(void)
{
	return transform_;
}

bool ItemBase::IsHitPlayer(VECTOR pPos, std::vector<int> otherModelIds)
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

void ItemBase::ChangeState(STATE state)
{
	// 状態の更新
	state_ = state;

	// 状態別の初期化処理
	switch (state_)
	{
	case ItemBase::STATE::NONE:
		break;
	case ItemBase::STATE::IN_MAP:
		break;
	case ItemBase::STATE::HIT:
		break;
	case ItemBase::STATE::PICK:
		break;
	case ItemBase::STATE::USE:
		break;
	}
}

ItemBase::STATE ItemBase::GetState(void)
{
	return state_;
}
