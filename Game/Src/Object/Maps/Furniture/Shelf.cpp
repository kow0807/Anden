#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/Transform.h"
#include "Shelf.h"

Shelf::Shelf(const Transform& transformParaent,
	VECTOR localPos, float localRot):transformParent_(transformParaent)
{
	transform_.pos = localPos;
	transform_.quaRotLocal= Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(localRot), 0.0f);
	ResourceManager& ins = ResourceManager::GetInstance();
	transform_.SetModel(
		ins.LoadModelDuplicate(ResourceManager::SRC::SHELF));
}

Shelf::~Shelf(void)
{
}

void Shelf::Init(void)
{
	// モデル制御の基本情報
	transform_.scl = { SCALE,SCALE, SCALE };
	// 回転なし
	transform_.quaRot = Quaternion();


	// モデル制御の基本情報更新
	transform_.Update();

}

void Shelf::Update(void)
{
}

void Shelf::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void Shelf::Release(void)
{
}

const VECTOR& Shelf::GetPos(void) const
{
	return transform_.pos;
}

Transform& Shelf::GetTransform(void)
{
	return transform_;
}
