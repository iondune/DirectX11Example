
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>


struct SimpleVertex
{
    DirectX::XMFLOAT3 Pos;
    DirectX::XMFLOAT3 Normal;

	SimpleVertex(DirectX::XMFLOAT3 const & Pos, DirectX::XMFLOAT3 const & Normal)
	{
		this->Pos = Pos;
		this->Normal = Normal;
	}
};

struct GeometryCreator
{
	static void MakeCube(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices);
	static void MakeSphere(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices, float const Radius, int const Tesselation);
};
