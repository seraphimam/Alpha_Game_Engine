#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

//namespace display {

	class Display_Window {
		public:
			Display_Window(int width, int height, std::string name);
			~Display_Window();

			bool shouldClose() {
				return glfwWindowShouldClose(window);
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
	


//}
