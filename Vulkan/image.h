#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {
	class mainProgram;
	class Image {
	public:
		Image(mainProgram** mainProgramPtr);
		~Image();
		void createColorResources();
		VkImage createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		VkImageView* getColorImageView();
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		VkImage* getColorImage();
		VkDeviceMemory* getColorImageMemory();
	private:
		mainProgram** mainProg;
		VkImageView colorImageView;
		VkDevice* device;
		VkImage colorImage;
		VkDeviceMemory colorImageMemory;
	};
}