#include "Graphics.h"
Graphics::Graphics(HWND hWnd, u32 width, u32 height)
	: DXContext{ width, height }, m_frameIndex{ 0 }, m_rtvDescriptorSize{ 0 },
	m_width{ width }, m_height{ height }, m_hwnd{ hWnd },
	m_viewport{ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height) },
	m_scissorRect{ 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) },
	m_constantBuffer{}
{
	WCHAR assetsPath[64];
	GetAssetPath(assetsPath, _countof(assetsPath));
	m_assetPath = assetsPath;
}

void Graphics::OnInit()
{
	LoadPipeline();
	LoadAssets();
	CheckRaytracingSupport();
	// Command lists are created in the recording state, but there is nothing
// to record yet. The main loop expects it to be closed, so close it now.
	m_commandList->Close();
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

void Graphics::ChangeRasterMode()
{
	m_raster = !m_raster;
	Sleep(100);
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
	u32 dxgiFactoryFlags = 0;
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
			else
			{
				PrintError(L"Couldn't initialize DebugLayers", DEBUGLAYER);
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

	HRESULT initDevice = D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
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


		// Describe and create a constant buffer view (CBV) descriptor heap.
		// Flags indicate that this descriptor heap can be bound to the pipeline 
		// and that descriptors contained in it can be referenced by a root table.
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
		cbvHeapDesc.NumDescriptors = 1;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		HRESULT createCBVHeap = m_device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_cbvHeap));
		if (FAILED(createCBVHeap))
		{
			PrintError(L"Couldn't create CBV heap", DEBUGLAYER);
		}


	}

	// Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (u32 n = 0; n < FrameCount; n++)
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

	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

	// This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

	if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
	{
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

	CD3DX12_DESCRIPTOR_RANGE1 ranges[1] = {};
	CD3DX12_ROOT_PARAMETER1 rootParameters[1] = {};

	ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_ALL);

	// Allow input layout and deny uneccessary access to certain pipeline stages.
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSigDesc = {};
	rootSigDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);
	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;
	HRESULT isRootSignatureSerialized = D3DX12SerializeVersionedRootSignature(&rootSigDesc, featureData.HighestVersion, &signature, &error);
	if (FAILED(isRootSignatureSerialized))
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
		u32 compileFlags;
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
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12U, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs.data(), (u32)inputElementDescs.size() };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = { reinterpret_cast<u8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
		psoDesc.PS = { reinterpret_cast<u8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
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


	const std::vector<Vertex> triangleVertices =
	{
		{ { -1.0f, 1.0f * m_aspectRatio, 0.0f },    { 0.60f, 0.60f, 1.0f, 1.0f } },
		{ { 1.0f,  -1.0f * m_aspectRatio, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  -1.0f,  -1.0f * m_aspectRatio, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f, 1.0f * m_aspectRatio, 0.0f },    { 0.60f, 0.60f, 1.0f, 1.0f } },

	};





	const u32 vertexBufferSize = sizeof(Vertex) * triangleVertices.size();
	const std::array<u32, 12> indexBufferList = { 0,1,2,0,3,1,4,5,6,4,7,5 };
	const u32 indexBufferSize = sizeof(u32) * indexBufferList.size();
	numOfIndicies = indexBufferList.size();

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

	// Initialize the index buffer view.
	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
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

	{
		const UINT constantBufferSize = sizeof(ConstantBuffer);    // CB size is required to be 256-byte aligned.

		CD3DX12_HEAP_PROPERTIES constantBufferUploadHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC constantBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);


		HRESULT isCBcreated = m_device->CreateCommittedResource(
			&constantBufferUploadHeap,
			D3D12_HEAP_FLAG_NONE,
			&constantBufferResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_constantBuffer));
		if (FAILED(isCBcreated))
		{
			PrintError(L"Couldn't create Constant buffer", DEBUGLAYER);
		}
		// Describe and create a constant buffer view.
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = constantBufferSize;
		m_device->CreateConstantBufferView(&cbvDesc, m_cbvHeap->GetCPUDescriptorHandleForHeapStart());


		// Map and initialize the constant buffer. We don't unmap this until the
		// app closes. Keeping things mapped for the lifetime of the resource is okay.
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		HRESULT isCBMapped = (m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCBvDataBegin)));
		if (FAILED(isCBMapped))
		{
			PrintError(L"Couldn't map CB buffer", DEBUGLAYER);
		}
		m_constantBufferData.position = {(m_width / 2) * m_aspectRatio, (m_height / 2) * m_aspectRatio };
		m_constantBufferData.borderThickness = 0.0f;
		m_constantBufferData.radius = 100.0f;
		m_constantBufferData.borderColor = { 0.0f, 0.0f, 0.0f, 0.0f };


		memcpy(m_pCBvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
	}

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

	ID3D12DescriptorHeap* ppHeaps[] = { m_cbvHeap.Get() };
	m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	m_commandList->SetGraphicsRootDescriptorTable(0, m_cbvHeap->GetGPUDescriptorHandleForHeapStart());


	// Indicate that the back buffer will be used as a render target.
	const auto renderBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_commandList->ResourceBarrier(1, &renderBarrier);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
	std::array<const f32, 4>clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	if (m_raster)
	{
		// Record commands.
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor.data(), 0, nullptr);
		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_commandList->IASetIndexBuffer(&m_indexBufferView);
		m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);

		m_commandList->DrawIndexedInstanced(numOfIndicies, 1, 0, 0, 0);
	}
	else
	{
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor.data(), 0, nullptr);
	}
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
	const u64 fence = m_fenceValue;
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

void Graphics::CheckRaytracingSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options5 = {};
	HRESULT checkFeatureSupport = m_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options5, sizeof(options5));
	if (FAILED(checkFeatureSupport))
	{
		PrintError(L"Feature level not supported", DEBUGLAYER);
	}
	if (options5.RaytracingTier < D3D12_RAYTRACING_TIER_1_0)
	{
		PrintError(L"Raytracing not supported on device", DEBUGLAYER);
	}
}
