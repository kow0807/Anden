#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/ResourceManager.h"
#include "PassiveItem.h"

PassiveItem::PassiveItem(VECTOR pos, float axis, PASSIVE passive)
{
	transform_.pos = pos;
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(axis), 0.0f);
	passive_ = passive;
}

PassiveItem::~PassiveItem(void)
{
}

void PassiveItem::Init(void)
{
	switch (passive_)
	{
	case PassiveItem::PASSIVE::NONE:
		break;
	case PassiveItem::PASSIVE::STRAW_DOLL:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::STRAW_DOLL));
		break;
	case PassiveItem::PASSIVE::TABI:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::TABI));
		break;
	case PassiveItem::PASSIVE::GREEN_PURSE:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::GREEN_PURSE));
		break;
	case PassiveItem::PASSIVE::HAND_MIRROR:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::HAND_MIRROR));
		break;
	}

	// 基底クラスの関数を呼ぶ
	ItemBase::Init();
}

void PassiveItem::Update(void)
{
	// 基底クラスの関数を呼ぶ
	ItemBase::Update();
}

void PassiveItem::Draw(void)
{
	// 基底クラスの関数を呼ぶ
	ItemBase::Draw();
}

PassiveItem::PASSIVE PassiveItem::GetPassive(void)
{
	return passive_;
}

void PassiveItem::ChangePassive(PASSIVE passive)
{
	// 状態の更新
	passive_ = passive;

	// 状態別の初期化処理
	switch (passive_)
	{
	case PassiveItem::PASSIVE::NONE:
		break;
	case PassiveItem::PASSIVE::STRAW_DOLL:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::STRAW_DOLL));
		break;
	case PassiveItem::PASSIVE::TABI:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::TABI));
		break;
	case PassiveItem::PASSIVE::GREEN_PURSE:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::GREEN_PURSE));
		break;
	case PassiveItem::PASSIVE::HAND_MIRROR:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::HAND_MIRROR));
		break;
	}
}

void PassiveItem::SetParam(void)
{
	// モデル制御の基本情報
	transform_.scl = { SCALE,SCALE,SCALE };
	transform_.quaRot = Quaternion();
	state_ = STATE::IN_MAP;
	transform_.Update();
}
