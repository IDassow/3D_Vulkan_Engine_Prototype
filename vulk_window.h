#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include<string>

namespace vWind {

class VulkWindow {
public:
	VulkWindow(int w, int h, std::string name);
	~VulkWindow();

	//in the case of a dangling pointer
	VulkWindow(const VulkWindow&) = delete;
	VulkWindow &operator=(const VulkWindow&) = delete;

	bool shouldClose() { return glfwWindowShouldClose(window); }
	VkExtent2D getExtent() { return { static_cast<uint32_t>(width) , static_cast<uint32_t>(height) };}
	bool wasWindowResized() { return framebufferResize; }
	void resetWindowResizeFlag() { framebufferResize = false; }

	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);


private:
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	void initWindow();

	int width;
	int height;
	bool framebufferResize = false;

	std::string windowName;
	GLFWwindow *window;
};

}//endof namespace vWind