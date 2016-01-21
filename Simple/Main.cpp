
#include <d3d11.h>
#include <dxgi.h>
#include <DWUT.h>
#include <DirectXColors.h>
#include <D3Dcompiler.h>

#include <cassert>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>


static int const WindowSizeX = 1024;
static int const WindowSizeY = 768;

int main()
{
	// Create Window

	dwutInit();
	dwutCreateWindow(WindowSizeX, WindowSizeY);

	// Create Device

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
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

	// Create Render Target View

	ID3D11Texture2D * BackBuffer = nullptr;
	ID3D11RenderTargetView * RenderTargetView = nullptr;
	assert(S_OK == SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackBuffer)));
	assert(S_OK == Device->CreateRenderTargetView(BackBuffer, nullptr, &RenderTargetView));
	BackBuffer->Release();
	
	// Viewport

	D3D11_VIEWPORT Viewport = {};
	Viewport.Width = (FLOAT) WindowSizeX;
	Viewport.Height = (FLOAT) WindowSizeY;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	ImmediateContext->RSSetViewports(1, &Viewport);

	// Shaders

	ID3D11VertexShader * VertexShader = nullptr;
	ID3D11InputLayout * VertexLayout = nullptr;
	ID3DBlob * ShaderBlob = nullptr;
	D3DReadFileToBlob(L"VertexShader.cso", &ShaderBlob);
	D3D11_INPUT_ELEMENT_DESC Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT NumElements = ARRAYSIZE(Layout);
	assert(S_OK == Device->CreateVertexShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), nullptr, &VertexShader));
	assert(S_OK == Device->CreateInputLayout(Layout, NumElements, ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), &VertexLayout));

	ID3D11PixelShader * PixelShader = nullptr;
	D3DReadFileToBlob(L"PixelShader.cso", &ShaderBlob);
	assert(S_OK == Device->CreatePixelShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), nullptr, &PixelShader));
	ShaderBlob->Release();
	
	// Mesh

	std::vector<float> const Vertices = {
		0.0f,  0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};

	std::vector<uint32_t> const Indices = {
		0, 1, 2
	};

	ID3D11Buffer * VertexBuffer = nullptr;
	D3D11_BUFFER_DESC VertexBufferDesc = {};
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(float) * Vertices.size();
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = Vertices.data();
	assert(S_OK == Device->CreateBuffer(&VertexBufferDesc, &InitData, &VertexBuffer));

	ID3D11Buffer * IndexBuffer = nullptr;
	D3D11_BUFFER_DESC IndexBufferDesc = {};
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(uint32_t) * Indices.size();
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	InitData.pSysMem = Indices.data();
	assert(S_OK == Device->CreateBuffer(&IndexBufferDesc, &InitData, &IndexBuffer));

	// Main Loop

	std::function<void()> Render = [&]
	{
		ImmediateContext->ClearRenderTargetView(RenderTargetView, DirectX::Colors::MidnightBlue);
		ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);

		UINT Stride = sizeof(float);
		UINT Offset = 0;
		ImmediateContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
		ImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		ImmediateContext->IASetInputLayout(VertexLayout);
		ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ImmediateContext->VSSetShader(VertexShader, nullptr, 0);
		ImmediateContext->PSSetShader(PixelShader, nullptr, 0);
		ImmediateContext->DrawIndexed(Indices.size(), 0, 0);

		SwapChain->Present(0, 0);
	};

	std::function<void()> Cleanup = [&]
	{
		VertexBuffer->Release();
		IndexBuffer->Release();

		VertexLayout->Release();

		VertexShader->Release();
		PixelShader->Release();

		RenderTargetView->Release();
		ImmediateContext->Release();
		SwapChain->Release();

		DebugDevice->Release();
		Device->Release();
	};

	dwutRenderCallback(Render);
	dwutCleanupCallback(Cleanup);
	dwutMainLoop();

	return 0;
}
