//
// Created by amenosora on 9/6/24.
//

#include "App.h"

namespace vk::engine
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
            window.PollEvents();
        }
        //vkDeviceWaitIdle(device.GetDevice());
    }
} // engine::vk