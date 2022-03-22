#include "app.h"

#include <array>
#include <stdexcept>

namespace dev {
	App::App() {
		loadModels();
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
		}

		vkDeviceWaitIdle(device.device());
	}

	void App::loadModels() {
		std::vector<Alpha_Model::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		model = std::make_unique<Alpha_Model>(device, vertices);
	}

	void App::createPipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void App::createPipeline() {
		//auto pipelineConfig = MyPipeline::defaultPipelineConfigInfo(SwapChain->width(), SwapChain->height());
		PipelineConfigInfo pipelineConfig{};
		MyPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = SwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		pipeline = std::make_unique<MyPipeline>(
			device,
			"shaders/shader_test.vert.spv",
			"shaders/shader_test.frag.spv",
			pipelineConfig);
	}

	void App::recreateSwapChain() {
		auto extent = Display_Window.getExtent();

		while (extent.width == 0 || extent.height == 0) {
			extent = Display_Window.getExtent();

			glfwWaitEvents();
		}

		vkDeviceWaitIdle(device.device());
		SwapChain = nullptr;
		SwapChain = std::make_unique<MyEngineSwapChain>(device, extent);
		createPipeline();
		
	}

	void App::createCommandBuffers() {
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

	void App::recordCommandBuffer(int imageIndex) {
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
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		VkClearValue clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

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

		pipeline->bind(commandBuffers[imageIndex]);
		//vkCmdDraw(commandBuffers[imageIndex], 3, 1, 0, 0);
		model->bind(commandBuffers[imageIndex]);
		model->draw(commandBuffers[imageIndex]);

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
	
	}

}