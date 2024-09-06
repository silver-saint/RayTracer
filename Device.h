//
// Created by amenosora on 9/6/24.
//

#pragma once
#include <vulkan/vulkan.h>
#include "Window.h"



namespace  engine::vk {
#ifdef NDEBUG
    const bool VALIDATIONLAYERS = false;
#else
    const bool VALIDATIONLAYERS = true;
#endif

    class Device
    {
    public:
        explicit Device(Window& windowRef);
        ~Device();
        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;
    private:
        void Init();
        void CreateInstance();
        VkInstance instance;
        Window& win;
    };
}