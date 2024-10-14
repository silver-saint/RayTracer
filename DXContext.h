#pragma once
#include "platform/WinInstance.h"
#include <wrl.h>
#include <d3d12.h>
#include <array>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <string>
#include "types.h"
#include <DirectXMath.h>

class DXContext
{
public:
	DXContext(u32 m_swapChainWidth, u32 m_swapChainHeight);
	virtual ~DXContext();
	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnSizeChanged(u32 width, u32 height, bool minimized) = 0;
	virtual void OnWindowMoved(i32 x, i32 y) {}
	virtual void OnDisplayChanged() {}

	virtual void OnDestroy() = 0;
	[[nodiscard]] u32 GetWidth() const { return m_width; }
	[[nodiscard]] u32 GetHeight() const { return m_height; }
	[[nodiscard]] RECT GetWindowBounds() const { return m_windowBounds; }
protected:
	u32 m_width;
	u32 m_height;
	f32 m_aspectRatio;
	RECT m_windowBounds;
	u32 m_adapterIDoverride;
private:


};

