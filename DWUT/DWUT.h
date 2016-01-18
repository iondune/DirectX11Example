
#pragma once

#include <Windows.h>
#include <functional>


void dwutInit();
void dwutCreateWindow(int const Width, int const Height);
HWND dwutGetWindowHandle();

void dwutRenderCallback(std::function<void()> Render);
void dwutCleanupCallback(std::function<void()> Cleanup);
void dwutMainLoop();
