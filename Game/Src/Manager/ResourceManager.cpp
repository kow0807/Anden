#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	Resource res;

	// スタートロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Title.png");
	resourcesMap_.emplace(SRC::START_LOGO, res);

	// エンドロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameOverLogo.png");
	resourcesMap_.emplace(SRC::GAMEOVER_LOGO, res);

	// クリアロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ClearLogo.png");
	resourcesMap_.emplace(SRC::CLEAR_LOGO, res);

	// 鈴UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Bell.png");
	resourcesMap_.emplace(SRC::BELL_UI, res);
	// 光る石UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/LightStone.png");
	resourcesMap_.emplace(SRC::LIGHT_STONE_UI, res);
	// 万華鏡UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Kaleidoscope.png");
	resourcesMap_.emplace(SRC::KALEIDOSCOPE_UI, res);
	// コンパスUI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Compass.png");
	resourcesMap_.emplace(SRC::COMPASS_UI, res);

	// 人形UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Doll.png");
	resourcesMap_.emplace(SRC::STRAW_DOLL_UI, res);
	// 靴UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Socks.png");
	resourcesMap_.emplace(SRC::TABI_UI, res);
	// 緑のバッグUI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/GreenBag.png");
	resourcesMap_.emplace(SRC::GREEN_PURSE_UI, res);
	// 手鏡UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/HandMirror.png");
	resourcesMap_.emplace(SRC::HAND_MIRROR_UI, res);

	// お守りUI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Charm.png");
	resourcesMap_.emplace(SRC::CHARM_UI, res);


	// プレイヤー
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Human/Human.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);


	// 敵1
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant.mv1");
	resourcesMap_.emplace(SRC::ENEMY, res);
	// 待機
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_IDLE, res);
	// 歩き
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Walking.mv1");
	resourcesMap_.emplace(SRC::ENEMY_WALK, res);
	// 走り
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Run.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RUN, res);

	// ゴールボックス
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "GoalBox/GoalBox.mv1");
	resourcesMap_.emplace(SRC::GOAL_BOX, res);

	// ゴール後ボックス
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "GoalBox/AfterGoalBox.mv1");
	resourcesMap_.emplace(SRC::AFTER_GOAL_BOX, res);

	//マップ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Map/Map.mv1");
	resourcesMap_.emplace(SRC::MAP, res);

	// 箪笥
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Furniture/Chest/Chest.mv1");
	resourcesMap_.emplace(SRC::CHEST, res);

	// 引き出し
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Furniture/Chest/Drawer.mv1");
	resourcesMap_.emplace(SRC::DRAWER, res);
	// ダミーの引き出し
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Dummy/dummyDrawers.mv1");
	resourcesMap_.emplace(SRC::DUMMY_DRAWER, res);

	// 棚
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Furniture/Shelf/Shelf.mv1");
	resourcesMap_.emplace(SRC::SHELF, res);
	// ダミー棚
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Dummy/dummyShelf.mv1");
	resourcesMap_.emplace(SRC::DUMMY_SHELF, res);

	// 隠れ箱
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Furniture/Chest/Chest.mv1");
	resourcesMap_.emplace(SRC::HIDEBOX, res);

	// 扉
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Area/door.mv1");
	resourcesMap_.emplace(SRC::DOOR, res);


	// 人形
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Doll/Doll.mv1");
	resourcesMap_.emplace(SRC::STRAW_DOLL, res);

	// 足袋
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Socks/Socks.mv1");
	resourcesMap_.emplace(SRC::TABI, res);

	// 緑の巾着
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Bag/GreenBag.mv1");
	resourcesMap_.emplace(SRC::GREEN_PURSE, res);

	// 手鏡
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/HandMirror/HandMirror.mv1");
	resourcesMap_.emplace(SRC::HAND_MIRROR, res);


	// 鈴
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Bell/Bell.mv1");
	resourcesMap_.emplace(SRC::BELL, res);

	// 光る石
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/LightStone/LightStone.mv1");
	resourcesMap_.emplace(SRC::LIGHT_STONE, res);

	// 光る石
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Kaleidoscope/Kaleidoscope.mv1");
	resourcesMap_.emplace(SRC::KALEIDOSCOPE, res);

	// コンパス(土台)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/compassBasefbx.mv1");
	resourcesMap_.emplace(SRC::COMPASS_BASE, res);

	// コンパス(針)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/compassbas.mv1");
	resourcesMap_.emplace(SRC::COMPASS_BAS, res);


	// お守り
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Charm/Charm.mv1");
	resourcesMap_.emplace(SRC::CHARM, res);

	// 移動画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/MoveImageKM.png");
	resourcesMap_.emplace(SRC::MOVE_IMAGE_KM, res);

	// カメラ操作画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/CameraControlImageKM.png");
	resourcesMap_.emplace(SRC::CAMERACONTROL_IMAGE_KM, res);

	// 走る画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/RunMoveImageKM.png");
	resourcesMap_.emplace(SRC::RUNMOVE_IMAGE_KM, res);

	// しゃがむ画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/CrouchImageKM.png");
	resourcesMap_.emplace(SRC::CROUCH_IMAGE_KM, res);

	// アクション画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/ActionImageKM.png");
	resourcesMap_.emplace(SRC::ACTION_IMAGE_KM, res);

	// アイテム使用画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/UseItemImageKM.png");
	resourcesMap_.emplace(SRC::USEITEM_IMAGE_KM, res);

	// アイテム選択画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/ChoiceImageKM.png");
	resourcesMap_.emplace(SRC::CHOICEITEM_IMAGE_KM, res);

	// 移動画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/MoveImageP.png");
	resourcesMap_.emplace(SRC::MOVE_IMAGE_P, res);

	// カメラ操作画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/CameraControlImageP.png");
	resourcesMap_.emplace(SRC::CAMERACONTROL_IMAGE_P, res);

	// 走る画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/RunMoveImageP.png");
	resourcesMap_.emplace(SRC::RUNMOVE_IMAGE_P, res);

	// しゃがむ画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/CrouchImageP.png");
	resourcesMap_.emplace(SRC::CROUCH_IMAGE_P, res);

	// アクション画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/ActionImageP.png");
	resourcesMap_.emplace(SRC::ACTION_IMAGE_P, res);

	// アイテム使用画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/UseItemImageP.png");
	resourcesMap_.emplace(SRC::USEITEM_IMAGE_P, res);

	// アイテム選択画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/ChoiceImageP.png");
	resourcesMap_.emplace(SRC::CHOICEITEM_IMAGE_P, res);

	// クリア条件画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/clearImageP.png");
	resourcesMap_.emplace(SRC::CLEAR_IMAGE, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

//int ResourceManager::LoadVertexShaderDuplicate(SRC src)
//{
//	Resource* res = _Load(src);
//	if (res == nullptr)
//	{
//		return -1;
//	}
//
//	int duId = LoadVertexShader(static_cast<>(res->handleId_));
//	res->duplicateModelIds_.push_back(duId);
//
//	return duId;
//}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
