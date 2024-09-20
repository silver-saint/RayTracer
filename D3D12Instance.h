#pragma once
#include "platform/WinInstance.h"
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12/d3dx12.h>
#include <array>
#include <../types.h>
class D3D12Instance
{
public:
	D3D12Instance() = default;
	D3D12Instance(HINSTANCE hInstance);
	D3D12Instance(const D3D12Instance& rhs) = delete;
	D3D12Instance& operator=(const D3D12Instance& rhs) = delete;
	~D3D12Instance();
private:
	static constexpr UINT bufferCount = 2;
	Microsoft::WRL::ComPtr<IDXGIFactory7> m_dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device14> m_device;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_queue;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvdescHeap;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_gCommandList;
	Microsoft::WRL::ComPtr<ID3D12Fence1> m_fence;
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource2>, bufferCount> backBuffers;
	ui64 m_fenceValue = 0;

private:
	void CreateDXGIFactory();
	void CreateD3D12Device();
	void CreateCommandQueue();
	void CreateSwapChain();
	void CreateDescriptorHeap();
	void CreateCommandAllocator();
	void CreateGraphicsCommandList();
	void CreateFence();
};

