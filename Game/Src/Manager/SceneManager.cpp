#include <chrono>
#include <DxLib.h>
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/GameOverScene.h"
#include "../Scene/ClearScene.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "CameraLight.h"
#include "SceneManager.h"


//#include <d3d11.h>
//#include <d3dcompiler.h>
//// グローバル変数
//ID3D11Device* device = nullptr;
//ID3D11DeviceContext* context = nullptr;
//ID3D11VertexShader* vertexShader = nullptr;
//ID3D11PixelShader* pixelShader = nullptr;
//ID3D11InputLayout* inputLayout = nullptr;

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = std::make_shared<Fader>();
	fader_->Init();

	// カメラ
	camera_ = std::make_shared<Camera>();
	camera_->Init();

	// カメラライト
	cameraLight_ = std::make_shared<CameraLight>();
	cameraLight_->Init();

	scene_ = std::make_shared<TitleScene>();
	scene_->Init();

	scene_ = std::make_shared <GameScene>();
	scene_->Init();

	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	isGameEnd_ = false;

	// 3D用の設定
	Init3D();

	// シェーダーの設定
	InitShader();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);

}

void SceneManager::Init3D(void)
{

	// 背景色設定
	SetBackgroundColor(0, 0, 0);
	//SetBackgroundColor(255, 255, 255);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);
	//SetUseLighting(false);

	// 正面から斜め下に向かったライト
	//ChangeLightTypeDir({ 1.00f, 1.00f, 1.00f });

	
}

void SceneManager::InitShader(void)
{
	//シェーダーファイルをロードしてシェーダーを作成






}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		scene_->Update();
	}

	// カメラ更新
	camera_->Update();

	cameraLight_->Update();
}

void SceneManager::Draw(void)
{
	
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	camera_->SetBeforeDraw();

	// 描画
	scene_->Draw();

	// 暗転・明転
	fader_->Draw();

}

void SceneManager::Destroy(void)
{

	//scene_->Release();
	//delete scene_;

	//delete fader_;

	//cameraLight_->Release();
	//delete cameraLight_;

	//camera_->Release();
	//delete camera_;

	delete instance_;

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}

std::weak_ptr<Camera> SceneManager::GetCamera(void) const
{
	return camera_;
}

std::weak_ptr<CameraLight> SceneManager::GetCameraLight(void) const
{
	return cameraLight_;
}

bool SceneManager::IsGameEnd(void)
{
	return isGameEnd_;
}

void SceneManager::SetGameEnd(bool isEnd)
{
	isGameEnd_ = isEnd;
}

SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;

	cameraLight_ = nullptr;

	camera_ = nullptr;
	
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	// リソースの解放
	ResourceManager::GetInstance().Release();

	// シーンを変更する
	sceneId_ = sceneId;

	//// 現在のシーンを解放
	//if (scene_ != nullptr)
	//{
	//	scene_->Release();
	//	delete scene_;
	//}

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = std::make_shared<TitleScene>();
		break;
	case SCENE_ID::GAME:
		scene_ = std::make_shared <GameScene>();
		break;
	case SCENE_ID::GAMEOVER:
		scene_ = std::make_shared<GameOverScene>();
		break;
	case SCENE_ID::CLEAR:
		scene_ = std::make_shared<ClearScene>();
		break;
	}

	scene_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}


