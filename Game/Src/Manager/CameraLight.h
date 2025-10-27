#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"


class CameraLight
{
public:

	CameraLight(void);
	~CameraLight(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	//ライトのハンドル
	int handle_;

	// ライトの位置
	VECTOR pos_;

	// ライトの向き
	VECTOR dir_;

	float OutAngle;	//影響角度
	float InAngle;	//減衰を始める角度
	float Range;	//影響最大距離
	float Atten0;	//ライトと頂点の距離に関係なく減衰する率を指定する引数
	float Atten1;	//ライトの距離に比例して減衰する率
	float Atten2;	//ライトの距離の二乗に比例して減衰する率

	// ライトを初期位置に戻す
	void SetDefault(void);

};

