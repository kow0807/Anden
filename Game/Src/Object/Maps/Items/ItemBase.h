#pragma once
#include <vector>
#include "../../Common/Transform.h"

class ItemBase
{
public:

	enum class STATE
	{
		NONE,
		IN_MAP,
		HIT,
		PICK,
		USE
	};

	// アイテムの大きさ
	static constexpr float SCALE = 0.3f;

	// コンストラクタ
	ItemBase(void);
	// デストラクタ
	virtual ~ItemBase(void);
	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	Transform& GetTransform(void);

	bool IsHitPlayer(VECTOR pPos, std::vector<int> otherModelIds);

	virtual void ChangeState(STATE state);
	STATE GetState(void);

protected:

	// パラメータ設定
	virtual void SetParam(void) = 0;

	STATE state_;

	// モデル制御の基本情報
	Transform transform_;
};

