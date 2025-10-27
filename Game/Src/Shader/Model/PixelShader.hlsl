//VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN 
#define PS_INPUT VertexToPixelLit

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParam:register(b4)
{
	float4 g_diff_color;
	float4 g_ambient_color;
	float3 g_light_dir;
	float dummy;
}

float4 main(PS_INPUT PSInput) :SV_TARGET0
{
	float4 color;
	
	//テクスチャの色を取得
	//color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
	//if (color.a < 0.01f)
	//{
	//	discard;
	//}

	color = float4(0.9f, 0.9f, 0.9f, 1.0f);


	Light light = g_common.light[0];
	float3 lightPos = light.position;

	Material material = g_common.material;

	//return color;

	PS_OUTPUT ret;
	float4 TextureDiffuseColor;
	float4 SpecularDiffuseColor;
	float3 Normal;
	float DiffuseAngleGen;
	float4 TotalDiffuse;
	float4 TotalSpecular;
	float3 V_to_Eye;
	float3 TempF3;
	float Temp;
	float3 lLightTemp;
	float lLightDistancePow2;
	float lLightGen;
	float3 lLightDir;
	float lLightDirectionCosA;

	//法線の準備
	Normal = normalize(PSInput.normal);

	//頂点座標から視点へのベクトルを正規化
	V_to_Eye = normalize(-PSInput.vwPos);

	//ディフューズカラーとスペキュラーカラーの蓄積値を初期化
	TotalDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	TotalSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//スポットライトの処理+++++++++++++++++++++++++++++++++(開始)

	//ライト方向ベクトルの計算
	lLightDir = normalize(PSInput.vwPos.xyz - lightPos.xyz);

	//return float4(abs(lLightDir.x), abs(lLightDir.y), abs(lLightDir.z), 1.0f);
	//return float4(lLightDir.xyz, 1.0f);

	//距離・スポットライト減衰値計算-------------(開始)

	//距離減衰計算------------------

	//頂点とライト位置との距離の二乗を求める
	lLightTemp = PSInput.vwPos.xyz - lightPos.xyz;
	lLightDistancePow2 = dot(lLightTemp, lLightTemp);

	//減衰率の計算　lLightGen = 1 / （減衰値0 + 減衰値1 * ( 距離 * 距離) ）
	lLightGen = 1.0f / (light.attenuation0 + light.attenuation1 * sqrt(lLightDistancePow2) + light.attenuation2 * lLightDistancePow2);

	//return float4(lLightGen,lLightGen,lLightGen,1.0f);
	//------------------------------

	//スポットライト減衰計算--------

	//ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積(即ち Cos a)を計算
	lLightDirectionCosA = dot(lLightDir, light.direction);

	//スポットライト減衰計算　pow( falloff, ((Cos a - Cos f) / (Cos q - Cos f)))
	lLightGen *= saturate(pow(abs(max(lLightDirectionCosA - light.spotParam0, 0.0f) * light.spotParam1), light.fallOff));


	//return float4(lLightGen,lLightGen,lLightGen,1.0f);
	
	//------------------------------

	//有効距離外だったら減衰率を最大にする処理
	lLightGen *= step(lLightDistancePow2, light.rangePow2);


	//return float4(lLightGen,lLightGen,lLightGen,1.0f);

	//距離・スポットライト減衰計算----------------(終了)

	//ディフューズ色計算

	//ディフューズ角度減衰率計算
	DiffuseAngleGen = saturate(dot(Normal, -lLightDir));
	//DiffuseAngleGen = abs(dot(Normal, -lLightDir));

	//ディフューズカラー蓄積値 += （ライトのディフューズカラー * マテリアルディフューズカラー * ディフューズ角度減衰率+ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの * 距離・スポットライトの角度減衰率）
	//TotalDiffuse += (light.diffuse * material.diffuse * DiffuseAngleGen + light.ambient) * lLightGen;
	TotalDiffuse += ((float4(light.diffuse.xyz, 1.0f) * material.diffuse * DiffuseAngleGen) + light.ambient) * lLightGen;


	//スペキュラーカラー計算

	//ハーフベクトルの計算
	TempF3 = normalize(V_to_Eye - lLightDir);

	
	// Temp = pow(max(0.0f,N*H),material.power.x)
	Temp = pow(max(0.0f, dot(Normal, TempF3)), material.power.x);

	//スペキュラーカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラーカラー
	TotalSpecular += Temp * lLightGen.x * (float4(light.specular.xyz, 1.0f));

	//スポットライトの処理+++++++++++++++++++++++++++++++++(終了)

	//出力カラー計算++++++++++++++++++++++++++++++++++++++++++++++++(開始)

	//TotalDiffuse = ライトディフーズカラー蓄積値 + (マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの)
	TotalDiffuse += material.ambientEmissive;

	// SpecularColor = ライトのスペキュラーカラー蓄積値 * マテリアルのスペキュラーカラー
	SpecularDiffuseColor = TotalSpecular * material.specular;

	// 出力カラー計算 = TotalDiffuse * テクスチャカラー + SpecularColor
	float4 retColor;
	//TextureDiffuseColor = tex2D(diffuseMapSampler, PSInput.uv.xy);
	//TextureDiffuseColor = float4(0.9f, 0.9f, 0.9f, 1.0f);

	TextureDiffuseColor = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv.xy);

	retColor.rgb = TextureDiffuseColor.rgb * TotalDiffuse.rgb + SpecularDiffuseColor.rgb;

	//アルファ値 = テクスチャアルファ * マテリアルのディフューズアルファ * 不透明度
	retColor.a = TextureDiffuseColor.a * material.diffuse.a * g_base.factorColor.a;

	//出力カラー計算++++++++++++++++++++++++++++++++++++++++++++++++(終了)

	//出力パラメータを返す
	return retColor;
}