#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	Resource res;

	// �X�^�[�g���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Title.png");
	resourcesMap_.emplace(SRC::START_LOGO, res);

	// �G���h���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameOverLogo.png");
	resourcesMap_.emplace(SRC::GAMEOVER_LOGO, res);

	// �N���A���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ClearLogo.png");
	resourcesMap_.emplace(SRC::CLEAR_LOGO, res);

	// ��UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Bell.png");
	resourcesMap_.emplace(SRC::BELL_UI, res);
	// �����UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/LightStone.png");
	resourcesMap_.emplace(SRC::LIGHT_STONE_UI, res);
	// ���؋�UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Kaleidoscope.png");
	resourcesMap_.emplace(SRC::KALEIDOSCOPE_UI, res);
	// �R���p�XUI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Compass.png");
	resourcesMap_.emplace(SRC::COMPASS_UI, res);

	// �l�`UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Doll.png");
	resourcesMap_.emplace(SRC::STRAW_DOLL_UI, res);
	// �CUI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Socks.png");
	resourcesMap_.emplace(SRC::TABI_UI, res);
	// �΂̃o�b�OUI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/GreenBag.png");
	resourcesMap_.emplace(SRC::GREEN_PURSE_UI, res);
	// �苾UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/HandMirror.png");
	resourcesMap_.emplace(SRC::HAND_MIRROR_UI, res);

	// �����UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Charm.png");
	resourcesMap_.emplace(SRC::CHARM_UI, res);


	// �v���C���[
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Human/Human.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);


	// �G1
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant.mv1");
	resourcesMap_.emplace(SRC::ENEMY, res);
	// �ҋ@
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_IDLE, res);
	// ����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Walking.mv1");
	resourcesMap_.emplace(SRC::ENEMY_WALK, res);
	// ����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Run.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RUN, res);

	// �S�[���{�b�N�X
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "GoalBox/GoalBox.mv1");
	resourcesMap_.emplace(SRC::GOAL_BOX, res);

	// �S�[����{�b�N�X
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "GoalBox/AfterGoalBox.mv1");
	resourcesMap_.emplace(SRC::AFTER_GOAL_BOX, res);

	//�}�b�v
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Map/Map.mv1");
	resourcesMap_.emplace(SRC::MAP, res);

	// �\�y
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Furniture/Chest/Chest.mv1");
	resourcesMap_.emplace(SRC::CHEST, res);

	// �����o��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Furniture/Chest/Drawer.mv1");
	resourcesMap_.emplace(SRC::DRAWER, res);
	// �_�~�[�̈����o��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Dummy/dummyDrawers.mv1");
	resourcesMap_.emplace(SRC::DUMMY_DRAWER, res);

	// �I
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Furniture/Shelf/Shelf.mv1");
	resourcesMap_.emplace(SRC::SHELF, res);
	// �_�~�[�I
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Dummy/dummyShelf.mv1");
	resourcesMap_.emplace(SRC::DUMMY_SHELF, res);

	// �B�ꔠ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Furniture/Chest/Chest.mv1");
	resourcesMap_.emplace(SRC::HIDEBOX, res);

	// ��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Area/door.mv1");
	resourcesMap_.emplace(SRC::DOOR, res);


	// �l�`
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Doll/Doll.mv1");
	resourcesMap_.emplace(SRC::STRAW_DOLL, res);

	// ����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Socks/Socks.mv1");
	resourcesMap_.emplace(SRC::TABI, res);

	// �΂̋В�
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Bag/GreenBag.mv1");
	resourcesMap_.emplace(SRC::GREEN_PURSE, res);

	// �苾
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/HandMirror/HandMirror.mv1");
	resourcesMap_.emplace(SRC::HAND_MIRROR, res);


	// ��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Bell/Bell.mv1");
	resourcesMap_.emplace(SRC::BELL, res);

	// �����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/LightStone/LightStone.mv1");
	resourcesMap_.emplace(SRC::LIGHT_STONE, res);

	// �����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Kaleidoscope/Kaleidoscope.mv1");
	resourcesMap_.emplace(SRC::KALEIDOSCOPE, res);

	// �R���p�X(�y��)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/compassBasefbx.mv1");
	resourcesMap_.emplace(SRC::COMPASS_BASE, res);

	// �R���p�X(�j)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/compassbas.mv1");
	resourcesMap_.emplace(SRC::COMPASS_BAS, res);


	// �����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Item/Charm/Charm.mv1");
	resourcesMap_.emplace(SRC::CHARM, res);

	// �ړ��摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/MoveImageKM.png");
	resourcesMap_.emplace(SRC::MOVE_IMAGE_KM, res);

	// �J��������摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/CameraControlImageKM.png");
	resourcesMap_.emplace(SRC::CAMERACONTROL_IMAGE_KM, res);

	// ����摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/RunMoveImageKM.png");
	resourcesMap_.emplace(SRC::RUNMOVE_IMAGE_KM, res);

	// ���Ⴊ�މ摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/CrouchImageKM.png");
	resourcesMap_.emplace(SRC::CROUCH_IMAGE_KM, res);

	// �A�N�V�����摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/ActionImageKM.png");
	resourcesMap_.emplace(SRC::ACTION_IMAGE_KM, res);

	// �A�C�e���g�p�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/UseItemImageKM.png");
	resourcesMap_.emplace(SRC::USEITEM_IMAGE_KM, res);

	// �A�C�e���I���摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/ChoiceImageKM.png");
	resourcesMap_.emplace(SRC::CHOICEITEM_IMAGE_KM, res);

	// �ړ��摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/MoveImageP.png");
	resourcesMap_.emplace(SRC::MOVE_IMAGE_P, res);

	// �J��������摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/CameraControlImageP.png");
	resourcesMap_.emplace(SRC::CAMERACONTROL_IMAGE_P, res);

	// ����摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/RunMoveImageP.png");
	resourcesMap_.emplace(SRC::RUNMOVE_IMAGE_P, res);

	// ���Ⴊ�މ摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/CrouchImageP.png");
	resourcesMap_.emplace(SRC::CROUCH_IMAGE_P, res);

	// �A�N�V�����摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/ActionImageP.png");
	resourcesMap_.emplace(SRC::ACTION_IMAGE_P, res);

	// �A�C�e���g�p�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/UseItemImageP.png");
	resourcesMap_.emplace(SRC::USEITEM_IMAGE_P, res);

	// �A�C�e���I���摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/ChoiceImageP.png");
	resourcesMap_.emplace(SRC::CHOICEITEM_IMAGE_P, res);

	// �N���A�����摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "controlImage/clearImageP.png");
	resourcesMap_.emplace(SRC::CLEAR_IMAGE, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

//int ResourceManager::LoadVertexShaderDuplicate(SRC src)
//{
//	Resource* res = _Load(src);
//	if (res == nullptr)
//	{
//		return -1;
//	}
//
//	int duId = LoadVertexShader(static_cast<>(res->handleId_));
//	res->duplicateModelIds_.push_back(duId);
//
//	return duId;
//}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
