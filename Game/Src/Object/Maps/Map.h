#pragma once
#include <memory>
#include <DxLib.h>
#include <vector>
#include "../Common/Transform.h"
#include"Items/PassiveItem.h"
#include"Items/ActiveItem.h"
#include"Items/DrawerActiveItem.h"

class Player;
class Chest;
class Shelf;
class DummyShelf;

class Map
{
public:
	// 定数
	static constexpr float SIZE_X = 7200.0f;
	static constexpr float SIZE_Y = 400.0f;
	static constexpr float SIZE_Z = 7200.0f;

	// 初期位置
	static constexpr float POS_X = 3600.0f;
	static constexpr float POS_Y = 0.0f;
	static constexpr float POS_Z = -3600.0f;

	static constexpr VECTOR GOAL_BOX_POS = { 1200.0f, 0.0f, -3600.0f };

	// コンストラクタ
	Map(Player& player);

	// デストラクタ
	~Map(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	int GetModelId(void);

	//パッシブアイテム
	bool IsClickPassiveItem(void);
	void PassiveItemUpdate(void);

	//アクティブアイテム
	bool IsClickActiveItem(void);
	void ActiveItemUpdate(void);
	void UseActiveItem(void);

	//引き出しに入ったアクティブアイテム
	bool IsClickDrawerActiveItem(void);
	void DrawerActiveItemUpdate(void);

	std::vector<std::shared_ptr<ActiveItem>> GetActiveItem(void);
	std::vector<std::shared_ptr<DrawerActiveItem>> GetDrawerActiveItem(void);

	bool IsClear(void);

private:

	//頂点シェーダー
	int vertexShaderHandle_;

	//ピクセルシェーダー
	int pixelShaderHandle_;

	//プレイヤー
	Player& player_;

	int itemGetSe_;

	//ゴールの箱
	int goalBoxModelId_;
	int afterGoalBoxModelId_;

	//マップ上のモデル
	std::vector<int> modelIds_;

	//プレイヤーと判定を取るモデル
	std::vector<int> obstacles_;

	//光る石番号
	std::vector<VECTOR> lightStoneNum_;
	int lightHandle[19];


	// パッシブアイテム
	std::vector<std::unique_ptr<PassiveItem>> passiveItems_;

	//アクティブアイテム
	std::vector<std::shared_ptr<ActiveItem>> activeItems_;

	//引き出しに入ったアクティブアイテム
	std::vector<std::shared_ptr<DrawerActiveItem>> drawerActiveItems_;

	// 箪笥
	std::vector<std::unique_ptr<Chest>> chests_;

	// 棚
	std::vector<std::unique_ptr<Shelf>> shelfs_;
	// ダミー棚
	std::vector<std::unique_ptr<DummyShelf>> dummyShelfs_;

	// モデル制御の基本情報
	Transform transform_;

	// 画像情報
	int PMoveImage_;					// 移動 
	int PCameraControlImage_;		// カメラ操作
	int PRunMoveImage_;				// 走る
	int PCrouchImage_;				// しゃがむ
	int PActionImage_;				// アクション
	int PUseItemImage_;				// アイテム使用
	int PChoiceItemImage_;			// アイテム選択

	int KMMoveImage_;					// 移動 
	int KMCameraControlImage_;		// カメラ操作
	int KMRunMoveImage_;				// 走る
	int KMCrouchImage_;				// しゃがむ
	int KMActionImage_;				// アクション
	int KMUseItemImage_;				// アイテム使用
	int KMChoiceItemImage_;			// アイテム選択

	int clearImage_;				// クリア条件

	float passiveDis_;
	float activeDis_;
	int bellSound_;
	int drawerSound_;

	bool goalBoxFlag_;

	void ImageInit(void);
	void ImageDraw(void);

	void DrawGoalBox(void);

	void DrawChest(void);
	void DrawShelf(void);
	void DrawDummyShelf(void);

	//パッシブアイテム
	void DrawPassiveItem(void);
	void MakePassiveItem(VECTOR pos, float axis, PassiveItem::PASSIVE passive);

	//アクティブアイテム
	void DrawActiveItem(void);
	void MakeActiveItem(VECTOR pos, float axis, ActiveItem::ACTIVE active);

	//引き出しに入ったアクティブアイテム
	void DrawDrawerActiveItem(void);
	void MakeDrawerActiveItem(VECTOR pos, float axis, DrawerActiveItem::ACTIVE active,int chestNum,int drawerNum);

	//鈴を使ったとき
	void BellUpdate(void);

	//光る石を使ったとき
	void LightSroneUpdate(void);

	//コンパスを使ったとき
	void CompassUpdate(void);

	// 箪笥の作成
	void MakeChest(VECTOR localPos, float localAddAxis);

	// 棚の作成
	void MakeShelf(VECTOR localPos, float localAddAxis);
	// ダミー棚の作成
	void MakeDummyShelf(VECTOR localPos, float localAddAxis);

	bool IsHitClearObject(void);
};

