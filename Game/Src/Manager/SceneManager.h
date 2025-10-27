#pragma once
#include <chrono>
#include <memory>
class SceneBase;
class Fader;
class Camera;
class CameraLight;

// �������܂��񂪁A�ǂ����Ă��g����������
#define mainCamera SceneManager::GetInstance().GetCamera().lock()

class SceneManager
{

public:

	static constexpr float DEFAULT_FPS = 60.0f;

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		GAMEOVER,
		CLEAR,
	};
	
	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void InitShader(void);
	void Update(void);
	void Draw(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// ��ԑJ��
	void ChangeScene(SCENE_ID nextId);

	// �V�[��ID�̎擾
	SCENE_ID GetSceneID(void);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	// �J�����̎擾
	std::weak_ptr<Camera> GetCamera(void) const;

	// �J�������C�g�̎擾
	std::weak_ptr<CameraLight> GetCameraLight(void) const;

	bool IsGameEnd(void);
	void SetGameEnd(bool isEnd);

private:



	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// �t�F�[�h
	std::shared_ptr<Fader> fader_;

	// �e��V�[��
	std::shared_ptr<SceneBase> scene_;

	// �J����
	std::shared_ptr<Camera> camera_;

	// �J�������C�g
	std::shared_ptr<CameraLight> cameraLight_;

	// �V�[���J�ڒ�����
	bool isSceneChanging_;

	// �f���^�^�C��
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	bool isGameEnd_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);
	// �R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager&);
	// �f�X�g���N�^�����l
	~SceneManager(void) = default;

	// �f���^�^�C�������Z�b�g����
	void ResetDeltaTime(void);

	// �V�[���J��
	void DoChangeScene(SCENE_ID sceneId);

	// �t�F�[�h
	void Fade(void);

};