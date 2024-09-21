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
	static constexpr UINT s_bufferCount = 2;
	Microsoft::WRL::ComPtr<IDXGIFactory7> m_dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device10> m_device;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_queue;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvdescHeap;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_gCommandList;
	Microsoft::WRL::ComPtr<ID3D12Fence1> m_fence;
	Microsoft::WRL::ComPtr<ID3D12Resource2> m_currentBackBuffer;
	Microsoft::WRL::ComPtr< ID3D12Debug6> m_debugController;
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource2>, s_bufferCount> m_backBuffers;
	
	ui32 m_fenceValue = 0;
	ui32 m_currentBackBufferIdx = 0;
	ui32 m_rtvDescriptorSize = 0;
	HANDLE m_fenceEvent = nullptr;
	i32 width;
	i32 height;
private:
	void CreateTheDebugLayer();
	void CreateTheDXGIFactory();
	void CreateTheD3D12Device();
	void CreateTheCommandQueue();
	void CreateTheSwapChain(HWND hWnd, ui32 width, ui32 height);
	void CreateTheRenderTargetView();
};