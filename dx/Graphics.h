#pragma once
#include "../DXContext.h"
#include <d3dx12.h>
#include "../ErrorHandler.h"


struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};
struct ConstantBuffer
{
    f32 radius;
    f32 borderThickness;
    DirectX::XMFLOAT2 position;
    DirectX::XMFLOAT4 borderColor;
    std::array<f32, 56> padding;
};
class Graphics : public DXContext
{
public:
	Graphics(HWND hWnd, u32 width, u32 height);
	Graphics(const Graphics& rhs) = delete;
	Graphics& operator=(const Graphics& rhs) = delete;
	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnDestroy() override;
    void ChangeRasterMode();
private:
    std::wstring GetFullAssetPath(LPCWSTR assetName);
    void GetAssetPath(_Out_writes_(pathSize) WCHAR* path, UINT pathSize);
    static const u32 FrameCount = 2;
    // Pipeline objects.
    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Device5> m_device;
    std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, FrameCount> m_renderTargets;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> m_commandList;
    u32 m_rtvDescriptorSize;
    std::wstring m_assetPath;
    //App resources
    Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    Microsoft::WRL::ComPtr<ID3D12Resource>m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer;
    ConstantBuffer m_constantBufferData;
    // Synchronization objects.
    size_t numOfIndicies;
    u32 m_frameIndex;
    HANDLE m_fenceEvent;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
    u64 m_fenceValue;
    u8* m_pCBvDataBegin;
    //swapchain
    u32 m_width;
    u32 m_height;
    //hanky panky
    static inline bool m_raster = false;
    //pass the hwnd object to here, this is dumb, but it gets the job done for now
    HWND m_hwnd;
    void LoadPipeline();
    void LoadAssets();
    void PopulateCommandList();
    void WaitForPreviousFrame();
    void CheckRaytracingSupport();
};