#pragma once
#include <memory>
#include "SceneBase.h"

class Grid;
class Map;
class WayPoint;
class Player;
class Enemy1;
class Enemy2;

class GameScene : public SceneBase
{
public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:


	//�O���b�g��
	std::unique_ptr<Grid> grid_;

	//���ׂẴ}�b�v
	std::shared_ptr<Map> map_;

	//�E�F�C�|�C���g
	std::shared_ptr<WayPoint> wayPoint_;

	std::shared_ptr<Player> player_;

	std::vector<std::unique_ptr<Enemy1>> enemy1s_;
	std::vector<std::unique_ptr<Enemy2>> enemy2s_;


	bool bgmFlag_;
	int noticeBgm_;
	int noticeBgmVol_;
	int clearSe_;

	int bellUi_;
	int lightStoneUi_;
	int kaleidoscopeUi_;
	int compassUi_;

	int strawDollUi_;
	int tabiUi_;
	int bagUi_;
	int handMirrorUi_;

	int charmUi_;

	float saveTime_;
	float timeSec_;
	int timeSec1_;
	int timeSec2_;
	int timeMin1_;
	int timeMin2_;
	int timeHour1_;
	int timeHour2_;

	void MakeEnemy1(VECTOR pos);
	void MakeEnemy2(VECTOR pos);

	void SoundUpdate(void);
	void UIDraw(void);
};

