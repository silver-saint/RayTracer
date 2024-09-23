#pragma once
#include <d3d11.h>
class Graphics
{
public:

	Graphics(HWND hWnd);
	Graphics(const Graphics& rhs) = delete;
	Graphics& operator=(const Graphics& rhs) = delete;
private:
//	DX::DebugLayer debugLayer;

};