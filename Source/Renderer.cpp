#include "Renderer.h"

#include <d3d11.h>

TPresent Renderer::OriginalPresentAddress = nullptr;
ID3D11Device* Renderer::Device = nullptr;
ID3D11DeviceContext* Renderer::Context = nullptr;
ID3D11RenderTargetView* Renderer::RenderTargetView = nullptr;
IDXGISwapChain* Renderer::Swapchain = nullptr;
HWND Renderer::RenderingWindow = nullptr;

void Renderer::Initialise(IDXGISwapChain* Sc)
{
    Swapchain = Sc;

    DXGI_SWAP_CHAIN_DESC SwapchainDesc = {};
    Swapchain->GetDesc(&SwapchainDesc);
    Swapchain->GetDevice(__uuidof(ID3D11Device), (void**)&Device);
    Device->GetImmediateContext(&Context);

    ID3D11Texture2D* BackBuffer = nullptr;
    Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
    Device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);
    BackBuffer->Release();

    RenderingWindow = SwapchainDesc.OutputWindow;
}

void Renderer::Present()
{
    Context->OMSetRenderTargets(1, &RenderTargetView, nullptr);
}