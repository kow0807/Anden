#include <DxLib.h>
#include "SceneBase.h"

SceneBase::SceneBase(void)
{
}

SceneBase::~SceneBase(void)
{
}

void SceneBase::Init(void)
{
	time_ = -1;
	hiTime_ = -1;
	secondTime_ = -1;
	thirdTime_ = -1;
	fourthTime_ = -1;
	fifthTime_ = -1;
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}

void SceneBase::Release(void)
{
}

bool SceneBase::IsHitLogo(int posX, int posY, float sizeX, float sizeY)
{
	int mPosX;
	int mPosY;

	GetMousePoint(&mPosX, &mPosY);
	if (mPosX > posX && mPosX < posX + sizeX &&
		mPosY > posY && mPosY < posY + sizeY)
	{
		return true;
	}
	return false;
}

void SceneBase::TimeSave(void)
{
	FILE* fp;
	errno_t err;

	// fopen_sを使用してファイルを書き込み専用で開く
	err = fopen_s(&fp, "Data/Save/Time.txt", "w");
	if (err == 0) {
		printf("ファイルが書き込み専用で正常に開かれました。\n");
		fprintf(fp, "%d\n", time_);
		fprintf(fp, "%d\n", hiTime_);
		fprintf(fp, "%d\n", secondTime_);
		fprintf(fp, "%d\n", thirdTime_);
		fprintf(fp, "%d\n", fourthTime_);
		fprintf(fp, "%d\n", fifthTime_);
		fclose(fp);
	}
	else {
		printf("書き込み専用でファイルを開けませんでした。エラーコード: %d\n", err);
	}
}

void SceneBase::TimeLoad(void)
{
	FILE* fp;
	errno_t err;

	// fopen_sを使用してファイルを読み取り専用で開く
	err = fopen_s(&fp, "Data/Save/Time.txt", "r");
	if (err == 0) {
		printf("ファイルが読み取り専用で正常に開かれました。\n");
		// ファイルからデータを読み取る処理...
		fscanf_s(fp, "%d\n", &time_);
		fscanf_s(fp, "%d\n", &hiTime_);
		fscanf_s(fp, "%d\n", &secondTime_);
		fscanf_s(fp, "%d\n", &thirdTime_);
		fscanf_s(fp, "%d\n", &fourthTime_);
		fscanf_s(fp, "%d\n", &fifthTime_);
		fclose(fp);
	}
	else {
		printf("読み取り専用でファイルを開けませんでした。エラーコード: %d\n", err);
	}
}
