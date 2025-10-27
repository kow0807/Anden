#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Player.h"
#include "../Object/Maps/Map.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgTitleLogo_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	player_ = std::make_shared<Player>();
	player_->Init();

	map_ = std::make_shared<Map>(*player_);
	map_->Init();

	state_ = STATE::NONE;

	imgTitleLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::START_LOGO).handleId_;

	// カメラモード：定点カメラ
	mainCamera->ChangeMode(Camera::MODE::FIXED_POINT);
	VECTOR pos = { 3657,40,-5822 };
	mainCamera->SetPos(pos);
	bgm_= LoadSoundMem("Data/Sound/titleBgm.mp3");
	bgmFlag_ = true;

	start_ = "Start";
	ranking_ = "Ranking";
	exit_ = "Exit";

	startPosX_ = 0.0f;
	startPosY_ = 460.0f;
	rankingPosX_ = 0.0f;
	rankingPosY_ = 500.0f;
	exitPosX_ = 0.0f;
	exitPosY_ = 540.0f;
	closePosX_ = 0.0f;
	closePosY_ = 500.0f;

	startSizeX_ = 0.0f;
	startSizeY_ = 28.0f;
	rankingSizeX_ = 0.0f;
	rankingSizeY_ = 28.0f;
	exitSizeX_ = 0.0f;
	exitSizeY_ = 28.0f;
	closeSizeX_ = 0.0f;
	closeSizeY_ = 24.0f;

	povFlag_ = false;
	TimeLoad();
}

void TitleScene::Update(void)
{

	if (bgmFlag_)
	{
		ChangeNextPlayVolumeSoundMem(170, bgm_);
		PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP);

		bgmFlag_ = false;
	}

	LogoUpdate();

	InputManager& ins = InputManager::GetInstance();

	DINPUT_JOYSTATE input;
	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//パッドが接続されているとき
	if (GetJoypadNum() > 0)
	{
		switch (state_)
		{
		case TitleScene::STATE::NONE:
			ChangeState(STATE::OUT_RANKING);
			break;
		case TitleScene::STATE::START:
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
			{
				StopSoundMem(bgm_);

				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			}
			break;
		case TitleScene::STATE::RANKING:
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
			{
				ChangeState(STATE::IN_RANKING);
			}
			break;
		case TitleScene::STATE::EXIT:
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
			{
				SceneManager::GetInstance().SetGameEnd(true);
			}
			break;
		case TitleScene::STATE::IN_RANKING:
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
			{
				ChangeState(STATE::RANKING);
			}
			break;
		}
	}
	else
	{
		switch (state_)
		{
		case TitleScene::STATE::NONE:
			ChangeState(STATE::OUT_RANKING);
			break;
		case TitleScene::STATE::START:
			if (ins.IsTrgMouseLeft())
			{
				StopSoundMem(bgm_);

				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			}
			break;
		case TitleScene::STATE::RANKING:
			if (ins.IsTrgMouseLeft())
			{
				ChangeState(STATE::IN_RANKING);
			}
			break;
		case TitleScene::STATE::EXIT:
			if (ins.IsTrgMouseLeft())
			{
				SceneManager::GetInstance().SetGameEnd(true);
			}
			break;
		case TitleScene::STATE::IN_RANKING:
			if (IsHitLogo(closePosX_, closePosY_, closeSizeX_, closeSizeY_)&& ins.IsTrgMouseLeft())
			{
				ChangeState(STATE::OUT_RANKING);
			}
			break;
		}
	}
}

void TitleScene::Draw(void)
{
	map_->Draw();

	// ロゴ描画
	switch (state_)
	{
	case TitleScene::STATE::NONE:
		break;
	case TitleScene::STATE::START:
		LogoDraw();
		break;
	case TitleScene::STATE::RANKING:
		LogoDraw();
		break;
	case TitleScene::STATE::IN_RANKING:
		RankingDraw();
		break;
	case TitleScene::STATE::OUT_RANKING:
		LogoDraw();
		break;
	case TitleScene::STATE::EXIT:
		LogoDraw();
		break;
	}
}

void TitleScene::Release(void)
{
	DeleteSoundMem(bgm_);
}

void TitleScene::LogoUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//パッドが接続されているとき
	if (GetJoypadNum() > 0)
	{
		switch (state_)
		{
		case TitleScene::STATE::NONE:
			ChangeState(STATE::START);
			break;
		case TitleScene::STATE::START:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 18000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeState(STATE::RANKING);
			}
			break;
		case TitleScene::STATE::RANKING:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 18000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeState(STATE::EXIT);
			}
			if (input.POV[0] == 0 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeState(STATE::START);
			}
			break;
		case TitleScene::STATE::EXIT:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 0 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeState(STATE::RANKING);
			}
			break;
		case TitleScene::STATE::OUT_RANKING:
			ChangeState(STATE::START);
			break;
		}
	}
	else
	{
		if (state_ != STATE::IN_RANKING)
		{
			if (IsHitLogo(startPosX_, startPosY_, startSizeX_, startSizeY_))
			{
				ChangeState(STATE::START);
			}
			else if (IsHitLogo(rankingPosX_, rankingPosY_, rankingSizeX_, rankingSizeY_))
			{
				ChangeState(STATE::RANKING);
			}
			else if (IsHitLogo(exitPosX_, exitPosY_, exitSizeX_, exitSizeY_))
			{
				ChangeState(STATE::EXIT);
			}
			else
			{
				ChangeState(STATE::OUT_RANKING);
			}
		}
	}
}

void TitleScene::LogoDraw(void)
{
	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	DrawRotaGraph(cx, cy - 80, 1.0, 0.0, imgTitleLogo_, true);

	SetFontSize(28);
	int len = (int)strlen(start_.c_str());
	int width = GetDrawStringWidth(start_.c_str(), len);
	startPosX_ = cx - (width / 2);
	startSizeX_ = width;
	DrawFormatString(startPosX_, startPosY_, 0x87cefa, start_.c_str());

	len = (int)strlen(ranking_.c_str());
	width = GetDrawStringWidth(ranking_.c_str(), len);
	rankingPosX_ = cx - (width / 2);
	rankingSizeX_ = width;
	DrawFormatString(rankingPosX_, rankingPosY_, 0x87cefa, ranking_.c_str());

	len = (int)strlen(exit_.c_str());
	width = GetDrawStringWidth(exit_.c_str(), len);
	exitPosX_ = cx - (width / 2);
	exitSizeX_ = width;
	DrawFormatString(exitPosX_, exitPosY_, 0x87cefa, exit_.c_str());

	switch (state_)
	{
	case TitleScene::STATE::NONE:
		break;
	case TitleScene::STATE::START:
		DrawLine(startPosX_, startPosY_ + 30.0f, startPosX_ + startSizeX_, startPosY_ + 30.0f, 0x87cefa);
		break;
	case TitleScene::STATE::RANKING:
		DrawLine(rankingPosX_, rankingPosY_ + 30.0f, rankingPosX_ + rankingSizeX_, rankingPosY_ + 30.0f, 0x87cefa);
		break;
	case TitleScene::STATE::EXIT:
		DrawLine(exitPosX_, exitPosY_ + 30.0f, exitPosX_ + exitSizeX_, exitPosY_ + 30.0f, 0x87cefa);
		break;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}

void TitleScene::RankingDraw(void)
{
	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;


	DrawBox(cx - 180, 120, cx + 180, 550, 0x000000, true);
	DrawBox(cx - 180, 120, cx + 180, 550, 0xffffff, false);
	DrawBox(cx - 180, 120, cx + 180, 180, 0xffffff, false);

	SetFontSize(32);
	std::string msg = "Clear Time";
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 135, 0xffffff, msg.c_str());

	SetFontSize(24);
	msg = "close";
	len = (int)strlen(msg.c_str());
	width = GetDrawStringWidth(msg.c_str(), len);

	closePosX_ = cx - (width / 2);
	closeSizeX_ = width;

	DrawFormatString(closePosX_, closePosY_, 0xffffff, msg.c_str());

	//パッドが接続されているとき
	if (GetJoypadNum() > 0)
	{
		DrawLine(closePosX_, closePosY_ + 30.0f, closePosX_ + closeSizeX_, closePosY_ + 30.0f, 0xffffff);
	}
	else
	{
		if (IsHitLogo(closePosX_, closePosY_, closeSizeX_, closeSizeY_))
		{
			DrawLine(closePosX_, closePosY_ + 30.0f, closePosX_ + closeSizeX_, closePosY_ + 30.0f, 0xffffff);
		}
	}
	SetFontSize(28);
	TimeDraw();

	SetFontSize(16);
}

void TitleScene::TimeDraw(void)
{
	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	int timeSec1;
	int timeSec2;
	int timeSec3;
	int timeMin1;
	int timeMin2;
	int timeMin3;
	int timeHour1;
	int timeHour2;
	int timeHour3;

	DrawString(cx - 130, 210, "1位：", 0xffffff);
	timeSec1 = hiTime_ % 60;

	if (timeSec1 < 10)
	{
		timeSec2 = timeSec1;
		timeSec3 = 0;
	}
	else
	{
		timeSec2 = timeSec1 % 10;
		timeSec3 = timeSec1 / 10;
	}

	timeMin1 = (hiTime_ / 60) % 60;
	if (timeMin1 < 10)
	{
		timeMin2 = timeMin1;
		timeMin3 = 0;
	}
	else
	{
		timeMin2 = timeMin1 % 10;
		timeMin3 = timeMin1 / 10;
	}

	timeHour1 = (hiTime_ / 3600) % 60;
	if (timeHour1 < 10)
	{
		timeHour2 = timeHour1;
		timeHour3 = 0;
	}
	else
	{
		timeHour2 = timeHour1 % 10;
		timeHour3 = timeHour1 / 10;
	}

	std::string str1 = std::to_string(timeSec2);
	std::string str2 = std::to_string(timeSec3);
	std::string str3 = std::to_string(timeMin2);
	std::string str4 = std::to_string(timeMin3);
	std::string str5 = std::to_string(timeHour2);
	std::string str6 = std::to_string(timeHour3);
	std::string msg = str6 + str5 + ":" + str4 + str3 +":"+ str2 + str1;
	if (hiTime_ >= 99999999)
	{
		msg = "-";
	}
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawString(cx - (width / 2) + 30, 210, msg.c_str(), 0xffffff);

	DrawString(cx - 130, 270, "2位：", 0xffffff);
	timeSec1 = secondTime_ % 60;

	if (timeSec1 < 10)
	{
		timeSec2 = timeSec1;
		timeSec3 = 0;
	}
	else
	{
		timeSec2 = timeSec1 % 10;
		timeSec3 = timeSec1 / 10;
	}

	timeMin1 = (secondTime_ / 60) % 60;
	if (timeMin1 < 10)
	{
		timeMin2 = timeMin1;
		timeMin3 = 0;
	}
	else
	{
		timeMin2 = timeMin1 % 10;
		timeMin3 = timeMin1 / 10;
	}

	timeHour1 = (secondTime_ / 3600) % 60;
	if (timeHour1 < 10)
	{
		timeHour2 = timeHour1;
		timeHour3 = 0;
	}
	else
	{
		timeHour2 = timeHour1 % 10;
		timeHour3 = timeHour1 / 10;
	}

	str1 = std::to_string(timeSec2);
	str2 = std::to_string(timeSec3);
	str3 = std::to_string(timeMin2);
	str4 = std::to_string(timeMin3);
	str5 = std::to_string(timeHour2);
	str6 = std::to_string(timeHour3);
	msg = str6 + str5 + ":" + str4 + str3 +":"+ str2 + str1;
	if (secondTime_ >= 99999999)
	{
		msg = "-";
	}
	len = (int)strlen(msg.c_str());
	width = GetDrawStringWidth(msg.c_str(), len);
	DrawString(cx - (width / 2) + 30, 270, msg.c_str(), 0xffffff);


	DrawString(cx - 130, 330, "3位：", 0xffffff);
	timeSec1 = thirdTime_ % 60;

	if (timeSec1 < 10)
	{
		timeSec2 = timeSec1;
		timeSec3 = 0;
	}
	else
	{
		timeSec2 = timeSec1 % 10;
		timeSec3 = timeSec1 / 10;
	}

	timeMin1 = (thirdTime_ / 60) % 60;
	if (timeMin1 < 10)
	{
		timeMin2 = timeMin1;
		timeMin3 = 0;
	}
	else
	{
		timeMin2 = timeMin1 % 10;
		timeMin3 = timeMin1 / 10;
	}

	timeHour1 = (thirdTime_ / 3600) % 60;
	if (timeHour1 < 10)
	{
		timeHour2 = timeHour1;
		timeHour3 = 0;
	}
	else
	{
		timeHour2 = timeHour1 % 10;
		timeHour3 = timeHour1 / 10;
	}

	str1 = std::to_string(timeSec2);
	str2 = std::to_string(timeSec3);
	str3 = std::to_string(timeMin2);
	str4 = std::to_string(timeMin3);
	str5 = std::to_string(timeHour2);
	str6 = std::to_string(timeHour3);
	msg = str6 + str5 + ":" + str4 + str3 + ":" + str2 + str1;
	if (thirdTime_ >= 99999999)
	{
		msg = "-";
	}
	len = (int)strlen(msg.c_str());
	width = GetDrawStringWidth(msg.c_str(), len);
	DrawString(cx - (width / 2) + 30, 330, msg.c_str(), 0xffffff);

	DrawString(cx - 130, 390, "4位：", 0xffffff);
	timeSec1 = fourthTime_ % 60;

	if (timeSec1 < 10)
	{
		timeSec2 = timeSec1;
		timeSec3 = 0;
	}
	else
	{
		timeSec2 = timeSec1 % 10;
		timeSec3 = timeSec1 / 10;
	}

	timeMin1 = (fourthTime_ / 60) % 60;
	if (timeMin1 < 10)
	{
		timeMin2 = timeMin1;
		timeMin3 = 0;
	}
	else
	{
		timeMin2 = timeMin1 % 10;
		timeMin3 = timeMin1 / 10;
	}

	timeHour1 = (fourthTime_ / 3600) % 60;
	if (timeHour1 < 10)
	{
		timeHour2 = timeHour1;
		timeHour3 = 0;
	}
	else
	{
		timeHour2 = timeHour1 % 10;
		timeHour3 = timeHour1 / 10;
	}

	str1 = std::to_string(timeSec2);
	str2 = std::to_string(timeSec3);
	str3 = std::to_string(timeMin2);
	str4 = std::to_string(timeMin3);
	str5 = std::to_string(timeHour2);
	str6 = std::to_string(timeHour3);
	msg = str6 + str5 + ":" + str4 + str3 + ":" + str2 + str1;
	if (fourthTime_ >= 99999999)
	{
		msg = "-";
	}
	len = (int)strlen(msg.c_str());
	width = GetDrawStringWidth(msg.c_str(), len);
	DrawString(cx - (width / 2) + 30, 390, msg.c_str(), 0xffffff);

	DrawString(cx - 130, 450, "5位：", 0xffffff);
	timeSec1 = fifthTime_ % 60;

	if (timeSec1 < 10)
	{
		timeSec2 = timeSec1;
		timeSec3 = 0;
	}
	else
	{
		timeSec2 = timeSec1 % 10;
		timeSec3 = timeSec1 / 10;
	}

	timeMin1 = (fifthTime_ / 60) % 60;
	if (timeMin1 < 10)
	{
		timeMin2 = timeMin1;
		timeMin3 = 0;
	}
	else
	{
		timeMin2 = timeMin1 % 10;
		timeMin3 = timeMin1 / 10;
	}

	timeHour1 = (fifthTime_ / 3600) % 60;
	if (timeHour1 < 10)
	{
		timeHour2 = timeHour1;
		timeHour3 = 0;
	}
	else
	{
		timeHour2 = timeHour1 % 10;
		timeHour3 = timeHour1 / 10;
	}

	str1 = std::to_string(timeSec2);
	str2 = std::to_string(timeSec3);
	str3 = std::to_string(timeMin2);
	str4 = std::to_string(timeMin3);
	str5 = std::to_string(timeHour2);
	str6 = std::to_string(timeHour3);
	msg = str6 + str5 + ":" + str4 + str3 + ":" + str2 + str1;
	if (fifthTime_ >= 99999999)
	{
		msg = "-";
	}
	len = (int)strlen(msg.c_str());
	width = GetDrawStringWidth(msg.c_str(), len);
	DrawString(cx - (width / 2) + 30, 450, msg.c_str(), 0xffffff);
}

void TitleScene::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case TitleScene::STATE::NONE:
		break;
	case TitleScene::STATE::START:
		break;
	case TitleScene::STATE::RANKING:
		break;
	case TitleScene::STATE::IN_RANKING:
		break;
	case TitleScene::STATE::OUT_RANKING:
		break;
	case TitleScene::STATE::EXIT:
		break;
	}
}
