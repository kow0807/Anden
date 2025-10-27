#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/Camera.h"
#include "ActiveItem.h"

ActiveItem::ActiveItem(VECTOR pos, float axis, ACTIVE active)
{
	transform_.pos = pos;
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(axis), 0.0f);
	active_ = active;
}

ActiveItem::~ActiveItem(void)
{
}

void ActiveItem::Init(void)
{
	switch (active_)
	{
	case ActiveItem::ACTIVE::NONE:
		break;
	case ActiveItem::ACTIVE::BELL:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::BELL));
		break;
	case ActiveItem::ACTIVE::LIGHT_STONE:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::LIGHT_STONE));

		//頂点シェーダーの読み込み
		vertexShaderHandle_ = LoadVertexShader("Data/Shader/LightStoneVertexShader.cso");

		//ピクセルシェーダーの読み込み
		pixelShaderHandle_ = LoadPixelShader("Data/Shader/LightStonePixelShader.cso");
		break;
	case ActiveItem::ACTIVE::KALEIDOSCOPE:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::KALEIDOSCOPE));
		break;
	case ActiveItem::ACTIVE::COMPASS:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::COMPASS_BASE));
		compassBasModelId_ = ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::COMPASS_BAS);
		compassBasPos_ = transform_.pos;
		compassBasQuaRot_ = transform_.quaRot;
		compassBasRot_ = compassBasQuaRot_.ToEuler();
		MV1SetScale(compassBasModelId_, { SCALE,SCALE,SCALE });
		break;
	case ActiveItem::ACTIVE::CHARM:
		transform_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
				ResourceManager::SRC::CHARM));
		break;
	}

	movePow_ = AsoUtility::VECTOR_ZERO;
	dir_ = AsoUtility::VECTOR_ZERO;
	bellCnt_ = BELL_ALIVE_TIME;
	speed_ = 2.0f;
	gravity_ = 0.0f;
	gravityPow_ = AsoUtility::VECTOR_ZERO;
	// 基底クラスの関数を呼ぶ
	ItemBase::Init();
}

void ActiveItem::Update(void)
{
	// 基底クラスの関数を呼ぶ
	ItemBase::Update();
	compassBasPos_ = transform_.pos;
	compassBasRot_ = compassBasQuaRot_.ToEuler();
	compassBasRot_.y += AsoUtility::Deg2RadF(90.0f);
	MV1SetPosition(compassBasModelId_, compassBasPos_);
	MV1SetRotationXYZ(compassBasModelId_, compassBasRot_);


}

void ActiveItem::Draw(void)
{
	if (active_ == ActiveItem::ACTIVE::LIGHT_STONE)
	{
		MV1SetUseOrigShader(true);
		SetUseVertexShader(vertexShaderHandle_);
		SetUsePixelShader(pixelShaderHandle_);
	}

	// 基底クラスの関数を呼ぶ
	ItemBase::Draw();
	switch (state_)
	{
	case ItemBase::STATE::NONE:
		break;
	case ItemBase::STATE::IN_MAP:
		MV1DrawModel(compassBasModelId_);
		break;
	case ItemBase::STATE::HIT:
		MV1DrawModel(compassBasModelId_);
		break;
	case ItemBase::STATE::PICK:
		break;
	case ItemBase::STATE::USE:
		MV1DrawModel(compassBasModelId_);
		break;
	}
	//DrawSphere3D(transform_.pos, 20.0f, 20, 0xff0000, 0xff0000, false);

	if (active_ == ActiveItem::ACTIVE::LIGHT_STONE)
	{
		SetUseVertexShader(-1);
		SetUsePixelShader(-1);
		MV1SetUseOrigShader(false);
	}
}

void ActiveItem::Move(VECTOR pPos, Quaternion pRot)
{
	switch (active_)
	{
	case ActiveItem::ACTIVE::NONE:
		break;
	case ActiveItem::ACTIVE::BELL:
		transform_.pos = pPos;
		transform_.quaRot = pRot;
		break;
	case ActiveItem::ACTIVE::LIGHT_STONE:
		transform_.pos = { pPos.x,10.0f,pPos.z };
		transform_.quaRot = pRot;
		break;
	case ActiveItem::ACTIVE::KALEIDOSCOPE:
		transform_.pos = pPos;
		transform_.quaRot = pRot;
		break;
	case ActiveItem::ACTIVE::COMPASS:
		transform_.pos = { 999999.9f,0.0f ,999999.9f };
		break;
	case ActiveItem::ACTIVE::CHARM:
		break;
	}
}

void ActiveItem::BellThrow(int modelId)
{
	// 移動前座標
	VECTOR prePos = transform_.pos;

	// 基本移動量の計算
	movePow_ = VScale(dir_, speed_);

	// 重力計算
	gravity_ += ADD_GRAVITY;
	if (gravity_ > MAX_GRAVITY)
	{
		gravity_ = MAX_GRAVITY;
	}
	gravityPow_ = VScale(AsoUtility::DIR_D, gravity_);

	// 移動後座標
	VECTOR movedPos = VAdd(transform_.pos, VAdd(movePow_, gravityPow_));

	// ステージとの衝突判定
	auto hits = MV1CollCheck_Sphere(modelId, -1, movedPos, COL_RADIUS);
	for (int i = 0; i < hits.HitNum; i++)
	{

		// 衝突情報
		auto hit = hits.Dim[i];

		// 法線
		VECTOR N = hit.Normal;

		// 現フレームの移動方向
		auto dir = VNorm(VSub(movedPos, prePos));
		// 反射ベクトル計算
		dir_ = VAdd(dir, VScale(N, 2.0f * (VDot(VScale(dir, -1.0f), N))));

		// 法線と上方向の内積
		float upDot = VDot(N, AsoUtility::DIR_U);

		// 地面と衝突しているか
		bool isGround = upDot > 0.9f;

		// 地面衝突時のガタガタ抑制
		if (isGround && speed_ < ADD_GRAVITY * 8.0f)
		{
			// 上下運動を無くす
			dir_.y = 0.0f;
			dir_ = VNorm(dir_);
		}

		// 反射時重力初期化
		gravity_ = REFRECT_GRAVITY;

		// 反射した分スピードも減少させる
		speed_ *= 0.8f;

		// スピードがある程度遅くなって、地面についたら停止させる
		if (speed_ < 0.5f && isGround)
		{
			// 衝突地点前に留める
			movedPos = prePos;
			speed_ = 0.0f;
		}
		else
		{

			// 衝突しない場所まで移動させる
			for (int i = 0; i < 10; i++)
			{
				auto polygon = hit.Position;
				auto hitCheck = HitCheck_Sphere_Triangle(
					movedPos, COL_RADIUS, polygon[0], polygon[1], polygon[2]);
				if (hitCheck)
				{
					movedPos = VAdd(transform_.pos, VScale(dir_, 1.0f));
				}
			}

		}
		break;

	}

	// 位置更新
	transform_.pos = movedPos;

	// 減速
	if (speed_ > 0.0f)
	{
		speed_ -= DECEL;
	}

	if (speed_ < 0.0f)
	{
		bellCnt_ = BELL_ALIVE_TIME;
		speed_ = 0.0f;
	}

	if (speed_ == 0.0f)
	{
		bellCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	}

	if (bellCnt_ < 0.0f)
	{
		ChangeState(STATE::NONE);
	}

	MV1CollResultPolyDimTerminate(hits);
}

void ActiveItem::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}

ActiveItem::ACTIVE ActiveItem::GetActive(void)
{
    return active_;
}

void ActiveItem::ChangeActive(ACTIVE active)
{
	// 状態の更新
	active_ = active;

	// 状態別の初期化処理
	switch (active_)
	{
	case ActiveItem::ACTIVE::NONE:
		break;
	case ActiveItem::ACTIVE::BELL:
		break;
	case ActiveItem::ACTIVE::LIGHT_STONE:
		break;
	case ActiveItem::ACTIVE::KALEIDOSCOPE:
		break;
	case ActiveItem::ACTIVE::COMPASS:
		break;
	case ActiveItem::ACTIVE::CHARM:
		break;
	}
}

void ActiveItem::ChangeState(STATE state)
{
	// 状態の更新
	state_ = state;

	// 状態別の初期化処理
	switch (state_)
	{
	case ItemBase::STATE::IN_MAP:
		break;
	case ItemBase::STATE::HIT:
		break;
	case ItemBase::STATE::PICK:
		break;
	case ItemBase::STATE::USE:
		movePow_ = transform_.quaRot.PosAxis(AsoUtility::DIR_F);
		movePow_ = VScale(movePow_, 30.0f);
		dir_ = VNorm(transform_.quaRot.PosAxis({ 0.0f, 0.2f, 1.0f }));
		speed_ = INITIAL_SPEED;
		gravity_ = INITIAL_GRAVITY;
		break;
	}
}

void ActiveItem::CompassBasUpdate(VECTOR pPos, VECTOR goalPos)
{
	//角度
	VECTOR angles = mainCamera->GetAngles();
	Quaternion rots = Quaternion::Euler(AsoUtility::Deg2RadF(angles.x), AsoUtility::Deg2RadF(angles.y), AsoUtility::Deg2RadF(angles.z));

	transform_.quaRot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(rots.y), AsoUtility::AXIS_Y);

	VECTOR relativeTPos = rots.PosAxis({40.0f,70.0f,100.0f});
	transform_.pos = VAdd(pPos, relativeTPos);

	VECTOR goalXZ = goalPos;
	goalXZ.y = 0.0f;

	auto posXZ = compassBasPos_;
	posXZ.y = 0.0f;

	VECTOR dir = VNorm(VSub(goalXZ, posXZ));

	Quaternion qua = Quaternion::LookRotation(dir);

	compassBasQuaRot_ = qua;
}

void ActiveItem::SetParam(void)
{
	// モデル制御の基本情報
	transform_.scl = { SCALE,SCALE,SCALE };
	transform_.quaRot = Quaternion();
	state_ = STATE::IN_MAP;
	transform_.Update();
}