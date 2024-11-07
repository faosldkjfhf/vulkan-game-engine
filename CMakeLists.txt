cmake_minimum_required(VERSION 3.10)

project(vulkan-game-engine VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(GLFW_BUILD_DOCS
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE BOOL "" FORCE)

option(USE_VENDORED "Use vendored libraries" ON)

if(USE_VENDORED)
  add_subdirectory(libraries/glfw EXCLUDE_FROM_ALL)
  add_subdirectory(libraries/glm EXCLUDE_FROM_ALL)
  add_subdirectory(libraries/shaderc EXCLUDE_FROM_ALL)
  find_package(Vulkan REQUIRED)
endif()

if(UNIX)
  execute_process(
    COMMAND
      ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
endif()

add_executable(main src/main.cpp)

add_subdirectory(src)
target_link_libraries(main PUBLIC ${Vulkan_LIBRARIES} glfw glm shaderc)
