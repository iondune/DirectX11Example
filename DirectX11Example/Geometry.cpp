
#include "Geometry.h"


HRESULT Mesh::Load(ID3D11Device * Device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = Vertices.data();
	hr = Device->CreateBuffer(&bd, &InitData, &VertexBuffer);
	if (FAILED(hr))
		return hr;

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * Indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = Indices.data();
	hr = Device->CreateBuffer(&bd, &InitData, &IndexBuffer);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void Mesh::Set(ID3D11DeviceContext * ImmediateContext)
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	ImmediateContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	ImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void Mesh::Release()
{
	if (VertexBuffer) VertexBuffer->Release();
	if (IndexBuffer) IndexBuffer->Release();
}


Mesh GeometryCreator::MakeCube()
{
	Mesh mesh;

	mesh.Vertices = std::vector<SimpleVertex>({
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),   DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),  DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),   DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),   DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),   DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),   DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),   DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
	});

	mesh.Indices = std::vector<unsigned int>({
		3, 1, 0,
		2, 1, 3,

		6, 4, 5,
		7, 4, 6,

		11, 9, 8,
		10, 9, 11,

		14, 12, 13,
		15, 12, 14,

		19, 17, 16,
		18, 17, 19,

		22, 20, 21,
		23, 20, 22
	});

	return mesh;
}


DirectX::XMFLOAT3 operator *(DirectX::XMFLOAT3 const & a, float const b)
{
	return DirectX::XMFLOAT3(a.x * b, a.y * b, a.z * b);
}

Mesh GeometryCreator::MakeSphere(float const Radius, int const Tesselation)
{
	Mesh mesh;

	int const HorizontalTesselation = Tesselation * 2;
	for (int i = 0; i < Tesselation; ++ i)
	{
		float const VerticalAngle = i / (Tesselation - 1.f) * DirectX::XM_PI;
		for (int j = 0; j < HorizontalTesselation; ++ j)
		{
			int const Index = j + HorizontalTesselation * i;
			float const HorizontalAngle = j / (HorizontalTesselation - 1.f) * 2.f * DirectX::XM_PI;
			DirectX::XMFLOAT3 const Position = DirectX::XMFLOAT3(cos(HorizontalAngle) * sin(VerticalAngle), cos(VerticalAngle), sin(HorizontalAngle) * sin(VerticalAngle));

			mesh.Vertices.push_back(SimpleVertex(Position * Radius, Position));

			if (i < Tesselation - 1 && j < HorizontalTesselation - 1)
			{
				mesh.Indices.push_back(Index + 0);
				mesh.Indices.push_back(Index + 1);
				mesh.Indices.push_back(Index + HorizontalTesselation + 1);

				mesh.Indices.push_back(Index + 0);
				mesh.Indices.push_back(Index + HorizontalTesselation + 1);
				mesh.Indices.push_back(Index + HorizontalTesselation);
			}
		}
	}

	return mesh;
}
