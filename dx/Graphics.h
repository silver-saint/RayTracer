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
	Graphics(HWND hWnd);
	Graphics(const Graphics& rhs) = delete;
	Graphics& operator=(const Graphics& rhs) = delete;
	void Draw(HWND hWnd);
private:
	static const i32 s_swapChainBufferCount = 2;
	Microsoft::WRL::ComPtr<IDXGIAdapter4> m_adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory2> m_dxgiFactory;
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
	HANDLE m_fenceEvent = nullptr;
private:
	void CreateDebugLayer();
	void CreateDXGIFactory();
	void CreateD3D12Device();
	void CreateCommandQueue();
	void CreateSwapChain(HWND hWnd);
	void CreateDescriptorHeap();
	void CreateCommandAllocator();
	void CreateGraphicsCommandList();
	void CreateFence();
	void RenderLoop();
	void ClearRenderTarget();
	void SubmitCommandList();
	void PresentFrame();

};