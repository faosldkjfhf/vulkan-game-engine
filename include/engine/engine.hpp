#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Engine {
class Engine {
public:
  Engine();
  ~Engine();
  void run();

private:
  void createInstance();
  void loop();

  GLFWwindow *_window;
  int _width;
  int _height;

  VkInstance _instance;
};
} // namespace Engine
