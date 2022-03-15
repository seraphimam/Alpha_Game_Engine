#pragma once
#include "device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace dev {
	class Alpha_Model {
		public:
			struct Vertex {
				glm::vec2 position;

				static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
				static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
			};
			Alpha_Model(MyDevice& device, const std::vector<Vertex>& vertices);
			//Alpha_Model();
			~Alpha_Model();

			Alpha_Model(const Alpha_Model&) = delete;
			Alpha_Model& operator = (const Alpha_Model&) = delete;

			void bind(VkCommandBuffer commandBuffer);
			void draw(VkCommandBuffer commandBuffer);

		private:
			void createVertexBuffers(const std::vector<Vertex>& vertices);

			MyDevice& device;
			VkBuffer vertexBuffer;
			VkDeviceMemory vertexBufferMemory;
			uint32_t vertexCount;
	};
}
