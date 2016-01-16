
#include "DWUT.h"
#include <cassert>

static HWND WindowHandle = 0;
static std::function<void()> RenderCallback;
static std::function<void()> CleanupCallback;

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_PAINT:
		if (RenderCallback)
		{
			RenderCallback();
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;

	}

	return 0;
}

void dwutInit()
{
	HINSTANCE Module = GetModuleHandle(0);

	WNDCLASSEX WindowClassDesc = { 0 };
	WindowClassDesc.cbSize = sizeof(WNDCLASSEX);
	WindowClassDesc.style = CS_HREDRAW | CS_VREDRAW;
	WindowClassDesc.lpfnWndProc = WindowProcedure;
	WindowClassDesc.hInstance = Module;
	WindowClassDesc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	WindowClassDesc.lpszClassName = "ionEngineWindowClass";
	assert(RegisterClassEx(&WindowClassDesc));
}

void dwutCreateWindow(int const Width, int const Height)
{
	HINSTANCE Module = GetModuleHandle(0);

	RECT WindowRect = { 0, 0, Width, Height };
	AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, false);

	WindowHandle = CreateWindow("ionEngineWindowClass", "DirectX 11 Example",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		nullptr,
		nullptr,
		Module,
		nullptr);

	ShowWindow(WindowHandle, SW_SHOW);
}

HWND dwutGetWindowHandle()
{
	return WindowHandle;
}

void dwutRenderCallback(std::function<void()> Render)
{
	RenderCallback = Render;
}

void dwutCleanupCallback(std::function<void()> Cleanup)
{
	CleanupCallback = Cleanup;
}

void dwutMainLoop()
{
	MSG Message = {};
	while (WM_QUIT != Message.message)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	if (CleanupCallback)
	{
		CleanupCallback();
	}
}
