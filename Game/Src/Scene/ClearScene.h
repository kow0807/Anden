#pragma once
#include "SceneBase.h"

class ClearScene : public SceneBase
{

public:

	// コンストラクタ
	ClearScene(void);

	// デストラクタ
	~ClearScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	void ClearTimeDraw(void);
private:
	int logo_;
};
