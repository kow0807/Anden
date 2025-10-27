//VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//　IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_1FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret;
	
	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
	float4 lLocalPosition;
	float4 lWorldPosition;
	float4 lViewPosition;

	float3 lWorldNrm;

	// float3 → float4
	lLocalPosition.xyz = VSInput.pos;
	lLocalPosition.w = 1.0f;

	// ローカル座標をワールド座標に変換(剛体)
	lWorldPosition.w = 1.0f;
	lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
	//ret.worldPos = lWorldPosition.xyz;

	// ワールド座標をビュー座標に変換
	lViewPosition.w = 1.0f;
	lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
	ret.vwPos = lViewPosition.xyz;

	// ビュー座標を射影座標に変換
	ret.svPos = mul(lViewPosition, g_base.projectionMatrix);

	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )

	// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )
	// UV座標
	ret.uv.x = VSInput.uv0.x;
	ret.uv.y = VSInput.uv0.y;

	// 法線をローカル空間からワールド空間へ変換
	//ret.normal = VSInput.norm;
	ret.normal = normalize(mul(VSInput.norm, (float3x3)g_base.localWorldMatrix));

	// オリジナルシェーダ用にビュー空間に更に変換
	ret.normal = normalize(mul(ret.normal, g_base.viewMatrix));

	// ディフューズカラー
	ret.diffuse = VSInput.diffuse;

	// フォグの強さ(0.0:フォグが濃い、1.0:フォグが薄い)
	//ret.fogFactor =
	//	clamp((g_fog_end - length(lWorldPosition.xyz - g_camera_pos)) / (g_fog_end - g_fog_start), 0.0, 1.0);
	// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了 )

	// 出力パラメータを返す
	return ret;


	//float4 lWorldPosition;	//ワールド座標
	//float4 lViewPosition;	//ビュー座標
	//float3 lWorldNrm;		//ワールドベクトル
	//float3 lViewNrm;		//ビューベクトル

	////頂点座標変換------------------(開始)

	////ローカル座標をワールド座標に変換
	//lWorldPosition.x = dot(VSInput.pos, g_base.localWorldMatrix[0]);
	//lWorldPosition.y = dot(VSInput.pos, g_base.localWorldMatrix[1]);
	//lWorldPosition.z = dot(VSInput.pos, g_base.localWorldMatrix[2]);
	//lWorldPosition.w = 1.0f;

	////ワールド座標をビュー座標に変換
	//lViewPosition.x = dot(lWorldPosition, g_base.viewMatrix[0]);
	//lViewPosition.y = dot(lWorldPosition, g_base.viewMatrix[1]);
	//lViewPosition.z = dot(lWorldPosition, g_base.viewMatrix[2]);
	//lViewPosition.w = 1.0f;

	////ビュー座標を射影座標に変換
 //   ret.svPos.x = dot(lViewPosition, g_base.projectionMatrix[0]);
 //   ret.svPos.y = dot(lViewPosition, g_base.projectionMatrix[1]);
 //   ret.svPos.z = dot(lViewPosition, g_base.projectionMatrix[2]);
 //   ret.svPos.w = dot(lViewPosition, g_base.projectionMatrix[3]);

	////頂点座標変換------------------(終了)

	////法線をビュー空間の角度に変換------(開始)
	//
	////ローカルベクトルをワールドベクトルに変換
 //   lWorldNrm.x = dot(VSInput.norm, g_base.localWorldMatrix[0].xyz);
 //   lWorldNrm.y = dot(VSInput.norm, g_base.localWorldMatrix[1].xyz);
 //   lWorldNrm.z = dot(VSInput.norm, g_base.localWorldMatrix[2].xyz);
	//
	////ワールドベクトルをビューベクトルに変換
 //   lViewNrm.x = dot(lWorldNrm, g_base.viewMatrix[0].xyz);
 //   lViewNrm.y = dot(lWorldNrm, g_base.viewMatrix[1].xyz);
 //   lViewNrm.z = dot(lWorldNrm, g_base.viewMatrix[2].xyz);
	//
	////法線をビューベクトルに変換()------(終了)
	//
	////出力パラメーターセット--------(開始)
	//
	////頂点座標を保存
 //   ret.vwPos = lViewPosition.xyz;
	//
	////法線を保存
 //   ret.normal = lViewNrm;
	//
	////テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
 //   ret.uv.x = dot(VSInput.uv0, g_otherMatrix.textureMatrix[0][0]);
 //   ret.uv.y = dot(VSInput.uv0, g_otherMatrix.textureMatrix[0][1]);
	//
	////出力パラメータをセット--------(終了)
	//
	////出力パラメータを返す
	//return ret;
}