#pragma once
#include <vector>
#include "../../Common/Transform.h"

class DummyDrawer
{
public:

	enum class STATE
	{
		CLOSE,
		CLOSE_MOVE,
		OPEN,
		OPEN_MOVE
	};

	//à¯Ç´èoÇµÇÃëÂÇ´Ç≥
	static constexpr float SCALE = 0.5f;

	DummyDrawer(const Transform& transformParaent,
		VECTOR localPos, float localRot);

	~DummyDrawer(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	Transform& GetTransform(void);

	const VECTOR& GetPos(void) const;
	void SetPos(VECTOR pos);

	void Move(void);
	bool IsHitPlayer(VECTOR pPos, std::vector<int> otherModelIds);

	STATE GetState(void);
	void ChangeState(STATE state);
private:

	STATE state_;

	const Transform& transformParent_;

	VECTOR localPos_;
	VECTOR localRot_;
	Transform transform_;

	float moveCnt_;
	VECTOR openPos_;
	VECTOR closePos_;

	void SyncParent(Transform& transform, VECTOR addAxis);

};

