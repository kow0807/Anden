#include "../Utility/AsoUtility.h"
#include "WayPoint.h"

WayPoint::WayPoint(void)
{

}

WayPoint::~WayPoint(void)
{
}

void WayPoint::Init(void)
{

	wayPoints_.emplace_back(VECTOR{ 3600.0f, 180.0f, -3520.0f });
	wayPoints_.emplace_back(VECTOR{ 3600.0f, 180.0f, -1800.0f });
	wayPoints_.emplace_back(VECTOR{ 3600.0f, 180.0f, -1120.0f });
	wayPoints_.emplace_back(VECTOR{ 3100.0f, 180.0f, -1080.0f });
	wayPoints_.emplace_back(VECTOR{ 1590.0f, 180.0f, -1080.0f });
	wayPoints_.emplace_back(VECTOR{ 1590.0f, 180.0f, -1590.0f });
	wayPoints_.emplace_back(VECTOR{ 620.0f, 180.0f, -1590.0f });
	wayPoints_.emplace_back(VECTOR{ 1590.0f, 180.0f, -530.0f });
	wayPoints_.emplace_back(VECTOR{ 610.0f, 180.0f, -530.0f });
	wayPoints_.emplace_back(VECTOR{ 610.0f, 180.0f, -1080.0f });
	wayPoints_.emplace_back(VECTOR{ -830.0f, 180.0f, -1080.0f });
	wayPoints_.emplace_back(VECTOR{ -1375.0f, 180.0f, -1080.0f });
	wayPoints_.emplace_back(VECTOR{ -1375.0f, 180.0f, -1610.0f });
	wayPoints_.emplace_back(VECTOR{ -1375.0f, 180.0f, -3220.0f });
	wayPoints_.emplace_back(VECTOR{ -1375.0f, 180.0f, -3640.0f });
	wayPoints_.emplace_back(VECTOR{ -1375.0f, 180.0f, -4380.0f });
	wayPoints_.emplace_back(VECTOR{ -1375.0f, 180.0f, -5350.0f });
	wayPoints_.emplace_back(VECTOR{ -1375.0f, 180.0f, -5990.0f });
	wayPoints_.emplace_back(VECTOR{ -740.0f, 180.0f, -5990.0f });
	wayPoints_.emplace_back(VECTOR{ 620.0f, 180.0f, -5990.0f });
	wayPoints_.emplace_back(VECTOR{ 620.0f, 180.0f, -5490.0f });
	wayPoints_.emplace_back(VECTOR{ 1600.0f, 180.0f, -5490.0f });
	wayPoints_.emplace_back(VECTOR{ 620.0f, 180.0f, -6530.0f });
	wayPoints_.emplace_back(VECTOR{ 1600.0f, 180.0f, -6530.0f });
	wayPoints_.emplace_back(VECTOR{ 1600.0f, 180.0f, -5990.0f });
	wayPoints_.emplace_back(VECTOR{ 3110.0f, 180.0f, -5990.0f });
	wayPoints_.emplace_back(VECTOR{ 3600.0f, 180.0f, -5990.0f });
	wayPoints_.emplace_back(VECTOR{ 4120.0f, 180.0f, -5990.0f });

	////28～
	//wayPoints_.emplace_back(VECTOR{ 4190.0f, 180.0f, -3520.0f });
	//wayPoints_.emplace_back(VECTOR{ 4190.0f, 180.0f, -4100.0f });
	//wayPoints_.emplace_back(VECTOR{ 3150.0f, 180.0f, -3520.0f });
	//wayPoints_.emplace_back(VECTOR{ 3100.0f, 180.0f, -1800.0f });
	//wayPoints_.emplace_back(VECTOR{ 4100.0f, 180.0f, -1120.0f });
	//wayPoints_.emplace_back(VECTOR{ 4100.0f, 180.0f, -600.0f });
	//wayPoints_.emplace_back(VECTOR{ 3100.0f, 180.0f, -600.0f });
	//wayPoints_.emplace_back(VECTOR{ -830.0f, 180.0f, -550.0f });
	//wayPoints_.emplace_back(VECTOR{ -830.0f, 180.0f, -1500.0f });
	//wayPoints_.emplace_back(VECTOR{ -1760.0f, 180.0f, -1610.0f });
	//wayPoints_.emplace_back(VECTOR{ -1700.0f, 180.0f, -3220.0f });
	//wayPoints_.emplace_back(VECTOR{ -900.0f, 180.0f, -3640.0f });
	//wayPoints_.emplace_back(VECTOR{ -900.0f, 180.0f, -4100.0f });
	//wayPoints_.emplace_back(VECTOR{ -900.0f, 180.0f, -3200.0f });
	//wayPoints_.emplace_back(VECTOR{ 700.0f, 180.0f, -3640.0f });
	//wayPoints_.emplace_back(VECTOR{ -1750.0f, 180.0f, -4380.0f });
	//wayPoints_.emplace_back(VECTOR{ -1800.0f, 180.0f, -5350.0f });
	//wayPoints_.emplace_back(VECTOR{ -1800.0f, 180.0f, -6300.0f });
	//wayPoints_.emplace_back(VECTOR{ 3110.0f, 180.0f, -6320.0f });
	//wayPoints_.emplace_back(VECTOR{ 4120.0f, 180.0f, -5400.0f });


	std::vector<int> links;

	links.clear();
	links.emplace_back(1);
	links.emplace_back(26);
	indexLinks_.emplace(0, links);

	links.clear();
	links.emplace_back(0);
	links.emplace_back(2);
	indexLinks_.emplace(1, links);

	links.clear();
	links.emplace_back(1);
	links.emplace_back(3);
	indexLinks_.emplace(2, links);

	links.clear();
	links.emplace_back(2);
	links.emplace_back(4);
	indexLinks_.emplace(3, links);

	links.clear();
	links.emplace_back(3);
	links.emplace_back(5);
	links.emplace_back(7);
	indexLinks_.emplace(4, links);

	links.clear();
	links.emplace_back(4);
	links.emplace_back(6);
	indexLinks_.emplace(5, links);

	links.clear();
	links.emplace_back(5);
	indexLinks_.emplace(6, links);

	links.clear();
	links.emplace_back(4);
	links.emplace_back(8);
	indexLinks_.emplace(7, links);

	links.clear();
	links.emplace_back(7);
	links.emplace_back(9);
	indexLinks_.emplace(8, links);

	links.clear();
	links.emplace_back(8);
	links.emplace_back(10);
	indexLinks_.emplace(9, links);

	links.clear();
	links.emplace_back(9);
	links.emplace_back(11);
	indexLinks_.emplace(10, links);

	links.clear();
	links.emplace_back(10);
	links.emplace_back(12);
	indexLinks_.emplace(11, links);

	links.clear();
	links.emplace_back(11);
	links.emplace_back(13);
	indexLinks_.emplace(12, links);

	links.clear();
	links.emplace_back(12);
	links.emplace_back(14);
	indexLinks_.emplace(13, links);

	links.clear();
	links.emplace_back(13);
	links.emplace_back(15);
	indexLinks_.emplace(14, links);

	links.clear();
	links.emplace_back(14);
	links.emplace_back(16);
	indexLinks_.emplace(15, links);

	links.clear();
	links.emplace_back(15);
	links.emplace_back(17);
	indexLinks_.emplace(16, links);

	links.clear();
	links.emplace_back(16);
	links.emplace_back(18);
	indexLinks_.emplace(17, links);

	links.clear();
	links.emplace_back(17);
	links.emplace_back(19);
	indexLinks_.emplace(18, links);

	links.clear();
	links.emplace_back(18);
	links.emplace_back(20);
	links.emplace_back(22);
	indexLinks_.emplace(19, links);

	links.clear();
	links.emplace_back(19);
	links.emplace_back(21);
	indexLinks_.emplace(20, links);

	links.clear();
	links.emplace_back(20);
	indexLinks_.emplace(21, links);

	links.clear();
	links.emplace_back(19);
	links.emplace_back(23);
	indexLinks_.emplace(22, links);

	links.clear();
	links.emplace_back(22);
	links.emplace_back(24);
	indexLinks_.emplace(23, links);

	links.clear();
	links.emplace_back(23);
	links.emplace_back(25);
	indexLinks_.emplace(24, links);

	links.clear();
	links.emplace_back(24);
	links.emplace_back(26);
	indexLinks_.emplace(25, links);

	links.clear();
	links.emplace_back(0);
	links.emplace_back(25);
	links.emplace_back(27);
	indexLinks_.emplace(26, links);

	links.clear();
	links.emplace_back(26);
	indexLinks_.emplace(27, links);


	oldIdx_ = 9999999;
}

void WayPoint::Update(void)
{
}

void WayPoint::Draw(void)
{
	for (int i = 0; i < wayPoints_.size(); i++)
	{
		DrawSphere3D(wayPoints_[i], 20.0f, 10, 0xff0000, 0xff0000, true);
	}

	DrawSphere3D(wayPoints_[wayPoints_.size()-1], 20.0f, 10, 0xffff00, 0xffff00, true);
	//DrawSphere3D(wayPoints_[1], 20.0f, 10, 0xff0000, 0xff0000, true);
	//DrawSphere3D(wayPoints_[2], 20.0f, 10, 0xff0000, 0xff0000, true);
	//DrawSphere3D(wayPoints_[3], 20.0f, 10, 0xff0000, 0xff0000, true);
	//DrawSphere3D(wayPoints_[4], 20.0f, 10, 0xff0000, 0xff0000, true);
	//DrawSphere3D(wayPoints_[5], 20.0f, 10, 0xff0000, 0xff0000, true);
	//DrawSphere3D(wayPoints_[6], 20.0f, 10, 0xff0000, 0xff0000, true);
	//DrawSphere3D(wayPoints_[7], 20.0f, 10, 0xff0000, 0xff0000, true);
	//DrawSphere3D(wayPoints_[8], 20.0f, 10, 0xff0000, 0xff0000, true);
}

void WayPoint::Release(void)
{
}

VECTOR WayPoint::GetPos(void)
{
	return pos_;
}

std::vector<VECTOR> WayPoint::GetWayPoints(void)
{
	return wayPoints_;
}

int WayPoint::GetNearWayPointIndex(VECTOR pos)
{

	float minLength = 9999999999999;
	int minIndex = -1;
	int idx = 0;
	for (const auto& point : wayPoints_)
	{
		VECTOR diff = VSub(pos, point);
		float dis = AsoUtility::SqrMagnitude(diff);
		if (dis < minLength)
		{
			minIndex = idx;
			minLength = dis;
		}
		idx++;
	}

	return minIndex;

}

int WayPoint::GetNextWayPointIndex(int startIdx, int goalIdx, int oldIdx)
{

	if (startIdx == goalIdx)
	{
		return goalIdx;
	}

	oldIdx_ = oldIdx;

	VECTOR startPos = wayPoints_[startIdx];
	VECTOR goalPos = wayPoints_[goalIdx];

	std::vector<int> links = indexLinks_[startIdx];

	if (links.size() == 1)
	{
		// 行き先が1つしかないので、次のウェイポイントが確定
		return links[0];
		// wayPoints_[links[0]];→次に行くウェイポイントの座標
	}
	else
	{
		float minLength = 9999999999999;
		int minIndex = -1;
		int idx = 0;
		for (const auto& linkIdx : links)
		{
			if (linkIdx == oldIdx_)
			{
				continue;
			}
			// ゴールに近い
			VECTOR diff = VSub(goalPos, wayPoints_[linkIdx]);
			float dis = AsoUtility::SqrMagnitude(diff);
			if (dis < minLength)
			{
				minIndex = linkIdx;
				minLength = dis;
			}
			idx++;
		}
		// ゴールに一番近いところが、次のウェイポイント
		return minIndex;
	}

	return -1;

}
