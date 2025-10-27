#include "../../Utility/AsoUtility.h"
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/Camera.h"
#include "../Player.h"
#include "../WayPoint.h"
#include "../Maps/Map.h"
#include "Enemy2.h"


Enemy2::Enemy2(Player& player, WayPoint& wayPoint, Map& map, VECTOR pos)
	:player_(player), wayPoint_(wayPoint), map_(map)
{
	//player_ = player;
	//wayPoint_ = wayPoint;
	//map_ = map;

	startPos_ = pos;
	transform_.pos = startPos_;
}

Enemy2::~Enemy2(void)
{
}

void Enemy2::Init(void)
{
	// 基底クラスの関数を呼ぶ
	EnemyBase::Init();

	state_ = STATE::NONE;

	noticeSe_ = LoadSoundMem("Data/Sound/notice.mp3");

	noticeBgmVol_ = 0;

	shakeCameraCnt_ = 0;

	ChangeState(STATE::STOP);
	wayNum_ = 1;
	oldIdx_ = 999999;
	oldIdx2_ = 999999;
	enemyNearPoint_ = wayPoint_.GetNearWayPointIndex(transform_.pos);
	targetNearPoint_ = wayPoint_.GetNearWayPointIndex(wayPoint_.GetWayPoints()[wayNum_]);
	nextPoint_ = wayPoint_.GetNextWayPointIndex(enemyNearPoint_, targetNearPoint_, wayNum_);
	isNotice_ = false;
	noticeTime_ = 0.0f;
	earBellFlag_ = false;
	bellPos_ = {};
	bellTime_ = 0.0f;
}

void Enemy2::Update(void)
{
	// 基底クラスの関数を呼ぶ
	EnemyBase::Update();

	shakeCameraCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	noticeTime_ -= SceneManager::GetInstance().GetDeltaTime();
	bellTime_ -= SceneManager::GetInstance().GetDeltaTime();

	EarBigSearch();
	//視覚
	if (EyeSearch() == true)
	{
		isNotice_ = true;
		noticeTime_ = 10.0f;
	}
	//聴覚
	if (EarSearch(EAR_AREA) == true && player_.GetState() == Player::STATE::WALK)
	{
		isNotice_ = true;
		noticeTime_ = 10.0f;
	}
	if (EarSearch(EAR_BIG_AREA) == true && player_.GetState() == Player::STATE::RUN)
	{
		isNotice_ = true;
		noticeTime_ = 10.0f;
	}

	if (noticeTime_ < 0.0f)
	{
		isNotice_ = false;
	}

	auto info = MV1CollCheck_Line(map_.GetModelId(), -1, { transform_.pos.x,60.0f,transform_.pos.z }, { player_.GetTransform().pos.x,60.0f,player_.GetTransform().pos.z });
	auto info2 = MV1CollCheck_Line(map_.GetModelId(), -1, { transform_.pos.x,60.0f,transform_.pos.z }, { bellPos_.x,60.0f,bellPos_.z });


	switch (state_)
	{
	case STATE::STOP:
		if (isNotice_ == true)
		{
			shakeCameraCnt_ = 0.8f;
			noticeBgmVol_ = 170;
			ChangeNextPlayVolumeSoundMem(210, noticeSe_);
			PlaySoundMem(noticeSe_, DX_PLAYTYPE_BACK);
			ChangeState(STATE::DIRECT_MOVE);
		}
		if (earBellFlag_)
		{
			ChangeState(STATE::DIRECT_BELL_MOVE);
		}
		break;

	case STATE::RETURN:
		ReturnUpdate();
		if (isNotice_ == true)
		{
			shakeCameraCnt_ = 0.8f;
			ChangeState(STATE::DIRECT_MOVE);
		}
		if (earBellFlag_)
		{
			ChangeState(STATE::DIRECT_BELL_MOVE);
		}
		break;

	case STATE::ROUTE_MOVE:
		UpdateRouteMove();
		if (info.HitFlag == 0)
		{
			ChangeState(STATE::DIRECT_MOVE);
		}
		if (isNotice_ == false)
		{
			ChangeState(STATE::RETURN);
		}
		break;
	case STATE::DIRECT_MOVE:
		UpdateDirectMove();
		if (info.HitFlag == 1)
		{
			ChangeState(STATE::ROUTE_MOVE);
		}
		if (isNotice_ == false)
		{
			ChangeState(STATE::RETURN);
		}
		break;

	case STATE::ROUTE_BELL_MOVE:
		UpdateRouteBellMove();
		if (bellTime_ < 0.0f)
		{
			ChangeState(STATE::RETURN);
		}
		if (isNotice_ == true)
		{
			shakeCameraCnt_ = 0.8f;
			ChangeState(STATE::DIRECT_MOVE);
		}
		break;

	case STATE::DIRECT_BELL_MOVE:
		UpdateDirectBellMove();
		if (info2.HitFlag == 1)
		{
			ChangeState(STATE::ROUTE_BELL_MOVE);
		}
		if (bellTime_ < 0.0f)
		{
			ChangeState(STATE::RETURN);
		}
		if (isNotice_ == true)
		{
			shakeCameraCnt_ = 0.8f;
			ChangeState(STATE::DIRECT_MOVE);
		}
		break;
	}

	if (shakeCameraCnt_ > 0)
	{
		mainCamera->ShakeCamera();
	}

	if (IsHitPlayer() == true)
	{
		if (player_.GetPassive() == Player::PASSIVE::STRAW_DOLL)
		{
			player_.StrawDollUpdate();
		}
		else
		{
			player_.SetIsAlive(false);
		}
	}
}

void Enemy2::Draw(void)
{
	MV1DrawModel(transform_.modelId);

	//if (EyeSearch() == true)
	//{
	//	// 視野範囲内：ディフューズカラーを赤色にする
	//	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
	//}
	//else
	//{
	//	// 視野範囲外：ディフューズカラーを灰色にする
	//	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.5f, 0.5f, 0.5f, 1.0f));
	//}

	//if (EarSearch(EAR_AREA) == true)
	//{
	//	// 視野範囲内：ディフューズカラーを青色にする
	//	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
	//}
	//else
	//{
	//	// 視野範囲外：ディフューズカラーを灰色にする
	//	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.5f, 0.5f, 0.5f, 1.0f));
	//}
	//// 視野範囲の描画
	//DrawDebugSearchRange();

	//DrawFormatString(10, 10, 0xffffff, "次のウェイポイント：%d", nextPoint_);
	//DrawFormatString(10, 30, 0xffffff, "目指しているウェイポイント：%d", targetNearPoint_);
	//DrawFormatString(10, 50, 0xffffff, "追跡時間：%2f", noticeTime_);
	//switch (state_)
	//{
	//case STATE::STOP:
	//	DrawString(10, 70, "敵の状態：ストップ", 0xffffff);
	//	break;

	//case STATE::ROUTE_MOVE:
	//	DrawString(10, 70, "敵の状態：追跡(障害物あり)", 0xffffff);
	//	break;

	//case STATE::DIRECT_MOVE:
	//	DrawString(10, 70, "敵の状態：追跡(障害物なし)", 0xffffff);
	//	break;
	//case STATE::RETURN:
	//	DrawString(10, 70, "敵の状態：戻る", 0xffffff);
	//	break;
	//case STATE::ROUTE_BELL_MOVE:
	//	DrawString(10, 70, "敵の状態：鈴発見(障害物あり)", 0xffffff);
	//	break;
	//case STATE::DIRECT_BELL_MOVE:
	//	DrawString(10, 70, "敵の状態：鈴発見(障害物なし)", 0xffffff);
	//	break;

	//}
}

void Enemy2::Release(void)
{
	DeleteSoundMem(noticeSe_);
}

void Enemy2::DrawDebugSearchRange(void)
{
	//聴覚
	DrawSphere3D(transform_.pos, EAR_AREA, 20, 0xff0000, 0xff0000, false);
	DrawSphere3D(transform_.pos, EAR_BIG_AREA, 20, 0xffff00, 0xffff00, false);

	//ラジアンに変換
	float viewRad = AsoUtility::Deg2RadF(VIEW_ANGLE);

	//角度から方向を取得
	VECTOR rad = transform_.quaRot.ToEuler();

	//前方方向
	float x1 = sinf(rad.y);
	float z1 = cosf(rad.y);

	//右斜め30度の方向
	float x2 = sinf(rad.y + viewRad);
	float z2 = cosf(rad.y + viewRad);

	//左斜め30度の方向
	float x3 = sinf(rad.y - viewRad);
	float z3 = cosf(rad.y - viewRad);

	//自分の座標
	VECTOR pos0 = transform_.pos;

	//前方方向
	VECTOR pos1 = transform_.pos;
	pos1.x += x1 * EYE_VIEW_RANGE;
	pos1.z += z1 * EYE_VIEW_RANGE;

	//右斜め30度の方向
	VECTOR pos2 = transform_.pos;
	pos2.x += x2 * EYE_VIEW_RANGE;
	pos2.z += z2 * EYE_VIEW_RANGE;

	//左斜め30度の方向
	VECTOR pos3 = transform_.pos;
	pos3.x += x3 * EYE_VIEW_RANGE;
	pos3.z += z3 * EYE_VIEW_RANGE;

	DrawTriangle3D(pos0, pos1, pos2, 0xffdead, true);
	DrawTriangle3D(pos0, pos3, pos1, 0xffdead, true);
}

void Enemy2::Wandering(void)
{
	VECTOR wayXZ = wayPoint_.GetWayPoints()[nextPoint_];
	wayXZ.y = 0.0f;

	auto posXZ = transform_.pos;
	posXZ.y = 0.0f;

	VECTOR dir = VNorm(VSub(wayXZ, posXZ));

	Quaternion qua = Quaternion::LookRotation(dir);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, qua, 0.1f);

	transform_.pos = VAdd(transform_.pos, VScale(dir, 6.0f));
}

void Enemy2::ReturnUpdate(void)
{
	Wandering();

	wayNum_ = nextPoint_;
	targetNearPoint_ = 21;

	if (IsHitWayPoint() == true)
	{
		enemyNearPoint_ = nextPoint_;
		oldIdx2_ = oldIdx_;
		oldIdx_ = nextPoint_;
		nextPoint_ = wayPoint_.GetNextWayPointIndex(enemyNearPoint_, targetNearPoint_, oldIdx2_);

		if (IsHitGoalWayPoint() == true)
		{
			ChangeState(STATE::STOP);
		}
	}
}

void Enemy2::UpdateRouteMove(void)
{
	Wandering();
	wayNum_ = nextPoint_;
	targetNearPoint_ = wayPoint_.GetNearWayPointIndex(player_.GetTransform().pos);

	if (IsHitWayPoint() == true)
	{
		enemyNearPoint_ = nextPoint_;
		oldIdx2_ = oldIdx_;
		oldIdx_ = nextPoint_;
		nextPoint_ = wayPoint_.GetNextWayPointIndex(enemyNearPoint_, targetNearPoint_, oldIdx2_);
	}
}

void Enemy2::UpdateDirectMove(void)
{
	VECTOR playerXZ = player_.GetTransform().pos;
	playerXZ.y = 0.0f;

	auto posXZ = transform_.pos;
	posXZ.y = 0.0f;

	VECTOR dir = VNorm(VSub(playerXZ, posXZ));

	Quaternion qua = Quaternion::LookRotation(dir);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, qua, 0.1f);

	transform_.pos = VAdd(transform_.pos, VScale(dir, 6.0f));
}

void Enemy2::UpdateRouteBellMove(void)
{
	Wandering();
	wayNum_ = nextPoint_;

	targetNearPoint_ = wayPoint_.GetNearWayPointIndex(bellPos_);

	if (IsHitWayPoint() == true)
	{
		enemyNearPoint_ = nextPoint_;
		oldIdx2_ = oldIdx_;
		oldIdx_ = nextPoint_;
		nextPoint_ = wayPoint_.GetNextWayPointIndex(enemyNearPoint_, targetNearPoint_, oldIdx2_);

		if (IsHitGoalWayPoint() == true)
		{
			ChangeState(STATE::DIRECT_BELL_MOVE);
		}
	}
}

void Enemy2::UpdateDirectBellMove(void)
{
	VECTOR bellXZ = bellPos_;
	bellXZ.y = 0.0f;

	auto posXZ = transform_.pos;
	posXZ.y = 0.0f;

		VECTOR diff = VSub(
			bellPos_,
		transform_.pos);
	float disPow = sqrtf(diff.x * diff.x + diff.z * diff.z);

	if (disPow > HIT_DISTANCE)
	{
		VECTOR dir = VNorm(VSub(bellXZ, posXZ));

		Quaternion qua = Quaternion::LookRotation(dir);
		transform_.quaRot = Quaternion::Slerp(transform_.quaRot, qua, 0.1f);

		transform_.pos = VAdd(transform_.pos, VScale(dir, 6.0f));
	}
}

bool Enemy2::IsHitPlayer(void)
{
	VECTOR pos1 = player_.GetTransform().pos;
	// 敵とプレイヤーとの衝突判定
	VECTOR diff = VSub(
		pos1,
		transform_.pos);
	float disPow = sqrtf(diff.x * diff.x + diff.z * diff.z);

	if (disPow <= HIT_DISTANCE)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool Enemy2::GetIsNotice(void)
{
	return isNotice_;
}

Enemy2::STATE Enemy2::GetState(void)
{
	return state_;
}

int Enemy2::GetNoticeBgmVol(void)
{
	return noticeBgmVol_;
}

void Enemy2::SetParam(void)
{
	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::ENEMY));
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.quaRot = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(60.0f), 0.0f);
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);

	//1つ目のアニメーション
	animIdle_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_IDLE);

	//2つ目のアニメーション
	animWalking_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_WALK);

	//3つ目のアニメーション
	animRun_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_RUN);

	transform_.Update();

}

bool Enemy2::EyeSearch(void)
{
	// プレイヤーの座標を取得
	VECTOR pPos = player_.GetTransform().pos;

	// エネミーからプレイヤーまでのベクトル
	VECTOR diff = VSub(pPos, transform_.pos);

	//視野範囲に入っているか判断
	float distance = std::pow(diff.x, 2.0f) + std::pow(diff.z, 2.0f);
	if (distance <= (std::pow(EYE_VIEW_RANGE, 2.0f)))
	{
		//自分から見たプレイヤーの角度を求める
		float rad = atan2(pPos.x - transform_.pos.x, pPos.z - transform_.pos.z);
		float viewRad = rad - transform_.rot.y;
		float viewDeg = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(viewRad)));

		//視野角内に入っているか判断
		if (viewDeg <= VIEW_ANGLE || viewDeg >= (360.0f - VIEW_ANGLE))
		{
			auto info = MV1CollCheck_Line(map_.GetModelId(), -1, { transform_.pos.x,60.0f,transform_.pos.z }, { player_.GetTransform().pos.x,60.0f,player_.GetTransform().pos.z });
			if (info.HitFlag == 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool Enemy2::EarSearch(float area)
{
	float EAR = area;
	VECTOR pos1 = player_.GetTransform().pos;

	VECTOR diff = VSub(
		pos1,
		transform_.pos);
	float disPow = sqrtf(diff.x * diff.x + diff.z * diff.z);

	if (disPow <= EAR)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Enemy2::EarBigSearch(void)
{
	for (const auto& a : map_.GetActiveItem()) {

		if (a->GetActive() == ActiveItem::ACTIVE::BELL)
		{
			if (a->GetState() == ActiveItem::STATE::USE)
			{
				VECTOR pos1 = a->GetTransform().pos;

				VECTOR diff = VSub(
					pos1,
					transform_.pos);
				float disPow = sqrtf(diff.x * diff.x + diff.z * diff.z);

				if (disPow <= EAR_BIG_AREA)
				{
					earBellFlag_ = true;
					bellPos_ = a->GetTransform().pos;
					bellTime_ = 5.0f;
					return;
				}
				else
				{
					earBellFlag_ = false;
				}
			}
		}
		else
		{
			earBellFlag_ = false;
		}
	}

	for (const auto& da : map_.GetDrawerActiveItem()) {

		if (da->GetActive() == DrawerActiveItem::ACTIVE::BELL)
		{
			if (da->GetState() == DrawerActiveItem::STATE::USE)
			{
				VECTOR pos1 = da->GetTransform().pos;

				VECTOR diff = VSub(
					pos1,
					transform_.pos);
				float disPow = sqrtf(diff.x * diff.x + diff.z * diff.z);

				if (disPow <= EAR_BIG_AREA)
				{
					earBellFlag_ = true;
					bellPos_ = da->GetTransform().pos;
					bellTime_ = 5.0f;
					return;
				}
				else
				{
					earBellFlag_ = false;
				}
			}
		}
		else
		{
			earBellFlag_ = false;
		}
	}
}

bool Enemy2::IsHitWayPoint(void)
{
	VECTOR pos1 = wayPoint_.GetWayPoints()[wayNum_];
	// 敵とウェイポイントとの衝突判定
	VECTOR diff = VSub(
		pos1,
		transform_.pos);
	float disPow = sqrtf(diff.x * diff.x + diff.z * diff.z);
	if (disPow < 10.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Enemy2::IsHitGoalWayPoint(void)
{
	// 敵とゴールウェイポイントとの衝突判定
	VECTOR diff = VSub(
		wayPoint_.GetWayPoints()[targetNearPoint_],
		transform_.pos);
	float disPow = sqrtf(diff.x * diff.x + diff.z * diff.z);
	if (disPow < 10.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Enemy2::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case STATE::STOP:
		MV1DetachAnim(transform_.modelId, animAttachNo_);
		// 再生するアニメーションの設定
		animAttachNo_ = MV1AttachAnim(transform_.modelId, 0, animIdle_);
		// アニメーション総時間の取得
		animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
		stepAnim_ = 0.0f;
		break;

	case STATE::RETURN:
		MV1DetachAnim(transform_.modelId, animAttachNo_);
		// 再生するアニメーションの設定
		animAttachNo_ = MV1AttachAnim(transform_.modelId, 0, animWalking_);
		// アニメーション総時間の取得
		animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
		stepAnim_ = 0.0f;

		nextPoint_ = wayPoint_.GetNearWayPointIndex(transform_.pos);
		oldIdx_ = 999999;

		noticeBgmVol_ = 0;

		break;
	case STATE::ROUTE_MOVE:
		oldIdx_ = 999999;
		enemyNearPoint_ = wayPoint_.GetNearWayPointIndex(transform_.pos);
		targetNearPoint_ = wayPoint_.GetNearWayPointIndex(player_.GetTransform().pos);
		nextPoint_ = wayPoint_.GetNextWayPointIndex(enemyNearPoint_, targetNearPoint_, oldIdx_);
		break;

	case STATE::DIRECT_MOVE:
		MV1DetachAnim(transform_.modelId, animAttachNo_);
		// 再生するアニメーションの設定
		animAttachNo_ = MV1AttachAnim(transform_.modelId, 0, animRun_);
		// アニメーション総時間の取得
		animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
		stepAnim_ = 0.0f;

		oldIdx_ = 999999;
		break;

	case STATE::ROUTE_BELL_MOVE:
		oldIdx_ = 999999;
		break;
	case STATE::DIRECT_BELL_MOVE:
		MV1DetachAnim(transform_.modelId, animAttachNo_);
		// 再生するアニメーションの設定
		animAttachNo_ = MV1AttachAnim(transform_.modelId, 0, animRun_);
		// アニメーション総時間の取得
		animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
		stepAnim_ = 0.0f;

		oldIdx_ = 999999;
		break;
	}
}