//
// Created by amenosora on 9/6/24.
//

#include "App.h"

namespace engine::vk
{
    App::App()
    {
        run();
    }

    App::~App() = default;

    void App::run()
    {

        while (window.IsOpen())
        {
            glfwPollEvents();
        }
        //vkDeviceWaitIdle(device.GetDevice());
    }
} // engine::vk