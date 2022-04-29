#pragma once

#include "GameObject.h"
#include <iostream>

namespace dev {

	GameObject::GameObject(MyDevice& device) : device{ device } {
		static id_t curID = 0;
		id = curID++;
		speed = 0.1f; //default speed

		if (id == 0) {
			inControl = true;
		}
		else {
			inControl = false;
		}
	}

	void GameObject::makeTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {

		std::vector<Alpha_Model::Vertex> triangle{
			{{x1, y1}, {1.0f, 0.0f, 0.0f}},
			{{x2, y2}, {0.0f, 1.0f, 0.0f}},
			{{x3, y3}, {0.0f, 0.0f, 1.0f}}
		};

		transform2d.positions.push_back({ x1, y1 });
		transform2d.positions.push_back({ x2, y2 });
		transform2d.positions.push_back({ x3, y3 });

		transform2d.center.x = (x1 + x2 + x3) / 3;
		transform2d.center.y = (y1 + y2 + y3) / 3;

		auto triangleModel = std::make_shared<Alpha_Model>(device, triangle);

		model = triangleModel;
	}//end makeTriangle

	void GameObject::makeSquare(float x, float y, float width, float height) {

		std::vector<Alpha_Model::Vertex> square{
			{{x, y}, {1.0f, 0.0f, 0.0f}},
			{{x, (y-height)}, {0.0f, 1.0f, 0.0f}},
			{{(x + width), (y - height)}, {0.0f, 0.0f, 1.0f}},
			{{x, y}, {1.0f, 0.0f, 0.0f}},
			{{(x + width), (y - height)}, {0.0f, 0.0f, 1.0f}},
			{{(x + width), y}, {0.0f, 1.0f, 0.0f}}
		};

		auto squareModel = std::make_shared<Alpha_Model>(device, square);

		transform2d.positions.push_back({ x, y });
		transform2d.positions.push_back({ x, (y - height) });
		transform2d.positions.push_back({ (x + width), (y - height) });
		transform2d.positions.push_back({ x, y });
		transform2d.positions.push_back({ (x + width), (y - height) });
		transform2d.positions.push_back({ (x + width), y });

		transform2d.center.x = (x + (x + width)) / 2;
		transform2d.center.y = (y + (y - height)) / 2;

		model = squareModel;
	}//end makeSquare

	void GameObject::makeCircle(float x, float y, float radius) {

		std::vector<Alpha_Model::Vertex> circle{
		};

		int steps = 60;
		float angle = glm::two_pi<float>() / steps;

		for (int i = 0; i < steps; i++) {
			float x1 = radius * glm::sin(angle * i) + x;
			float x2 = radius * glm::sin(angle * (i + 1)) + x;
			float y1 = radius * glm::cos(angle * i) + y;
			float y2 = radius * glm::cos(angle * (i + 1)) + y;

			circle.push_back(
				{ {x, y}, {0.0f, 1.0f, 0.0f} }
			);

			transform2d.positions.push_back({ x, y });

			circle.push_back(
				{ {x1, y1}, {0.0f, 1.0f, 0.0f} }
			);

			transform2d.positions.push_back({ x1, y1 });

			circle.push_back(
				{ {x2, y2}, {0.0f, 1.0f, 0.0f} }
			);

			transform2d.positions.push_back({ x2, y2 });

		}//end create cirle for loop

		auto circleModel = std::make_shared<Alpha_Model>(device, circle);

		transform2d.center.x = x;
		transform2d.center.y = y;

		model = circleModel;
	}//end makeCircle();

	void GameObject::moveUp() {
		transform2d.translation.y -= 0.1f;
		/*for (int i = 0; i < transform2d.positions.size(); i++) {
			transform2d.positions[i].y -= 0.1f;
		}*/
	}

	void GameObject::moveDown(){
		transform2d.translation.y += 0.1f;
		/*for (int i = 0; i < transform2d.positions.size(); i++) {
			transform2d.positions[i].y += 0.1f;
		}*/
	}

	void GameObject::moveLeft() {
		transform2d.translation.x -= 0.1f;
		/*for (int i = 0; i < transform2d.positions.size(); i++) {
			transform2d.positions[i].x -= 0.1f;
		}*/
	}

	void GameObject::moveRight() {
		transform2d.translation.x += 0.1f;
		/*for (int i = 0; i < transform2d.positions.size(); i++) {
			transform2d.positions[i].x += 0.1f;
		}*/
	}

	void GameObject::rotate(float angle) {
		//glm::vec2 origin;
		//origin = transform2d.positions[0];
		//std::cout << "before rotate: " << transform2d.rotation << "\n";

		transform2d.rotation = glm::mod(transform2d.rotation + angle, 360.0f);
		

		//std::cout << "after rotate: " << transform2d.rotation << "\n";

		/*for (int i = 0; i < transform2d.positions.size(); i++) {
			
			
		}*/
	}

	std::vector<glm::vec2> GameObject::calcPos() {
		std::vector<glm::vec2> vert;
		float rad = glm::radians(transform2d.rotation);

		for (int i = 0; i < transform2d.positions.size(); i++) {
			vert.push_back( transform2d.positions[i] );
		}

		for (int i = 0; i < vert.size(); i++) {
			float newX = vert[i].x - transform2d.center.x;
			float newY = vert[i].y - transform2d.center.y;

			float rotatedX = newX * glm::cos(rad) - newY * glm::sin(rad);
			float rotatedY = newX * glm::sin(rad) + newY * glm::cos(rad);

			vert[i].x = rotatedX + transform2d.center.x;
			vert[i].y = rotatedY + transform2d.center.y;

			vert[i].x += transform2d.translation.x;
			vert[i].y += transform2d.translation.y;

			vert[i].x = vert[i].x * transform2d.scale.x;
			vert[i].y = vert[i].y * transform2d.scale.y;
		}

		return vert;
	}

	void GameObject::bindVertices() {
		std::vector<Alpha_Model::Vertex> v{
		};

		std::vector<glm::vec2> newVerties = calcPos();

		for (int i = 0; i < newVerties.size(); i++) {
			//std::cout << "printX: " << newVerties[i].x << "\n";
			v.push_back({ newVerties[i], color });
		}
		
		model->createVertexBuffers(v);
	}
}