#pragma once
#include "../platform/WinInstance.h"
#include <wrl.h>
#include <d3d12.h>
#include <array>
#include <dxgi1_6.h>
#include <../types.h>
class DXContext
{
public:
	DXContext(ui32 m_swapChainWidth, ui32 m_swapChainHeight);
	virtual ~DXContext();
	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;
protected:
	void GetHardwareAdapter(IDXGIFactory1* pFactory,IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter);
private:
	f32 m_aspectRatio;
	ui32 m_swapChainWidth;
	ui32 m_swapChainHeight;
};

