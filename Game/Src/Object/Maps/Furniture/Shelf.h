#pragma once
#include <vector>
#include "../../Common/Transform.h"

class Shelf
{
public:

	//’I‚Ì‘å‚«‚³
	static constexpr float SCALE = 0.4f;

	Shelf(const Transform& transformParaent,
		VECTOR localPos, float localRot);

	~Shelf(void);

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

