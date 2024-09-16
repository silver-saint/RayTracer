#include "dx/App.h"
#include "stl.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow) {
    
    stl::LinkedList head(5);
    head.Add(6);
    head.Add(7);
    head.Add(8);
    head.Add(9);
    head.PrintLinkedList();

    /*
    dx::engine::App app;
    return EXIT_SUCCESS;
    */
}
