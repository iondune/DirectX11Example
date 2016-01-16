
#pragma once

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

struct Geometry
{
	static void MakeCube(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices);
	static void MakeSphere(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices, float const Radius, int const Tesselation);
	static void LoadOBJ(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices, std::string const & FileName);

	static void CalculateNormals(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices);
};
