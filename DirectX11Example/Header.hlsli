
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 Eye;

	float4 LightPosition;
	float4 LightColor;

	float Specularity;
}

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Norm : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float3 WorldPosition : TEXCOORD1;
};
