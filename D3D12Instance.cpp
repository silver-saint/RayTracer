#include "D3D12Instance.h"

D3D12Instance::D3D12Instance(HINSTANCE hInstance)
{


}

D3D12Instance::~D3D12Instance()
{
}

void D3D12Instance::CreateDXGIFactory()
{
    CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_dxgiFactory));
}

void D3D12Instance::CreateD3D12Device()
{
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&m_device));
}

void D3D12Instance::CreateCommandQueue()
{
    const D3D12_COMMAND_QUEUE_DESC queueDesc = {
        .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
        .NodeMask = 0
    };
    m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_queue));
}

void D3D12Instance::CreateSwapChain()
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
        .BufferCount = bufferCount,
        .Scaling = DXGI_SCALING_STRETCH,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
        .Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING,
    };
    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;

    m_dxgiFactory->CreateSwapChainForHwnd(m_queue.Get(), 0, &swapChainDesc, nullptr, nullptr, &swapChain1);
    swapChain1.As(&m_swapChain);

}

void D3D12Instance::CreateDescriptorHeap()
{
    const D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc =
    {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = bufferCount,
    };
    m_device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_rtvdescHeap));
    const auto rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvdescHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT currentBuffer = 0; currentBuffer < bufferCount; currentBuffer++)
    {
        m_swapChain->GetBuffer(currentBuffer, IID_PPV_ARGS(&backBuffers[currentBuffer]));
        m_device->CreateRenderTargetView(backBuffers[currentBuffer].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(rtvDescriptorSize);
    }
}

void D3D12Instance::CreateCommandAllocator()
{
    m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
}

void D3D12Instance::CreateGraphicsCommandList()
{
    m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr , IID_PPV_ARGS(&m_gCommandList));
    //initially have it at closed
    m_gCommandList->Close();
}

void D3D12Instance::CreateFence()
{
    m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    HANDLE fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!fenceEvent)
    {
        MessageBox(nullptr, L"Event couldn't be created", L"Create Event Failed", MB_OK);
        return;
    }


}

