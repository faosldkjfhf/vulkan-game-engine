#include "engine/engine.hpp"

#include "engine/debugUtil.hpp"
#include "engine/vulkanUtil.hpp"

#include <cstring>

namespace engine {

Engine::Engine() : _width(800), _height(600) {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  _window = glfwCreateWindow(_width, _height, "Vulkan", nullptr, nullptr);

  createInstance();
  setupDebugMessenger();
  pickPhysicalDevice();
  createLogicalDevice();
}

Engine::~Engine() {
  if (debug::enableValidationLayers) {
    debug::destroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
  }

  vkDestroyInstance(_instance, nullptr);
  vkDestroyDevice(_device, nullptr);

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
  if (debug::enableValidationLayers && !debug::checkValidationLayerSupport()) {
    throw std::runtime_error("validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Vulkan Game Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  auto extensions = debug::getRequiredExtensions();

  // instance create info
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
  if (debug::enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(debug::validationLayers.size());
    createInfo.ppEnabledLayerNames = debug::validationLayers.data();

    debug::populateDebugMessengerCreateInfo(debugMessengerCreateInfo);
    createInfo.pNext =
        (VkDebugUtilsMessengerCreateInfoEXT *)&debugMessengerCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }

  std::cout << "instance created" << std::endl;
}

void Engine::setupDebugMessenger() {
  if (!debug::enableValidationLayers)
    return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  debug::populateDebugMessengerCreateInfo(createInfo);

  if (debug::createDebugUtilsMessengerEXT(_instance, &createInfo, nullptr,
                                          &_debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger!");
  }
}

void Engine::pickPhysicalDevice() {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

  for (const auto &device : devices) {
    if (vulkan::isDeviceSuitable(device)) {
      _physicalDevice = device;
      break;
    }
  }

  if (_physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

void Engine::createLogicalDevice() {
  vulkan::QueueFamilyIndices indices =
      vulkan::findQueueFamilies(_physicalDevice);

  VkDeviceQueueCreateInfo queueCreateInfo{};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
  queueCreateInfo.queueCount = 1;
  float queuePriority = 1.0f;
  queueCreateInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeatures{};

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.queueCreateInfoCount = 1;
  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount = 0;

  if (debug::enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(debug::validationLayers.size());
    createInfo.ppEnabledLayerNames = debug::validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
}

}; // namespace engine
