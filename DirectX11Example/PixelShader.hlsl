
#include "Header.hlsli"


float4 main(PS_INPUT Input) : SV_Target
{
	const float3 Normal = normalize(Input.Norm);

	const float3 View = normalize(Eye.xyz - Input.WorldPosition);
	const float3 Light = normalize(LightPosition.xyz - Input.WorldPosition);
	const float3 Half = normalize(Light + View);
	const float3 Reflection = -normalize(Light - 2 * dot(Light, Normal) * Normal);

	const float DiffuseComponent = saturate(dot(Light, Normal));
	const float SpecularComponent = pow(saturate(dot(Half, Normal)), Specularity);

	float4 FinalColor = 0;
	float3 ResultColor = (DiffuseComponent + SpecularComponent) * LightColor.rgb;
	FinalColor = float4(ResultColor.r, ResultColor.g, ResultColor.b, 1);
	return FinalColor;
}
