
#include "Header.hlsli"


float4 main(PS_INPUT Input) : SV_Target
{
	float4 FinalColor = 0;
	float3 Normal = normalize(Input.Norm);

	const float3 View = normalize(Eye - Input.WorldPosition);
	const float3 Light = normalize(LightDir.xyz - Input.WorldPosition);
	const float3 Half = normalize(Light + View);
	const float3 Reflection = -normalize(Light - 2 * dot(Light, Normal) * Normal);

	float3 DiffuseComponent = saturate(dot(Light, Normal));
	float3 SpecularComponent = pow(saturate(dot(Half, Normal)), Specularity);

	FinalColor += float4(DiffuseComponent + SpecularComponent, 1) * LightColor;

	FinalColor.a = 1;
	return FinalColor;
}
