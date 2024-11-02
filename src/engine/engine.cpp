#include "engine/engine.hpp"
#include <iostream>
#include <stdexcept>

namespace Engine {

Engine::Engine() : _width(800), _height(600) {

  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  _window = glfwCreateWindow(_width, _height, "Vulkan", nullptr, nullptr);

  createInstance();
}

Engine::~Engine() {
  vkDestroyInstance(_instance, nullptr);
  glfwDestroyWindow(_window);
  glfwTerminate();
}

void Engine::run() { loop(); }

void Engine::loop() {
  while (!glfwWindowShouldClose(_window)) {
    glfwPollEvents();
  }
}

void Engine::createInstance() {
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 3, 0);
  appInfo.pEngineName = "Vulkan Game Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 3, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  // instance create info
  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;

  if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }

  std::cout << "instance created" << std::endl;
}
}; // namespace Engine
