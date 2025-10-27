#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/Transform.h"
#include "Drawer.h"
#include "DummyDrawers.h"
#include "Chest.h"

Chest::Chest(VECTOR localPos, float localRot)
{
	transform_.pos = localPos;
	transform_.quaRot = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(localRot), 0.0f);
	ResourceManager& ins = ResourceManager::GetInstance();
	transform_.SetModel(
		ins.LoadModelDuplicate(ResourceManager::SRC::CHEST));
}

Chest::~Chest(void)
{
}

void Chest::Init(void)
{
	// モデル制御の基本情報
	transform_.scl = { SCALE, SCALE, SCALE };
	// 回転なし
	//transform_.quaRot = Quaternion();
	// ローカル回転(正面をZの正方向に合わせる)
	transform_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	MakeDrawers({ 0.0f,130.0f,-3.0f }, { 0.0f });
	MakeDrawers({ 0.0f,76.0f,-3.0f }, { 0.0f });
	MakeDrawers({ 0.0f,20.0f,-3.0f }, { 0.0f });
	MakeDummyDrawers({ 0.0f,130.0f,-3.0f }, { 0.0f });
	MakeDummyDrawers({ 0.0f,76.0f,-3.0f }, { 0.0f });
	MakeDummyDrawers({ 0.0f,20.0f,-3.0f }, { 0.0f });

	// モデル制御の基本情報更新
	transform_.Update();
}

void Chest::Update(void)
{
	for (const auto& d : drawers_) {
		d->Update();
	}
	for (const auto& dd : dummyDrawers_) {
		dd->Update();
	}

	transform_.Update();
}

void Chest::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	for (const auto& d : drawers_) {
		d->Draw();
	}
	//for (const auto& dd : dummyDrawers_) {
	//	dd->Draw();
	//}

}

void Chest::Release(void)
{
}

Transform& Chest::GetTransform(void)
{
	return transform_;
}

const VECTOR& Chest::GetPos(void) const
{
	return transform_.pos;
}

const std::vector<std::unique_ptr<Drawer>>& Chest::GetDrawers(void) const
{
	return drawers_;
}

const std::vector<std::unique_ptr<DummyDrawer>>& Chest::GetDummyDrawers(void) const
{
	return dummyDrawers_;
}

void Chest::MakeDrawers(VECTOR localPos, float localAddAxis)
{
	Drawer* drawer = new Drawer(transform_, localPos, localAddAxis);

	// 初期化
	drawer->Init();
	drawers_.emplace_back(drawer);
}

void Chest::MakeDummyDrawers(VECTOR localPos, float localAddAxis)
{
	DummyDrawer* dummyDrawer = new DummyDrawer(transform_, localPos, localAddAxis);

	// 初期化
	dummyDrawer->Init();
	dummyDrawers_.emplace_back(dummyDrawer);
}
