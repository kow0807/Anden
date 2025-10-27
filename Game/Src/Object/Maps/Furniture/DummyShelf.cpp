#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/Transform.h"
#include "DummyShelf.h"

DummyShelf::DummyShelf(const Transform& transformParaent,
	VECTOR localPos, float localRot) :transformParent_(transformParaent)
{
	transform_.pos = localPos;
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(localRot), 0.0f);
	ResourceManager& ins = ResourceManager::GetInstance();
	transform_.SetModel(
		ins.LoadModelDuplicate(ResourceManager::SRC::DUMMY_SHELF));
}

DummyShelf::~DummyShelf(void)
{
}

void DummyShelf::Init(void)
{
	// モデル制御の基本情報
	transform_.scl = { SCALE,SCALE, SCALE };
	// 回転なし
	transform_.quaRot = Quaternion();


	// モデル制御の基本情報更新
	transform_.Update();

}

void DummyShelf::Update(void)
{
}

void DummyShelf::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void DummyShelf::Release(void)
{
}

const VECTOR& DummyShelf::GetPos(void) const
{
	return transform_.pos;
}

Transform& DummyShelf::GetTransform(void)
{
	return transform_;
}
