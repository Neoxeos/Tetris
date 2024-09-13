#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
class Shape : public sf::RectangleShape
{
public:
	// rectangles will have these dimensions
	Shape();
	Shape(std::string type, int id, int w, int h);
	std::string type; 
	int id;
	int w = 28;
	int h = 28;
	bool toDestroy = false;
	std::vector<Shape> container;

	std::vector<Shape>& generate();
	std::vector<Shape>& LShape();
	std::vector<Shape>& SquareShape();
	std::vector<Shape>& TShape();
	std::vector<Shape>& LineShape();
};