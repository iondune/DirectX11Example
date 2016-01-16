
#include "Geometry.h"


DirectX::XMFLOAT3 operator *(DirectX::XMFLOAT3 const & a, float const b)
{
	return DirectX::XMFLOAT3(a.x * b, a.y * b, a.z * b);
}

void GeometryCreator::MakeCube(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices)
{
	Vertices = std::vector<SimpleVertex>({
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

	Indices = std::vector<unsigned int>({
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
}

void GeometryCreator::MakeSphere(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices, float const Radius, int const Tesselation)
{
	int const HorizontalTesselation = Tesselation * 2;
	for (int i = 0; i < Tesselation; ++ i)
	{
		float const VerticalAngle = i / (Tesselation - 1.f) * DirectX::XM_PI;
		for (int j = 0; j < HorizontalTesselation; ++ j)
		{
			int const Index = j + HorizontalTesselation * i;
			float const HorizontalAngle = j / (HorizontalTesselation - 1.f) * 2.f * DirectX::XM_PI;
			DirectX::XMFLOAT3 const Position = DirectX::XMFLOAT3(cos(HorizontalAngle) * sin(VerticalAngle), cos(VerticalAngle), sin(HorizontalAngle) * sin(VerticalAngle));

			Vertices.push_back(SimpleVertex(Position * Radius, Position));

			if (i < Tesselation - 1 && j < HorizontalTesselation - 1)
			{
				Indices.push_back(Index + 0);
				Indices.push_back(Index + 1);
				Indices.push_back(Index + HorizontalTesselation + 1);

				Indices.push_back(Index + 0);
				Indices.push_back(Index + HorizontalTesselation + 1);
				Indices.push_back(Index + HorizontalTesselation);
			}
		}
	}
}
