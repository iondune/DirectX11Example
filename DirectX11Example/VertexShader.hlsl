
#include "Header.hlsli"


PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT) 0;
	output.Pos = mul(float4(input.Pos, 1), World);
	output.WorldPosition = output.Pos.xyz;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Norm = mul(float4(input.Norm, 1), World).xyz;

	return output;
}
