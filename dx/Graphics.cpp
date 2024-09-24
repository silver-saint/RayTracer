#include "Graphics.h"

Graphics::Graphics(HWND hWnd, ui32 width, ui32 height)
	: DXContext{width, height}, m_frameIndex {0}, m_rtvDescriptorSize{0},
    m_width{width}, m_height{height}, m_hwnd(hWnd)
{
}
void PrintError(const std::wstring& errorMsg, bool DebugLayerOn)
{
    MessageBox(nullptr, errorMsg.c_str(), L"Error", MB_OK);
    (DebugLayerOn ? DebugBreak() : exit(1));
}
void Graphics::OnInit()
{
	LoadPipeline();
	LoadAssets();
}

void Graphics::OnUpdate()
{
}

void Graphics::OnRender()
{
    PopulateCommandList();

    // Execute the command list.
    std::array<ID3D12CommandList*, 1> ppCommandLists = { m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(ppCommandLists.size(), ppCommandLists.data());

    // Present the frame.
    HRESULT isFramePresented = m_swapChain->Present(1, 0);
    if (FAILED(isFramePresented))
    {
        PrintError(L"Frame couldn't be presented.", DEBUGLAYER);
    }

    WaitForPreviousFrame();
}

void Graphics::OnDestroy()
{
    WaitForPreviousFrame();

    CloseHandle(m_fenceEvent);
}

void Graphics::LoadPipeline()
{
    ui32 dxgiFactoryFlags = 0;
    if (DEBUGLAYER)
    {
        // Enable the debug layer (requires the Graphics Tools "optional feature").
        // NOTE: Enabling the debug layer after device creation will invalidate the active device.
        {
            Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
            {
                debugController->EnableDebugLayer();

                // Enable additional debug layers.
                dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            }
        }
    }
    Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
    HRESULT initDXGI = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
    if (FAILED(initDXGI))
    {
        PrintError(L"Initialization of DXGI has failed.", DEBUGLAYER);
    }
    Microsoft::WRL::ComPtr<IDXGIAdapter1> hardwareAdapter;
    GetHardwareAdapter(factory.Get(), &hardwareAdapter, true);
    
    HRESULT initDevice = D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&m_device));
    if (FAILED(initDevice))
    {
        PrintError(L"Initialization of Device has failed.", DEBUGLAYER);
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    HRESULT initCmdQueue = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));

    if (FAILED(initCmdQueue))
    {
        PrintError(L"Initialization of Command Queue has failed.", DEBUGLAYER);
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = m_width;
    swapChainDesc.Height = m_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
    HRESULT initSwapChain = factory->CreateSwapChainForHwnd(
        m_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
        m_hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain);
    swapChain.As(&m_swapChain);
    if (FAILED(initSwapChain))
    {
        PrintError(L"Initialization of SwapChain has failed.", DEBUGLAYER);
    }
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
    //create descriptor heap
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FrameCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        HRESULT initDescHeap = m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
        if (FAILED(initDescHeap))
        {
            PrintError(L"Initialization of SwapChain has failed.", DEBUGLAYER);
        }
        m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // Create frame resources.
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each frame.
        for (ui32 n = 0; n < FrameCount; n++)
        {
            HRESULT getCurrentBuffer = m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
            if (FAILED(getCurrentBuffer))
            {
                PrintError(L"Initialization of the current buffer has failed.", DEBUGLAYER);
            }
            m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, m_rtvDescriptorSize);
        }
    }
   HRESULT createCMDAllocator = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
   if (FAILED(createCMDAllocator))
   {
       PrintError(L"Initialization of the Command Allocator has failed.", DEBUGLAYER);
   }
}

void Graphics::LoadAssets()
{
    // Create the command list.
    HRESULT initCMDList = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList));
    if (FAILED(initCMDList))
    {
        PrintError(L"Initialization of the Command Allocator has failed.", DEBUGLAYER);
    }
    // Command lists are created in the recording state, but there is nothing
   // to record yet. The main loop expects it to be closed, so close it now.
    m_commandList->Close();


    // Create synchronization objects.
    {
        HRESULT createFence = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));

        if (FAILED(createFence))
        {
            PrintError(L"Initialization of the fence object has failed.", DEBUGLAYER);
        }
        m_fenceValue = 1;

        // Create an event handle to use for frame synchronization.
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_fenceEvent == nullptr)
        {
            PrintError(L"Initialization of the event object has failed.", DEBUGLAYER);
        }
    }
}

void Graphics::PopulateCommandList()
{
    // Command list allocators can only be reset when the associated 
  // command lists have finished execution on the GPU; apps should use 
  // fences to determine GPU execution progress.
    HRESULT resetAllocator = m_commandAllocator->Reset();
    if (FAILED(resetAllocator))
    {
        PrintError(L"Couldn't reset Command Allocator.", DEBUGLAYER);
    }

    // However, when ExecuteCommandList() is called on a particular command 
  // list, that command list can then be reset at any time and must be before 
  // re-recording.
    HRESULT resetCMDList = m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get());
    if (FAILED(resetCMDList))
    {
        PrintError(L"Couldn't reset Command list.", DEBUGLAYER);
    }
    // Indicate that the back buffer will be used as a render target.
    const auto renderBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &renderBarrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);

    // Record commands.
    std::array<const f32, 4>clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor.data(), 0, nullptr);

    // Indicate that the back buffer will now be used to present.
    const auto presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commandList->ResourceBarrier(1, &presentBarrier);

   HRESULT closeCmdList = m_commandList->Close();
   if (FAILED(closeCmdList))
   {
       PrintError(L"Failed to close Command List", DEBUGLAYER);
   }
}

void Graphics::WaitForPreviousFrame()
{
    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
    // sample illustrates how to use fences for efficient resource usage and to
    // maximize GPU utilization.

    // Signal and increment the fence value.
    const ui64 fence = m_fenceValue;
    HRESULT isCmdQueueSignaled = m_commandQueue->Signal(m_fence.Get(), fence);
    if (FAILED(isCmdQueueSignaled))
    {
        PrintError(L"Failed to Signal Command Queue", DEBUGLAYER);
    }
    m_fenceValue++;

    // Wait until the previous frame is finished.
    if (m_fence->GetCompletedValue() < fence)
    {
        HRESULT isFenceEventCompleted = m_fence->SetEventOnCompletion(fence, m_fenceEvent);
        if (FAILED(isFenceEventCompleted))
        {
            PrintError(L"Failed to complete fence event", DEBUGLAYER);
        }
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}
