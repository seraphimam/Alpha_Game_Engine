#pragma once

//#include "device.h"
#include "pipeline_test.h"
#include "swap_chain.h"
#include "display_window.h"
#include "Alpha_Model.h"

#include <memory>
#include <vector>

namespace dev {
	class App {
		public:
			static constexpr int WIDTH = 800;
			static constexpr int HEIGHT = 600;

			App();
			~App();

			App(const App&) = delete;
			App& operator = (const App&) = delete;

			void run();

		private:
			void loadModels();
			void createPipelineLayout();
			void createPipeline();
			void createCommandBuffers();
			void drawFrame();

			Display_Window Display_Window{ WIDTH, HEIGHT, "Hello Vulkan!" };
			MyDevice device{ Display_Window };
			MyEngineSwapChain MyEngineSwapChain{ device, Display_Window.getExtent()};
			
			std::unique_ptr<MyPipeline> pipeline;
			VkPipelineLayout pipelineLayout;
			std::vector<VkCommandBuffer> commandBuffers;
			std::unique_ptr<Alpha_Model> model;
			
			/*MyPipeline MyPipeline{
				device,
				"shaders/shader_test.vert.spv", "shaders/shader_test.frag.spv",
				MyPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT) 
			};*/
	};
}