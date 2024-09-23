//#include "App/App.h"
#include "../platform/WinInstance.h"
#include <iostream>
#include <DirectXMath.h>



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow) {
    DirectX::XMVECTOR u = { -1,3,2 };
    DirectX::XMVECTOR v = { 3,-4,1 };
    DirectX::XMVECTOR resu = DirectX::XMVector3Normalize(u);
    DirectX::XMVECTOR resv = DirectX::XMVector3Normalize(v);

/*
    dx::engine::App app;
    return EXIT_SUCCESS;
  */  
}


