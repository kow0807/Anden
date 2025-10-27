#pragma once
#include <vector>
#include <map>
#include "Common/Transform.h"


class WayPoint
{
public:

	// コンストラクタ
	WayPoint(void);
	// デストラクタ
	~WayPoint(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void);
	std::vector<VECTOR> GetWayPoints(void);

	int GetNearWayPointIndex(VECTOR pos);
	int GetNextWayPointIndex(int startIdx, int goalIdx,int oldIdx);
private:
	// モデル制御の基本情報
	Transform transform_;

	VECTOR pos_;

	std::vector<VECTOR> wayPoints_;
	std::map<int, std::vector<int>> indexLinks_;

	int oldIdx_;
};

