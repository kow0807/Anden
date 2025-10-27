#pragma once
#include "EnemyBase.h"

class Player;
class WayPoint;
class Map;

class Enemy2 : public EnemyBase
{
public:

	//プレイヤーと当たる距離
	static constexpr float HIT_DISTANCE = 40.0f;

	// 視野の広さ
	static constexpr float EYE_VIEW_RANGE = 300.0f;
	static constexpr float EAR_AREA = 300.0f;
	static constexpr float EAR_BIG_AREA = 1300.0f;

	// 視野角
	static constexpr float VIEW_ANGLE = 30.0f;

	static constexpr float SPEED_MOVE = 3.0f;

	enum class STATE
	{
		NONE,
		STOP,
		RETURN,
		ROUTE_MOVE,
		DIRECT_MOVE,
		ROUTE_BELL_MOVE,
		DIRECT_BELL_MOVE,
	};

	Enemy2(Player& player, WayPoint& wayPoint, Map& map, VECTOR pos);
	~Enemy2(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void DrawDebugSearchRange(void);

	//徘徊
	void Wandering(void);
	void ReturnUpdate(void);
	void UpdateRouteMove(void);
	void UpdateDirectMove(void);
	void UpdateRouteBellMove(void);
	void UpdateDirectBellMove(void);

	bool IsHitPlayer(void);
	bool GetIsNotice(void);
	STATE GetState(void);
	int GetNoticeBgmVol(void);

protected:

	Player& player_;
	WayPoint& wayPoint_;
	Map& map_;

	int animIdle_;
	int animWalking_;
	int animRun_;

	VECTOR startPos_;

	STATE state_;

	int noticeSe_;

	int noticeBgmVol_;

	float shakeCameraCnt_;

	//ウェイポイント
	int wayNum_;
	int oldIdx_;
	int oldIdx2_;

	int enemyNearPoint_;
	int targetNearPoint_;
	int nextPoint_;

	// 検知フラグ
	bool isNotice_;
	float noticeTime_;

	// パラメータ設定
	void SetParam(void) override;

	// 索敵
	bool EyeSearch(void);
	bool EarSearch(float area);
	void EarBigSearch(void);

	bool earBellFlag_;
	VECTOR bellPos_;
	float bellTime_;

	bool IsHitWayPoint(void);
	bool IsHitGoalWayPoint(void);

	void ChangeState(STATE state);
};
