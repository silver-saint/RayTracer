//
// Created by amenosora on 9/6/24.
//

#pragma once
#include <memory>
#include <vector>
#include "types.h"
#include "Window.h"
#include "Device.h"
namespace engine::vk {

class App
    {
    public:
        App();
        ~App();
        App& operator=(const App&) = delete;
        App(const App&) = delete;
        void run();
    private:
        static constexpr ui32 WIDTH = 800;
        static constexpr ui32 HEIGHT = 800;
        Window window{ WIDTH, HEIGHT, "Rectangle" };
        Device device{ window };
    };
} // engine::vk

