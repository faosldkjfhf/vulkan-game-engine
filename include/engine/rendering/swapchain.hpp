#pragma once

#include "types.hpp"
#include <vector>

namespace engine {

struct QueueFamilyIndices;

} // namespace engine

namespace rendering {

struct SwapchainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

class Swapchain {
public:
  Swapchain(VkSurfaceKHR &surface);
  ~Swapchain();

  void querySwapchainSupportDetails(VkPhysicalDevice &device);
  void create(VkDevice &device, GLFWwindow *window,
              engine::QueueFamilyIndices indices);

  VkSurfaceFormatKHR chooseSwapSurfaceFormat(VkFormat format,
                                             VkColorSpaceKHR colorSpace);
  VkPresentModeKHR chooseSwapPresentMode(VkPresentModeKHR presentMode);
  VkExtent2D chooseSwapExtent(GLFWwindow *window);

private:
  VkSwapchainKHR _swapchain;
  std::vector<VkImage> _swapchainImages;
  SwapchainSupportDetails _details;
  VkSurfaceKHR _surface;

  VkFormat _format;
  VkExtent2D _extent;
};

} // namespace rendering
