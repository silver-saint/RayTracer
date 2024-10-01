#include "Graphics.h"

Graphics::Graphics(HWND hWnd, ui32 width, ui32 height)
	: DXContext{width, height}, m_frameIndex {0}, m_rtvDescriptorSize{0},
    m_width{width}, m_height{height}, m_hwnd(hWnd),
    m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
    m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height))
{
    WCHAR assetsPath[64];
    GetAssetPath(assetsPath, _countof(assetsPath));
    m_assetPath = assetsPath;
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

std::wstring Graphics::GetFullAssetPath(LPCWSTR assetName)
{
    return m_assetPath + assetName;
}

void Graphics::GetAssetPath(_Out_writes_(pathSize) WCHAR* path, UINT pathSize)
{
    
    if (path == nullptr)
    {
        PrintError(L"Invalid path", DEBUGLAYER);
    }

    DWORD size = GetCurrentDirectory(MAX_PATH, path);
    if (size == 0 || size == pathSize)
    {
        // Method failed or path was truncated.
        PrintError(L"path was truncated", DEBUGLAYER);

    }

    WCHAR* lastChar = path + wcslen(path) - 1;
    if (lastChar)
    {
        *(lastChar + 1) = L'\\';
        *(lastChar + 2) = L'\0';
    }
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
    //create empty rootsignature
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc = { 0,NULL,0,NULL,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT };
    Microsoft::WRL::ComPtr<ID3DBlob> signature;
    Microsoft::WRL::ComPtr<ID3DBlob> error;
    HRESULT isRootSignatureSerialized = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    if(FAILED(isRootSignatureSerialized))
    {
        PrintError(L"Root signature hasn't been serialized.", DEBUGLAYER);
    }
    HRESULT isRootSignatureCreated = m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
    if (FAILED(isRootSignatureCreated))
    {
        PrintError(L"Root signature wasn't able to be created.", DEBUGLAYER);
    }
    // Create the pipeline state, which includes compiling and loading shaders.
    {
        Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
        Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

        // Enable better shader debugging with the graphics debugging tools.
        ui32 compileFlags;
        (DEBUGLAYER) ? compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION : compileFlags = 0;
        

      HRESULT didVertexShadersCompile = D3DCompileFromFile(GetFullAssetPath(L"Shaders\\vertShader.hlsl").c_str(), nullptr, nullptr, "main", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
        
      if (FAILED(didVertexShadersCompile))
      {
          PrintError(L"Failed to compile vertex shaders!", DEBUGLAYER);
      }
      HRESULT didPixelShadersCompile = D3DCompileFromFile(GetFullAssetPath(L"Shaders\\pixelShader.hlsl").c_str(), nullptr, nullptr, "main", "ps_5_0", compileFlags, 0, &pixelShader, nullptr);
      if (FAILED(didPixelShadersCompile))
      {
          PrintError(L"Failed to compile pixel shaders!", DEBUGLAYER);
      }
        // Define the vertex input layout.
        std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs.data(), (ui32)inputElementDescs.size()};
        psoDesc.pRootSignature = m_rootSignature.Get();
        psoDesc.VS = { reinterpret_cast<ui8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
        psoDesc.PS = { reinterpret_cast<ui8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;
        HRESULT createGraphicsPipeline = (m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
        if (FAILED(createGraphicsPipeline))
        {
            PrintError(L"Failed to create Graphics Pipeline", DEBUGLAYER);
        }
    }

    // Create the command list.
    HRESULT initCMDList = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList));
    if (FAILED(initCMDList))
    {
        PrintError(L"Initialization of the Command Allocator has failed.", DEBUGLAYER);
    }
    // Command lists are created in the recording state, but there is nothing
   // to record yet. The main loop expects it to be closed, so close it now.
    m_commandList->Close();



    const std::vector<Vertex> triangleVertices =
    {
        { { -1.0f, 1.0f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 1.0f,  -1.0f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { {  -1.0f,  -1.0f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { {  1.0f, 1.0f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }
    };
    const ui32 vertexBufferSize =  sizeof(Vertex) * triangleVertices.size();
    const std::array<DWORD, 6> indexBufferList = { 0,1,2,0,3,1 };
    const ui32 indexBufferSize =  sizeof(DWORD) * indexBufferList.size();


    // Note: using upload heaps to transfer static data like vert buffers is not 
   // recommended. Every time the GPU needs it, the upload heap will be marshalled 
   // over. Please read up on Default Heap usage. An upload heap is used here for 
   // code simplicity and because there are very few verts to actually transfer.
    
   //Initialize the index buffer view.
    const CD3DX12_HEAP_PROPERTIES indexUploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    const CD3DX12_RESOURCE_DESC indexBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
    HRESULT indexUploadDataToUploadHeap = (m_device->CreateCommittedResource(
        &indexUploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &indexBufferResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_indexBuffer)));
    if (FAILED(indexUploadDataToUploadHeap))
    {
        PrintError(L"Failed to upload data to upload heap", DEBUGLAYER);
    }
    //copy the triangle data to index buffer;
    UINT8* pIndexDataBegin;
    CD3DX12_RANGE readIndexRange(0, 0);        // We do not intend to read from this resource on the CPU.
    HRESULT mapIndexBuffer = (m_indexBuffer->Map(0, &readIndexRange, reinterpret_cast<void**>(&pIndexDataBegin)));
    if (FAILED(mapIndexBuffer))
    {
        PrintError(L"Failed to map indicies data to buffer.", DEBUGLAYER);
    }
    memcpy(pIndexDataBegin, indexBufferList.data(), indexBufferSize);
    m_indexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
    m_indexBufferView.Format = DXGI_FORMAT_R32_FLOAT;
    m_indexBufferView.SizeInBytes = indexBufferSize;


    const CD3DX12_HEAP_PROPERTIES vertexUploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    const CD3DX12_RESOURCE_DESC vertexBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    HRESULT vertexUploadDataToUploadHeap = (m_device->CreateCommittedResource(
        &vertexUploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &vertexBufferResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)));
    if (FAILED(vertexUploadDataToUploadHeap))
    {
        PrintError(L"Failed to upload data to upload heap", DEBUGLAYER);
    }
    

    // Copy the triangle data to the vertex buffer.
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readVertexRange(0, 0);        // We do not intend to read from this resource on the CPU.
    HRESULT mapVertexBuffer = (m_vertexBuffer->Map(0, &readVertexRange, reinterpret_cast<void**>(&pVertexDataBegin)));
    if (FAILED(mapVertexBuffer))
    {
        PrintError(L"Failed to map vertices data to buffer.", DEBUGLAYER);
    }
    memcpy(pVertexDataBegin, triangleVertices.data(), vertexBufferSize);
    m_vertexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    m_vertexBufferView.SizeInBytes = vertexBufferSize;


    // Create synchronization objects and wait until assets have been uploaded to the GPU.
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


        // Wait for the command list to execute; we are reusing the same command 
        // list in our main loop but for now, we just want to wait for setup to 
        // complete before continuing.
        WaitForPreviousFrame();
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

    // Set necessary state.
    m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    m_commandList->RSSetViewports(1, &m_viewport);
    m_commandList->RSSetScissorRects(1, &m_scissorRect);



    // Indicate that the back buffer will be used as a render target.
    const auto renderBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &renderBarrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    // Record commands.
    std::array<const f32, 4>clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor.data(), 0, nullptr);
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_commandList->IASetIndexBuffer(&m_indexBufferView);
    m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    m_commandList->DrawIndexedInstanced(6, 1, 0,0,0);

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
