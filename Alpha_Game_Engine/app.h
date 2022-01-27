#pragma once

#include "display_window.h"
#include "pipeline_test.h"

//namespace display {
	class App {
		public:
			static constexpr int WIDTH = 800;
			static constexpr int HEIGHT = 600;

			void run();

		private:
			Display_Window Display_Window{ WIDTH, HEIGHT, "Hello Vulkan!" };
			MyPipeline MyPipeline{ "shaders/shader_test.vert.spv", "shaders/shader_test.frag.spv" };
	};
//}