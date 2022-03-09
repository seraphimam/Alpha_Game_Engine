#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace dev {

	class Display_Window {
		public:
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

			void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		private:
			void initWindow();
			const int width;
			const int height;

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
