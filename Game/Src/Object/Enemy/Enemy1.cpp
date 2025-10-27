#include "../../Utility/AsoUtility.h"
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/Camera.h"
#include "../Player.h"
#include "../WayPoint.h"
#include "../Maps/Map.h"
#include "Enemy1.h"


Enemy1::Enemy1(Player& player, WayPoint& wayPoint, Map& map, VECTOR pos)
	:player_(player), wayPoint_(wayPoint), map_(map)
{
	player_ = player;
	wayPoint_ = wayPoint;

	transform_.pos = pos;
}

Enemy1::~Enemy1(void)
{
}

void Enemy1::Init(void)
{
	// ���N���X�̊֐����Ă�
	EnemyBase::Init();

	state_ = STATE::NONE;
	noticeSe_ = LoadSoundMem("Data/Sound/notice.mp3");

	noticeBgmVol_ = 0;

	shakeCameraCnt_ = 0;

	ChangeState(STATE::PATROL);
	wayNum_ = GetRand(wayPoint_.GetWayPoints().size() - 1);
	oldIdx_ = 9999999;
	oldIdx2_ = 9999999;
	enemyNearPoint_ = wayPoint_.GetNearWayPointIndex(transform_.pos);
	targetNearPoint_ = wayPoint_.GetNearWayPointIndex(wayPoint_.GetWayPoints()[wayNum_]);
	nextPoint_ = wayPoint_.GetNextWayPointIndex(enemyNearPoint_, targetNearPoint_, wayNum_);
	isNotice_ = false;
	noticeTime_ = 0.0f;
}

void Enemy1::Update(void)
{
	// ���N���X�̊֐����Ă�
	EnemyBase::Update();

	shakeCameraCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	noticeTime_ -= SceneManager::GetInstance().GetDeltaTime();

	//���o
	if (EyeSearch() == true)
	{
		isNotice_ = true;
		noticeTime_ = 10.0f;
	}
	//���o
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

	switch (state_)
	{
	case STATE::PATROL:
		UpdatePatrol();
		if (isNotice_ == true)
		{
			shakeCameraCnt_ = 0.8f;
			noticeBgmVol_ = 170;
			ChangeNextPlayVolumeSoundMem(210, noticeSe_);
			PlaySoundMem(noticeSe_, DX_PLAYTYPE_BACK);
			ChangeState(STATE::DIRECT_MOVE);
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
			ChangeState(STATE::PATROL);
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
			ChangeState(STATE::PATROL);
		}
		break;

	case STATE::STOP:
		if (info.HitFlag == 0)
		{
			ChangeState(STATE::DIRECT_MOVE);
		}
		if (isNotice_ == false)
		{
			ChangeState(STATE::PATROL);
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

void Enemy1::Draw(void)
{
	MV1DrawModel(transform_.modelId);

	//if (EyeSearch() == true)
	//{
	//	// ����͈͓��F�f�B�t���[�Y�J���[��ԐF�ɂ���
	//	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
	//}
	//else
	//{
	//	// ����͈͊O�F�f�B�t���[�Y�J���[���D�F�ɂ���
	//	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.5f, 0.5f, 0.5f, 1.0f));
	//}

	//if (EarSearch(EAR_AREA) == true)
	//{
	//	// ����͈͓��F�f�B�t���[�Y�J���[��F�ɂ���
	//	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
	//}
	//else
	//{
	//	// ����͈͊O�F�f�B�t���[�Y�J���[���D�F�ɂ���
	//	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.5f, 0.5f, 0.5f, 1.0f));
	//}

	// ����͈͂̕`��
	//DrawDebugSearchRange();

	//DrawFormatString(10, 10, 0xffffff, "���̃E�F�C�|�C���g�F%d", nextPoint_);
	//DrawFormatString(10, 30, 0xffffff, "�ڎw���Ă���E�F�C�|�C���g�F%d", targetNearPoint_);
	//DrawFormatString(10, 50, 0xffffff, "�ǐՎ��ԁF%2f", noticeTime_);
	//switch (state_)
	//{
	//case STATE::PATROL:
	//	DrawString(10, 70, "�G�̏�ԁF�p�g���[��", 0xffffff);
	//	break;

	//case STATE::ROUTE_MOVE:
	//	DrawString(10, 70, "�G�̏�ԁF�ǐ�(��Q������)", 0xffffff);
	//	break;

	//case STATE::DIRECT_MOVE:
	//	DrawString(10, 70, "�G�̏�ԁF�ǐ�(��Q���Ȃ�)", 0xffffff);
	//	break;

	//}
}

void Enemy1::Release(void)
{
	DeleteSoundMem(noticeSe_);
}

void Enemy1::DrawDebugSearchRange(void)
{
	//���o
	DrawSphere3D(transform_.pos, EAR_AREA, 20, 0xff0000, 0xff0000, false);
	DrawSphere3D(transform_.pos, EAR_BIG_AREA, 20, 0xff0000, 0xff0000, false);

	//���W�A���ɕϊ�
	float viewRad = AsoUtility::Deg2RadF(VIEW_ANGLE);

	//�p�x����������擾
	VECTOR rad = transform_.quaRot.ToEuler();

	//�O������
	float x1 = sinf(rad.y);
	float z1 = cosf(rad.y);

	//�E�΂�30�x�̕���
	float x2 = sinf(rad.y + viewRad);
	float z2 = cosf(rad.y + viewRad);

	//���΂�30�x�̕���
	float x3 = sinf(rad.y - viewRad);
	float z3 = cosf(rad.y - viewRad);

	//�����̍��W
	VECTOR pos0 = transform_.pos;

	//�O������
	VECTOR pos1 = transform_.pos;
	pos1.x += x1 * EYE_VIEW_RANGE;
	pos1.z += z1 * EYE_VIEW_RANGE;

	//�E�΂�30�x�̕���
	VECTOR pos2 = transform_.pos;
	pos2.x += x2 * EYE_VIEW_RANGE;
	pos2.z += z2 * EYE_VIEW_RANGE;

	//���΂�30�x�̕���
	VECTOR pos3 = transform_.pos;
	pos3.x += x3 * EYE_VIEW_RANGE;
	pos3.z += z3 * EYE_VIEW_RANGE;

	DrawTriangle3D(pos0, pos1, pos2, 0xffdead, true);
	DrawTriangle3D(pos0, pos3, pos1, 0xffdead, true);
}


void Enemy1::Wandering(void)
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

void Enemy1::UpdatePatrol(void)
{
	Wandering();
	wayNum_ = nextPoint_;

	if (IsHitWayPoint() == true)
	{
		enemyNearPoint_ = nextPoint_;
		oldIdx2_ = oldIdx_;
		oldIdx_ = nextPoint_;
		nextPoint_ = wayPoint_.GetNextWayPointIndex(enemyNearPoint_, targetNearPoint_, oldIdx2_);

		if (IsHitGoalWayPoint() == true)
		{
			oldIdx_ = 999999;
			targetNearPoint_ = GetRand(wayPoint_.GetWayPoints().size() - 1);
			nextPoint_ = wayPoint_.GetNextWayPointIndex(enemyNearPoint_, targetNearPoint_, oldIdx_);
		}
	}
}

void Enemy1::UpdateRouteMove(void)
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
		if (IsHitGoalWayPoint() == true)
		{
			ChangeState(STATE::STOP);
		}
	}
}

void Enemy1::UpdateDirectMove(void)
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

bool Enemy1::IsHitPlayer(void)
{
	VECTOR pos1 = player_.GetTransform().pos;
	// �G�ƃv���C���[�Ƃ̏Փ˔���
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

bool Enemy1::GetIsNotice(void)
{
	return isNotice_;
}

Enemy1::STATE Enemy1::GetState(void)
{
	return state_;
}

int Enemy1::GetNoticeBgmVol(void)
{
	return noticeBgmVol_;
}


void Enemy1::SetParam(void)
{
	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::ENEMY));
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.quaRot = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f);
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);

	//1�ڂ̃A�j���[�V����
	animIdle_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_IDLE);

	//2�ڂ̃A�j���[�V����
	animWalking_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_WALK);

	//3�ڂ̃A�j���[�V����
	animRun_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_RUN);

	transform_.Update();

}

bool Enemy1::EyeSearch(void)
{
	// �v���C���[�̍��W���擾
	VECTOR pPos = player_.GetTransform().pos;

	// �G�l�~�[����v���C���[�܂ł̃x�N�g��
	VECTOR diff = VSub(pPos, transform_.pos);

	//����͈͂ɓ����Ă��邩���f
	float distance = std::pow(diff.x, 2.0f) + std::pow(diff.z, 2.0f);
	if (distance <= (std::pow(EYE_VIEW_RANGE, 2.0f)))
	{
		//�������猩���v���C���[�̊p�x�����߂�
		float rad = atan2(pPos.x - transform_.pos.x, pPos.z - transform_.pos.z);
		float viewRad = rad - transform_.rot.y;
		float viewDeg = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(viewRad)));

		//����p���ɓ����Ă��邩���f
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

bool Enemy1::EarSearch(float area)
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

bool Enemy1::IsHitWayPoint(void)
{
	VECTOR pos1 = wayPoint_.GetWayPoints()[wayNum_];
	// �G�ƃE�F�C�|�C���g�Ƃ̏Փ˔���
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

bool Enemy1::IsHitGoalWayPoint(void)
{
	// �G�ƃS�[���E�F�C�|�C���g�Ƃ̏Փ˔���
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

void Enemy1::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case STATE::PATROL:
		MV1DetachAnim(transform_.modelId, animAttachNo_);
		// �Đ�����A�j���[�V�����̐ݒ�
		animAttachNo_ = MV1AttachAnim(transform_.modelId, 0, animWalking_);
		// �A�j���[�V���������Ԃ̎擾
		animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
		stepAnim_ = 0.0f;
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
		// �Đ�����A�j���[�V�����̐ݒ�
		animAttachNo_ = MV1AttachAnim(transform_.modelId, 0, animRun_);
		// �A�j���[�V���������Ԃ̎擾
		animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
		stepAnim_ = 0.0f;
		oldIdx_ = 999999;
		break;
	case STATE::STOP:
		MV1DetachAnim(transform_.modelId, animAttachNo_);
		// �Đ�����A�j���[�V�����̐ݒ�
		animAttachNo_ = MV1AttachAnim(transform_.modelId, 0, animIdle_);
		// �A�j���[�V���������Ԃ̎擾
		animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
		stepAnim_ = 0.0f;
		oldIdx_ = 999999;
		break;
	}
}
