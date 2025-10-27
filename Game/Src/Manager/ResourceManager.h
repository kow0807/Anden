#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		START_LOGO,
		GAMEOVER_LOGO,
		CLEAR_LOGO,

		BELL_UI,
		LIGHT_STONE_UI,
		KALEIDOSCOPE_UI,
		COMPASS_UI,

		STRAW_DOLL_UI,
		TABI_UI,
		GREEN_PURSE_UI,
		HAND_MIRROR_UI,

		CHARM_UI,

		PLAYER,
		ENEMY,
		ENEMY_IDLE,
		ENEMY_WALK,
		ENEMY_RUN,

		MAP,
		GOAL_BOX,
		AFTER_GOAL_BOX,
		CHEST,
		DRAWER,
		DUMMY_DRAWER,
		SHELF,
		DUMMY_SHELF,
		HIDEBOX,
		DOOR,

		STRAW_DOLL,
		TABI,
		GREEN_PURSE,
		HAND_MIRROR,

		BELL,
		LIGHT_STONE,
		KALEIDOSCOPE,
		COMPASS_BASE,
		COMPASS_BAS,

		CHARM,

		MOVE_IMAGE_KM,
		CAMERACONTROL_IMAGE_KM,
		RUNMOVE_IMAGE_KM,
		CROUCH_IMAGE_KM,
		ACTION_IMAGE_KM,
		USEITEM_IMAGE_KM,
		CHOICEITEM_IMAGE_KM,

		MOVE_IMAGE_P,
		CAMERACONTROL_IMAGE_P,
		RUNMOVE_IMAGE_P,
		CROUCH_IMAGE_P,
		ACTION_IMAGE_P,
		USEITEM_IMAGE_P,
		CHOICEITEM_IMAGE_P,

		CLEAR_IMAGE

	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

	////���\�[�X�̕������[�h(���_�V�F�[�_�[�p)
	//int LoadVertexShaderDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void) = default;

	// �������[�h
	Resource* _Load(SRC src);

};

