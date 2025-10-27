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

	// ���N���X�̊֐����Ă�
	ItemBase::Init();
}

void PassiveItem::Update(void)
{
	// ���N���X�̊֐����Ă�
	ItemBase::Update();
}

void PassiveItem::Draw(void)
{
	// ���N���X�̊֐����Ă�
	ItemBase::Draw();
}

PassiveItem::PASSIVE PassiveItem::GetPassive(void)
{
	return passive_;
}

void PassiveItem::ChangePassive(PASSIVE passive)
{
	// ��Ԃ̍X�V
	passive_ = passive;

	// ��ԕʂ̏���������
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
	// ���f������̊�{���
	transform_.scl = { SCALE,SCALE,SCALE };
	transform_.quaRot = Quaternion();
	state_ = STATE::IN_MAP;
	transform_.Update();
}
