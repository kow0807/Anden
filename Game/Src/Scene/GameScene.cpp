#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Maps/Map.h"
#include "../Object/WayPoint.h"
#include "../Manager/Camera.h"
#include "../Manager/CameraLight.h"
#include "../Object/Player.h"
#include "../Object/Enemy/Enemy1.h"
#include "../Object/Enemy/Enemy2.h"
#include "../Object/Grid.h"
#include "GameScene.h"

GameScene::GameScene(void)
{

}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	ChangeFont("ＭＳ 明朝");

	//グリット線初期化
	grid_ = std::make_unique<Grid>();
	grid_->Init();

	player_ = std::make_shared<Player>();
	player_->Init();

	map_ = std::make_shared<Map>(*player_);
	map_->Init();

	wayPoint_ = std::make_shared<WayPoint>();
	wayPoint_->Init();

	MakeEnemy1({ -1380.0f, 10.0f, -1700.0f });
	MakeEnemy1({ -1380.0f, 10.0f, -5800.0f });
	MakeEnemy2({ 1600.0f, 10.0f, -5500.0f });

	// カメラモード変更
	mainCamera->SetFollow(&player_->GetTransform());
	mainCamera->ChangeMode(Camera::MODE::FOLLOW);
	mainCamera->SetAngles({ -10.0f,0.0f,0.0f });

	bgmFlag_ = true;
	noticeBgm_ = LoadSoundMem("Data/Sound/noticeBgm.mp3");
	noticeBgmVol_ = 0;
	clearSe_ = LoadSoundMem("Data/Sound/clear.mp3");

	bellUi_= ResourceManager::GetInstance().Load(ResourceManager::SRC::BELL_UI).handleId_;
	lightStoneUi_= ResourceManager::GetInstance().Load(ResourceManager::SRC::LIGHT_STONE_UI).handleId_;
	kaleidoscopeUi_= ResourceManager::GetInstance().Load(ResourceManager::SRC::KALEIDOSCOPE_UI).handleId_;
	compassUi_= ResourceManager::GetInstance().Load(ResourceManager::SRC::COMPASS_UI).handleId_;

	strawDollUi_= ResourceManager::GetInstance().Load(ResourceManager::SRC::STRAW_DOLL_UI).handleId_;
	tabiUi_= ResourceManager::GetInstance().Load(ResourceManager::SRC::TABI_UI).handleId_;
	bagUi_= ResourceManager::GetInstance().Load(ResourceManager::SRC::GREEN_PURSE_UI).handleId_;
	handMirrorUi_= ResourceManager::GetInstance().Load(ResourceManager::SRC::HAND_MIRROR_UI).handleId_;

	charmUi_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARM_UI).handleId_;

	saveTime_ = 0.0f;
	timeSec_ = 0.0f;
	timeSec1_ = 0;
	timeSec2_ = 0;
	timeMin1_ = 0;
	timeMin2_ = 0;
	timeHour1_ = 0;
	timeHour2_ = 0;
	TimeLoad();
}

void GameScene::Update(void)
{
	saveTime_ += SceneManager::GetInstance().GetDeltaTime();
	timeSec_ += SceneManager::GetInstance().GetDeltaTime();
	timeSec1_ = timeSec_;
	if (timeSec1_ >= 10)
	{
		timeSec_ = 0;
		timeSec1_ = 0;
		timeSec2_++;
	}
	if (timeSec2_ >= 6)
	{
		timeSec2_ = 0;
		timeMin1_++;
	}
	if (timeMin1_ >= 10)
	{
		timeMin1_ = 0;
		timeMin2_++;
	}
	if (timeMin2_ >= 6)
	{
		timeMin2_ = 0;
		timeHour1_++;
	}
	if (timeHour1_ >= 10)
	{
		timeHour1_ = 0;
		timeHour2_++;
	}

	grid_->Update();
	player_->Update();

	for (const auto& e1 : enemy1s_)
	{
		e1->Update();
	}
	for (const auto& e2 : enemy2s_)
	{
		e2->Update();
	}

	SoundUpdate();

	wayPoint_->Update();
	map_->Update();
	player_->AfterUpdate();

	if (player_->GetIsAlive() == false)
	{
		StopSoundMem(noticeBgm_);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

	if (map_->IsClear() == true)
	{
		StopSoundMem(noticeBgm_);
		ChangeNextPlayVolumeSoundMem(170, clearSe_);
		PlaySoundMem(clearSe_, DX_PLAYTYPE_BACK);
		time_ = saveTime_;

		if (hiTime_ >= time_)
		{
			fifthTime_ = fourthTime_;
			fourthTime_ = thirdTime_;
			thirdTime_ = secondTime_;
			secondTime_ = hiTime_;
			hiTime_ = time_;
		}
		else if (secondTime_ >= time_)
		{
			fifthTime_ = fourthTime_;
			fourthTime_ = thirdTime_;
			thirdTime_ = secondTime_;
			secondTime_ = time_;
		}
		else if (thirdTime_ >= time_)
		{
			fifthTime_ = fourthTime_;
			fourthTime_ = thirdTime_;
			thirdTime_ = time_;
		}
		else if (fourthTime_ >= time_)
		{
			fifthTime_ = fourthTime_;
			fourthTime_ = time_;
		}
		else if (fifthTime_ >= time_)
		{
			fifthTime_ = time_;
		}

		TimeSave();

		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}
}

void GameScene::Draw(void)
{
	//grid_->Draw();


	map_->Draw();
	//wayPoint_->Draw();
	mainCamera->Draw();

	for (const auto& e1 : enemy1s_)
	{
		e1->Draw();
	}
	for (const auto& e2 : enemy2s_)
	{
		e2->Draw();
	}

	player_->Draw();

	// カメラライト


	// デバッグ
//DrawSphere3D(wayPoint_->GetWayPoints()[0], 10.0f , 20, 0xff0000, 0xff0000, false);

//VECTOR tPos = camera->GetTargetPos();

////カプセル
//DrawCapsule3D({ player_->GetTransform().pos.x,player_->GetTransform().pos.y + 140.0f,player_->GetTransform().pos.z },
//	tPos,
//	20.0f, 18, 0x00ffff, 0x00ffff, false);

	DrawBox(Application::SCREEN_SIZE_X / 2 - 32, Application::SCREEN_SIZE_Y / 2 - 1,
		Application::SCREEN_SIZE_X / 2 - 15, Application::SCREEN_SIZE_Y / 2 + 1,
		0xffffff, true);

	DrawBox(Application::SCREEN_SIZE_X / 2 + 32, Application::SCREEN_SIZE_Y / 2 - 1,
		Application::SCREEN_SIZE_X / 2 + 15, Application::SCREEN_SIZE_Y / 2 + 1,
		0xffffff, true);

	DrawBox(Application::SCREEN_SIZE_X / 2 - 1, Application::SCREEN_SIZE_Y / 2 - 32,
		Application::SCREEN_SIZE_X / 2 + 1, Application::SCREEN_SIZE_Y / 2 - 15,
		0xffffff, true);

	DrawBox(Application::SCREEN_SIZE_X / 2 - 1, Application::SCREEN_SIZE_Y / 2 + 32,
		Application::SCREEN_SIZE_X / 2 + 1, Application::SCREEN_SIZE_Y / 2 + 15,
		0xffffff, true);

	UIDraw();


	DINPUT_JOYSTATE input;
	int i;
	int Color;

	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//// 画面に構造体の中身を描画
	//Color = GetColor(255, 255, 255);
	//DrawFormatString(0, 0, Color, "X:%d Y:%d Z:%d",
	//	input.X, input.Y, input.Z);
	//DrawFormatString(0, 16, Color, "Rx:%d Ry:%d Rz:%d",
	//	input.Rx, input.Ry, input.Rz);
	//DrawFormatString(0, 32, Color, "Slider 0:%d 1:%d",
	//	input.Slider[0], input.Slider[1]);
	//DrawFormatString(0, 48, Color, "POV 0:%d 1:%d 2:%d 3:%d",
	//	input.POV[0], input.POV[1],
	//	input.POV[2], input.POV[3]);
	//DrawString(0, 64, "Button", Color);
	//for (i = 0; i < 32; i++)
	//{
	//	DrawFormatString(64 + i % 8 * 64, 64 + i / 8 * 16, Color,
	//		"%2d:%d", i, input.Buttons[i]);
	//}

}

void GameScene::Release(void)
{
	DeleteSoundMem(noticeBgm_);
	DeleteSoundMem(clearSe_);
}

void GameScene::MakeEnemy1(VECTOR pos)
{
	Enemy1* enemy1 = new Enemy1(*player_, *wayPoint_, *map_, pos);

	enemy1->Init();
	enemy1s_.emplace_back(enemy1);
}

void GameScene::MakeEnemy2(VECTOR pos)
{
	Enemy2* enemy2 = new Enemy2(*player_, *wayPoint_, *map_, pos);

	enemy2->Init();
	enemy2s_.emplace_back(enemy2);
}

void GameScene::SoundUpdate(void)
{
	if (bgmFlag_)
	{
		PlaySoundMem(noticeBgm_, DX_PLAYTYPE_LOOP);
		bgmFlag_ = false;
	}

	ChangeVolumeSoundMem(noticeBgmVol_, noticeBgm_);

	for (const auto& e1 : enemy1s_)
	{
		if (e1->GetNoticeBgmVol() == 170)
		{
			noticeBgmVol_ = e1->GetNoticeBgmVol();
		}
		if (e1->GetIsNotice() == true)
		{
			return;
		}
	}
	for (const auto& e2 : enemy2s_)
	{
		if (e2->GetNoticeBgmVol() == 170)
		{
			noticeBgmVol_ = e2->GetNoticeBgmVol();
		}
		if (e2->GetIsNotice() == true)
		{
			return;
		}
	}
	noticeBgmVol_--;
	if (noticeBgmVol_ < 40)
	{
		noticeBgmVol_ = 40;
	}
}

void GameScene::UIDraw(void)
{
	SetFontSize(13);

	DrawGraph(10, 10, bellUi_, true);
	DrawFormatString(28, 38, 0xffffff, "×%d", player_->GetBellCount());

	DrawGraph(60, 10, lightStoneUi_, true);
	DrawFormatString(74, 38, 0xffffff, "×%d", player_->GetLightStoneCount());

	DrawGraph(106, 10, kaleidoscopeUi_, true);
	DrawFormatString(124, 38, 0xffffff, "×%d", player_->GetKaleidoScopeCount());

	DrawGraph(160, 10, compassUi_, true);
	DrawFormatString(180, 38, 0xffffff, "×%d", player_->GetCompassCount());

	SetFontSize(18);

	switch (player_->GetActive())
	{
	case Player::ACTIVE::BELL:
		DrawBox(18, 15, 62, 55, 0xffffff, false);
		DrawString(18, 70, "鈴：音で一部の敵を誘導する",0xffffff);
		break;
	case Player::ACTIVE::LIGHT_STONE:
		DrawBox(63, 15, 107, 55, 0xffffff, false);
		DrawString(18, 70, "光る石：置いて目印にできる", 0xffffff);
		break;
	case Player::ACTIVE::KALEIDOSCOPE:
		DrawBox(113, 15, 160, 55, 0xffffff, false);
		DrawString(18, 70, "万華鏡：使うとワープする", 0xffffff);
		break;
	case Player::ACTIVE::COMPASS:
		DrawBox(165, 15, 215, 55, 0xffffff, false);
		DrawString(18, 70, "コンパス：ゴールの位置を指す", 0xffffff);
		break;
	}


	switch (player_->GetPassive())
	{
	case Player::PASSIVE::STRAW_DOLL:
		DrawGraph(10, 105, strawDollUi_, true);
		DrawString(18, 165, "藁人形：敵に捕まると一度だけワープする", 0xffffff);
		break;
	case Player::PASSIVE::TABI:
		DrawGraph(10, 105, tabiUi_, true);
		DrawString(18, 165, "靴：足が少し早くなる", 0xffffff);
		break;
	case Player::PASSIVE::GREEN_PURSE:
		DrawGraph(10, 105, bagUi_, true);
		DrawString(18, 165, "緑の巾着：スタミナの回復速度が上がる", 0xffffff);
		break;
	case Player::PASSIVE::HAND_MIRROR:
		DrawGraph(10, 105, handMirrorUi_, true);
		DrawString(18, 165, "手鏡：足の速さとスタミナの回復速度が下がる\n　　　持っていても良いことはない", 0xffffff);
		break;
	}
	DrawGraph(870, 50, charmUi_, true);

	SetFontSize(28);
	DrawFormatString(930, 57, 0xffffff, "%d/5", player_->GetCharmCount());
	DrawFormatString(870, 20, 0xffffff, "%d%d:%d%d:%d%d",timeHour2_, timeHour1_,timeMin2_,timeMin1_,timeSec2_,timeSec1_);
	SetFontSize(18);
}
