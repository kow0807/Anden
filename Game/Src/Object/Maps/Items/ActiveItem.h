#pragma once
#include <math.h>
#include "ItemBase.h"

class ActiveItem : public ItemBase
{
public:

	enum class ACTIVE
	{
		NONE,
		BELL,
		LIGHT_STONE,
		KALEIDOSCOPE,
		COMPASS,
		CHARM
	};


	// 衝突用球体半径
	const float COL_RADIUS = 20.0f;

	// 初期スピード
	const float INITIAL_SPEED = 15.0f;

	// 減速
	const float DECEL = 0.005f;

	// 初期重力
	const float INITIAL_GRAVITY = 0.0f;

	// 反射時初期重力
	const float REFRECT_GRAVITY = 2.0f;

	// 加算重力
	const float ADD_GRAVITY = 9.8f / 40.0f;

	// 最大重力
	const float MAX_GRAVITY = 30.0f;

	// ベルの生存時間
	const float BELL_ALIVE_TIME = 5.0f;

	// 光源の最大数
	const int LIGHT_MAX_NUM = 20;

	ActiveItem(VECTOR pos, float axis, ACTIVE active);
	~ActiveItem(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Move(VECTOR pPos, Quaternion pRot);

	void BellThrow(int modelId);

	void SetPos(VECTOR pos);

	ACTIVE GetActive(void);
	void ChangeActive(ACTIVE active);

	void ChangeState(STATE state)override;

	//コンパスの針の動き
	void CompassBasUpdate(VECTOR pPos, VECTOR goalPos);

protected:

	int compassBasModelId_;
	VECTOR compassBasPos_;
	VECTOR compassBasRot_;
	Quaternion compassBasQuaRot_;

	ACTIVE active_;

	float speed_;
	VECTOR movePow_;
	float gravity_;
	VECTOR gravityPow_;

	VECTOR dir_;

	float bellCnt_;

	//頂点シェーダー
	int vertexShaderHandle_;

	//ピクセルシェーダー
	int pixelShaderHandle_;

	int lightHandle[20];

	// パラメータ設定
	void SetParam(void) override;
};
