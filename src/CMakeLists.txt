add_subdirectory(engine)

# includes
target_include_directories(main PUBLIC ${Vulkan_INCLUDE_DIRS})
target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/include/engine)
