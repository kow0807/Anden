#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Manager/Camera.h"
#include "../../Utility/AsoUtility.h"
#include "../../Object/Maps/Furniture/Chest.h"
#include "../../Object/Maps/Furniture/Drawer.h"
#include "../../Object/Maps/Furniture/DummyDrawers.h"
#include "../../Object/Maps/Furniture/Shelf.h"
#include "../../Object/Maps/Furniture/DummyShelf.h"
#include "../Player.h"
#include "Map.h"

Map::Map(Player& player) :player_(player)
{
	vertexShaderHandle_ = -1;
	pixelShaderHandle_ = -1;

	player_ = player;
}

Map::~Map(void)
{
}

void Map::Init(void)
{
	//���_�V�F�[�_�[�̓ǂݍ���
	vertexShaderHandle_ = LoadVertexShader("Data/Shader/VertexShader.cso");

	//�s�N�Z���V�F�[�_�[�̓ǂݍ���
	pixelShaderHandle_ = LoadPixelShader("Data/Shader/PixelShader.cso");

	itemGetSe_= LoadSoundMem("Data/Sound/itemGet.mp3");
	goalBoxModelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::GOAL_BOX);
	MV1SetScale(goalBoxModelId_, {0.6f,0.6f,0.6f });
	MV1SetPosition(goalBoxModelId_, GOAL_BOX_POS);
	MV1SetRotationXYZ(goalBoxModelId_, {});

	afterGoalBoxModelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::AFTER_GOAL_BOX);
	MV1SetScale(afterGoalBoxModelId_, {0.6f,0.6f,0.6f });
	MV1SetPosition(afterGoalBoxModelId_, GOAL_BOX_POS);
	MV1SetRotationXYZ(afterGoalBoxModelId_, {});

	modelIds_.emplace_back(goalBoxModelId_);
	obstacles_.emplace_back(goalBoxModelId_);

	modelIds_.emplace_back(afterGoalBoxModelId_);
	obstacles_.emplace_back(afterGoalBoxModelId_);

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::MAP));

	float scale = 0.25f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { POS_X, POS_Y, POS_Z };
	transform_.quaRot = Quaternion::Euler(0.0f,0.0f,0.0f);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	passiveDis_ = 999999999.9f;
	activeDis_ = 999999999.9f;
	bellSound_ = LoadSoundMem("Data/Sound/bell.mp3");
	drawerSound_ = LoadSoundMem("Data/Sound/Drawer.mp3");

	goalBoxFlag_ = true;

	modelIds_.emplace_back(transform_.modelId);
	obstacles_.emplace_back(transform_.modelId);

	// ���f���S�̂̎Q�Ɨp���b�V�����\�z
	MV1SetupReferenceMesh(transform_.modelId, -1, TRUE);

	MakeShelf({ 4150.0f, 48.0f, -2910.0f },
		{ 0.0f });
	MakeDummyShelf({ 4150.0f, 48.0f, -2910.0f },
		{ 0.0f });

	MakeShelf({ 3900.0f, 48.0f, -2910.0f },
		{ 0.0f });
	MakeDummyShelf({ 3900.0f, 48.0f, -2910.0f },
		{ 0.0f });

	MakeShelf({ 3060.0f, 48.0f, -2910.0f },
		{ 0.0f });
	MakeDummyShelf({ 3060.0f, 48.0f, -2910.0f },
		{ 0.0f });
	MakeShelf({ 2800.0f, 48.0f, -2910.0f },
		{ 0.0f });
	MakeDummyShelf({ 2800.0f, 48.0f, -2910.0f },
		{ 0.0f });

	MakeShelf({ 4220.0f, 48.0f, -6540.0f },
		{ 0.0f });
	MakeDummyShelf({ 4220.0f, 48.0f, -6540.0f },
		{ 0.0f });
	MakeShelf({ 4220.0f, 48.0f, -6625.0f },
		{ 0.0f });
	MakeDummyShelf({ 4220.0f, 48.0f, -6625.0f },
		{ 0.0f });
	MakeShelf({ 3870.0f, 48.0f, -6540.0f },
		{ 0.0f });
	MakeDummyShelf({ 3870.0f, 48.0f, -6540.0f },
		{ 0.0f });
	MakeShelf({ 3870.0f, 48.0f, -6625.0f },
		{ 0.0f });
	MakeDummyShelf({ 3870.0f, 48.0f, -6625.0f },
		{ 0.0f });
	MakeShelf({ 3520.0f, 48.0f, -6540.0f },
		{ 0.0f });
	MakeDummyShelf({ 3520.0f, 48.0f, -6540.0f },
		{ 0.0f });
	MakeShelf({ 3520.0f, 48.0f, -6625.0f },
		{ 0.0f });
	MakeDummyShelf({ 3520.0f, 48.0f, -6625.0f },
		{ 0.0f });
	MakeShelf({ 3170.0f, 48.0f, -6540.0f },
		{ 0.0f });
	MakeDummyShelf({ 3170.0f, 48.0f, -6540.0f },
		{ 0.0f });
	MakeShelf({ 3170.0f, 48.0f, -6625.0f },
		{ 0.0f });
	MakeDummyShelf({ 3170.0f, 48.0f, -6625.0f },
		{ 0.0f });

	MakeShelf({ 310.0f, 48.0f, -6300.0f },
		{ 90.0f });
	MakeDummyShelf({ 310.0f, 48.0f, -6300.0f },
		{ 90.0f });

	MakeShelf({ 310.0f, 48.0f, -5300.0f },
		{ 90.0f });
	MakeDummyShelf({ 310.0f, 48.0f, -5300.0f },
		{ 90.0f });
	MakeShelf({ 750.0f, 48.0f, -5200.0f },
		{ 0.0f });
	MakeDummyShelf({ 750.0f, 48.0f, -5200.0f },
		{ 0.0f });

	MakeShelf({ -1700.0f, 48.0f, -5080.0f },
		{ 0.0f });
	MakeDummyShelf({ -1700.0f, 48.0f, -5080.0f },
		{ 0.0f });
	MakeShelf({ -1930.0f, 48.0f, -5300.0f },
		{ 90.0f });
	MakeDummyShelf({ -1930.0f, 48.0f, -5300.0f },
		{ 90.0f });

	MakeShelf({ -2110.0f, 48.0f, -4550.0f },
		{ 90.0f });
	MakeDummyShelf({ -2110.0f, 48.0f, -4550.0f },
		{ 90.0f });
	MakeShelf({ -2110.0f, 48.0f, -4300.0f },
		{ 90.0f });
	MakeDummyShelf({ -2110.0f, 48.0f, -4300.0f },
		{ 90.0f });
	MakeShelf({ -2110.0f, 48.0f, -4050.0f },
		{ 90.0f });
	MakeDummyShelf({ -2110.0f, 48.0f, -4050.0f },
		{ 90.0f });

	MakeShelf({ -1140.0f, 48.0f, -3300.0f },
		{ 90.0f });
	MakeDummyShelf({ -1140.0f, 48.0f, -3300.0f },
		{ 90.0f });

	MakeShelf({ -1610.0f, 48.0f, -1300.0f },
		{ 90.0f });
	MakeDummyShelf({ -1610.0f, 48.0f, -1300.0f },
		{ 90.0f });
	MakeShelf({ -1610.0f, 48.0f, -1070.0f },
		{ 90.0f });
	MakeDummyShelf({ -1610.0f, 48.0f, -1070.0f },
		{ 90.0f });

	MakeShelf({ -640.0f, 48.0f, -700.0f },
		{ 90.0f });
	MakeDummyShelf({ -640.0f, 48.0f, -700.0f },
		{ 90.0f });
	MakeShelf({ -640.0f, 48.0f, -450.0f },
		{ 90.0f });
	MakeDummyShelf({ -640.0f, 48.0f, -450.0f },
		{ 90.0f });
	MakeShelf({ -1900.0f, 48.0f, -330.0f },
		{ 0.0f });
	MakeDummyShelf({ -1900.0f, 48.0f, -330.0f },
		{ 0.0f });
	MakeShelf({ -1600.0f, 48.0f, -330.0f },
		{ 0.0f });
	MakeDummyShelf({ -1600.0f, 48.0f, -330.0f },
		{ 0.0f });

	MakeShelf({ 850.0f, 48.0f, -1300.0f },
		{ 0.0f });
	MakeDummyShelf({ 850.0f, 48.0f, -1300.0f },
		{ 0.0f });

	MakeShelf({ 1400.0f, 48.0f, -1300.0f },
		{ 0.0f });
	MakeDummyShelf({ 1400.0f, 48.0f, -1300.0f },
		{ 0.0f });

	MakeShelf({ 2740.0f, 48.0f, -650.0f },
		{ 90.0f });
	MakeDummyShelf({ 2740.0f, 48.0f, -650.0f },
		{ 90.0f });

	MakeShelf({ 4320.0f, 48.0f, -1250.0f },
		{ 90.0f });
	MakeDummyShelf({ 4320.0f, 48.0f, -1250.0f },
		{ 90.0f });

	MakeShelf({ 3200.0f, 48.0f, -1310.0f },
		{ 0.0f });
	MakeDummyShelf({ 3200.0f, 48.0f, -1310.0f },
		{ 0.0f });
	MakeShelf({ 2720.0f, 48.0f, -1450.0f },
		{ 90.0f });
	MakeDummyShelf({ 2720.0f, 48.0f, -1450.0f },
		{ 90.0f });


	MakeChest({ 3950.0f, 58.0f, -3940.0f },
		{ 0.0f });

	MakeChest({ 2720.0f, 58.0f, -3350.0f },
		{ -90.0f });

	MakeChest({ 4480.0f, 58.0f, -5650.0f },
		{ 90.0f });
	MakeChest({ 4480.0f, 58.0f, -5320.0f },
		{ 90.0f });
	MakeChest({ 3840.0f, 58.0f, -5650.0f },
		{ -90.0f });
	MakeChest({ 3840.0f, 58.0f, -5320.0f },
		{ -90.0f });

	MakeChest({ 1930.0f, 58.0f, -6400.0f },
		{ 90.0f });
	MakeChest({ 1930.0f, 58.0f, -6650.0f },
		{ 90.0f });

	MakeChest({ 880.0f, 58.0f, -6300.0f },
		{ 90.0f });

	//
	MakeChest({ 1930.0f, 58.0f, -5450.0f },
		{ 90.0f });

	MakeChest({ -730.0f, 58.0f, -6840.0f },
		{ 180.0f });

	MakeChest({ -1600.0f, 58.0f, -6520.0f },
		{ 180.0f });
	MakeChest({ -1120.0f, 58.0f, -6400.0f },
		{ 90.0f });

	MakeChest({ -1930.0f, 58.0f, -3550.0f },
		{ -90.0f });
	MakeChest({ -1930.0f, 58.0f, -3150.0f },
		{ -90.0f });

	MakeChest({ -1130.0f, 58.0f, -4100.0f },
		{ -90.0f });
	MakeChest({ -1130.0f, 58.0f, -4300.0f },
		{ -90.0f });
	
	MakeChest({ -700.0f, 58.0f, -2900.0f },
		{ 0.0f });
	MakeChest({ -950.0f, 58.0f, -2900.0f },
		{ 0.0f });

	MakeChest({ -2130.0f, 58.0f, -1600.0f },
		{ -90.0f });
	MakeChest({ -2130.0f, 58.0f, -1200.0f },
		{ -90.0f });

	MakeChest({ -840.0f, 58.0f, -1760.0f },
		{ 180.0f });

	MakeChest({ -2130.0f, 58.0f, -700.0f },
		{ -90.0f });
	MakeChest({ -2130.0f, 58.0f, -500.0f },
		{ -90.0f });

	MakeChest({ 320.0f, 58.0f, -1450.0f },
		{ -90.0f });
	MakeChest({ 320.0f, 58.0f, -1750.0f },
		{ -90.0f });

	MakeChest({ 1940.0f, 58.0f, -1450.0f },
		{ 90.0f });

	MakeChest({ 320.0f, 58.0f, -700.0f },
		{ -90.0f });
	MakeChest({ 320.0f, 58.0f, -450.0f },
		{ -90.0f });

	MakeChest({ 1500.0f, 58.0f, -240.0f },
		{ 0.0f });
	MakeChest({ 1920.0f, 58.0f, -500.0f },
		{ 90.0f });

	MakeChest({ 3500.0f, 58.0f, -810.0f },
		{ 180.0f });
	MakeChest({ 3400.0f, 58.0f, -300.0f },
		{ 0.0f });
	MakeChest({ 4320.0f, 58.0f, -500.0f },
		{ 90.0f });


	MakePassiveItem({ 3060.0f, 55.0f, -2910.0f }, 60.0f, PassiveItem::PASSIVE::TABI);
	MakePassiveItem({ 310.0f, 140.0f, -6270.0f }, 60.0f, PassiveItem::PASSIVE::STRAW_DOLL);
	MakePassiveItem({ 2740.0f, 140.0f, -680.0f }, 60.0f, PassiveItem::PASSIVE::HAND_MIRROR);
	MakePassiveItem({ 880.0f, 58.0f, -1300.0f }, 60.0f, PassiveItem::PASSIVE::GREEN_PURSE);

	MakeActiveItem({ 4190.0f, 138.0f, -2900.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);

	MakeActiveItem({ 4160.0f, 138.0f, -2930.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ 3900.0f, 140.0f, -2910.0f }, 180.0f, ActiveItem::ACTIVE::COMPASS);
	MakeActiveItem({ 3020.0f, 140.0f, -2890.0f }, 180.0f, ActiveItem::ACTIVE::BELL);
	MakeActiveItem({ 2850.0f, 140.0f, -2920.0f }, 180.0f, ActiveItem::ACTIVE::BELL);
	MakeActiveItem({ 2780.0f, 138.0f, -2900.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ 4200.0f, 138.0f, -6640.0f }, 180.0f, ActiveItem::ACTIVE::CHARM);
	MakeActiveItem({ 3880.0f, 58.0f, -6640.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ 3820.0f, 138.0f, -6530.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ 3500.0f, 138.0f, -6530.0f }, 120.0f, ActiveItem::ACTIVE::KALEIDOSCOPE);
	MakeActiveItem({ 3500.0f, 140.0f, -6640.0f }, 180.0f, ActiveItem::ACTIVE::BELL);
	MakeActiveItem({ 3150.0f, 140.0f, -6530.0f }, 180.0f, ActiveItem::ACTIVE::BELL);
	MakeActiveItem({ 330.0f, 138.0f, -5320.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ -1910.0f, 138.0f, -5330.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ -2100.0f, 138.0f, -4530.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ -2120.0f, 140.0f, -4320.0f }, 180.0f, ActiveItem::ACTIVE::BELL);
	MakeActiveItem({ -1130.0f, 138.0f, -3300.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ -1140.0f, 138.0f, -3260.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ -1620.0f, 138.0f, -1320.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ -640.0f, 138.0f, -700.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ -650.0f, 138.0f, -730.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ -640.0f, 58.0f, -430.0f }, 180.0f, ActiveItem::ACTIVE::BELL);
	MakeActiveItem({ -1930.0f, 58.0f, -320.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ 1430.0f, 140.0f, -1290.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ 1380.0f, 58.0f, -1290.0f }, 180.0f, ActiveItem::ACTIVE::BELL);
	MakeActiveItem({ 4320.0f, 58.0f, -1220.0f }, 180.0f, ActiveItem::ACTIVE::BELL);
	MakeActiveItem({ 4310.0f, 138.0f, -1270.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ 4330.0f, 138.0f, -1290.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ 2700.0f, 138.0f, -1480.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);
	MakeActiveItem({ 2710.0f, 138.0f, -1450.0f }, 180.0f, ActiveItem::ACTIVE::LIGHT_STONE);

	for (int i = 0; i < lightStoneNum_.size(); i++)
	{
		// �|�C���g���C�g�n���h�����쐬
		lightHandle[i] = CreatePointLightHandle(VGet(lightStoneNum_[i].x, lightStoneNum_[i].y, lightStoneNum_[i].z),
			7000.0f, 1.016523f, 0.000100f, 0.000010f);

		// �|�C���g���C�g�̃A���r�G���g�J���[�𖳌��ɂ���
		SetLightAmbColorHandle(lightHandle[i], GetColorF(0.0f, 0.0f, 0.0f, 0.0f));

		// �|�C���g���C�g�̃f�B�t���[�Y�J���[�������ԐF�ɂ���
		SetLightDifColorHandle(lightHandle[i], GetColorF(0.0f, 0.0f, 1.0f, 0.0f));

		// �O���[�o���A���r�G���g���C�g( ������ )���Q�O���̖��邳�ɂ���
		SetGlobalAmbientLight(GetColorF(0.2f, 0.2f, 0.2f, 0.0f));
	}


	MakeDrawerActiveItem({ 3700.0f, 92.0f, -7700.0f }, 180.0f, DrawerActiveItem::ACTIVE::CHARM, 0, 0);
	MakeDrawerActiveItem({ 3700.0f, 65.0f, -7700.0f }, 180.0f, DrawerActiveItem::ACTIVE::BELL, 1, 1);
	MakeDrawerActiveItem({ 3700.0f, 38.0f, -7700.0f }, 180.0f, DrawerActiveItem::ACTIVE::BELL, 2, 2);
	MakeDrawerActiveItem({ 3700.0f, 92.0f, -7700.0f }, 180.0f, DrawerActiveItem::ACTIVE::BELL, 4, 0);
	MakeDrawerActiveItem({ 3700.0f, 92.0f, -7700.0f }, 180.0f, DrawerActiveItem::ACTIVE::BELL, 7, 0);
	MakeDrawerActiveItem({ 3700.0f, 38.0f, -7700.0f }, 180.0f, DrawerActiveItem::ACTIVE::CHARM, 9, 2);
	MakeDrawerActiveItem({ 3700.0f, 65.0f, -7700.0f }, 180.0f, DrawerActiveItem::ACTIVE::CHARM, 10, 1);
	MakeDrawerActiveItem({ 3700.0f, 38.0f, -7700.0f }, 120.0f, DrawerActiveItem::ACTIVE::KALEIDOSCOPE, 12, 2);
	MakeDrawerActiveItem({ 3700.0f, 92.0f, -7700.0f }, 120.0f, DrawerActiveItem::ACTIVE::CHARM, 13, 0);
	MakeDrawerActiveItem({ 3700.0f, 92.0f, -7700.0f }, 120.0f, DrawerActiveItem::ACTIVE::BELL, 16, 0);
	MakeDrawerActiveItem({ 3700.0f, 65.0f, -7700.0f }, 120.0f, DrawerActiveItem::ACTIVE::BELL, 20, 1);
	MakeDrawerActiveItem({ 3700.0f, 38.0f, -7700.0f }, 120.0f, DrawerActiveItem::ACTIVE::CHARM, 21, 2);
	MakeDrawerActiveItem({ 3700.0f, 38.0f, -7700.0f }, 120.0f, DrawerActiveItem::ACTIVE::BELL, 22, 2);
	MakeDrawerActiveItem({ 3700.0f, 92.0f, -7700.0f }, 120.0f, DrawerActiveItem::ACTIVE::CHARM, 24, 0);
	MakeDrawerActiveItem({ 3700.0f, 38.0f, -7700.0f }, 120.0f, DrawerActiveItem::ACTIVE::KALEIDOSCOPE, 28, 2);
	MakeDrawerActiveItem({ 3700.0f, 65.0f, -7700.0f }, 120.0f, DrawerActiveItem::ACTIVE::BELL, 30, 1);
	MakeDrawerActiveItem({ 3700.0f, 92.0f, -7700.0f }, 120.0f, DrawerActiveItem::ACTIVE::CHARM, 33, 0);

	ImageInit();

}

void Map::Update(void)
{
	auto& ins = InputManager::GetInstance();

	player_.Collision(obstacles_);

	PassiveItemUpdate();
	ActiveItemUpdate();
	DrawerActiveItemUpdate();
	UseActiveItem();

	for (int i = 0; i < lightStoneNum_.size(); i++)
	{
		SetLightPositionHandle(lightHandle[i], VGet(lightStoneNum_[i].x, lightStoneNum_[i].y, lightStoneNum_[i].z));
	}

	//�p�b�h���ڑ�����Ă���Ƃ�
	if (GetJoypadNum() > 0)
	{
		for (const auto& c : chests_) {
			c->Update();
			auto& drawers = c->GetDrawers();
			auto& dummyDrawers = c->GetDummyDrawers();
			int i = 0;

			for (const auto& d : drawers)
			{
				if (d->GetState() == Drawer::STATE::CLOSE)
				{
					if (d->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == true && ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1,InputManager::JOYPAD_BTN::RIGHT))
					{
						d->ChangeState(Drawer::STATE::OPEN_MOVE);
						ChangeNextPlayVolumeSoundMem(170, drawerSound_);
						PlaySoundMem(drawerSound_, DX_PLAYTYPE_BACK);
						break;
					}
				}
				else if (d->GetState() == Drawer::STATE::OPEN)
				{
					if (d->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == true && ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
					{
						d->ChangeState(Drawer::STATE::CLOSE_MOVE);
						ChangeNextPlayVolumeSoundMem(170, drawerSound_);
						PlaySoundMem(drawerSound_, DX_PLAYTYPE_BACK);
						break;
					}
				}
				else
				{
					//���̒i�̈����o���̏ォ��J���Ȃ��悤�ɂ���
					if (d->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == true && ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
					{
						break;
					}
				}
				dummyDrawers[i]->SetPos(drawers[i]->GetPos());
				i++;
			}
		}
	}
	else
	{
		for (const auto& c : chests_) {
			c->Update();
			auto& drawers = c->GetDrawers();
			auto& dummyDrawers = c->GetDummyDrawers();
			int i = 0;

			for (const auto& d : drawers)
			{
				if (d->GetState() == Drawer::STATE::CLOSE)
				{
					if (d->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == true && ins.IsTrgMouseLeft() == true)
					{
						d->ChangeState(Drawer::STATE::OPEN_MOVE);
						ChangeNextPlayVolumeSoundMem(170, drawerSound_);
						PlaySoundMem(drawerSound_, DX_PLAYTYPE_BACK);
						break;
					}
				}
				else if (d->GetState() == Drawer::STATE::OPEN)
				{
					if (d->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == true && ins.IsTrgMouseLeft() == true)
					{
						d->ChangeState(Drawer::STATE::CLOSE_MOVE);
						ChangeNextPlayVolumeSoundMem(170, drawerSound_);
						PlaySoundMem(drawerSound_, DX_PLAYTYPE_BACK);
						break;
					}
				}
				else
				{
					//���̒i�̈����o���̏ォ��J���Ȃ��悤�ɂ���
					if (d->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == true && ins.IsTrgMouseLeft() == true)
					{
						break;
					}
				}
				dummyDrawers[i]->SetPos(drawers[i]->GetPos());
				i++;
			}
		}
	}
	transform_.Update();
}

void Map::Draw(void)
{
	MV1SetUseOrigShader(true);
	SetUseVertexShader(vertexShaderHandle_);
	SetUsePixelShader(pixelShaderHandle_);

	MV1DrawModel(transform_.modelId);

	DrawChest();
	DrawShelf();
	//DrawDummyShelf();

	//�A�C�e���̕`��
	DrawPassiveItem();
	DrawActiveItem();
	DrawDrawerActiveItem();
	DrawGoalBox();

	ImageDraw();

	SetUseVertexShader(-1);
	SetUsePixelShader(-1);
	MV1SetUseOrigShader(false);


	if (IsHitClearObject())
	{
		if (player_.GetCharmCount() < 5)
		{
			DrawString(Application::SCREEN_SIZE_X / 2 - 75, Application::SCREEN_SIZE_Y / 2 + 35, "����肪����Ȃ�", 0xffffff);
		}
		else
		{
			DrawString(Application::SCREEN_SIZE_X / 2 - 55, Application::SCREEN_SIZE_Y / 2 + 35, "������u��", 0xffffff);
		}
	}

	//DrawSphere3D(mainCamera->GetTargetPos(), 20.0f, 20, 0xff0000, 0xff0000, false);
}

void Map::Release(void)
{
	DeleteShader(vertexShaderHandle_);
	DeleteShader(pixelShaderHandle_);
	DeleteSoundMem(bellSound_);
	DeleteSoundMem(drawerSound_);
	DeleteSoundMem(itemGetSe_);
}

int Map::GetModelId(void)
{
	return transform_.modelId;
}

void Map::ImageInit(void)
{
#pragma region �p�b�g����

	// �ړ�����
	PMoveImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::MOVE_IMAGE_P).handleId_;

	// �J��������
	PCameraControlImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CAMERACONTROL_IMAGE_P).handleId_;

	// ����
	PRunMoveImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RUNMOVE_IMAGE_P).handleId_;

	// ���Ⴊ��
	PCrouchImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CROUCH_IMAGE_P).handleId_;

	// �A�N�V����
	PActionImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ACTION_IMAGE_P).handleId_;

	// �A�C�e���g�p
	PUseItemImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::USEITEM_IMAGE_P).handleId_;

	// �A�C�e���I��
	PChoiceItemImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHOICEITEM_IMAGE_P).handleId_;

#pragma endregion


#pragma region �L�[�}�E�X����

	// �ړ�����
	KMMoveImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::MOVE_IMAGE_KM).handleId_;

	// �J��������
	KMCameraControlImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CAMERACONTROL_IMAGE_KM).handleId_;

	// ����
	KMRunMoveImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RUNMOVE_IMAGE_KM).handleId_;

	// ���Ⴊ��
	KMCrouchImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CROUCH_IMAGE_KM).handleId_;

	// �A�N�V����
	KMActionImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ACTION_IMAGE_KM).handleId_;

	// �A�C�e���g�p
	KMUseItemImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::USEITEM_IMAGE_KM).handleId_;

	// �A�C�e���I��
	KMChoiceItemImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHOICEITEM_IMAGE_KM).handleId_;

#pragma endregion

	// �N���A����
	clearImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CLEAR_IMAGE).handleId_;
}

void Map::ImageDraw(void)
{
	//�p�b�h���ڑ�����Ă���ꍇ
	if (GetJoypadNum() > 0)
	{

		// �ړ�����
		DrawBillboard3D(VGet(4170.0f, 170.0f, -4000.0f), 0.0f, 0.0f, 100.0f, 0.0f, PMoveImage_, true);

		// �J��������
		DrawBillboard3D(VGet(4080.0f, 170.0f, -4000.0f), 0.0f, 0.0f, 100.0f, 0.0f, PCameraControlImage_, true);

		// ����
		DrawBillboard3D(VGet(4170.0f, 170.0f, -3600.0f), 0.0f, 0.0f, 100.0f, 0.0f, PRunMoveImage_, true);

		// ���Ⴊ��
		DrawBillboard3D(VGet(4000.0f, 170.0f, -3600.0f), 0.0f, 0.0f, 100.0f, 0.0f, PCrouchImage_, true);

		// �A�N�V����
		DrawBillboard3D(VGet(4000.0f, 170.0f, -3200.0f), 0.0f, 0.0f, 100.0f, 0.0f, PActionImage_, true);

		// �A�C�e���g�p
		DrawBillboard3D(VGet(3200.0f, 170.0f, -3500.0f), 0.0f, 0.0f, 100.0f, 0.0f, PUseItemImage_, true);

		// �A�C�e���I��
		DrawBillboard3D(VGet(3200.0f, 170.0f, -3200.0f), 0.0f, 0.0f, 100.0f, 0.0f, PChoiceItemImage_, true);
	}
	else
	{
		// �ړ�����
		DrawBillboard3D(VGet(4170.0f, 170.0f, -4000.0f), 0.0f, 0.0f, 100.0f, 0.0f, KMMoveImage_, true);

		// �J��������
		DrawBillboard3D(VGet(4080.0f, 170.0f, -4000.0f), 0.0f, 0.0f, 100.0f, 0.0f, KMCameraControlImage_, true);

		// ����
		DrawBillboard3D(VGet(4170.0f, 170.0f, -3600.0f), 0.0f, 0.0f, 100.0f, 0.0f, KMRunMoveImage_, true);

		// ���Ⴊ��
		DrawBillboard3D(VGet(4000.0f, 170.0f, -3600.0f), 0.0f, 0.0f, 100.0f, 0.0f, KMCrouchImage_, true);

		// �A�N�V����
		DrawBillboard3D(VGet(4000.0f, 170.0f, -3200.0f), 0.0f, 0.0f, 100.0f, 0.0f, KMActionImage_, true);

		// �A�C�e���g�p
		DrawBillboard3D(VGet(3200.0f, 170.0f, -3500.0f), 0.0f, 0.0f, 100.0f, 0.0f, KMUseItemImage_, true);

		// �A�C�e���I��
		DrawBillboard3D(VGet(3200.0f, 170.0f, -3200.0f), 0.0f, 0.0f, 100.0f, 0.0f, KMChoiceItemImage_, true);
	}

	// �A�C�e���I��
	DrawBillboard3D(VGet(3600.0f, 170.0f, -3200.0f), 0.0f, 0.0f, 100.0f, 0.0f, clearImage_, true);
}

void Map::DrawGoalBox(void)
{
	if (goalBoxFlag_)
	{
		MV1DrawModel(goalBoxModelId_);
	}
	else
	{
		MV1DrawModel(afterGoalBoxModelId_);
	}
}

void Map::DrawChest(void)
{
	for (const auto& c : chests_) {
		c->Draw();

		//DrawFormatString(540, 480, 0xffffff, "�����o�� = (%2f,%2f,%2f)", c->GetDrawers()[0]->GetTransform().pos.x, c->GetDrawers()[0]->GetTransform().pos.y, c->GetDrawers()[0]->GetTransform().pos.z);
	}
}

void Map::DrawShelf(void)
{
	for (const auto& s : shelfs_) {
		s->Draw();
	}
}

void Map::DrawDummyShelf(void)
{
	for (const auto& ds : dummyShelfs_) {
		ds->Draw();
	}
}

void Map::MakeChest(VECTOR localPos, float localAddAxis)
{
	Chest* chest = new Chest(localPos, localAddAxis);

	chest->Init();
	obstacles_.emplace_back(chest->GetTransform().modelId);
	modelIds_.emplace_back(chest->GetTransform().modelId);

	for (const auto& d : chest->GetDrawers())
	{
		modelIds_.emplace_back(d->GetTransform().modelId);
	}

	for (const auto& dd : chest->GetDummyDrawers())
	{
		obstacles_.emplace_back(dd->GetTransform().modelId);
	}

	chests_.emplace_back(chest);
}

void Map::MakeShelf(VECTOR localPos, float localAddAxis)
{
	Shelf* shelf = new Shelf(transform_, localPos, localAddAxis);

	shelf->Init();
	shelfs_.emplace_back(shelf);
	for (const auto& s : shelfs_)
	{
		modelIds_.emplace_back(s->GetTransform().modelId);
	}
}

void Map::MakeDummyShelf(VECTOR localPos, float localAddAxis)
{
	DummyShelf* dummyShelf = new DummyShelf(transform_, localPos, localAddAxis);

	dummyShelf->Init();
	obstacles_.emplace_back(dummyShelf->GetTransform().modelId);
	dummyShelfs_.emplace_back(dummyShelf);
}

void Map::DrawPassiveItem(void)
{
	for (const auto& p : passiveItems_)
	{
		p->Draw();
	}
}

void Map::MakePassiveItem(VECTOR pos, float axis, PassiveItem::PASSIVE passive)
{
	// �p�b�V�u�A�C�e���̎��̂��쐬
	PassiveItem* passiveItem = new PassiveItem(pos, axis, passive);
	// ������
	passiveItem->Init();
	passiveItems_.emplace_back(passiveItem);
	for (const auto& p : passiveItems_)
	{
		modelIds_.emplace_back(p->GetTransform().modelId);
	}
}

void Map::DrawActiveItem(void)
{
	for (const auto& a : activeItems_)
	{
		a->Draw();
	}
}

void Map::MakeActiveItem(VECTOR pos, float axis, ActiveItem::ACTIVE active)
{
	// �A�N�e�B�u�A�C�e���̎��̂��쐬
	ActiveItem* activeItem = new ActiveItem(pos, axis, active);
	// ������
	activeItem->Init();
	activeItems_.emplace_back(activeItem);

	if (active == ActiveItem::ACTIVE::LIGHT_STONE)
	{
		lightStoneNum_.emplace_back(pos);
	}

	for (const auto& a : activeItems_)
	{
		modelIds_.emplace_back(a->GetTransform().modelId);
	}
}

void Map::DrawDrawerActiveItem(void)
{
	for (const auto& da : drawerActiveItems_)
	{
		da->Draw();
	}
}

void Map::MakeDrawerActiveItem(VECTOR pos, float axis, DrawerActiveItem::ACTIVE active, int chestNum, int drawerNum)
{
	// �����o���ɓ������A�N�e�B�u�A�C�e���̎��̂��쐬
	DrawerActiveItem* drawerActiveItem = new DrawerActiveItem(pos, axis, active, chestNum, drawerNum);
	// ������
	drawerActiveItem->Init();
	drawerActiveItems_.emplace_back(drawerActiveItem);
	for (const auto& d : drawerActiveItems_)
	{
		modelIds_.emplace_back(d->GetTransform().modelId);
	}
}

void Map::BellUpdate(void)
{
	bool isUse = false;

	for (const auto& a : activeItems_)
	{
		if (a->GetActive() == ActiveItem::ACTIVE::BELL && isUse == false)
		{
			if (a->GetState() == ItemBase::STATE::PICK)
			{
				ChangeNextPlayVolumeSoundMem(170, bellSound_);
				PlaySoundMem(bellSound_, DX_PLAYTYPE_BACK);

				isUse = true;
				a->ChangeState(ActiveItem::STATE::USE);
				break;
			}
		}
	}

	for (const auto& da : drawerActiveItems_)
	{
		if (da->GetActive() == DrawerActiveItem::ACTIVE::BELL && isUse == false)
		{
			if (da->GetState() == ItemBase::STATE::PICK)
			{
				isUse = true;
				da->ChangeState(ActiveItem::STATE::USE);
				break;
			}
		}
	}
}


void Map::LightSroneUpdate(void)
{
	for (const auto& a : activeItems_)
	{
		if (a->GetActive() == ActiveItem::ACTIVE::LIGHT_STONE)
		{
			if (a->GetState() == ItemBase::STATE::PICK)
			{
				a->ChangeState(ItemBase::STATE::IN_MAP);
				break;
			}
		}
	}

	for (const auto& da : drawerActiveItems_)
	{
		if (da->GetActive() == DrawerActiveItem::ACTIVE::LIGHT_STONE)
		{
			if (da->GetState() == ItemBase::STATE::PICK)
			{
				da->ChangeState(ItemBase::STATE::IN_MAP);
				break;
			}
		}
	}
}

void Map::CompassUpdate(void)
{
	for (const auto& a : activeItems_)
	{
		if (a->GetActive() == ActiveItem::ACTIVE::COMPASS)
		{
			a->CompassBasUpdate({ player_.GetTransform().topPos.x,player_.GetTransform().topPos.y - 110.0f,player_.GetTransform().topPos.z }, GOAL_BOX_POS);

			if (a->GetState() == ItemBase::STATE::PICK)
			{
				a->ChangeState(ItemBase::STATE::USE);
				break;
			}
		}
	}

	for (const auto& da : drawerActiveItems_)
	{
		if (da->GetActive() == DrawerActiveItem::ACTIVE::COMPASS)
		{
			da->CompassBasUpdate({ player_.GetTransform().topPos.x,player_.GetTransform().topPos.y - 110.0f,player_.GetTransform().topPos.z }, GOAL_BOX_POS );

			if (da->GetState() == ItemBase::STATE::PICK)
			{
				da->ChangeState(ItemBase::STATE::USE);
				break;
			}
		}
	}
}


bool Map::IsClickPassiveItem(void)
{
	auto& ins = InputManager::GetInstance();

	//�p�b�h���ڑ�����Ă���Ƃ�
	if (GetJoypadNum() > 0)
	{
		for (const auto& p : passiveItems_)
		{
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) && p->GetState() == ItemBase::STATE::HIT)
			{
				return true;
			}
		}
	}
	else
	{
		for (const auto& p : passiveItems_)
		{
			if (ins.IsTrgMouseLeft() == true && p->GetState() == ItemBase::STATE::HIT)
			{
				return true;
			}
		}
	}
	return false;
}

void Map::PassiveItemUpdate(void)
{
	for (auto& p : passiveItems_)
	{
		auto info = MV1CollCheck_Line(transform_.modelId, -1,
			p->GetTransform().pos,
			{ player_.GetTransform().pos.x,60.0f,player_.GetTransform().pos.z });

		//�p�b�V�u�A�C�e�����͂������ɂ��鎞
		if (p->GetState() == ItemBase::STATE::IN_MAP && info.HitFlag == 0)
		{
			if (p->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == true)
			{
				p->ChangeState(ItemBase::STATE::HIT);
			}
		}

		if (p->GetState() == ItemBase::STATE::HIT)
		{
			//�p�b�V�u�A�C�e���ɓ͂��Ȃ��Ȃ�����
			if (p->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == false)
			{
				p->ChangeState(ItemBase::STATE::IN_MAP);
			}

			//�p�b�V�u�A�C�e�����E�����Ƃ�
			if (IsClickPassiveItem() == true)
			{
				ChangeNextPlayVolumeSoundMem(170, itemGetSe_);
				PlaySoundMem(itemGetSe_, DX_PLAYTYPE_BACK);

				//�v���C���[�������Ă���p�b�V�u�A�C�e���ɂ���ċ�����ς���
				//(��F�m�l�`�������Ă��鎞�ɑ��܂���肷��Ƙm�l�`�����̏�ɒu����)
				switch (player_.GetPassive())
				{
					//�v���C���[�����������Ă��Ȃ��Ƃ�
				case Player::PASSIVE::NONE:
					switch (p->GetPassive())
					{
					case PassiveItem::PASSIVE::NONE:
						break;
					case PassiveItem::PASSIVE::STRAW_DOLL:
						player_.ChangePassive(Player::PASSIVE::STRAW_DOLL);
						break;
					case PassiveItem::PASSIVE::TABI:
						player_.ChangePassive(Player::PASSIVE::TABI);
						break;
					case PassiveItem::PASSIVE::GREEN_PURSE:
						player_.ChangePassive(Player::PASSIVE::GREEN_PURSE);
						break;
					case PassiveItem::PASSIVE::HAND_MIRROR:
						player_.ChangePassive(Player::PASSIVE::HAND_MIRROR);
						break;
					}
					p->ChangeState(ItemBase::STATE::PICK);
					break;
					//�v���C���[���m�l�`�������Ă���Ƃ�
				case Player::PASSIVE::STRAW_DOLL:
					switch (p->GetPassive())
					{
					case PassiveItem::PASSIVE::NONE:
						break;
					case PassiveItem::PASSIVE::STRAW_DOLL:
						player_.ChangePassive(Player::PASSIVE::STRAW_DOLL);
						break;
					case PassiveItem::PASSIVE::TABI:
						player_.ChangePassive(Player::PASSIVE::TABI);
						break;
					case PassiveItem::PASSIVE::GREEN_PURSE:
						player_.ChangePassive(Player::PASSIVE::GREEN_PURSE);
						break;
					case PassiveItem::PASSIVE::HAND_MIRROR:
						player_.ChangePassive(Player::PASSIVE::HAND_MIRROR);
						break;
					}
					p->ChangePassive(PassiveItem::PASSIVE::STRAW_DOLL);
					break;
					//�v���C���[�����܂������Ă���Ƃ�
				case Player::PASSIVE::TABI:
					switch (p->GetPassive())
					{
					case PassiveItem::PASSIVE::NONE:
						break;
					case PassiveItem::PASSIVE::STRAW_DOLL:
						player_.ChangePassive(Player::PASSIVE::STRAW_DOLL);
						break;
					case PassiveItem::PASSIVE::TABI:
						player_.ChangePassive(Player::PASSIVE::TABI);
						break;
					case PassiveItem::PASSIVE::GREEN_PURSE:
						player_.ChangePassive(Player::PASSIVE::GREEN_PURSE);
						break;
					case PassiveItem::PASSIVE::HAND_MIRROR:
						player_.ChangePassive(Player::PASSIVE::HAND_MIRROR);
						break;
					}
					p->ChangePassive(PassiveItem::PASSIVE::TABI);
					break;
					//�v���C���[���΂̋В��������Ă���Ƃ�
				case Player::PASSIVE::GREEN_PURSE:
					switch (p->GetPassive())
					{
					case PassiveItem::PASSIVE::NONE:
						break;
					case PassiveItem::PASSIVE::STRAW_DOLL:
						player_.ChangePassive(Player::PASSIVE::STRAW_DOLL);
						break;
					case PassiveItem::PASSIVE::TABI:
						player_.ChangePassive(Player::PASSIVE::TABI);
						break;
					case PassiveItem::PASSIVE::GREEN_PURSE:
						player_.ChangePassive(Player::PASSIVE::GREEN_PURSE);
						break;
					case PassiveItem::PASSIVE::HAND_MIRROR:
						player_.ChangePassive(Player::PASSIVE::HAND_MIRROR);
						break;
					}
					p->ChangePassive(PassiveItem::PASSIVE::GREEN_PURSE);
					break;
				case Player::PASSIVE::HAND_MIRROR:
					switch (p->GetPassive())
					{
					case PassiveItem::PASSIVE::NONE:
						break;
					case PassiveItem::PASSIVE::STRAW_DOLL:
						player_.ChangePassive(Player::PASSIVE::STRAW_DOLL);
						break;
					case PassiveItem::PASSIVE::TABI:
						player_.ChangePassive(Player::PASSIVE::TABI);
						break;
					case PassiveItem::PASSIVE::GREEN_PURSE:
						player_.ChangePassive(Player::PASSIVE::GREEN_PURSE);
						break;
					case PassiveItem::PASSIVE::HAND_MIRROR:
						player_.ChangePassive(Player::PASSIVE::HAND_MIRROR);
						break;
					}
					p->ChangePassive(PassiveItem::PASSIVE::HAND_MIRROR);
					break;
				}
			}
		}

		p->Update();
	}
}

bool Map::IsClickActiveItem(void)
{
	auto& ins = InputManager::GetInstance();

	//�p�b�h���ڑ�����Ă���Ƃ�
	if (GetJoypadNum() > 0)
	{
		for (const auto& a : activeItems_)
		{
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) && a->GetState() == ItemBase::STATE::HIT)
			{
				return true;
			}
		}
	}
	else
	{
		for (const auto& a : activeItems_)
		{
			if (ins.IsTrgMouseLeft() == true && a->GetState() == ItemBase::STATE::HIT)
			{
				return true;
			}
		}
	}
	return false;
}

void Map::ActiveItemUpdate(void)
{
	for (auto& a : activeItems_)
	{
		//�A�N�e�B�u�A�C�e�����͂������ɂ��鎞
		if (a->GetState() == ItemBase::STATE::IN_MAP)
		{
			if (a->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == true)
			{
				a->ChangeState(ItemBase::STATE::HIT);
			}
		}

		if (a->GetState() == ItemBase::STATE::HIT)
		{
			//�A�N�e�B�u�A�C�e���ɓ͂��Ȃ��Ȃ�����
			if (a->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == false)
			{
				a->ChangeState(ItemBase::STATE::IN_MAP);
			}

			//�A�N�e�B�u�A�C�e�����E�����Ƃ�
			if (IsClickActiveItem() == true)
			{
				ChangeNextPlayVolumeSoundMem(170, itemGetSe_);
				PlaySoundMem(itemGetSe_, DX_PLAYTYPE_BACK);

				switch (a->GetActive())
				{
				case ActiveItem::ACTIVE::NONE:
					break;
				case ActiveItem::ACTIVE::BELL:
					player_.PickBell();
					break;
				case ActiveItem::ACTIVE::LIGHT_STONE:
					player_.PickLightStone();
					break;
				case ActiveItem::ACTIVE::KALEIDOSCOPE:
					player_.PickKaleidoScope();
					break;
				case ActiveItem::ACTIVE::COMPASS:
					player_.PickCompass();
					break;
				case ActiveItem::ACTIVE::CHARM:
					player_.PickCharm();
					break;
				}
				a->ChangeState(ItemBase::STATE::PICK);
				break;
			}
		}

		//�A�N�e�B�u�A�C�e���������Ă���Ƃ�
		if (a->GetState() == ItemBase::STATE::PICK)
		{
			VECTOR pos = { player_.GetTransform().pos.x, 150.0f, player_.GetTransform().pos.z };
			//�p�x
			VECTOR angles = mainCamera->GetAngles();
			Quaternion rots = Quaternion::Euler(AsoUtility::Deg2RadF(angles.x), AsoUtility::Deg2RadF(angles.y), AsoUtility::Deg2RadF(angles.z));
			a->Move(pos, rots);
		}

		a->Update();
	}

}

bool Map::IsClickDrawerActiveItem(void)
{
	auto& ins = InputManager::GetInstance();

	//�p�b�h���ڑ�����Ă���Ƃ�
	if (GetJoypadNum() > 0)
	{
		for (const auto& da : drawerActiveItems_)
		{
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) && da->GetState() == ItemBase::STATE::HIT)
			{
				return true;
			}
		}
	}
	else
	{
		for (const auto& da : drawerActiveItems_)
		{
			if (ins.IsTrgMouseLeft() == true && da->GetState() == ItemBase::STATE::HIT)
			{
				return true;
			}
		}
	}
	return false;

}

void Map::DrawerActiveItemUpdate(void)
{
	for (auto& da : drawerActiveItems_)
	{
		//�A�N�e�B�u�A�C�e�����͂������ɂ��鎞
		if (da->GetState() == ItemBase::STATE::IN_MAP)
		{
			if (da->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == true)
			{
				da->ChangeState(ItemBase::STATE::HIT);
			}
		}

		if (da->GetState() == ItemBase::STATE::HIT)
		{
			//�A�N�e�B�u�A�C�e���ɓ͂��Ȃ��Ȃ�����
			if (da->IsHitPlayer(player_.GetTransform().topPos, modelIds_) == false)
			{
				da->ChangeState(ItemBase::STATE::IN_MAP);
			}

			//�A�N�e�B�u�A�C�e�����E�����Ƃ�
			if (IsClickDrawerActiveItem() == true)
			{
				ChangeNextPlayVolumeSoundMem(255, itemGetSe_);
				PlaySoundMem(itemGetSe_, DX_PLAYTYPE_BACK);

				switch (da->GetActive())
				{
				case DrawerActiveItem::ACTIVE::NONE:
					break;
				case DrawerActiveItem::ACTIVE::BELL:
					player_.PickBell();
					break;
				case DrawerActiveItem::ACTIVE::LIGHT_STONE:
					player_.PickLightStone();
					break;
				case DrawerActiveItem::ACTIVE::KALEIDOSCOPE:
					player_.PickKaleidoScope();
					break;
				case DrawerActiveItem::ACTIVE::COMPASS:
					player_.PickCompass();
					break;
				case DrawerActiveItem::ACTIVE::CHARM:
					player_.PickCharm();
					break;
				}
				da->ChangeState(ItemBase::STATE::PICK);
				break;
			}
		}

		//�A�N�e�B�u�A�C�e���������Ă���Ƃ�
		if (da->GetState() == ItemBase::STATE::PICK)
		{
			VECTOR pos = { player_.GetTransform().pos.x, 150.0f, player_.GetTransform().pos.z };
			//�p�x
			VECTOR angles = mainCamera->GetAngles();
			Quaternion rots = Quaternion::Euler(AsoUtility::Deg2RadF(angles.x), AsoUtility::Deg2RadF(angles.y), AsoUtility::Deg2RadF(angles.z));
			da->Move(pos, rots);
		}

		if (da->GetState() == ItemBase::STATE::IN_MAP || da->GetState() == ItemBase::STATE::HIT)
		{
			da->GetTransform().pos.x = chests_[da->GetChestNum()]->GetDrawers()[da->GetDrawerNum()]->GetPos().x;
			da->GetTransform().pos.z = chests_[da->GetChestNum()]->GetDrawers()[da->GetDrawerNum()]->GetPos().z;
		}
		da->Update();
	}
}

std::vector<std::shared_ptr<ActiveItem>> Map::GetActiveItem(void)
{
	return activeItems_;
}

std::vector<std::shared_ptr<DrawerActiveItem>> Map::GetDrawerActiveItem(void)
{
	return drawerActiveItems_;
}


void Map::UseActiveItem(void)
{
	auto& ins = InputManager::GetInstance();

	//�p�b�h���ڑ�����Ă���Ƃ�
	if (GetJoypadNum() > 0)
	{
		if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT))
		{
			switch (player_.GetActive())
			{
			case Player::ACTIVE::NONE:
				break;
			case Player::ACTIVE::BELL:
				BellUpdate();
				player_.UseBell();
				break;
			case Player::ACTIVE::LIGHT_STONE:
				LightSroneUpdate();
				player_.UseLightStone();
				break;
			case Player::ACTIVE::KALEIDOSCOPE:
				player_.UseKaleidoScope();
				break;
			}
		}
	}
	else
	{
		if (ins.IsTrgMouseRight() == true)
		{
			switch (player_.GetActive())
			{
			case Player::ACTIVE::NONE:
				break;
			case Player::ACTIVE::BELL:
				BellUpdate();
				player_.UseBell();
				break;
			case Player::ACTIVE::LIGHT_STONE:
				LightSroneUpdate();
				player_.UseLightStone();
				break;
			case Player::ACTIVE::KALEIDOSCOPE:
				player_.UseKaleidoScope();
				break;
			}
		}
	}

	if (player_.GetActive() == Player::ACTIVE::COMPASS && player_.GetCompassCount() >= 1)
	{
		CompassUpdate();
	}
	else
	{
		for (const auto& a : activeItems_)
		{
			if (a->GetActive() == ActiveItem::ACTIVE::COMPASS)
			{
				if (a->GetState() == ItemBase::STATE::USE)
				{
					a->ChangeState(ItemBase::STATE::PICK);
					break;
				}
			}
		}
	}

	for (auto& a : activeItems_)
	{
		if (a->GetState() == ActiveItem::STATE::USE)
		{
			switch (a->GetActive())
			{
			case ActiveItem::ACTIVE::BELL:
				a->BellThrow(transform_.modelId);
				break;
			}
		}
	}

	for (auto& da : drawerActiveItems_)
	{
		if (da->GetState() == DrawerActiveItem::STATE::USE)
		{
			switch (da->GetActive())
			{
			case DrawerActiveItem::ACTIVE::BELL:
				da->BellThrow(transform_.modelId);
				break;
			}
		}
	}
}

bool Map::IsHitClearObject(void)
{
	bool isHitOther = false;

	VECTOR pos1 = { player_.GetTransform().pos.x,player_.GetTransform().pos.y + 14.0f ,player_.GetTransform().pos.z };
	VECTOR pos2 = mainCamera->GetTargetPos();

	auto info = MV1CollCheck_Line(goalBoxModelId_, -1, pos1, pos2);

	if (info.HitFlag == 1)
	{
		return true;
	}
	return false;
}

bool Map::IsClear(void)
{
	auto& ins = InputManager::GetInstance();

	//�p�b�h���ڑ�����Ă���Ƃ�
	if (GetJoypadNum() > 0)
	{
		if (IsHitClearObject())
		{
			if (player_.GetCharmCount() >= 5)
			{
				if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
				{
					goalBoxFlag_ = false;
					return true;
				}
			}
		}
	}
	else
	{
		if (IsHitClearObject())
		{
			if (player_.GetCharmCount() >= 5)
			{
				if (ins.IsTrgMouseLeft() == true)
				{
					goalBoxFlag_ = false;
					return true;
				}
			}
		}
	}
	return false;
}
