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
//// �O���[�o���ϐ�
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

	// �J����
	camera_ = std::make_shared<Camera>();
	camera_->Init();

	// �J�������C�g
	cameraLight_ = std::make_shared<CameraLight>();
	cameraLight_->Init();

	scene_ = std::make_shared<TitleScene>();
	scene_->Init();

	scene_ = std::make_shared <GameScene>();
	scene_->Init();

	isSceneChanging_ = false;

	// �f���^�^�C��
	preTime_ = std::chrono::system_clock::now();

	isGameEnd_ = false;

	// 3D�p�̐ݒ�
	Init3D();

	// �V�F�[�_�[�̐ݒ�
	InitShader();

	// �����V�[���̐ݒ�
	DoChangeScene(SCENE_ID::TITLE);

}

void SceneManager::Init3D(void)
{

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 0, 0);
	//SetBackgroundColor(255, 255, 255);

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// ���C�g�̐ݒ�
	SetUseLighting(true);
	//SetUseLighting(false);

	// ���ʂ���΂߉��Ɍ����������C�g
	//ChangeLightTypeDir({ 1.00f, 1.00f, 1.00f });

	
}

void SceneManager::InitShader(void)
{
	//�V�F�[�_�[�t�@�C�������[�h���ăV�F�[�_�[���쐬






}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// �f���^�^�C��
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

	// �J�����X�V
	camera_->Update();

	cameraLight_->Update();
}

void SceneManager::Draw(void)
{
	
	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	// �J�����ݒ�
	camera_->SetBeforeDraw();

	// �`��
	scene_->Draw();

	// �Ó]�E���]
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

	// �t�F�[�h�������I����Ă���V�[����ς���ꍇ�����邽�߁A
	// �J�ڐ�V�[���������o�ϐ��ɕێ�
	waitSceneId_ = nextId;

	// �t�F�[�h�A�E�g(�Ó])���J�n����
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

	// �f���^�^�C��
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

	// ���\�[�X�̉��
	ResourceManager::GetInstance().Release();

	// �V�[����ύX����
	sceneId_ = sceneId;

	//// ���݂̃V�[�������
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
		// ���]��
		if (fader_->IsEnd())
		{
			// ���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())
		{
			// ���S�ɈÓ]���Ă���V�[���J��
			DoChangeScene(waitSceneId_);
			// �Ó]���疾�]��
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}


