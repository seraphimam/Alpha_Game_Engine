#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace dev {

	class Display_Window {
		public:
			int width;
			int height;
			Display_Window(int width, int height, std::string name);
			~Display_Window();

			Display_Window(const Display_Window&) = delete;
			Display_Window& operator = (const Display_Window&) = delete;

			bool shouldClose() {
				return glfwWindowShouldClose(window);
			}

			VkExtent2D getExtent() {
				return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
			}

			bool wasWindowResized() {
				return framebufferResized;
			}
			
			void resetWindowResizedFlag() {
				framebufferResized = false;
			}

			void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

			bool framebufferResized = false;

		private:
			static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
			void initWindow();
			//int width;
			//int height;
			//bool framebufferResized = false;

			std::string windowName;
			GLFWwindow* window;

			const int get_width() {
				return width;
			}

			const int get_height() {
				return height;
			}
	};
	


}
