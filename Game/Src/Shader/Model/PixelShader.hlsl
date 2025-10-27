//VS/PS����
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
	
	//�e�N�X�`���̐F���擾
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

	//�@���̏���
	Normal = normalize(PSInput.normal);

	//���_���W���王�_�ւ̃x�N�g���𐳋K��
	V_to_Eye = normalize(-PSInput.vwPos);

	//�f�B�t���[�Y�J���[�ƃX�y�L�����[�J���[�̒~�ϒl��������
	TotalDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	TotalSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//�X�|�b�g���C�g�̏���+++++++++++++++++++++++++++++++++(�J�n)

	//���C�g�����x�N�g���̌v�Z
	lLightDir = normalize(PSInput.vwPos.xyz - lightPos.xyz);

	//return float4(abs(lLightDir.x), abs(lLightDir.y), abs(lLightDir.z), 1.0f);
	//return float4(lLightDir.xyz, 1.0f);

	//�����E�X�|�b�g���C�g�����l�v�Z-------------(�J�n)

	//���������v�Z------------------

	//���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
	lLightTemp = PSInput.vwPos.xyz - lightPos.xyz;
	lLightDistancePow2 = dot(lLightTemp, lLightTemp);

	//�������̌v�Z�@lLightGen = 1 / �i�����l0 + �����l1 * ( ���� * ����) �j
	lLightGen = 1.0f / (light.attenuation0 + light.attenuation1 * sqrt(lLightDistancePow2) + light.attenuation2 * lLightDistancePow2);

	//return float4(lLightGen,lLightGen,lLightGen,1.0f);
	//------------------------------

	//�X�|�b�g���C�g�����v�Z--------

	//���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���(���� Cos a)���v�Z
	lLightDirectionCosA = dot(lLightDir, light.direction);

	//�X�|�b�g���C�g�����v�Z�@pow( falloff, ((Cos a - Cos f) / (Cos q - Cos f)))
	lLightGen *= saturate(pow(abs(max(lLightDirectionCosA - light.spotParam0, 0.0f) * light.spotParam1), light.fallOff));


	//return float4(lLightGen,lLightGen,lLightGen,1.0f);
	
	//------------------------------

	//�L�������O�������猸�������ő�ɂ��鏈��
	lLightGen *= step(lLightDistancePow2, light.rangePow2);


	//return float4(lLightGen,lLightGen,lLightGen,1.0f);

	//�����E�X�|�b�g���C�g�����v�Z----------------(�I��)

	//�f�B�t���[�Y�F�v�Z

	//�f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate(dot(Normal, -lLightDir));
	//DiffuseAngleGen = abs(dot(Normal, -lLightDir));

	//�f�B�t���[�Y�J���[�~�ϒl += �i���C�g�̃f�B�t���[�Y�J���[ * �}�e���A���f�B�t���[�Y�J���[ * �f�B�t���[�Y�p�x������+���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� * �����E�X�|�b�g���C�g�̊p�x�������j
	//TotalDiffuse += (light.diffuse * material.diffuse * DiffuseAngleGen + light.ambient) * lLightGen;
	TotalDiffuse += ((float4(light.diffuse.xyz, 1.0f) * material.diffuse * DiffuseAngleGen) + light.ambient) * lLightGen;


	//�X�y�L�����[�J���[�v�Z

	//�n�[�t�x�N�g���̌v�Z
	TempF3 = normalize(V_to_Eye - lLightDir);

	
	// Temp = pow(max(0.0f,N*H),material.power.x)
	Temp = pow(max(0.0f, dot(Normal, TempF3)), material.power.x);

	//�X�y�L�����[�J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����[�J���[
	TotalSpecular += Temp * lLightGen.x * (float4(light.specular.xyz, 1.0f));

	//�X�|�b�g���C�g�̏���+++++++++++++++++++++++++++++++++(�I��)

	//�o�̓J���[�v�Z++++++++++++++++++++++++++++++++++++++++++++++++(�J�n)

	//TotalDiffuse = ���C�g�f�B�t�[�Y�J���[�~�ϒl + (�}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z��������)
	TotalDiffuse += material.ambientEmissive;

	// SpecularColor = ���C�g�̃X�y�L�����[�J���[�~�ϒl * �}�e���A���̃X�y�L�����[�J���[
	SpecularDiffuseColor = TotalSpecular * material.specular;

	// �o�̓J���[�v�Z = TotalDiffuse * �e�N�X�`���J���[ + SpecularColor
	float4 retColor;
	//TextureDiffuseColor = tex2D(diffuseMapSampler, PSInput.uv.xy);
	//TextureDiffuseColor = float4(0.9f, 0.9f, 0.9f, 1.0f);

	TextureDiffuseColor = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv.xy);

	retColor.rgb = TextureDiffuseColor.rgb * TotalDiffuse.rgb + SpecularDiffuseColor.rgb;

	//�A���t�@�l = �e�N�X�`���A���t�@ * �}�e���A���̃f�B�t���[�Y�A���t�@ * �s�����x
	retColor.a = TextureDiffuseColor.a * material.diffuse.a * g_base.factorColor.a;

	//�o�̓J���[�v�Z++++++++++++++++++++++++++++++++++++++++++++++++(�I��)

	//�o�̓p�����[�^��Ԃ�
	return retColor;
}