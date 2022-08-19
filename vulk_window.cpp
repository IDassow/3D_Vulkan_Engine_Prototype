#include "vulk_window.h"

#include <stdexcept>
namespace vWind{

VulkWindow::VulkWindow(int w, int h, std::string name)
	:width{ w }, height{ h }, windowName{name} {
	initWindow();
}

VulkWindow::~VulkWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void VulkWindow::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow( width, height, windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void VulkWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface");
	}
}

void VulkWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto vulkWindow = reinterpret_cast<VulkWindow*>(glfwGetWindowUserPointer(window));
	vulkWindow->framebufferResize = true;
	vulkWindow->width = width;
	vulkWindow->height = height;
}


}//endof namespace vWind