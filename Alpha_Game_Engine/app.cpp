#include "app.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>
#include <iostream>

namespace dev {
		
	struct SimplePushConstantData {
		glm::mat2 transform{ 1.f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	App::App() {
		std::cout << "app test\n";
		loadGameObjects(m);
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}

	App::~App() {
		vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
	}

	void App::run() {
		while (!Display_Window.shouldClose()) {
			glfwPollEvents();
			drawFrame();
			//glfwSetKeyCallback(Display_Window.getGLFW(), key_callback);
			if (glfwGetKey(Display_Window.getGLFW(), GLFW_KEY_E) == GLFW_PRESS && !hold) {
				m = (m + 1) % 3;
				loadGameObjects(m);
				hold = 1;
			}

			if (glfwGetKey(Display_Window.getGLFW(), GLFW_KEY_Q) == GLFW_PRESS && !hold) {
				objects[0].rotate(5.0f);

				//std::cout << "rotating\n";

				/*
				for (int i = 0; i < objects[0].transform2d.positions.size(); i++) {
					std::cout << "X" << i << ": " << objects[0].transform2d.positions[i].x << "\n";
					std::cout << "Y" << i << ": " << objects[0].transform2d.positions[i].y << "\n";
				}
				*/
					
			}

			if (glfwGetKey(Display_Window.getGLFW(), GLFW_KEY_E) == GLFW_RELEASE && hold) {
				hold = 0;
			}

			if (glfwGetKey(Display_Window.getGLFW(), GLFW_KEY_F)) {
				objects[0].transform2d.scale.x += 0.2f;
				objects[0].transform2d.scale.y += 0.2f;
			}

			if (glfwGetKey(Display_Window.getGLFW(), GLFW_KEY_W)) {
				objects[0].moveUp();
				//for (auto& obj : objects) {
				//	obj.transform2d.translation.y -= 0.1f;
				//}
			}

			if (glfwGetKey(Display_Window.getGLFW(), GLFW_KEY_A)) {
				objects[0].moveLeft();
			}

			if (glfwGetKey(Display_Window.getGLFW(), GLFW_KEY_S)) {
				objects[0].moveDown();
			}

			if (glfwGetKey(Display_Window.getGLFW(), GLFW_KEY_D)) {
				objects[0].moveRight();
			}
		}

		vkDeviceWaitIdle(device.device());
	}

	/*void* key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	}*/

	/*void App::loadModels() {
		std::cout << "load model test\n";
		std::vector<Alpha_Model::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		model = std::make_unique<Alpha_Model>(device, vertices);
	}*/

	void App::loadGameObjects(int mode) {
		objects.clear();

		if (mode == 1) {
			GameObject sq = GameObject(device);

			sq.makeSquare(0.5f, 0.5f, 0.3f, 0.3f);
			sq.color = { 0.1f, 0.8f, 0.1f };

			objects.push_back(std::move(sq));
		}
		else if (mode == 0) {
			GameObject triangle = GameObject(device);

			triangle.makeTriangle(-0.8f, -0.5f , -0.8f, -0.8f, -0.5f, -0.5f );
			triangle.color = { 0.1f, 0.8f, 0.1f };

			objects.push_back(std::move(triangle));
		}
		else {
			GameObject circle = GameObject(device);

			circle.makeCircle(0.8f, 0.8f, 0.3f);
			circle.color = { 0.1f, 0.8f, 0.1f };

			objects.push_back(std::move(circle));
		}
	}

	/*
	void App::loadGameObjects(int mode) {
		//std::cout << "load GO test\n";
		objects.clear();
		std::vector<Alpha_Model::Vertex> vertices{
			{{-0.2f, 0.2f}, {1.0f, 0.0f, 0.0f}},
			{{-0.2f, -0.2f}, {0.0f, 1.0f, 0.0f}},
			{{0.2f, 0.2f}, {0.0f, 0.0f, 1.0f}},
			{{-0.2f, -0.2f}, {0.0f, 1.0f, 0.0f}},
			{{0.2f, 0.2f}, {0.0f, 0.0f, 1.0f}},
			{{0.2f, -0.2f}, {1.0f, 1.0f, 1.0f}}
		};

		std::vector<Alpha_Model::Vertex> vertices2{
			{{-0.8f, -0.8f}, {1.0f, 0.0f, 0.0f}},
			{{-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.8f, -0.5f}, {0.0f, 0.0f, 1.0f}},
		};

		std::vector<Alpha_Model::Vertex> vt{
			{ {0.8f, -0.8f}, { 1.0f, 0.0f, 0.0f }},
			{ {0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} },
			{ {0.8f, -0.5f}, {0.0f, 0.0f, 1.0f} }
		};

		std::vector<Alpha_Model::Vertex> vertices3{
		};

		int steps = 60;
		float angle = glm::two_pi<float>() / steps;

		for (int i = 0; i < steps; i++) {
			float x1 = 0.5 * glm::sin(angle * i);
			float x2 = 0.5 * glm::sin(angle * (i+1));
			float y1 = 0.5 * glm::cos(angle * i);
			float y2 = 0.5 * glm::cos(angle * (i + 1));

			vertices3.push_back(
				{ {x1, y1}, {0.0f, 1.0f, 0.0f} }
			);

			vertices3.push_back(
				{ {x2, y2}, {0.0f, 1.0f, 0.0f} }
			);

			vertices3.push_back(
				{ {0, 0}, {0.0f, 1.0f, 0.0f} }
			);
		}

		if (mode == 1) {
			auto model = std::make_shared<Alpha_Model>(device, vertices);

			GameObject sq = GameObject(device);
			sq.model = model;
			sq.color = { 0.1f, 0.8f, 0.1f };
			//sq.transform2d.translation.x = 0.2f;
			//sq.transform2d.scale = { 1.0f, 1.0f };
			//sq.transform2d.rotation = 0.25f * glm::two_pi<float>();

			objects.push_back(std::move(sq));
		}
		else if (mode == 0) {
			auto model2 = std::make_shared<Alpha_Model>(device, vertices2);
			auto mod = std::make_shared<Alpha_Model>(device, vt);
			auto triangle = GameObject::createGameObject();
			triangle.model = model2;
			triangle.color = { 0.1f, 0.8f, 0.1f };
			auto t1 = GameObject::createGameObject();
			t1.model = mod;
			t1.color = { 0.1f, 0.8f, 0.1f };
			//triangle.transform2d.translation.x = 0.2f;
			//triangle.transform2d.scale = { 1.0f, 1.0f };
			//triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

			objects.push_back(std::move(triangle));
			objects.push_back(std::move(t1));
		}
		else {
			auto model3 = std::make_shared<Alpha_Model>(device, vertices3);
			auto circle = GameObject::createGameObject();
			circle.model = model3;
			circle.color = { 0.1f, 0.8f, 0.1f };
			//circle.transform2d.translation.x = 0.2f;
			//circle.transform2d.scale = { 1.0f, 1.0f };
			//circle.transform2d.rotation = 0.25f * glm::two_pi<float>();

			objects.push_back(std::move(circle));
		}
		
	}//end loadGameObject()
	*/

	void App::createPipelineLayout() {
		std::cout << "pipeline layout test\n";

		VkPushConstantRange pushRange{};
		pushRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushRange.offset = 0;
		pushRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushRange;

		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void App::createPipeline() {
		assert(SwapChain != nullptr && "Cannot create pipeline before swap chain");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before layout");
		//std::cout << "pipeline test\n";
		//auto pipelineConfig = MyPipeline::defaultPipelineConfigInfo(SwapChain->width(), SwapChain->height());
		PipelineConfigInfo pipelineConfig{};
		MyPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = SwapChain->getRenderPass();
		std::cout << "render pass test\n";
		pipelineConfig.pipelineLayout = pipelineLayout;

		std::cout << "layout test\n";
		pipeline = std::make_unique<MyPipeline>(
			device,
			"shaders/shader_test.vert.spv",
			"shaders/shader_test.frag.spv",
			pipelineConfig);
		std::cout << "pipeline setup test\n";
	}

	void App::recreateSwapChain() {
		std::cout << "swap chain create test\n";
		auto extent = Display_Window.getExtent();

		//std::cout << "before while test\n";
		while (extent.width == 0 || extent.height == 0) {

			//std::cout << "while test\n";
			extent = Display_Window.getExtent();

			//std::cout << "before wait test\n";
			glfwWaitEvents();
		}

		//std::cout << "after while test\n";
		vkDeviceWaitIdle(device.device());

		if (SwapChain == nullptr) {
			SwapChain = std::make_unique<MyEngineSwapChain>(device, extent);
		}
		else {
			SwapChain = std::make_unique<MyEngineSwapChain>(device, extent, std::move(SwapChain));
		
			if (SwapChain->imageCount() != commandBuffers.size()) {
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		std::cout << "after idle test\n";
		SwapChain = nullptr;
		SwapChain = std::make_unique<MyEngineSwapChain>(device, extent);

		std::cout << "swap chain unique test\n";
		createPipeline();
		
	}

	void App::createCommandBuffers() {
		std::cout << "command buffer test\n";
		commandBuffers.resize(SwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = device.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers");
		}

		/*for static scale window
		for (int i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = MyEngineSwapChain->getRenderPass();
			renderPassInfo.framebuffer = MyEngineSwapChain->getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = MyEngineSwapChain->getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			VkClearValue clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		
			pipeline->bind(commandBuffers[i]);
			//vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
			model->bind(commandBuffers[i]);
			model->draw(commandBuffers[i]);

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
		*/

	}//end createCommandBuffers()

	void App::freeCommandBuffers() {
		vkFreeCommandBuffers(device.device(), device.getCommandPool(),
			static_cast<float>(commandBuffers.size()),
			commandBuffers.data()
		);

		commandBuffers.clear();
	}

	void App::recordCommandBuffer(int imageIndex) {
		//std::cout << "image index: " + imageIndex;
		/*static int maxFrameRate = 60;
		static int frame = 0;
		frame = (frame + 1) % maxFrameRate;*/

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = SwapChain->getRenderPass();
		renderPassInfo.framebuffer = SwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = SwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		//background color
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		/*VkClearValue clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = &clearColor;*/

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(SwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(SwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, SwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		renderGameObjects(commandBuffers[imageIndex]);

		//pipeline->bind(commandBuffers[imageIndex]);
		////vkCmdDraw(commandBuffers[imageIndex], 3, 1, 0, 0);
		//model->bind(commandBuffers[imageIndex]);

		//
		//SimplePushConstantData pushData{};
		//pushData.offset = { -1.0f + frame * 0.05f, 0.0f  };
		//pushData.color = { 0.0f, 0.0f, 0.0f };

		//vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout, 
		//	VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
		//	sizeof(SimplePushConstantData), &pushData);

		//model->draw(commandBuffers[imageIndex]);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}//end recordCommandBuffer

	void App::drawFrame() {
		uint32_t imageIndex;
		auto result = SwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to aquire swap chain image");
		}

		recordCommandBuffer(imageIndex);

		result = SwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Display_Window.wasWindowResized()) {
			Display_Window.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}
		
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}
	
	}//end drawFrame

	void App::renderGameObjects(VkCommandBuffer commandBuffer) {

		//int i = 0;
		//	for (auto& obj : objects) {
		//		i += 1;
		//		//obj.transform2d.translation.y = 0.2f * i;
		//		obj.transform2d.rotation = glm::mod<float>(obj.transform2d.rotation + 0.001f * i, 2.0f * glm::pi<float>());
		//	}
		

		pipeline->bind(commandBuffer);

		for (auto& obj : objects) {
			//obj.transform2d.scale = { 1.0f, 1.0f };
			//obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());
			//obj.rotate(30.0f);

			SimplePushConstantData pushData{};
			pushData.offset = obj.transform2d.translation;
			pushData.color = obj.color;
			pushData.transform = obj.transform2d.mat2();

			vkCmdPushConstants(commandBuffer, pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
				sizeof(SimplePushConstantData), &pushData);

			
			obj.bindVertices();
			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}

	}//end renderGameObjects

}