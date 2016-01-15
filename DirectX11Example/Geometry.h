
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

struct Mesh
{
	std::vector<SimpleVertex> Vertices;
	std::vector<unsigned int> Indices;

	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;

	HRESULT Load(ID3D11Device * Device);
	void Set(ID3D11DeviceContext * ImmediateContext);
	void Release();
};

struct GeometryCreator
{
	static Mesh MakeCube();
	static Mesh MakeSphere(float const Radius, int const Tesselation);
};
