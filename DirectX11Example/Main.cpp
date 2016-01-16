
#include "DWUT.h"
#include <d3d11.h>
#include <dxgi.h>
#include <cassert>

#include <string>
#include <fstream>
#include <iostream>

#include "Geometry.h"

#include <DirectXColors.h>


static int const WindowSizeX = 1024;
static int const WindowSizeY = 768;


struct ShaderBlob
{
	char * Data;
	size_t Length;

	size_t GetBufferSize()
	{
		return Length;
	}

	char * GetBufferPointer()
	{
		return Data;
	}

	void Release()
	{
		delete Data;
	}
};


char *ReadFileAsByteArray(std::string const & FileName, size_t * OutSize)
{
	std::ifstream FileHandle(FileName, std::ios::binary);

	if (! FileHandle.is_open())
	{
		std::cerr << "Failed to open file " << FileName << std::endl;
		return nullptr;
	}
	char *ByteArray = nullptr;
	size_t FileSize = 0;

	FileHandle.seekg(0, std::ios::end);
	ByteArray = new char[FileSize = (size_t) FileHandle.tellg()];
	if (OutSize)
		*OutSize = FileSize;
	FileHandle.seekg(0, std::ios::beg);

	FileHandle.read(ByteArray, FileSize);
	return ByteArray;
}


ShaderBlob *Helper_LoadShaderBlob(std::string const & FileName)
{
	ShaderBlob * Blob = new ShaderBlob();
	Blob->Data = ReadFileAsByteArray(FileName, & Blob->Length);
	return Blob;
}


struct SConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
	DirectX::XMFLOAT4 Eye;

	DirectX::XMFLOAT4 LightPosition;
	DirectX::XMFLOAT4 LightColor;

	float Specularity;
};

int main()
{
	// Create Window

	dwutInit();
	dwutCreateWindow(WindowSizeX, WindowSizeY);

	// Create Device

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferDesc.Width = WindowSizeX;
	SwapChainDesc.BufferDesc.Height = WindowSizeY;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = dwutGetWindowHandle();
	SwapChainDesc.Windowed = true;

	IDXGISwapChain * SwapChain = nullptr;
	ID3D11Device * Device = nullptr;
	ID3D11DeviceContext * ImmediateContext = nullptr;

	UINT CreateDeviceFlags = 0;
#ifdef _DEBUG
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE,
		NULL, CreateDeviceFlags,
		NULL, 0,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&SwapChain,
		&Device,
		NULL,
		&ImmediateContext);

	ID3D11Debug * DebugDevice = nullptr;
	Device->QueryInterface(IID_PPV_ARGS(&DebugDevice));

	// Create Render Target

	ID3D11Texture2D * BackBuffer = nullptr;
	ID3D11RenderTargetView * RenderTargetView = nullptr;
	assert(S_OK == SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackBuffer)));
	assert(S_OK == Device->CreateRenderTargetView(BackBuffer, nullptr, &RenderTargetView));
	BackBuffer->Release();

	// Create Depth Stencil Texture

	ID3D11Texture2D * DepthStencilTexture = nullptr;
	D3D11_TEXTURE2D_DESC DepthDesc;
	ZeroMemory(&DepthDesc, sizeof(DepthDesc));
	DepthDesc.Width = WindowSizeX;
	DepthDesc.Height = WindowSizeY;
	DepthDesc.MipLevels = 1;
	DepthDesc.ArraySize = 1;
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.CPUAccessFlags = 0;
	DepthDesc.MiscFlags = 0;
	assert(S_OK == Device->CreateTexture2D(&DepthDesc, nullptr, &DepthStencilTexture));

	ID3D11DepthStencilView * DepthStencilView = nullptr;
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(DSVDesc));
	DSVDesc.Format = DepthDesc.Format;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;
	assert(S_OK == Device->CreateDepthStencilView(DepthStencilTexture, &DSVDesc, &DepthStencilView));
	DepthStencilTexture->Release();

	// Viewport

	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT) WindowSizeX;
	Viewport.Height = (FLOAT) WindowSizeY;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	ImmediateContext->RSSetViewports(1, &Viewport);

	// Shader

	ID3D11VertexShader * VertexShader = nullptr;
	ID3D11InputLayout * VertexLayout = nullptr;
	ShaderBlob* VSBlob = Helper_LoadShaderBlob("VertexShader.cso");
	D3D11_INPUT_ELEMENT_DESC Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT NumElements = ARRAYSIZE(Layout);
	assert(S_OK == Device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &VertexShader));
	assert(S_OK == Device->CreateInputLayout(Layout, NumElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &VertexLayout));
	VSBlob->Release();

	ID3D11PixelShader * PixelShader = nullptr;
	ShaderBlob* pPSBlob = Helper_LoadShaderBlob("PixelShader.cso");
	assert(S_OK == Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &PixelShader));
	pPSBlob->Release();

	// Constant Buffer

	ID3D11Buffer * ConstantBuffer = nullptr;

	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth = sizeof(SConstantBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	assert(S_OK == Device->CreateBuffer(&BufferDesc, nullptr, &ConstantBuffer));

	DirectX::XMMATRIX const WorldMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX const ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, WindowSizeX / (FLOAT) WindowSizeY, 0.01f, 100.0f);

	// Mesh

	std::vector<SimpleVertex> Vertices;
	std::vector<uint32_t> Indices;
	ID3D11Buffer * VertexBuffer = nullptr;
	ID3D11Buffer * IndexBuffer = nullptr;

	//Geometry::MakeSphere(Vertices, Indices, 1.f, 128);
	Geometry::LoadOBJ(Vertices, Indices, "bunny.obj");

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = Vertices.data();
	assert(S_OK == Device->CreateBuffer(&bd, &InitData, &VertexBuffer));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint32_t) * Indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = Indices.data();
	assert(S_OK == Device->CreateBuffer(&bd, &InitData, &IndexBuffer));
	
	// Main Loop

	float Time = 0;

	std::function<void()> Render = [&]
	{
		Time += 0.001f;

		ImmediateContext->ClearRenderTargetView(RenderTargetView, DirectX::Colors::MidnightBlue);
		ImmediateContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

		UINT Stride = sizeof(SimpleVertex);
		UINT Offset = 0;
		ImmediateContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
		ImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		ImmediateContext->IASetInputLayout(VertexLayout);
		ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DirectX::XMVECTOR const Eye = DirectX::XMVectorSet(0.0f, 0.1f, 0.3f, 0.0f);
		DirectX::XMVECTOR const At = DirectX::XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
		DirectX::XMVECTOR const Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMMATRIX const ViewMatrix = DirectX::XMMatrixLookAtLH(Eye, At, Up);

		float const Specularity = 80;
		float const Roughness = 0.5f;

		SConstantBuffer cbData;
		cbData.World = XMMatrixTranspose(WorldMatrix);
		cbData.View = XMMatrixTranspose(ViewMatrix);
		cbData.Projection = XMMatrixTranspose(ProjectionMatrix);
		cbData.Eye = DirectX::XMFLOAT4(Eye.m128_f32[0], Eye.m128_f32[1], Eye.m128_f32[2], 1);

		cbData.LightPosition = DirectX::XMFLOAT4(3, -15, 3, 1);
		cbData.LightColor = DirectX::XMFLOAT4(1, 1, 1, 1);

		cbData.Specularity = Specularity;
		ImmediateContext->UpdateSubresource(ConstantBuffer, 0, nullptr, &cbData, 0, 0);

		ImmediateContext->VSSetShader(VertexShader, nullptr, 0);
		ImmediateContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
		ImmediateContext->PSSetShader(PixelShader, nullptr, 0);
		ImmediateContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);
		ImmediateContext->DrawIndexed(Indices.size(), 0, 0);

		ImmediateContext->IASetInputLayout(nullptr);
		ImmediateContext->PSSetShader(nullptr, nullptr, 0);
		ImmediateContext->VSSetShader(nullptr, nullptr, 0);

		SwapChain->Present(0, 0);
	};
	
	std::function<void()> Cleanup = [&]
	{
		VertexBuffer->Release();
		IndexBuffer->Release();
		ConstantBuffer->Release();

		VertexLayout->Release();

		VertexShader->Release();
		PixelShader->Release();

		RenderTargetView->Release();
		DepthStencilView->Release();

		ImmediateContext->Release();
		SwapChain->Release();

		//DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		DebugDevice->Release();
		Device->Release();
	};

	dwutRenderCallback(Render);
	dwutCleanupCallback(Cleanup);
	dwutMainLoop();

	return 0;
}
