//VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

//�@IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_1FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret;
	
	// ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �J�n )
	float4 lLocalPosition;
	float4 lWorldPosition;
	float4 lViewPosition;

	float3 lWorldNrm;

	// float3 �� float4
	lLocalPosition.xyz = VSInput.pos;
	lLocalPosition.w = 1.0f;

	// ���[�J�����W�����[���h���W�ɕϊ�(����)
	lWorldPosition.w = 1.0f;
	lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
	//ret.worldPos = lWorldPosition.xyz;

	// ���[���h���W���r���[���W�ɕϊ�
	lViewPosition.w = 1.0f;
	lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
	ret.vwPos = lViewPosition.xyz;

	// �r���[���W���ˉe���W�ɕϊ�
	ret.svPos = mul(lViewPosition, g_base.projectionMatrix);

	// ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �I�� )

	// ���̑��A�s�N�Z���V�F�[�_�ֈ��p&������ ++++++++++++( �J�n )
	// UV���W
	ret.uv.x = VSInput.uv0.x;
	ret.uv.y = VSInput.uv0.y;

	// �@�������[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
	//ret.normal = VSInput.norm;
	ret.normal = normalize(mul(VSInput.norm, (float3x3)g_base.localWorldMatrix));

	// �I���W�i���V�F�[�_�p�Ƀr���[��ԂɍX�ɕϊ�
	ret.normal = normalize(mul(ret.normal, g_base.viewMatrix));

	// �f�B�t���[�Y�J���[
	ret.diffuse = VSInput.diffuse;

	// �t�H�O�̋���(0.0:�t�H�O���Z���A1.0:�t�H�O������)
	//ret.fogFactor =
	//	clamp((g_fog_end - length(lWorldPosition.xyz - g_camera_pos)) / (g_fog_end - g_fog_start), 0.0, 1.0);
	// ���̑��A�s�N�Z���V�F�[�_�ֈ��p&������ ++++++++++++( �I�� )

	// �o�̓p�����[�^��Ԃ�
	return ret;


	//float4 lWorldPosition;	//���[���h���W
	//float4 lViewPosition;	//�r���[���W
	//float3 lWorldNrm;		//���[���h�x�N�g��
	//float3 lViewNrm;		//�r���[�x�N�g��

	////���_���W�ϊ�------------------(�J�n)

	////���[�J�����W�����[���h���W�ɕϊ�
	//lWorldPosition.x = dot(VSInput.pos, g_base.localWorldMatrix[0]);
	//lWorldPosition.y = dot(VSInput.pos, g_base.localWorldMatrix[1]);
	//lWorldPosition.z = dot(VSInput.pos, g_base.localWorldMatrix[2]);
	//lWorldPosition.w = 1.0f;

	////���[���h���W���r���[���W�ɕϊ�
	//lViewPosition.x = dot(lWorldPosition, g_base.viewMatrix[0]);
	//lViewPosition.y = dot(lWorldPosition, g_base.viewMatrix[1]);
	//lViewPosition.z = dot(lWorldPosition, g_base.viewMatrix[2]);
	//lViewPosition.w = 1.0f;

	////�r���[���W���ˉe���W�ɕϊ�
 //   ret.svPos.x = dot(lViewPosition, g_base.projectionMatrix[0]);
 //   ret.svPos.y = dot(lViewPosition, g_base.projectionMatrix[1]);
 //   ret.svPos.z = dot(lViewPosition, g_base.projectionMatrix[2]);
 //   ret.svPos.w = dot(lViewPosition, g_base.projectionMatrix[3]);

	////���_���W�ϊ�------------------(�I��)

	////�@�����r���[��Ԃ̊p�x�ɕϊ�------(�J�n)
	//
	////���[�J���x�N�g�������[���h�x�N�g���ɕϊ�
 //   lWorldNrm.x = dot(VSInput.norm, g_base.localWorldMatrix[0].xyz);
 //   lWorldNrm.y = dot(VSInput.norm, g_base.localWorldMatrix[1].xyz);
 //   lWorldNrm.z = dot(VSInput.norm, g_base.localWorldMatrix[2].xyz);
	//
	////���[���h�x�N�g�����r���[�x�N�g���ɕϊ�
 //   lViewNrm.x = dot(lWorldNrm, g_base.viewMatrix[0].xyz);
 //   lViewNrm.y = dot(lWorldNrm, g_base.viewMatrix[1].xyz);
 //   lViewNrm.z = dot(lWorldNrm, g_base.viewMatrix[2].xyz);
	//
	////�@�����r���[�x�N�g���ɕϊ�()------(�I��)
	//
	////�o�̓p�����[�^�[�Z�b�g--------(�J�n)
	//
	////���_���W��ۑ�
 //   ret.vwPos = lViewPosition.xyz;
	//
	////�@����ۑ�
 //   ret.normal = lViewNrm;
	//
	////�e�N�X�`�����W�ϊ��s��ɂ��ϊ����s�������ʂ̃e�N�X�`�����W���Z�b�g
 //   ret.uv.x = dot(VSInput.uv0, g_otherMatrix.textureMatrix[0][0]);
 //   ret.uv.y = dot(VSInput.uv0, g_otherMatrix.textureMatrix[0][1]);
	//
	////�o�̓p�����[�^���Z�b�g--------(�I��)
	//
	////�o�̓p�����[�^��Ԃ�
	//return ret;
}