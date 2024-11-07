#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace engine {
class Engine {
public:
  Engine();
  ~Engine();
  void run();

private:
  void loop();

  void createInstance();
  void setupDebugMessenger();
  void pickPhysicalDevice();
  void createLogicalDevice();

  GLFWwindow *_window;
  int _width;
  int _height;

  VkInstance _instance;
  VkDebugUtilsMessengerEXT _debugMessenger;

  VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
  VkDevice _device;
  VkQueue _graphicsQueue;
};
} // namespace engine
