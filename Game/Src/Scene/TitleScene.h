#pragma once
#include <string>
#include "SceneBase.h"

class Player;
class Map;

class TitleScene : public SceneBase
{

public:

	enum class STATE
	{
		NONE,
		START,
		RANKING,
		IN_RANKING,
		OUT_RANKING,
		EXIT,
	};

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	STATE state_;
	// タイトルロゴ
	int imgTitleLogo_;

	//すべてのマップ
	std::shared_ptr<Map> map_;
	std::shared_ptr<Player> player_;

	int bgm_;
	bool bgmFlag_;

	std::string start_;
	std::string ranking_;
	std::string exit_;

	float startPosX_;
	float startPosY_;
	float rankingPosX_;
	float rankingPosY_;
	float exitPosX_;
	float exitPosY_;
	float closePosX_;
	float closePosY_;

	float startSizeX_;
	float startSizeY_;
	float rankingSizeX_;
	float rankingSizeY_;
	float exitSizeX_;
	float exitSizeY_;
	float closeSizeX_;
	float closeSizeY_;

	float povFlag_;

	// ロゴ
	void LogoUpdate(void);
	void LogoDraw(void);

	void RankingDraw(void);
	void TimeDraw(void);

	void ChangeState(STATE state);
};
