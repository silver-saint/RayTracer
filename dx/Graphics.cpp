#include "Graphics.h"

Graphics::Graphics(HWND hWnd)
{
    Draw(hWnd);
}

void Graphics::Draw(HWND hWnd)
{
    if (DEBUGLAYER)
    {
        CreateDebugLayer();
    }
   CreateDXGIFactory();
   CreateD3D12Device();
   CreateCommandQueue();
   CreateSwapChain(hWnd);
   CreateDescriptorHeap();
   CreateCommandAllocator();
   CreateGraphicsCommandList();
   CreateFence();
   RenderLoop();
   ClearRenderTarget();
   SubmitCommandList();
   PresentFrame();
}

void Graphics::CreateDebugLayer()
{
    D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController));
    m_debugController->EnableDebugLayer();
}

void Graphics::CreateDXGIFactory()
{
    CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_dxgiFactory));

	UINT createFactoryFlags = 0;
#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	HRESULT initDXGIFactory = CreateDXGIFactory2(createFactoryFlags,IID_PPV_ARGS(&m_dxgiFactory));
	if (FAILED(initDXGIFactory))
	{
		MessageBox(nullptr, L"Couldn't init Factory", L"Error", MB_OK);
		return;

	}

void Graphics::CreateD3D12Device()
{
    HRESULT deviceCreated = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&m_device));
    
    if (FAILED(deviceCreated))
    {
        MessageBox(nullptr, L"Initialization of device failed", L"Error", MB_OK);
        return;
    }
}

void Graphics::CreateCommandQueue()
{
    const D3D12_COMMAND_QUEUE_DESC queueDesc = {
        .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
        .NodeMask = 0
    };
    m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_queue));
}

void Graphics::CreateSwapChain(HWND hWnd)
{
    const DXGI_SWAP_CHAIN_DESC1 swapChainDesc =
    {
        .Width = 800,
        .Height = 600,
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
        .Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING,
    };
    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;

    m_dxgiFactory->CreateSwapChainForHwnd(m_queue.Get(), hWnd, &swapChainDesc, nullptr, nullptr, &swapChain1);
    swapChain1.As(&m_swapChain);

}

void Graphics::CreateDescriptorHeap()
{
    const D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc =
    {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = s_bufferCount,
    };
    m_device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_rtvdescHeap));
    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvdescHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT currentBuffer = 0; currentBuffer < s_bufferCount; currentBuffer++)
    {
        m_swapChain->GetBuffer(currentBuffer, IID_PPV_ARGS(&m_backBuffers[currentBuffer]));
        m_device->CreateRenderTargetView(m_backBuffers[currentBuffer].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(m_rtvDescriptorSize);
    }
}

void Graphics::CreateCommandAllocator()
{
    m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
}

void Graphics::CreateGraphicsCommandList()
{
    m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_gCommandList));
    //initially have it at closed
    m_gCommandList->Close();
}

void Graphics::CreateFence()
{
    m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!m_fenceEvent)
    {
        MessageBox(nullptr, L"Event couldn't be created", L"Create Event Failed", MB_OK);
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

void Graphics::ClearRenderTarget()
{
    const CD3DX12_RESOURCE_BARRIER renderStateBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_currentBackBuffer.Get(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_gCommandList->ResourceBarrier(1, &renderStateBarrier);
    std::array<f32, 4> clearColor = { 0.4f, 0.4f, 0.4f, 1.0f };
    const CD3DX12_CPU_DESCRIPTOR_HANDLE rtv{ m_rtvdescHeap->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(m_currentBackBufferIdx), m_rtvDescriptorSize };
    m_gCommandList->ClearRenderTargetView(rtv, clearColor.data(), 0, nullptr);
}

void Graphics::SubmitCommandList()
{
    const CD3DX12_RESOURCE_BARRIER presentStateBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_currentBackBuffer.Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_gCommandList->ResourceBarrier(1, &presentStateBarrier);
    m_gCommandList->Close();
    std::array<ID3D12CommandList* const, 1> commandLists = { m_gCommandList.Get() };
    m_queue->ExecuteCommandLists(commandLists.size(), commandLists.data());
}

void Graphics::PresentFrame()
{
    m_queue->Signal(m_fence.Get(), m_fenceValue++);
    m_swapChain->Present(0, 0);
    m_fence->SetEventOnCompletion(m_fenceValue - 1, m_fenceEvent);
    if (::WaitForSingleObject(m_fenceEvent, INFINITE) == WAIT_FAILED)
    {
        MessageBox(nullptr, L"Wait for fence failed", L"Error", MB_OK);
    }
}
