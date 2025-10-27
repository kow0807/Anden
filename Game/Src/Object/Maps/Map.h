#pragma once
#include <memory>
#include <DxLib.h>
#include <vector>
#include "../Common/Transform.h"
#include"Items/PassiveItem.h"
#include"Items/ActiveItem.h"
#include"Items/DrawerActiveItem.h"

class Player;
class Chest;
class Shelf;
class DummyShelf;

class Map
{
public:
	// �萔
	static constexpr float SIZE_X = 7200.0f;
	static constexpr float SIZE_Y = 400.0f;
	static constexpr float SIZE_Z = 7200.0f;

	// �����ʒu
	static constexpr float POS_X = 3600.0f;
	static constexpr float POS_Y = 0.0f;
	static constexpr float POS_Z = -3600.0f;

	static constexpr VECTOR GOAL_BOX_POS = { 1200.0f, 0.0f, -3600.0f };

	// �R���X�g���N�^
	Map(Player& player);

	// �f�X�g���N�^
	~Map(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	int GetModelId(void);

	//�p�b�V�u�A�C�e��
	bool IsClickPassiveItem(void);
	void PassiveItemUpdate(void);

	//�A�N�e�B�u�A�C�e��
	bool IsClickActiveItem(void);
	void ActiveItemUpdate(void);
	void UseActiveItem(void);

	//�����o���ɓ������A�N�e�B�u�A�C�e��
	bool IsClickDrawerActiveItem(void);
	void DrawerActiveItemUpdate(void);

	std::vector<std::shared_ptr<ActiveItem>> GetActiveItem(void);
	std::vector<std::shared_ptr<DrawerActiveItem>> GetDrawerActiveItem(void);

	bool IsClear(void);

private:

	//���_�V�F�[�_�[
	int vertexShaderHandle_;

	//�s�N�Z���V�F�[�_�[
	int pixelShaderHandle_;

	//�v���C���[
	Player& player_;

	int itemGetSe_;

	//�S�[���̔�
	int goalBoxModelId_;
	int afterGoalBoxModelId_;

	//�}�b�v��̃��f��
	std::vector<int> modelIds_;

	//�v���C���[�Ɣ������郂�f��
	std::vector<int> obstacles_;

	//����Δԍ�
	std::vector<VECTOR> lightStoneNum_;
	int lightHandle[19];


	// �p�b�V�u�A�C�e��
	std::vector<std::unique_ptr<PassiveItem>> passiveItems_;

	//�A�N�e�B�u�A�C�e��
	std::vector<std::shared_ptr<ActiveItem>> activeItems_;

	//�����o���ɓ������A�N�e�B�u�A�C�e��
	std::vector<std::shared_ptr<DrawerActiveItem>> drawerActiveItems_;

	// �\�y
	std::vector<std::unique_ptr<Chest>> chests_;

	// �I
	std::vector<std::unique_ptr<Shelf>> shelfs_;
	// �_�~�[�I
	std::vector<std::unique_ptr<DummyShelf>> dummyShelfs_;

	// ���f������̊�{���
	Transform transform_;

	// �摜���
	int PMoveImage_;					// �ړ� 
	int PCameraControlImage_;		// �J��������
	int PRunMoveImage_;				// ����
	int PCrouchImage_;				// ���Ⴊ��
	int PActionImage_;				// �A�N�V����
	int PUseItemImage_;				// �A�C�e���g�p
	int PChoiceItemImage_;			// �A�C�e���I��

	int KMMoveImage_;					// �ړ� 
	int KMCameraControlImage_;		// �J��������
	int KMRunMoveImage_;				// ����
	int KMCrouchImage_;				// ���Ⴊ��
	int KMActionImage_;				// �A�N�V����
	int KMUseItemImage_;				// �A�C�e���g�p
	int KMChoiceItemImage_;			// �A�C�e���I��

	int clearImage_;				// �N���A����

	float passiveDis_;
	float activeDis_;
	int bellSound_;
	int drawerSound_;

	bool goalBoxFlag_;

	void ImageInit(void);
	void ImageDraw(void);

	void DrawGoalBox(void);

	void DrawChest(void);
	void DrawShelf(void);
	void DrawDummyShelf(void);

	//�p�b�V�u�A�C�e��
	void DrawPassiveItem(void);
	void MakePassiveItem(VECTOR pos, float axis, PassiveItem::PASSIVE passive);

	//�A�N�e�B�u�A�C�e��
	void DrawActiveItem(void);
	void MakeActiveItem(VECTOR pos, float axis, ActiveItem::ACTIVE active);

	//�����o���ɓ������A�N�e�B�u�A�C�e��
	void DrawDrawerActiveItem(void);
	void MakeDrawerActiveItem(VECTOR pos, float axis, DrawerActiveItem::ACTIVE active,int chestNum,int drawerNum);

	//����g�����Ƃ�
	void BellUpdate(void);

	//����΂��g�����Ƃ�
	void LightSroneUpdate(void);

	//�R���p�X���g�����Ƃ�
	void CompassUpdate(void);

	// �\�y�̍쐬
	void MakeChest(VECTOR localPos, float localAddAxis);

	// �I�̍쐬
	void MakeShelf(VECTOR localPos, float localAddAxis);
	// �_�~�[�I�̍쐬
	void MakeDummyShelf(VECTOR localPos, float localAddAxis);

	bool IsHitClearObject(void);
};

