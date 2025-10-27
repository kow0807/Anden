#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Init(void)
{
	// カメラモード：定点カメラ
	mainCamera->ChangeMode(Camera::MODE::FIXED_POINT);
	logo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMEOVER_LOGO).handleId_;
}

void GameOverScene::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();

	DINPUT_JOYSTATE input;
	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (ins.IsTrgDown(KEY_INPUT_SPACE) || input.Buttons[0] > 0 || input.Buttons[1] > 0 || input.Buttons[2] > 0 || input.Buttons[3] > 0 || input.Buttons[4] > 0 || input.Buttons[5] > 0)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameOverScene::Draw(void)
{
	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	DrawRotaGraph(cx, cy - 80, 1.0, 0.0, logo_, true);
	std::string msg;
	if (GetJoypadNum() > 0)
	{
		msg = "Push any Buttun";
	}
	else
	{
		msg = "Push Space";
	}
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 400, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);
}

void GameOverScene::Release(void)
{
}
