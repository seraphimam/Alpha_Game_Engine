#include "app.h"

//namespace display {
	void App::run() {
		while (!Display_Window.shouldClose()) {
			glfwPollEvents();
		}
	}
//}