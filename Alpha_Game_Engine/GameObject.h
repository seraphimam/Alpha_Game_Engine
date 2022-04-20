#pragma once

#include "Alpha_Model.h"

#include <memory>

namespace dev {
	struct Transform2d {
		glm::vec2 translation{ };
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation;

		glm::mat2 mat2() { 
			const float sin = glm::sin(rotation);
			const float cos = glm::cos(rotation);

			glm::mat2 rotationMatrix{
				{cos, sin},
				{-sin, cos}
			};

			glm::mat2 scaledMatrix{
				{scale.x, 0.0f},
				{0.0f, scale.y}
			};
			return rotationMatrix * scaledMatrix;
		}
	};

	class GameObject {
		public:
			using id_t = unsigned int;

			id_t id;

			static GameObject createGameObject() {
				static id_t curID = 0;
				return GameObject{ curID++ };
			}

			GameObject(const GameObject&) = delete;
			GameObject& operator=(const GameObject&) = delete;
			GameObject(GameObject&&) = default;
			GameObject& operator=(GameObject&&) = default;

			std::shared_ptr<Alpha_Model> model{};
			glm::vec3 color{};
			Transform2d transform2d{};
		private:
			GameObject(id_t objID) : id{objID} {}

	};

}