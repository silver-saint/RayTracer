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
	CreateTheFrameResource();
	CreateACommandAllocator();
	CreateACommandList();
	CreateTheFenceValue();
	RenderLoop();
	ClearRenderTargetView();
	ClearBuffer();
	PresentBuffer();
	SubmitCommandQueue();
	PresentFrame();
}

void Graphics::CreateTheDebugLayer()
{
	if (DEBUGLAYER)
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
		else
		{
			MessageBox(nullptr, L"Couldn't init DebugLayer", L"Error", MB_OK);
			return;
		}
	}
}

void Graphics::CreateTheDXGIFactory()
{
	HRESULT initDXGIFactory = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
	if (FAILED(initDXGIFactory))
	{
		MessageBox(nullptr, L"Couldn't init Factory", L"Error", MB_OK);
		return;

	}

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
	m_swapChain.Reset();
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
	HRESULT rtvHeapSizeAcquired = m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	if (FAILED(rtvHeapSizeAcquired))
	{
		MessageBox(nullptr, L"Acquiring of RTV heap size failed", L"Error", MB_OK);
		return;
	}
}

void Graphics::CreateTheFrameResource()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvdescHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT buffer = 0; buffer < m_backBuffers.size(); buffer++)
	{
		HRESULT GetCurrentBuffer = m_swapChain->GetBuffer(buffer, IID_PPV_ARGS(&m_backBuffers[buffer]));
		if (FAILED(GetCurrentBuffer))
		{
			MessageBox(nullptr, L"Couldn't acquire buffer.", L"Error", MB_OK);
			return;
		}
		m_device->CreateRenderTargetView(m_backBuffers[buffer].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(m_rtvDescriptorSize);
	}
}

void Graphics::CreateACommandAllocator()
{
	HRESULT cmdAllocCreated = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	if (FAILED(cmdAllocCreated))
	{
		MessageBox(nullptr, L"Initialization of cmd allocator failed", L"Error", MB_OK);
		return;
	}
}

void Graphics::CreateACommandList()
{
	HRESULT cmdList = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_gCommandList));
	if (FAILED(cmdList))
	{
		MessageBox(nullptr, L"Initialization of cmdlist failed", L"Error", MB_OK);
		return;
	}
	m_gCommandList->Close();
}

void Graphics::CreateTheFenceValue()
{
	m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)); 

	//fence signaling
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (!m_fenceEvent)
	{
		MessageBox(nullptr, L"Failed to create fence event", L"Error", MB_OK);
		return;
	}
}

void Graphics::RenderLoop()
{
	m_currentBackBufferIdx = m_swapChain->GetCurrentBackBufferIndex();
	m_currentBackBuffer = m_backBuffers[m_currentBackBufferIdx];
	m_commandAllocator->Reset();
	m_gCommandList->Reset(m_commandAllocator.Get(), nullptr);
}

void Graphics::ClearRenderTargetView()
{
	const auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_currentBackBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_gCommandList->ResourceBarrier(1, &barrier);
}

void Graphics::ClearBuffer()
{
	std::array<f32, 4> clearColor = { 0.0f, 0.5f, 0.0f, 1.0f };
	const CD3DX12_CPU_DESCRIPTOR_HANDLE rtv{ m_rtvdescHeap->GetCPUDescriptorHandleForHeapStart(), INT(m_currentBackBufferIdx), m_rtvDescriptorSize };
	m_gCommandList->ClearRenderTargetView(rtv, clearColor.data(), 0, nullptr);
}

void Graphics::PresentBuffer()
{
	const auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_currentBackBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_gCommandList->ResourceBarrier(1, &barrier);
}

void Graphics::SubmitCommandQueue()
{
	m_gCommandList->Close();
	std::array<ID3D12CommandList* const, 1> commandLists = { m_gCommandList.Get() };
	m_queue->ExecuteCommandLists(commandLists.size(), commandLists.data());
	m_queue->Signal(m_fence.Get(), m_fenceValue++);
}

void Graphics::PresentFrame()
{
	m_swapChain->Present(0, 0);
	m_fence->SetEventOnCompletion(m_fenceValue - 1, m_fenceEvent);
	if (::WaitForSingleObject(m_fenceEvent, INFINITE) == WAIT_FAILED)
	{
		MessageBox(nullptr, L"Failed to wait for event", L"Error", MB_OK);
		return;
	}
}
