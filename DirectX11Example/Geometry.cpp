
#include "Geometry.h"
#include "tiny_obj_loader.h"
#include <iostream>


DirectX::XMFLOAT3 operator *(DirectX::XMFLOAT3 const & a, float const b)
{
	return DirectX::XMFLOAT3(a.x * b, a.y * b, a.z * b);
}
DirectX::XMFLOAT3 operator -(DirectX::XMFLOAT3 const & a, DirectX::XMFLOAT3 const & b)
{
	return DirectX::XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}

DirectX::XMFLOAT3 CrossProduct(DirectX::XMFLOAT3 const & a, DirectX::XMFLOAT3 const & b)
{
	return DirectX::XMFLOAT3(
		a.y * b.z - b.y * a.z,
		a.z * b.x - b.z * a.x,
		a.x * b.y - b.x * a.y);
}

void Geometry::MakeCube(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices)
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

void Geometry::MakeSphere(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices, float const Radius, int const Tesselation)
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

void Geometry::LoadOBJ(std::vector<SimpleVertex> & Vertices, std::vector<uint32_t> & Indices, std::string const & FileName)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(shapes, materials, err, FileName.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	assert(ret);
	assert(shapes.size() == 1);
	assert((shapes[0].mesh.indices.size() % 3) == 0);
	assert((shapes[0].mesh.positions.size() % 3) == 0);

	for (size_t f = 0; f < shapes[0].mesh.indices.size() / 3; ++ f)
	{
		Indices.push_back(shapes[0].mesh.indices[3 * f + 0]);
		Indices.push_back(shapes[0].mesh.indices[3 * f + 1]);
		Indices.push_back(shapes[0].mesh.indices[3 * f + 2]);
	}

	for (size_t v = 0; v < shapes[0].mesh.positions.size() / 3; ++ v)
	{
		Vertices.push_back(SimpleVertex(DirectX::XMFLOAT3(
			shapes[0].mesh.positions[3 * v + 0],
			shapes[0].mesh.positions[3 * v + 1],
			shapes[0].mesh.positions[3 * v + 2]), DirectX::XMFLOAT3()));
	}

	CalculateNormals(Vertices, Indices);
}

void Geometry::CalculateNormals(std::vector<SimpleVertex>& Vertices, std::vector<uint32_t>& Indices)
{
	assert((Indices.size() % 3) == 0);

	for (size_t f = 0; f < Indices.size() / 3; ++ f)
	{
		DirectX::XMFLOAT3 const A = Vertices[Indices[3 * f + 1]].Pos - Vertices[Indices[3 * f + 0]].Pos;
		DirectX::XMFLOAT3 const B = Vertices[Indices[3 * f + 2]].Pos - Vertices[Indices[3 * f + 0]].Pos;
		DirectX::XMFLOAT3 const Normal = CrossProduct(A, B);

		Vertices[Indices[3 * f + 0]].Normal = Normal;
		Vertices[Indices[3 * f + 1]].Normal = Normal;
		Vertices[Indices[3 * f + 2]].Normal = Normal;
	}
}
