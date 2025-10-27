#pragma once
#include <vector>
#include <memory>
#include "../../Common/Transform.h"

class Drawer;
class DummyDrawer;

class Chest
{
public:
	//�����o���̑傫��
	static constexpr float SCALE = 0.5f;

	Chest(VECTOR localPos, float localRot);

	~Chest(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	Transform& GetTransform(void);


	const VECTOR& GetPos(void) const;
	const std::vector<std::unique_ptr<Drawer>>& GetDrawers(void) const;
	const std::vector<std::unique_ptr<DummyDrawer>>& GetDummyDrawers(void) const;


private:

	std::vector<std::unique_ptr<Drawer>> drawers_;
	std::vector<std::unique_ptr<DummyDrawer>> dummyDrawers_;

	Transform transform_;

	// �����o���̍쐬
	void MakeDrawers(VECTOR localPos, float localAddAxis);
	// �_�~�[�̈����o���̍쐬
	void MakeDummyDrawers(VECTOR localPos, float localAddAxis);
};

