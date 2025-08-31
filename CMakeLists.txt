cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(mygame LANGUAGES CXX CUDA)
enable_language(CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)

# Force all targets (including subdirectories) to use the same runtime
if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Disable extra builds for glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

find_package(CUDAToolkit REQUIRED)

# Add subdirectories AFTER setting CMAKE_MSVC_RUNTIME_LIBRARY
add_subdirectory(thirdparty/glfw-3.3.2)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/stb_image)
add_subdirectory(thirdparty/stb_truetype)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/imgui-docking)

# Collect all .cpp and .cu sources
file(GLOB_RECURSE CPP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE CU_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cu")

# Combine all sources
set(ALL_SOURCES ${CPP_SOURCES} ${CU_SOURCES})

# Set CUDA language property for .cu files only
set_source_files_properties(${CU_SOURCES} PROPERTIES LANGUAGE CUDA)

# Create executable with all sources
add_executable("${CMAKE_PROJECT_NAME}" ${ALL_SOURCES})

# Use native architecture detection to avoid version conflicts
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CUDA_ARCHITECTURES "native")

# Alternative: specify multiple architectures for better compatibility
# set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CUDA_ARCHITECTURES "75;86;89")

if(WIN32)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
else()
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="../resources/")
endif()

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE
		glm glfw glad stb_image stb_truetype imgui
		CUDA::cudart CUDA::cuda_driver
)