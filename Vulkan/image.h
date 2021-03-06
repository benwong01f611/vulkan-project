#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {
	class Device;
	class SwapChain;
	class Model;
	class Memory;
	class CommandBuffer;
	class Image {
	public:
		Image(Device& deviceRef, SwapChain& swapChainRef, Model& modelRef, Memory& memoryRef, CommandBuffer& commandBufferRef);
		~Image();
		void createColorResources();
		VkImage createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		void createDepthResources();
		VkFormat findDepthFormat();
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkImage& getColorImage();
		VkImageView& getColorImageView();
		VkDeviceMemory& getColorImageMemory();
		VkImage& getDepthImage();
		VkImageView& getDepthImageView();
		VkDeviceMemory& getDepthImageMemory();
		VkImageView& getTextureImageView();
		VkSampler& getTextureSampler();
		void createTexture();
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
		void cleanImages();

	private:
		VkImage colorImage;
		VkDeviceMemory colorImageMemory;
		VkImageView colorImageView;

		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;

		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;

		VkSampler textureSampler;
		std::string texture_path = "obj/viking_room.png";
		uint32_t mipLevels;

		Device& device;
		SwapChain& swapChain;
		VkDevice& logicalDevice;
		Model& model;
		Memory& memory;
		CommandBuffer& commandBuffer;
	};
}