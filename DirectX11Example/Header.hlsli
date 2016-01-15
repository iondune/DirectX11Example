
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 LightDir;
	float4 LightColor;
	float4 OutputColor;
	float3 Eye;
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
