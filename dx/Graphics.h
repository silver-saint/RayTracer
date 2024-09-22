#pragma once
#include "../platform/WinInstance.h"
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12/d3dx12.h>
#include <array>
#include <../types.h>


class Graphics
{
public:
#ifdef NDEBUG
	const bool DEBUGLAYER = false;
#else
	const bool DEBUGLAYER = true;
#endif
	Graphics(HWND hWnd, ui32 width, ui32 height);
	Graphics(const Graphics& rhs) = delete;
	Graphics& operator=(const Graphics& rhs) = delete;
	void Draw(HWND hWnd, ui32 width, ui32 height);
private:
	static const i32 s_swapChainBufferCount = 2;
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
	std::array<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, 2> m_CommandAllocators;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, s_swapChainBufferCount> m_swapChainBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
	i32 m_currBackBuffer = 0;
	ui64 m_CurrentFence = 0;
	ui32 m_rtvDescriptorSize = 0;
	ui32 m_dsvDescriptorSize = 0;
	ui32 m_cbvSrvUavDescriptorSize = 0;
	D3D_DRIVER_TYPE m_d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	D3D12_VIEWPORT m_screenViewport;
	D3D12_RECT m_scissorRect;
private:
	void CreateTheDebugLayer();
	void CreateTheDXGIFactory();
	void CreateTheD3D12Device();
	void CreateTheCommandQueue();
	void CreateTheSwapChain(HWND hWnd, ui32 width, ui32 height);
	void CreateTheRenderTargetView();
	void CreateTheFrameResource();
	void CreateACommandAllocator();
	void CreateACommandList();
	void CreateTheFenceValue();
	void RenderLoop();
	void ClearRenderTargetView();
	void ClearBuffer();
	void PresentBuffer();
	void SubmitCommandQueue();
	void PresentFrame();
};