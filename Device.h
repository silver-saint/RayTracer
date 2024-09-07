#pragma once
#include "types.h"
#include "Window.h"
#include <vulkan/vulkan.h>
#include <vector>


namespace vk::engine
{

#ifdef NDEBUG
		const bool VALIDATIONLAYERS = false;
#else
		const bool VALIDATIONLAYERS = true;
#endif

		class Device
		{
		public:
			Device(Window& windowRef);
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
