#pragma once

#include <cstdint>
#include <Windows.h>

typedef HRESULT(__stdcall* TPresent)(struct IDXGISwapChain*, UINT, UINT);

class Renderer
{
public:
    static void Initialise(struct IDXGISwapChain* Sc);
    static void Present();
public:
    static TPresent OriginalPresentAddress;

    static struct ID3D11Device* Device;
    static struct ID3D11DeviceContext* Context;
    static struct ID3D11RenderTargetView* RenderTargetView;
    static struct IDXGISwapChain* Swapchain;
    static HWND RenderingWindow;

    static constexpr int16_t Dx11PresentIndex = 8;
};