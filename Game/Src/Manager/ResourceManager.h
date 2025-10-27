#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		START_LOGO,
		GAMEOVER_LOGO,
		CLEAR_LOGO,

		BELL_UI,
		LIGHT_STONE_UI,
		KALEIDOSCOPE_UI,
		COMPASS_UI,

		STRAW_DOLL_UI,
		TABI_UI,
		GREEN_PURSE_UI,
		HAND_MIRROR_UI,

		CHARM_UI,

		PLAYER,
		ENEMY,
		ENEMY_IDLE,
		ENEMY_WALK,
		ENEMY_RUN,

		MAP,
		GOAL_BOX,
		AFTER_GOAL_BOX,
		CHEST,
		DRAWER,
		DUMMY_DRAWER,
		SHELF,
		DUMMY_SHELF,
		HIDEBOX,
		DOOR,

		STRAW_DOLL,
		TABI,
		GREEN_PURSE,
		HAND_MIRROR,

		BELL,
		LIGHT_STONE,
		KALEIDOSCOPE,
		COMPASS_BASE,
		COMPASS_BAS,

		CHARM,

		MOVE_IMAGE_KM,
		CAMERACONTROL_IMAGE_KM,
		RUNMOVE_IMAGE_KM,
		CROUCH_IMAGE_KM,
		ACTION_IMAGE_KM,
		USEITEM_IMAGE_KM,
		CHOICEITEM_IMAGE_KM,

		MOVE_IMAGE_P,
		CAMERACONTROL_IMAGE_P,
		RUNMOVE_IMAGE_P,
		CROUCH_IMAGE_P,
		ACTION_IMAGE_P,
		USEITEM_IMAGE_P,
		CHOICEITEM_IMAGE_P,

		CLEAR_IMAGE

	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

	////リソースの複製ロード(頂点シェーダー用)
	//int LoadVertexShaderDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};

