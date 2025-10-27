#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "CameraLight.h"

CameraLight::CameraLight(void)
{
    handle_ = false;
	pos_ = { 0.0f,0.0f,0.0f };
	dir_ = { 0.0f,0.0f,0.0f };
    OutAngle = 0.0f;
    InAngle = 0.0f;
    Range = 0.0f;
    Atten0 = 0.0f;
    Atten1 = 0.0f;
    Atten2 = 0.0f;
}

CameraLight::~CameraLight(void)
{
}

void CameraLight::Init(void)
{
	//カメラライトの設定
	SetDefault();
}

void CameraLight::Update(void)
{

    // VBキーで OutAngle の値を変更
    if (CheckHitKey(KEY_INPUT_V) == 1)
    {
        OutAngle += 1.0f;
    }
    if (CheckHitKey(KEY_INPUT_B) == 1)
    {
        OutAngle -= 1.0f;
    }

    // NMキーで InAngle の値を変更
    if (CheckHitKey(KEY_INPUT_N) == 1)
    {
        InAngle += 1.0f;
    }
    if (CheckHitKey(KEY_INPUT_M) == 1)
    {
        InAngle -= 1.0f;
    }

    // FGキーでライトの影響範囲を変更
    if (CheckHitKey(KEY_INPUT_F) == 1)
    {
        Range += 20.0f;
    }
    if (CheckHitKey(KEY_INPUT_G) == 1)
    {
        Range -= 20.0f;
    }

    // HJキーでライトの距離減衰パラメータ０の値を変更
    if (CheckHitKey(KEY_INPUT_H) == 1)
    {
        Atten0 += 0.001f;
    }
    if (CheckHitKey(KEY_INPUT_J) == 1)
    {
        Atten0 -= 0.001f;
    }

    // KLキーでライトの距離減衰パラメータ１の値を変更
    if (CheckHitKey(KEY_INPUT_K) == 1)
    {
        Atten1 += 0.00001f;
    }
    if (CheckHitKey(KEY_INPUT_L) == 1)
    {
        Atten1 -= 0.00001f;
    }

    // RTキーでライトの距離減衰パラメータ２の値を変更
    if (CheckHitKey(KEY_INPUT_R) == 1)
    {
        Atten2 += 0.0000001f;
    }
    if (CheckHitKey(KEY_INPUT_T) == 1)
    {
        Atten2 -= 0.0000001f;
    }

    // 角度の値を補正
    if (OutAngle < 0.0f) OutAngle = 0.0f;
    if (OutAngle > 180.0f) OutAngle = 180.0f;
    if (InAngle < 0.0f) InAngle = 0.0f;
    if (InAngle > OutAngle) InAngle = OutAngle;

    // 影響距離の値を補正
    if (Range < 0.0f) Range = 0.0f;

    // 距離減衰パラメータの値を補正
    if (Atten0 < 0.0f) Atten0 = 0.0f;
    if (Atten1 < 0.0f) Atten1 = 0.0f;
    if (Atten2 < 0.0f) Atten2 = 0.0f;

    /*d = ライトから頂点への距離
        ライトの影響力(％) = 100.0f / (Atten0 + Atten1 * d + Atten2 * d * d)*/

    // Ｙ軸のマイナス方向のスポットライトを設定
    // メインライト
    //handle_ = ChangeLightTypeSpot(
    //    VGet(pos_.x, pos_.y, pos_.y),
    //    VGet(dir_.x, dir_.y, dir_.z),
    //    OutAngle,
    //    InAngle,
    //    Range,
    //    Atten0,
    //    Atten1,
    //    Atten2);
    //ChangeLightTypeSpot(
    //    VGet(pos_.x, pos_.y, pos_.y),
    //    VGet(dir_.x, dir_.y, dir_.z),
    //    OutAngle,
    //    InAngle,
    //    Range,
    //    Atten0,
    //    Atten1,
    //    Atten2);

    //SetLightAngleHandle(handle_, OutAngle, InAngle);

    // サブライト
    //SetLightRangeAttenHandle(handle_, Range, Atten0, Atten1, Atten2);

    // メインライト
    //SetLightRangeAtten(Range, Atten0, Atten1, Atten2);

#pragma region ライトの座標と方向

    //ライトにカメラを持ってくる

    //ライトの座標はカメラと同じにする
    pos_.x = mainCamera->GetPos().x;
    pos_.y = mainCamera->GetPos().y + 50.0f;
    pos_.z = mainCamera->GetPos().z;

    //カメラの回転に基づいてカメラライトの方向を計算
    //dir_.x = camera->GetAngles().y * camera->GetAngles().x;
    //dir_.z = camera->GetAngles().y * camera->GetAngles().x;

    //カメラの方向
    VECTOR direction = VNorm(VSub(mainCamera->GetTargetPos(), mainCamera->GetPos()));

    dir_.x = direction.x;
    dir_.y = direction.y;
    dir_.z = direction.z;

    //ライトの設定を更新
    //SetLightPositionHandle(handle_, VGet(pos_.x, pos_.y, pos_.y));
    //SetLightDirectionHandle(handle_, VGet(dir_.x, dir_.y, dir_.z));


    ////ライトの設定を更新
    //SetLightPosition(VGet(pos_.x, pos_.y, pos_.y));
    //SetLightDirection(VGet(dir_.x, dir_.y, dir_.z));


#pragma endregion

	
}

void CameraLight::Draw(void)
{
 //   DrawFormatString(0, 64, 0xffffff, "LightPos = (%2f,%2f,%2f)", pos_.x, pos_.y, pos_.z);
	//DrawFormatString(0, 80, 0xffffff, "LightDir = (%2f,%2f,%2f)", dir_.x, dir_.y, dir_.z);

 //   // パラメータの内容を画面に表示
 //   DrawFormatString(560, 520, GetColor(255, 255, 255), "Key:V.B  OutAngle( 度 ) %f", OutAngle);
 //   DrawFormatString(560, 536, GetColor(255, 255, 255), "Key:N.M  InAngle( 度 )  %f", InAngle);
 //   DrawFormatString(560, 552, GetColor(255, 255, 255), "Key:F.G  Range          %f", Range);
 //   DrawFormatString(560, 568, GetColor(255, 255, 255), "Key:H.J  Atten0         %f", Atten0);
 //   DrawFormatString(560, 584, GetColor(255, 255, 255), "Key:K.L  Atten1         %f", Atten1);
 //   DrawFormatString(560, 600, GetColor(255, 255, 255), "Key:R.T  Atten2         %f / 100.0f", Atten2 * 100.0f);

}

void CameraLight::Release(void)
{
	//DeleteLightHandle(handle_);
}

void CameraLight::SetDefault(void)
{
    pos_ = VGet(0.0f, 0.0f, 0.0f);

    dir_ = VGet(0.0f, 0.0f, -1.0f);

	// 各パラメータを初期化
	OutAngle = DX_PI_F / 4.0f;
	InAngle = DX_PI_F / 4.0f;
	Range = 2000.0f;
	Atten0 = 0.0f;
	Atten1 = 0.0006f;
	Atten2 = 0.0f;

	//SetLightTypeHandle(handle_, DX_LIGHTTYPE_SPOT);
	//SetLightPositionHandle(handle_, VGet(pos_.x, pos_.y, pos_.y));
	//SetLightDirectionHandle(handle_, VGet(dir_.x, dir_.y, dir_.z));
 //   SetLightAngleHandle(handle_, OutAngle, InAngle);
 //   SetLightRangeAttenHandle(handle_, Range, Atten0, Atten1, Atten2);

}