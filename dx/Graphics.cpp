#include "Graphics.h"

Graphics::Graphics(HWND hWnd, ui32 width, ui32 height)
{
	Draw(hWnd, width, height);
}

void Graphics::Draw(HWND hWnd, ui32 width, ui32 height)
{
	if (DEBUGLAYER)
	{
		CreateTheDebugLayer();
	}
	CreateTheDXGIFactory();
	CreateTheD3D12Device();
	CreateTheCommandQueue();
	CreateTheSwapChain(hWnd, width, height);
	CreateTheRenderTargetView();
}

void Graphics::CreateTheDebugLayer()
{
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController))))
	{
		m_debugController->EnableDebugLayer();
	}
}

void Graphics::CreateTheDXGIFactory()
{
	CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_dxgiFactory));

}

void Graphics::CreateTheD3D12Device()
{
	HRESULT deviceCreated = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&m_device));

	if (FAILED(deviceCreated))
	{
		MessageBox(nullptr, L"Initialization of device failed", L"Error", MB_OK);
		return;
	}
}

void Graphics::CreateTheCommandQueue()
{
	const D3D12_COMMAND_QUEUE_DESC queueDesc = {
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
		.NodeMask = 0
	};
	HRESULT cmdQueueCreated = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_queue));
	if (FAILED(cmdQueueCreated))
	{
		MessageBox(nullptr, L"Initialization of cmd queue failed", L"Error", MB_OK);
		return;
	}
}

void Graphics::CreateTheSwapChain(HWND hWnd, ui32 width, ui32 height)
{
	const DXGI_SWAP_CHAIN_DESC1 swapChainDesc =
	{
		.Width = width,
		.Height = height,
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.Stereo = FALSE,
		.SampleDesc =
		{
			.Count = 1,
			.Quality = 0
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = s_bufferCount,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = 0,
	};
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
	HRESULT SwapChainCreated = m_dxgiFactory->CreateSwapChainForHwnd(m_queue.Get(), hWnd, &swapChainDesc, nullptr, nullptr, &swapChain1);
	if (FAILED(SwapChainCreated))
	{
		MessageBox(nullptr, L"Initialization of SwapChain failed", L"Error", MB_OK);
		return;
	}
	swapChain1.As(&m_swapChain);

}

void Graphics::CreateTheRenderTargetView()
{
	const D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeap =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		.NumDescriptors = s_bufferCount,
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		.NodeMask = 0,
	};
	HRESULT rtvHeapCreated = m_device->CreateDescriptorHeap(&rtvDescHeap, IID_PPV_ARGS(&m_rtvdescHeap));
	if (FAILED(rtvHeapCreated))
	{
		MessageBox(nullptr, L"Initialization of RTV heap failed", L"Error", MB_OK);
		return;
	}
}
