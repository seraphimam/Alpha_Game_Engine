#pragma once

#include "GameObject.h"
#include <iostream>

namespace dev {

	GameObject::GameObject(MyDevice& device) : device{ device } {
		static id_t curID = 0;
		id = curID++;
		speed = 0.02f; //default speed

		if (id == 0) {
			inControl = true;
		}
		else {
			inControl = false;
		}
	}

	void GameObject::makeTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {

		isCircle = false;

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

		isCircle = false;
		r = -0.1f;
		w = width;
		h = height;

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

		isCircle = true;
		w = -0.1f;
		h = -0.1f;
		r = radius;

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
		transform2d.translation.y -= speed;
		/*for (int i = 0; i < transform2d.positions.size(); i++) {
			transform2d.positions[i].y -= 0.1f;
		}*/
	}

	void GameObject::moveDown(){
		transform2d.translation.y += speed;
		/*for (int i = 0; i < transform2d.positions.size(); i++) {
			transform2d.positions[i].y += 0.1f;
		}*/
	}

	void GameObject::moveLeft() {
		transform2d.translation.x -= speed;
		/*for (int i = 0; i < transform2d.positions.size(); i++) {
			transform2d.positions[i].x -= 0.1f;
		}*/
	}

	void GameObject::moveRight() {
		transform2d.translation.x += speed;
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

	//collision for only square and circle
	bool GameObject::ifObjectsCollide(GameObject& target) {
		std::vector<glm::vec2> selfCoords = calcPos();
		std::vector<glm::vec2> targetCoords = target.calcPos();

		float MinX = selfCoords[0].x;
		float MaxX = selfCoords[0].x;
		float MinY = selfCoords[0].y;
		float MaxY = selfCoords[0].y;

		float tMinX = targetCoords[0].x;
		float tMaxX = targetCoords[0].x;
		float tMinY = targetCoords[0].y;
		float tMaxY = targetCoords[0].y;

		for (int i = 1; i < selfCoords.size(); i++) {
			if (MinX > selfCoords[i].x) {
				MinX = selfCoords[i].x;
			}

			if (MaxX < selfCoords[i].x) {
				MaxX = selfCoords[i].x;
			}

			if (MinY > selfCoords[i].y) {
				MinY = selfCoords[i].y;
			}

			if (MaxY < selfCoords[i].y) {
				MaxY = selfCoords[i].y;
			}
		}

		for (int i = 1; i < targetCoords.size(); i++) {
			if (tMinX > targetCoords[i].x) {
				tMinX = targetCoords[i].x;
			}

			if (tMaxX < targetCoords[i].x) {
				tMaxX = targetCoords[i].x;
			}

			if (tMinY > targetCoords[i].y) {
				tMinY = targetCoords[i].y;
			}

			if (tMaxY < targetCoords[i].y) {
				tMaxY = targetCoords[i].y;
			}
		}

		if (isCircle && target.isCircle) {
			if (glm::distance(selfCoords[0], targetCoords[0]) > r + target.r) {
				color = { 0.0f, 1.0f, 0.0f };
				target.color = { 0.0f, 1.0f, 0.0f };
				return false;
			}
			else {
				color = { 1.0f, 0.0f, 0.0f };
				target.color = { 1.0f, 0.0f, 0.0f };
				return true;
			}
		}
		else if (isCircle) {

			std::vector<glm::vec2> p4 = { 
				{tMinX, tMinY},
				{tMinX, tMaxY},
				{tMaxX, tMinY},
				{tMaxX, tMaxY}
			};

			for (int i = 0; i < p4.size(); i++) {
				if (glm::distance(selfCoords[0], p4[i]) <= r) {
					color = { 1.0f, 0.0f, 0.0f };
					target.color = { 1.0f, 0.0f, 0.0f };
					return true;
				}
				else if(i == p4.size() - 1){
					color = { 0.0f, 1.0f, 0.0f };
					target.color = { 0.0f, 1.0f, 0.0f };
					return false;
				}
			}
			
		}
		else if (target.isCircle) {

			std::vector<glm::vec2> p4 = {
				{MinX, MinY},
				{MinX, MaxY},
				{MaxX, MinY},
				{MaxX, MaxY}
			};

			for (int i = 0; i < p4.size(); i++) {
				if (glm::distance(targetCoords[0], p4[i]) <= target.r) {
					color = { 1.0f, 0.0f, 0.0f };
					target.color = { 1.0f, 0.0f, 0.0f };
					return true;
				}
				else if (i == p4.size() - 1) {
					color = { 0.0f, 1.0f, 0.0f };
					target.color = { 0.0f, 1.0f, 0.0f };
					return false;
				}
			}
		}
		//2squares
		else {
			if (tMinX <= MaxX && tMinX >= MinX && tMaxY <= MaxY && tMaxY >= MinY) {
				color = { 1.0f, 0.0f, 0.0f };
				target.color = { 1.0f, 0.0f, 0.0f };
				return true;
			}
			else if (tMinX <= MaxX && tMinX >= MinX && tMinY <= MaxY && tMinY >= MinY) {
				color = { 1.0f, 0.0f, 0.0f };
				target.color = { 1.0f, 0.0f, 0.0f };
				return true;
			}
			else if (tMaxX <= MaxX && tMaxX >= MinX && tMaxY <= MaxY && tMaxY >= MinY) {
				color = { 1.0f, 0.0f, 0.0f };
				target.color = { 1.0f, 0.0f, 0.0f };
				return true;
			}
			else if (tMaxX <= MaxX && tMaxX >= MinX && tMinY <= MaxY && tMinY >= MinY) {
				color = { 1.0f, 0.0f, 0.0f };
				target.color = { 1.0f, 0.0f, 0.0f };
				return true;
			}
			else{
				color = { 0.0f, 1.0f, 0.0f };
				target.color = { 0.0f, 1.0f, 0.0f };
				return false;
			}
		}

	}//end collision for sq/circle


	//bool GameObject::ifObjectsCollide(GameObject& target) {
	//	//calculate position of 2 objects after transformations
	//	std::vector<glm::vec2> selfCoords = calcPos();
	//	std::vector<glm::vec2> targetCoords = target.calcPos();

	//	/*float MinX = 2.0f;
	//	float MinY = 2.0f;

	//	float MaxX = -2.0f;
	//	float MaxY = -2.0f;

	//	float targetMinX = 2.0f;
	//	float targetMinY = 2.0f;

	//	float targetMaxX = -2.0f;
	//	float targetMaxY = -2.0f;

	//	for (int i = 0; i < selfCoords.size(); i++) {
	//		MinX = glm::min(selfCoords[i].x, MinX);
	//		MinY = glm::min(selfCoords[i].y, MinX);
	//		MaxX = glm::max(selfCoords[i].x, MaxX);
	//		MaxY = glm::max(selfCoords[i].y, MaxY);
	//	}

	//	for (int i = 0; i < targetCoords.size(); i++) {
	//		MinX = glm::min(targetCoords[i].x, MinX);
	//		MinY = glm::min(targetCoords[i].y, MinX);
	//		MaxX = glm::max(targetCoords[i].x, MaxX);
	//		MaxY = glm::max(targetCoords[i].y, MaxY);
	//	}*/

	//	//if both shapes are not circle
	//	//if (!isCircle && !target.isCircle) {
	//		std::vector<glm::vec3> selfEq = getshapeEquations(selfCoords);
	//		std::vector<glm::vec3> targetEq = getshapeEquations(targetCoords);

	//		//std::cout << "self: " << selfEq[0].x;

	//		std::vector<glm::vec2> self2p;
	//		std::vector<glm::vec2> target2p;
	//		//std::cout << "poi test\n";

	//		for (int i = 0; i < selfEq.size(); i++) {
	//			self2p.clear();
	//			target2p.clear();

	//			for (int j = 0; j < targetEq.size(); j++) {
	//				glm::vec2 poi = getIntersection(selfEq[i], targetEq[j]);

	//				std::cout << "X: " << poi.x << " Y: " << poi.y << "\n";
	//				std::cout << "selfX1: " << selfCoords[i].x << " selfY1: " << selfCoords[i].y << "\n";
	//				std::cout << "targetX1: " << targetCoords[i].x << " targetY1: " << targetCoords[i].y << "\n";

	//				if (i % 3 == 0) {
	//					self2p.push_back(selfCoords[i]);
	//					self2p.push_back(selfCoords[(i + 1)]);
	//				}
	//				else if (i % 3 == 1) {
	//					self2p.push_back(selfCoords[(i - 1)]);
	//					self2p.push_back(selfCoords[(i + 1)]);
	//				}
	//				else {
	//					self2p.push_back(selfCoords[i]);
	//					self2p.push_back(selfCoords[(i - 1)]);
	//				}

	//				if (j % 3 == 0) {
	//					target2p.push_back(targetCoords[j]);
	//					target2p.push_back(targetCoords[(j + 1)]);
	//				}
	//				else if (j % 3 == 1) {
	//					target2p.push_back(targetCoords[(j - 1)]);
	//					target2p.push_back(targetCoords[(j + 1)]);
	//				}
	//				else {
	//					target2p.push_back(targetCoords[j]);
	//					target2p.push_back(targetCoords[(j - 1)]);
	//				}


	//				if (isPointInShapeLine(poi, self2p) && isPointInShapeLine(poi, target2p)) {
	//					color = { 1.0f, 0.0f, 0.0f };
	//					target.color = { 1.0f, 0.0f, 0.0f };
	//					return true;
	//				}
	//				else {
	//					color = { 0.1f, 0.8f, 0.1f };
	//					target.color = { 0.1f, 0.8f, 0.1f };
	//					return false;
	//				}
	//			}
	//		}
	//		
	//	//}
	//	//if one of the shapes is circle
	//	//else {
	//	//	if (isCircle) {
	//	//		if (target.isCircle) {

	//	//		}
	//	//		else {

	//	//		}
	//	//	}
	//	//	else {

	//	//	}
	//	//}//end else (one of the shape is circle)

	//}//end ifObjectsCollide

	//return ax+by+c if format (a->x), (b->y), (c->z)
	glm::vec3 GameObject::getLineEquation(glm::vec2 p1, glm::vec2 p2) {
		glm::vec3 points;

		if (p1.x == p2.x) {
			points.x = 1.0f;
			points.y = 0.0f;
			points.z = p1.x;
		}
		else if (p1.y == p2.y) {
			points.x = 0.0f;
			points.y = 1.0f;
			points.z = p1.y;
		}
		else {
			float slope = (p2.y - p1.y) / (p2.x - p1.x);
			//std::cout << "\nSlope: " << slope << "\n";

			//y - y1 = m(x - x1)
			//mx - y - mx1 + y1
			//a = m, b = -1, c = -mx1 + y1
			points.x = slope;
			points.y = -1.0f;
			points.z = p1.y - (slope * p1.x);
		}

		

		return points;

	}//end getLineEquation

	//gets line equations between all perimeters of shape forming triangles
	std::vector<glm::vec3> GameObject::getshapeEquations(std::vector<glm::vec2> shape)
	{
		std::vector<glm::vec3> equations;
		glm::vec3 eq;

		for (int i = 0; i < shape.size(); i += 3) {
			eq = getLineEquation(shape[i], shape[(i + 1)]);

			equations.push_back(eq);

			eq = getLineEquation(shape[i], shape[(i + 2)]);

			equations.push_back(eq);

			eq = getLineEquation(shape[(i + 1)], shape[(i + 2)]);

			equations.push_back(eq);
		}

		return equations;

	}//end getshapePerimeterEquations
	
	//check if 2 lines intersects and returns point of intersection
	glm::vec2 GameObject::getIntersection(glm::vec3 line1, glm::vec3 line2) {
		glm::vec2 intersection = { 2.0f,2.0f };

		//x = (b*c2 - b2*c) / (a*b2 - a2*b)
		if (line1.y == -1.0f && line2.y == -1.0f) {
			intersection.x = (line1.y * line2.z - line2.y * line1.z) / (line1.x * line2.y - line2.x * line1.y);
			intersection.y = (line1.x * intersection.x + line1.z) / (-1.0f * line1.y);
		}
		//line2 is vertical or horizontal
		else if (line1.y == -1.0f) {
			if (line2.x == 0.0f) {
				//line2 y = n;
				intersection.x = (line2.z - line1.z) / line1.x;
				intersection.y = line2.z;
			}
			else {
				//line2 x = n;
				intersection.x = line2.z;
				intersection.y = (line2.z - line1.z) / line1.x;
				
			}
		}
		//line1 is vertical or horizontal
		else {
			if (line1.x == 0.0f) {
				//line1 y = n;
				intersection.x = (line1.z - line2.z) / line2.x;
				intersection.y = line1.z;
			}
			else {
				//line2 x = n;
				intersection.x = line1.z;
				intersection.y = (line1.z - line2.z) / line2.x;

			}

		}

		

		return intersection;
	}

	bool GameObject::isPointInShapeLine(glm::vec2 point, std::vector<glm::vec2> twoPointCoords) {
		if ((point.x <= twoPointCoords[0].x && point.x >= twoPointCoords[1].x) || (point.x >= twoPointCoords[0].x && point.x <= twoPointCoords[1].x)) {
			if ((point.y <= twoPointCoords[0].y && point.y >= twoPointCoords[1].y) || (point.y >= twoPointCoords[0].y && point.y <= twoPointCoords[1].y)) {
				return true;
			}
		}

		return false;
	}
}//end dev