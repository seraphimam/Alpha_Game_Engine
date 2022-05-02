#pragma once

#include "Alpha_Model.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <string>
#include <vector>

namespace dev {
	struct Transform2d {
		glm::vec2 translation{ 0.0f, 0.0f };
		glm::vec2 scale{ 1.0f, 1.0f };
		std::vector<glm::vec2> positions;
		glm::vec2 center = { 0.0f, 0.0f };
		float rotation = 0.0f;

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
			bool inControl;
			bool isCircle = false;
			const std::string type;
			float speed;
			float w = -0.1f;
			float h = -0.1f;
			float r = -0.1f;

			GameObject(MyDevice& device);

			void makeTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
			void makeSquare(float x, float y, float width, float height);
			void makeCircle(float x1, float y1, float radius);

			void moveUp();
			void moveDown();
			void moveLeft();
			void moveRight();

			void rotate(float angle);

			void bindVertices();

			bool ifObjectsCollide(GameObject& target);

			std::vector<glm::vec2> calcPos();

			GameObject(const GameObject&) = delete;
			GameObject& operator=(const GameObject&) = delete;
			GameObject(GameObject&&) = default;
			GameObject& operator=(GameObject&&) = default;

			glm::vec3 getLineEquation(glm::vec2 p1, glm::vec2 p2);
			std::vector<glm::vec3> getshapeEquations(std::vector<glm::vec2> shape);
			glm::vec2 getIntersection(glm::vec3 line1, glm::vec3 line2);
			bool isPointInShapeLine(glm::vec2 point, std::vector<glm::vec2> shape);

			std::shared_ptr<Alpha_Model> model{};
			glm::vec3 color{0.0f, 0.0f, 0.0f};
			Transform2d transform2d{};
		private:
			//GameObject(id_t objID);
			
			MyDevice& device;

	};

}