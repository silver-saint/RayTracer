#pragma once
#include "Device.h"

namespace vk::engine
{
	class SwapChain
	{
	public:
		SwapChain() = delete;
		SwapChain(Device& deviceRef);
		~SwapChain();
		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;

	private:
		//init
		void Init();
		//SwapChain
		void CreateSwapChain();
		Device& device;
		VkSwapchainKHR swapChain;
	};
}
