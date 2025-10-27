#pragma once
#include <vector>
#include "../../Common/Transform.h"

class DummyShelf
{
public:

	//’I‚Ì‘å‚«‚³
	static constexpr float SCALE = 0.4f;

	DummyShelf(const Transform& transformParaent,
		VECTOR localPos, float localRot);

	~DummyShelf(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	const VECTOR& GetPos(void) const;
	Transform& GetTransform(void);


private:

	const Transform& transformParent_;

	Transform transform_;

};

