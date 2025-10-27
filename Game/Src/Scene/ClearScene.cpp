#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "ClearScene.h"

ClearScene::ClearScene(void)
{
}

ClearScene::~ClearScene(void)
{
}

void ClearScene::Init(void)
{
	// カメラモード：定点カメラ
	mainCamera->ChangeMode(Camera::MODE::FIXED_POINT);
	logo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CLEAR_LOGO).handleId_;
	TimeLoad();
}

void ClearScene::Update(void)
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

void ClearScene::Draw(void)
{
	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;
	//DrawRotaGraph(cx, cy-160, 1.0, 0.0, logo_, true);
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
	
	ClearTimeDraw();
	SetFontSize(16);

}

void ClearScene::Release(void)
{
}

void ClearScene::ClearTimeDraw(void)
{
	SetFontSize(40);

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

	DrawString(cx - 130, 210, "クリアタイム", 0xffffff);
	timeSec1 = time_ % 60;

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

	timeMin1 = (time_ / 60) % 60;
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

	timeHour1 = (time_ / 3600) % 60;
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
	std::string msg = str6 + str5 + ":" + str4 + str3 + ":" + str2 + str1;
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawString(cx - (width / 2), 260, msg.c_str(), 0xffffff);
}
