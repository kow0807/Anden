#pragma once
#include "../Common/Transform.h"

class EnemyBase
{
public:

	// アニメーションの再生速度
	static constexpr float SPEED_ANIM = 60.0f;

	// 回転量(deg)
	static constexpr float SPEED_ROT = 8.0f;

	EnemyBase(void);
	virtual ~EnemyBase(void);
	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	const Transform& GetTransform(void) const;

protected:

	// アニメーションのアタッチ番号
	int animAttachNo_;

	// アニメーションの総再生時間
	float animTotalTime_;

	// 再生中のアニメーション時間
	float stepAnim_;

	// パラメータ設定
	virtual void SetParam(void) = 0;

	// アニメーション
	void Animation(void);

	// モデル制御の基本情報
	Transform transform_;

	// 遅延回転
	void LazyRotation(float goalRot);
};
