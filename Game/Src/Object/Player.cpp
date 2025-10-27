#include"../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Utility/AsoUtility.h"
#include"../Manager/Camera.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER));
	transform_.pos = { 4000.0f, 70.0f, -4200.0f };
	transform_.topPos = { transform_.pos.x,180.0f,transform_.pos.z };
	transform_.downPos = { transform_.pos.x,60.0f,transform_.pos.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f);
	transform_.scl = { 2.0f,2.0f,2.0f };
	transform_.Update();
	transform_.stamina = STAMINA_MAX;
	state_ = STATE::IDLE;
	passive_ = PASSIVE::NONE;
	active_ = ACTIVE::BELL;
	moveDir_ = AsoUtility::VECTOR_ZERO;

	warpSe_= LoadSoundMem("Data/Sound/warp.mp3");
	stoneSe_ = LoadSoundMem("Data/Sound/stone.mp3");

	isAlive_ = true;
	povFlag_ = false;
	speed_ = 0.0f;
	tabiSpeed_ = 1.0f;
	staminaRecoverySpeed_ = 1.0f;
	bellCount_ = 0;
	lightStoneCount_ = 0;
	kaleidoScopeCount_ = 0;
	compassCount_ = 0;
	charmCount_ = 0;
	mouseWheelRotVol_ = 0;
	movedPos_ = { 4000.0f, 70.0f, -4200.0f };
}

void Player::Update(void)
{
	transform_.topPos.x = transform_.pos.x;
	transform_.topPos.z = transform_.pos.z;
	transform_.downPos.x = transform_.pos.x;
	transform_.downPos.z = transform_.pos.z;

	DINPUT_JOYSTATE input;
	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);
	auto& ins = InputManager::GetInstance();

	//パッドが接続されているとき
	if (GetJoypadNum() > 0)
	{
		if (input.Z > 0)
		{
			if (transform_.topPos.y > 100.0f)
			{
				transform_.topPos.y -= 10;
			}
			else
			{
				transform_.topPos.y = 100.0f;
			}
		}
		else
		{
			if (transform_.topPos.y < 180.0f)
			{
				transform_.topPos.y += 10;
			}
			else
			{
				transform_.topPos.y = 180.0f;
			}
		}

		if (transform_.stamina > STAMINA_MAX)
		{
			transform_.stamina = STAMINA_MAX;
		}

		switch (active_)
		{
		case Player::ACTIVE::NONE:
			ChangeActive(ACTIVE::BELL);
			break;
		case Player::ACTIVE::BELL:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 9000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::LIGHT_STONE);
			}
			break;
		case Player::ACTIVE::LIGHT_STONE:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 9000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::KALEIDOSCOPE);
			}
			if (input.POV[0] == 27000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::BELL);
			}
			break;
		case Player::ACTIVE::KALEIDOSCOPE:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 9000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::COMPASS);
			}
			if (input.POV[0] == 27000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::LIGHT_STONE);
			}
			break;
		case Player::ACTIVE::COMPASS:
			if (input.POV[0] == -1)
			{
				povFlag_ = true;
			}
			if (input.POV[0] == 27000 && povFlag_ == true)
			{
				povFlag_ = false;
				ChangeActive(ACTIVE::KALEIDOSCOPE);
			}
			break;
		}
	}
	else
	{
		if (ins.IsNew(KEY_INPUT_LCONTROL) || input.Z > 0)
		{
			if (transform_.topPos.y > 100.0f)
			{
				transform_.topPos.y -= 10;
			}
			else
			{
				transform_.topPos.y = 100.0f;
			}
		}
		else
		{
			if (transform_.topPos.y < 180.0f)
			{
				transform_.topPos.y += 10;
			}
			else
			{
				transform_.topPos.y = 180.0f;
			}
		}

		if (transform_.stamina > STAMINA_MAX)
		{
			transform_.stamina = STAMINA_MAX;
		}

		mouseWheelRotVol_ += ins.GetMouseWheelRotV();
		if (mouseWheelRotVol_ < 0)
		{
			mouseWheelRotVol_ = 0;
		}
		if (mouseWheelRotVol_ > 12)
		{
			mouseWheelRotVol_ = 12;
		}
		SelectActiveItem();
	}
	transform_.Update();
}

void Player::AfterUpdate(void)
{
	Move();
	transform_.pos = movedPos_;
}

void Player::Draw(void)
{
	//MV1DrawModel(transform_.modelId);
	DrawStamina(transform_.stamina, STAMINA_MAX);

	//DrawFormatString(10, 280, 0xffffff, "%2f,%2f,%2f", transform_.pos.x, transform_.pos.y, transform_.pos.z);
	//DrawFormatString(10, 90, 0xffffff, "プレイヤーのスピード：%2f", speed_);
	//switch (state_)
	//{
	//case STATE::IDLE:
	//	DrawString(10, 110, "プレイヤーの状態：待機", 0xffffff);
	//	break;

	//case STATE::WALK:
	//	DrawString(10, 110, "プレイヤーの状態：歩き", 0xffffff);
	//	break;

	//case STATE::RUN:
	//	DrawString(10, 110, "プレイヤーの状態：走り", 0xffffff);
	//	break;

	//case STATE::FATIGUE:
	//	DrawString(10, 110, "プレイヤーの状態：疲労", 0xffffff);
	//	break;

	//case STATE::SNEAK:
	//	DrawString(10, 110, "プレイヤーの状態：スニーク", 0xffffff);
	//	break;
	//}

	//switch (passive_)
	//{
	//case Player::PASSIVE::NONE:
	//	DrawString(10, 130, "パッシブアイテム：所持していない", 0xffffff);
	//	break;
	//case Player::PASSIVE::STRAW_DOLL:
	//	DrawString(10, 130, "パッシブアイテム：藁人形", 0xffffff);
	//	break;
	//case Player::PASSIVE::TABI:
	//	DrawString(10, 130, "パッシブアイテム：足袋", 0xffffff);
	//	break;
	//case Player::PASSIVE::GREEN_PURSE:
	//	DrawString(10, 130, "パッシブアイテム：緑の巾着", 0xffffff);
	//	break;
	//case Player::PASSIVE::HAND_MIRROR:
	//	DrawString(10, 130, "パッシブアイテム：割れた手鏡", 0xffffff);
	//	break;
	//}

	//DrawFormatString(10, 150, 0xffffff, "鈴：%d", bellCount_);
	//DrawFormatString(10, 170, 0xffffff, "光る石：%d", lightStoneCount_);
	//DrawFormatString(10, 190, 0xffffff, "マウス：%d", mouseWheelRotVol_);

	//switch (active_)
	//{
	//case Player::ACTIVE::NONE:
	//	DrawString(10, 210, "アクティブアイテム：所持していない", 0xffffff);
	//	break;
	//case Player::ACTIVE::BELL:
	//	DrawString(10, 210, "アクティブアイテム：鈴", 0xffffff);
	//	break;
	//case Player::ACTIVE::LIGHT_STONE:
	//	DrawString(10, 210, "アクティブアイテム：光る石", 0xffffff);
	//	break;
	//case Player::ACTIVE::KALEIDOSCOPE:
	//	DrawString(10, 210, "アクティブアイテム：万華鏡", 0xffffff);
	//	break;
	//case Player::ACTIVE::COMPASS:
	//	DrawString(10, 210, "アクティブアイテム：コンパス", 0xffffff);
	//	break;
	//}
}

void Player::Release(void)
{
	DeleteSoundMem(warpSe_);
	DeleteSoundMem(stoneSe_);
}

Transform& Player::GetTransform(void)
{
	return transform_;
}

VECTOR Player::GetMoveDir(void) const
{
	return moveDir_;
}

Player::STATE Player::GetState(void)
{
	return state_;
}

void Player::ChangePassive(PASSIVE passive)
{
	// 状態の更新
	passive_ = passive;

	// 状態別の初期化処理
	switch (passive_)
	{
	case Player::PASSIVE::NONE:
		tabiSpeed_ = 1.0f;
		staminaRecoverySpeed_ = 1.0f;
		break;
	case Player::PASSIVE::STRAW_DOLL:
		tabiSpeed_ = 1.0f;
		staminaRecoverySpeed_ = 1.0f;
		break;
	case Player::PASSIVE::TABI:
		tabiSpeed_ = 1.2f;
		staminaRecoverySpeed_ = 1.0f;
		break;
	case Player::PASSIVE::GREEN_PURSE:
		tabiSpeed_ = 1.0f;
		staminaRecoverySpeed_ = 3.0f;
		break;
	case Player::PASSIVE::HAND_MIRROR:
		tabiSpeed_ = 0.5f;
		staminaRecoverySpeed_ = 0.5f;
		break;
	}
}

Player::PASSIVE Player::GetPassive(void)
{
	return passive_;
}

void Player::ChangeActive(ACTIVE active)
{
	// 状態の更新
	active_ = active;

	// 状態別の初期化処理
	switch (active_)
	{
	case Player::ACTIVE::NONE:
		break;
	case Player::ACTIVE::BELL:
		break;
	case Player::ACTIVE::LIGHT_STONE:
		break;
	case Player::ACTIVE::KALEIDOSCOPE:
		break;
	case Player::ACTIVE::COMPASS:
		break;
	}
}

Player::ACTIVE Player::GetActive(void)
{
	return active_;
}

void Player::SelectActiveItem(void)
{
	auto& ins = InputManager::GetInstance();	
	if (mouseWheelRotVol_ >= 0 && mouseWheelRotVol_ < 3) { ChangeActive(ACTIVE::BELL); }
	if (mouseWheelRotVol_ >= 3 && mouseWheelRotVol_ < 6) { ChangeActive(ACTIVE::LIGHT_STONE); }
	if (mouseWheelRotVol_ >= 6 && mouseWheelRotVol_ < 9) { ChangeActive(ACTIVE::KALEIDOSCOPE); }
	if (mouseWheelRotVol_ >= 9 && mouseWheelRotVol_ < 12) { ChangeActive(ACTIVE::COMPASS); }
}

void Player::StrawDollUpdate(void)
{
	ChangeNextPlayVolumeSoundMem(170, warpSe_);
	PlaySoundMem(warpSe_, DX_PLAYTYPE_BACK);

	movedPos_ = { -1300.0f, 70.0f, -600.0f };
	transform_.pos = { -1300.0f, 70.0f, -600.0f };
	ChangePassive(PASSIVE::NONE);
}

void Player::PickBell(void)
{
	bellCount_++;
}

void Player::UseBell(void)
{
	if (bellCount_ > 0)
	{
		bellCount_--;
	}
}

int Player::GetBellCount(void)
{
	return bellCount_;
}

void Player::PickLightStone(void)
{
	lightStoneCount_++;
}

void Player::UseLightStone(void)
{
	if (lightStoneCount_ > 0)
	{
		ChangeNextPlayVolumeSoundMem(170, stoneSe_);
		PlaySoundMem(stoneSe_, DX_PLAYTYPE_BACK);

		lightStoneCount_--;
	}
}

int Player::GetLightStoneCount(void)
{
	return lightStoneCount_;
}

void Player::PickKaleidoScope(void)
{
	kaleidoScopeCount_++;
}

void Player::UseKaleidoScope(void)
{
	if (kaleidoScopeCount_ > 0)
	{
		ChangeNextPlayVolumeSoundMem(170, warpSe_);
		PlaySoundMem(warpSe_, DX_PLAYTYPE_BACK);

		movedPos_= { -1300.0f, 70.0f, -600.0f };
		transform_.pos = { -1300.0f, 70.0f, -600.0f };
		kaleidoScopeCount_--;
	}
}

int Player::GetKaleidoScopeCount(void)
{
	return kaleidoScopeCount_;
}

void Player::PickCompass(void)
{
	compassCount_++;
}

int Player::GetCompassCount(void)
{
	return compassCount_;
}

void Player::PickCharm(void)
{
	charmCount_++;
}

int Player::GetCharmCount(void)
{
	return charmCount_;
}

void Player::SetIsAlive(bool isAlive)
{
	isAlive_ = isAlive;
}

bool Player::GetIsAlive(void)
{
	return isAlive_;
}

void Player::Collision(std::vector<int> modelIds)
{
	for (const auto& modelId : modelIds)
	{
		// カプセルとの衝突判定
		auto hits = MV1CollCheck_Capsule(
			modelId, -1,
			transform_.topPos, transform_.downPos, 30.0f);

		// 衝突した複数のポリゴンと衝突回避するまで、
		// プレイヤーの位置を移動させる
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];
			// 地面と異なり、衝突回避位置が不明なため、何度か移動させる
			// この時、移動させる方向は、移動前座標に向いた方向であったり、
			// 衝突したポリゴンの法線方向だったりする
			for (int tryCnt = 0; tryCnt < 12; tryCnt++)
			{
				// 再度、モデル全体と衝突検出するには、効率が悪過ぎるので、
				// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
				int pHit = HitCheck_Capsule_Triangle(
					transform_.topPos, transform_.downPos, 30.0f,
					hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					// 法線の方向にちょっとだけ移動させる
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					//// カプセルも一緒に移動させる
					transform_.topPos.x = movedPos_.x;
					transform_.topPos.z = movedPos_.z;
					transform_.downPos.x = movedPos_.x;
					transform_.downPos.z = movedPos_.z;
					transform_.pos = movedPos_;
					transform_.Update();
					continue;
				}
				break;
			}
		}
		// 検出したポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);
	}
}

void Player::Move(void)
{
	DINPUT_JOYSTATE input;
	moveDir_ = AsoUtility::VECTOR_ZERO;

	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//パッドが接続されている場合
	if (GetJoypadNum() > 0)
	{
		if (input.Y < -500)
		{
			moveDir_ = AsoUtility::DIR_F;
		}
		if (input.Y > 500)
		{
			moveDir_ = AsoUtility::DIR_B;
		}
		if (input.X < -500)
		{
			moveDir_ = AsoUtility::DIR_L;
		}
		if (input.X > 500)
		{
			moveDir_ = AsoUtility::DIR_R;
		}
		//待機
		if (input.X == 0 && input.Y == 0)
		{
			ChangeState(STATE::IDLE);
			moveDir_ = VScale(moveDir_, speed_);
		}
		//歩き
		else if (input.Buttons[4] == 0)
		{
			ChangeState(STATE::WALK);
			moveDir_ = VScale(moveDir_, speed_);
		}

		//しゃがみ
		if (input.Z > 0)
		{
			ChangeState(STATE::SNEAK);
			moveDir_ = VScale(moveDir_, speed_);
			if (transform_.stamina < STAMINA_MAX)
			{
				transform_.stamina += staminaRecoverySpeed_;
			}
		}
		//ダッシュ
		else if (input.Buttons[4])
		{
			if (input.X != 0 || input.Y != 0)
			{
				if (transform_.stamina > 0)
				{
					ChangeState(STATE::RUN);
					moveDir_ = VScale(moveDir_, speed_);
					transform_.stamina--;
				}
				//疲労
				else
				{
					ChangeState(STATE::FATIGUE);
					moveDir_ = VScale(moveDir_, speed_);
				}
			}
			else if (transform_.stamina < STAMINA_MAX)
			{
				transform_.stamina += staminaRecoverySpeed_;
			}
		}
		else if (transform_.stamina < STAMINA_MAX)
		{
			transform_.stamina += staminaRecoverySpeed_;
		}
	}

	auto& ins = InputManager::GetInstance();

	//パッドが接続されていない場合
	if (GetJoypadNum() == 0)
	{
		if (ins.IsNew(KEY_INPUT_W)) { moveDir_ = AsoUtility::DIR_F; }
		if (ins.IsNew(KEY_INPUT_S)) { moveDir_ = AsoUtility::DIR_B; }
		if (ins.IsNew(KEY_INPUT_A)) { moveDir_ = AsoUtility::DIR_L; }
		if (ins.IsNew(KEY_INPUT_D)) { moveDir_ = AsoUtility::DIR_R; }

		//待機
		if (!ins.IsNew(KEY_INPUT_W) && !ins.IsNew(KEY_INPUT_S) && !ins.IsNew(KEY_INPUT_A) && !ins.IsNew(KEY_INPUT_D))
		{
			ChangeState(STATE::IDLE);
			moveDir_ = VScale(moveDir_, speed_);
		}
		//歩き
		else if (!ins.IsNew(KEY_INPUT_LSHIFT))
		{
			ChangeState(STATE::WALK);
			moveDir_ = VScale(moveDir_, speed_);
		}

		//しゃがみ
		if (ins.IsNew(KEY_INPUT_LCONTROL))
		{
			ChangeState(STATE::SNEAK);
			moveDir_ = VScale(moveDir_, speed_);
			if (transform_.stamina < STAMINA_MAX)
			{
				transform_.stamina += staminaRecoverySpeed_;
			}
		}
		//ダッシュ
		else if (ins.IsNew(KEY_INPUT_LSHIFT))
		{
			if (ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_D))
			{
				if (transform_.stamina > 0)
				{
					ChangeState(STATE::RUN);
					moveDir_ = VScale(moveDir_, speed_);
					transform_.stamina--;
				}
				//疲労
				else
				{
					ChangeState(STATE::FATIGUE);
					moveDir_ = VScale(moveDir_, speed_);
				}
			}
			else if (transform_.stamina < STAMINA_MAX)
			{
				transform_.stamina += staminaRecoverySpeed_;
			}
		}
		else if (transform_.stamina < STAMINA_MAX)
		{
			transform_.stamina += staminaRecoverySpeed_;
		}
	}

	//移動処理
	if (!AsoUtility::EqualsVZero(moveDir_))
	{
		//角度
		VECTOR angles = mainCamera->GetAngles();

		Quaternion rots = Quaternion::Euler(AsoUtility::Deg2RadF(0.0f), AsoUtility::Deg2RadF(angles.y), AsoUtility::Deg2RadF(0.0f));

		//現在の向きから進行方向を取得
		VECTOR direction = rots.PosAxis(moveDir_);

		VECTOR movePow = VScale(direction, SPEED);

		movedPos_ = VAdd(transform_.pos, movePow);
		targetPos_ = VAdd(targetPos_, movePow);
	}
}

void Player::DrawStamina(float stamina, int staminaMax)
{
	int r = 0;
	int g = 255;
	int b = 0;

	float ratio = stamina / (float)staminaMax;

	// 緑（Green）は減少し、赤（Red）は増加
	g = static_cast<int>(ratio * 255);
	r = static_cast<int>((1.0f - ratio) * 255);
	b = 0;  // 青成分は0に固定

	if (g > 255)
	{
		g = 255;
	}
	if (r < 0)
	{
		r = 0;
	}

	int color = GetColor(r, g, b);

	//枠を描画
	DrawBox(STAMINA_POS_X - STAMINA_SIZE_X, STAMINA_POS_Y, STAMINA_POS_X + STAMINA_SIZE_X, STAMINA_POS_Y + STAMINA_SIZE_Y, 0xffffff, FALSE);

	//ゲージを描画
	DrawBox(STAMINA_POS_X, STAMINA_POS_Y, STAMINA_POS_X + STAMINA_SIZE_X * stamina / staminaMax, STAMINA_POS_Y + STAMINA_SIZE_Y, color, TRUE);

	//ゲージを描画
	DrawBox(STAMINA_POS_X - STAMINA_SIZE_X * stamina / staminaMax, STAMINA_POS_Y, STAMINA_POS_X, STAMINA_POS_Y + STAMINA_SIZE_Y, color, TRUE);
}

void Player::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case STATE::IDLE:
		speed_ = 0.0f * tabiSpeed_;
		break;

	case STATE::WALK:
		speed_ = 1.0f * tabiSpeed_;
		break;

	case STATE::RUN:
		speed_ = 2.4f * tabiSpeed_;
		break;

	case STATE::FATIGUE:
		speed_ = 1.0f * tabiSpeed_;
		break;

	case STATE::SNEAK:
		speed_ = 0.5f * tabiSpeed_;
		break;
	}
}
