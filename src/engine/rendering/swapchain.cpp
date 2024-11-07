#include "rendering/swapchain.hpp"
#include "engine.hpp"
#include <algorithm>
#include <stdexcept>

namespace rendering {

Swapchain::Swapchain(VkSurfaceKHR &surface) : _surface(surface) {}

Swapchain::~Swapchain() {}

void Swapchain::querySwapchainSupportDetails(VkPhysicalDevice &device) {
  _details = {};
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface,
                                            &_details.capabilities);

  uint32_t formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

  if (formatCount != 0) {
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount,
                                         formats.data());
    _details.formats = formats;
  }

  uint32_t presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount,
                                            nullptr);
  if (presentModeCount != 0) {
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, _surface, &presentModeCount, presentModes.data());
    _details.presentModes = presentModes;
  }
}

VkSurfaceFormatKHR
Swapchain::chooseSwapSurfaceFormat(VkFormat format,
                                   VkColorSpaceKHR colorSpace) {
  for (const auto &availableFormat : _details.formats) {
    if (availableFormat.format == format &&
        availableFormat.colorSpace == colorSpace) {
      return availableFormat;
    }
  }

  return _details.formats[0];
}

VkPresentModeKHR
Swapchain::chooseSwapPresentMode(VkPresentModeKHR presentMode) {
  for (const auto &availablePresentMode : _details.presentModes) {
    if (availablePresentMode == presentMode) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::chooseSwapExtent(GLFWwindow *window) {
  VkSurfaceCapabilitiesKHR capabilities;
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  }

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                             static_cast<uint32_t>(height)};

  actualExtent.width =
      std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                 capabilities.maxImageExtent.width);
  actualExtent.height =
      std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                 capabilities.maxImageExtent.height);

  return actualExtent;
}

void Swapchain::create(VkDevice &device, GLFWwindow *window,
                       engine::QueueFamilyIndices indices) {
  VkSurfaceFormatKHR format = chooseSwapSurfaceFormat(
      VK_FORMAT_B8G8R8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
  VkPresentModeKHR presentMode =
      chooseSwapPresentMode(VK_PRESENT_MODE_MAILBOX_KHR);
  VkExtent2D extent = chooseSwapExtent(window);

  uint32_t imageCount = _details.capabilities.minImageCount + 1;
  if (_details.capabilities.maxImageCount > 0 &&
      imageCount > _details.capabilities.maxImageCount) {
    imageCount = _details.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = _surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = format.format;
  createInfo.imageColorSpace = format.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                   indices.presentFamily.value()};

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  createInfo.preTransform = _details.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &_swapchain) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain!");
  }

  vkGetSwapchainImagesKHR(device, _swapchain, &imageCount, nullptr);
  _swapchainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(device, _swapchain, &imageCount,
                          _swapchainImages.data());

  _format = format.format;
  _extent = extent;
}

} // namespace rendering
