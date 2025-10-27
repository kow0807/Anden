#pragma once

class SceneBase
{

public:

	// �R���X�g���N�^
	SceneBase(void);

	// �f�X�g���N�^
	virtual ~SceneBase(void) = 0;

	// ����������
	virtual void Init(void) = 0;

	// �X�V�X�e�b�v
	virtual void Update(void) = 0;

	// �`�揈��
	virtual void Draw(void) = 0;

	// �������
	virtual void Release(void) = 0;

protected:

	int time_;
	int hiTime_;
	int secondTime_;
	int thirdTime_;
	int fourthTime_;
	int fifthTime_;

	bool IsHitLogo(int posX, int posY, float sizeX, float sizeY);

	// �X�R�A�̃Z�[�u
	void TimeSave(void);

	// �X�R�A�̃��[�h
	void TimeLoad(void);

};
